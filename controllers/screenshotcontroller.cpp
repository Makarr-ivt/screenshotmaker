#include "screenshotcontroller.h"

#include <QThread>
#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>
#include <QGuiApplication>
#include <QScreen>

#include "fullscreencapturestrategy.h"
#include "rectanglecapturestrategy.h"
#include "rectangleselectionoverlay.h"
#include "fileexportservice.h"
#include "clipboardservice.h"
#include "capturemanager.h"

ScreenshotController::ScreenshotController(QObject *parent)
    : QObject(parent),
    m_captureManager(),
    m_fileExportService(),
    m_clipboardService()
{
}

ScreenshotController::~ScreenshotController() = default;

void ScreenshotController::setImage(const QPixmap &pix)
{
    m_currentImage = pix;
    emit imageChanged();
}

QPixmap ScreenshotController::currentImage() const
{
    return m_currentImage;
}

bool ScreenshotController::hasImage() const
{
    return !m_currentImage.isNull();
}

void ScreenshotController::captureFullScreen(QWidget *windowToHide)
{
    // Если передали окно — скрываем его на время захвата (чтобы не попасть в скриншот)
    bool wasMax = false;
    QRect originalGeom;
    if (windowToHide) {
        wasMax = windowToHide->isMaximized();
        originalGeom = windowToHide->geometry();
        windowToHide->hide();
        // даём время скрыться
        QThread::msleep(200);
    }

    m_captureManager.setStrategy(std::make_unique<FullScreenCaptureStrategy>());
    QPixmap screenshot = m_captureManager.capture();

    if (windowToHide) {
        windowToHide->show();
        if (wasMax) windowToHide->showMaximized(); else windowToHide->setGeometry(originalGeom);
        windowToHide->activateWindow();
        windowToHide->raise();
    }

    if (screenshot.isNull()) {
        emit errorOccured("Не удалось сделать скриншот экрана.");
        return;
    }

    setImage(screenshot);
    emit infoMessage("Скриншот сделан.");
}

void ScreenshotController::startAreaSelection(QWidget *windowToHide)
{
    // Скрываем окно, делаем полноэкранный снимок, показываем overlay и ждем выбор
    bool wasMax = false;
    QRect originalGeom;
    if (windowToHide) {
        wasMax = windowToHide->isMaximized();
        originalGeom = windowToHide->geometry();
        windowToHide->hide();
    }

    // небольшой таймаут — чтобы окно успело скрыться
    QTimer::singleShot(100, this, [this, windowToHide, wasMax, originalGeom]() {
        // 1. Захват полного экрана
        m_captureManager.setStrategy(std::make_unique<FullScreenCaptureStrategy>());
        m_fullScreenShot = m_captureManager.capture();

        if (m_fullScreenShot.isNull()) {
            emit errorOccured("Не удалось сделать снимок экрана для выбора области.");
            if (windowToHide) {
                windowToHide->show();
                if (wasMax) windowToHide->showMaximized(); else windowToHide->setGeometry(originalGeom);
                windowToHide->activateWindow();
                windowToHide->raise();
            }
            return;
        }

        // 2. Создаем overlay
        RectangleSelectionOverlay *overlay = new RectangleSelectionOverlay();
        overlay->setAttribute(Qt::WA_DeleteOnClose);

        // сигнал - отмена
        connect(overlay, &RectangleSelectionOverlay::selectionCancelled, this, [this, windowToHide, wasMax, originalGeom]() {
            if (windowToHide) {
                windowToHide->show();
                if (wasMax) windowToHide->showMaximized(); else windowToHide->setGeometry(originalGeom);
                windowToHide->activateWindow();
                windowToHide->raise();
            }
        });

        // сигнал - завершение выбора
        connect(overlay, &RectangleSelectionOverlay::selectionCompleted, this,
                [this, overlay, windowToHide, wasMax, originalGeom](const QRect &area) {
                    // закрываем overlay (он удалится автоматически)
                    overlay->close();

                    if (!area.isValid() || area.width() <= 10 || area.height() <= 10) {
                        emit errorOccured("Выделенная область слишком мала или некорректна.");
                    } else {
                        QScreen *primaryScreen = QGuiApplication::primaryScreen();
                        if (!primaryScreen) {
                            emit errorOccured("Не удалось получить экран.");
                        } else {
                            qreal dpr = primaryScreen->devicePixelRatio();
                            QRect scaledArea(
                                qRound(area.x() * dpr),
                                qRound(area.y() * dpr),
                                qRound(area.width() * dpr),
                                qRound(area.height() * dpr)
                                );

                            QRect intersected = scaledArea.intersected(m_fullScreenShot.rect());
                            if (!intersected.isValid() || intersected.isEmpty()) {
                                emit errorOccured("Выбранная область вне границ экрана.");
                            } else {
                                QPixmap areaScreenshot = m_fullScreenShot.copy(intersected);
                                if (areaScreenshot.isNull()) {
                                    emit errorOccured("Не удалось вырезать выбранную область.");
                                } else {
                                    areaScreenshot.setDevicePixelRatio(dpr);
                                    setImage(areaScreenshot);
                                    emit infoMessage("Область захвачена.");
                                }
                            }
                        }
                    }

                    // Восстанавливаем окно (MainWindow)
                    if (windowToHide) {
                        windowToHide->show();
                        if (wasMax) windowToHide->showMaximized(); else windowToHide->setGeometry(originalGeom);
                        windowToHide->activateWindow();
                        windowToHide->raise();
                    }
                });

        // также на случай закрытия overlay — восстановим окно
        connect(overlay, &QObject::destroyed, this, [windowToHide, wasMax, originalGeom]() {
            if (windowToHide) {
                windowToHide->show();
                if (wasMax) windowToHide->showMaximized(); else windowToHide->setGeometry(originalGeom);
                windowToHide->activateWindow();
                windowToHide->raise();
            }
        });

        overlay->showFullScreen();
    });
}

bool ScreenshotController::saveImageToFile(QWidget *parent)
{
    if (m_currentImage.isNull()) {
        emit errorOccured("Нет изображения для сохранения.");
        return false;
    }

    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss");
    QString defaultFileName = QDir::homePath() + "/Снимок_" + timestamp + ".png";

    QString fileName = QFileDialog::getSaveFileName(
        parent,
        "Сохранить скриншот",
        defaultFileName,
        m_fileExportService.formatFilter()
        );

    if (fileName.isEmpty()) return false;

    bool ok = m_fileExportService.saveToFile(m_currentImage, fileName);
    if (!ok) {
        emit errorOccured("Не удалось сохранить файл.");
        return false;
    }

    emit infoMessage(QString("Скриншот сохранен в:\n%1").arg(fileName));
    return true;
}

void ScreenshotController::copyImageToClipboard()
{
    if (m_currentImage.isNull()) {
        emit errorOccured("Нет изображения для копирования.");
        return;
    }

    m_clipboardService.copyToClipboard(m_currentImage);
    emit infoMessage("Скриншот скопирован в буфер обмена.");
}
