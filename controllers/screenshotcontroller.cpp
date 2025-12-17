#include "screenshotcontroller.h"

#include <QGuiApplication>
#include <QClipboard>
#include <QFileDialog>
#include <QFileInfo>
#include <QMap>
#include <QScreen>
#include <QTimer>

#include "fullscreencapturestrategy.h"
#include "rectangleselectionoverlay.h"

ScreenshotController::ScreenshotController(QObject *parent)
    : QObject(parent)
{
    m_fullScreenStrategy = std::make_unique<FullScreenCaptureStrategy>();
}

bool ScreenshotController::hasImage() const
{
    return !m_currentImage.isNull();
}

QPixmap ScreenshotController::currentImage() const
{
    return m_currentImage;
}

void ScreenshotController::setImage(const QPixmap &pixmap)
{
    m_currentImage = pixmap;
    emit imageChanged();
}

void ScreenshotController::captureFullScreen(QWidget *parentWidget)
{
    if (!parentWidget) {
        emit errorOccured("Не удалось получить окно приложения.");
        return;
    }

    parentWidget->hide();

    // Даём Qt время убрать окно с экрана
    QTimer::singleShot(100, this, [this, parentWidget]() {
        QPixmap pixmap = m_fullScreenStrategy->capture();

        parentWidget->show();
        parentWidget->raise();
        parentWidget->activateWindow();

        if (pixmap.isNull()) {
            emit errorOccured("Не удалось сделать скриншот экрана.");
            return;
        }

        setImage(pixmap);
        emit infoMessage("Скриншот экрана сделан");
    });
}

void ScreenshotController::startAreaSelection(QWidget *parentWidget)
{
    if (!parentWidget) {
        emit errorOccured("Не удалось получить окно приложения.");
        return;
    }

    // 1. Скрываем главное окно
    parentWidget->hide();

    // 2. Даём Qt время убрать его с экрана
    QTimer::singleShot(100, this, [this, parentWidget]() {

        if (m_overlay) {
            m_overlay->close();
            m_overlay->deleteLater();
        }

        m_overlay = new RectangleSelectionOverlay();
        m_overlay->show();
        m_overlay->raise();
        m_overlay->activateWindow();

        // Выбор завершён
        connect(m_overlay, &RectangleSelectionOverlay::selectionCompleted,
                this, &ScreenshotController::onAreaSelected);

        // Отмена выбора — возвращаем окно
        connect(m_overlay, &RectangleSelectionOverlay::selectionCancelled,
                this, [this, parentWidget]() {
                    parentWidget->show();
                    parentWidget->raise();
                    parentWidget->activateWindow();
                    emit infoMessage("Выделение области отменено");
                });
    });
}



void ScreenshotController::onAreaSelected(const QRect &rect)
{
    if (rect.isNull()) {
        emit errorOccured("Выделена пустая область.");
        return;
    }

    QRect captureRect = rect;

    // Убираем overlay
    if (m_overlay) {
        m_overlay->hide();
        m_overlay->deleteLater();
        m_overlay = nullptr;
    }

    QWidget *mainWindow = qobject_cast<QWidget*>(parent());

    // Даём Qt время убрать overlay
    QTimer::singleShot(50, this, [this, captureRect, mainWindow]() {

        QScreen *screen = QGuiApplication::primaryScreen();
        if (!screen) {
            emit errorOccured("Не удалось получить экран.");
            return;
        }

        QPixmap pixmap = screen->grabWindow(
            0,
            captureRect.x(),
            captureRect.y(),
            captureRect.width(),
            captureRect.height()
            );

        // Возвращаем главное окно
        if (mainWindow) {
            mainWindow->show();
            mainWindow->raise();
            mainWindow->activateWindow();
        }

        if (pixmap.isNull()) {
            emit errorOccured("Не удалось сделать скриншот области.");
            return;
        }

        setImage(pixmap);
        emit infoMessage("Скриншот области сделан");
    });
}

void ScreenshotController::copyImageToClipboard()
{
    if (!hasImage()) {
        emit errorOccured("Нет изображения для копирования.");
        return;
    }

    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setPixmap(m_currentImage);

    emit infoMessage("Скриншот скопирован в буфер обмена");
}

bool ScreenshotController::saveImageToFile(QWidget *parentWidget)
{
    if (!hasImage()) {
        emit errorOccured("Нет изображения для сохранения.");
        return false;
    }

    QString filePath = QFileDialog::getSaveFileName(
        parentWidget,
        "Сохранить скриншот",
        QString(),
        fileDialogFilter()
        );

    if (filePath.isEmpty())
        return false;

    QString format = formatFromExtension(filePath);
    bool success = m_currentImage.save(filePath, format.toUtf8().constData());

    if (!success) {
        emit errorOccured("Не удалось сохранить файл.");
        return false;
    }

    emit infoMessage("Скриншот сохранён");
    return true;
}

QString ScreenshotController::fileDialogFilter() const
{
    return "PNG (*.png);;JPEG (*.jpg *.jpeg);;BMP (*.bmp);;All Files (*.*)";
}

QString ScreenshotController::formatFromExtension(const QString &filePath) const
{
    QFileInfo info(filePath);
    QString ext = info.suffix().toLower();

    static const QMap<QString, QString> map = {
        {"png", "PNG"},
        {"jpg", "JPG"},
        {"jpeg", "JPG"},
        {"bmp", "BMP"}
    };

    return map.value(ext, "PNG");
}
