#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <QMessageBox>
#include <QPushButton>
#include <QStatusBar>

#include "screenshotcontroller.h"
#include "screenshotviewer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_controller(new ScreenshotController(this))
    , m_viewer(nullptr)
{
    ui->setupUi(this);

    // Оборачиваем существующий QLabel (ui->screenshotLabel) в ScreenshotViewer
    m_viewer = new ScreenshotViewer(ui->screenshotLabel, this);

    // Подписываемся на сигналы контроллера
    connect(m_controller, &ScreenshotController::imageChanged, this, &MainWindow::onControllerImageChanged);
    connect(m_controller, &ScreenshotController::errorOccured, this, &MainWindow::onControllerError);
    connect(m_controller, &ScreenshotController::infoMessage, this, &MainWindow::onControllerInfo);

    // Инициализируем состояние UI
    m_viewer->clear();
    updateUIStates();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_captureFullScreenButton_clicked()
{
    // Передаём окно контроллеру, чтобы тот временно скрывал его при захвате
    m_controller->captureFullScreen(this);
}

void MainWindow::on_captureAreaButton_clicked()
{
    m_controller->startAreaSelection(this);
}

void MainWindow::on_saveButton_clicked()
{
    if (!m_controller->hasImage()) {
        QMessageBox::warning(this, "Нет скриншота", "Сначала сделайте скриншот, чтобы сохранить его.");
        return;
    }

    bool success = m_controller->saveImageToFile(this);
    if (success) {
        // Короткая визуальная подсказка на кнопке (временная)
        showTemporaryButtonFeedback(ui->saveButton, "Сохранено!");
    }
}

void MainWindow::on_copyToClipboardButton_clicked()
{
    if (!m_controller->hasImage()) {
        QMessageBox::warning(this, "Нет скриншота", "Сначала сделайте скриншот.");
        return;
    }

    m_controller->copyImageToClipboard();
    showTemporaryButtonFeedback(ui->copyToClipboardButton, "Скопировано!");
}

void MainWindow::onControllerImageChanged()
{
    QPixmap pix = m_controller->currentImage();
    m_viewer->setImage(pix);
    updateUIStates();
}

void MainWindow::onControllerError(const QString &msg)
{
    QMessageBox::warning(this, "Ошибка", msg);
}

void MainWindow::onControllerInfo(const QString &msg)
{
    // Короткое информационное сообщение — можно показывать в статусбар
    statusBar()->showMessage(msg, 2000);
}

void MainWindow::updateUIStates()
{
    bool hasImage = m_controller->hasImage();
    ui->saveButton->setEnabled(hasImage);
    ui->copyToClipboardButton->setEnabled(hasImage);

    // Обновление infoLabel (информация о размере)
    if (hasImage) {
        QSize s = m_controller->currentImage().size();
        ui->infoLabel->setText(QString("Размер: %1×%2").arg(s.width()).arg(s.height()));
    } else {
        ui->infoLabel->setText("Размер: 0×0");
    }
}

void MainWindow::showTemporaryButtonFeedback(QWidget *buttonWidget, const QString &text, int msec)
{
    QPushButton *btn = qobject_cast<QPushButton*>(buttonWidget);
    if (!btn) return;

    QString orig = btn->text();
    btn->setText(text);
    QTimer::singleShot(msec, this, [btn, orig]() { btn->setText(orig); });
}

// При изменении размера окна просто обновим отображение через viewer
void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    // подстраиваем отображение изображения (viewer сам масштабирует по метке)
    if (m_controller->hasImage()) {
        m_viewer->setImage(m_controller->currentImage());
    }
}
