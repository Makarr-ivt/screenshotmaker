#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QThread>
#include <QTimer>
#include <QResizeEvent>
#include <QGuiApplication>
#include <QClipboard>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    updateUI();

    // Подключаем кнопку выхода
    connect(ui->exitButton, &QPushButton::clicked, this, &MainWindow::close);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_captureFullScreenButton_clicked()
{
    // Сохраняем состояние окна перед скрытием
    bool wasMaximized = this->isMaximized();
    QRect originalGeometry = this->geometry();

    // Скрываем окно на время захвата
    this->hide();

    // Даем время окну скрыться
    QThread::msleep(200);

    // Захватываем экран
    m_currentScreenshot = m_capturer.captureFullScreen();

    // Показываем окно обратно
    this->show();

    // Восстанавливаем состояние окна
    if (wasMaximized) {
        this->showMaximized();
    } else {
        this->setGeometry(originalGeometry);
    }

    // Если скриншот удался, обновляем UI
    if (!m_currentScreenshot.isNull()) {
        updateScreenshotDisplay();
        updateInfoLabel();

        // Активируем кнопки действий
        ui->saveButton->setEnabled(true);
        ui->copyToClipboardButton->setEnabled(true);

        // Временно меняем заголовок для обратной связи
        this->setWindowTitle("ScreenshotMaker - Захвачено!");
        QTimer::singleShot(1500, this, [this]() {
            this->setWindowTitle("ScreenshotMaker");
        });
    } else {
        // Обработка ошибки
        ui->screenshotLabel->setText("Ошибка захвата экрана");
    }
}

void MainWindow::on_captureAreaButton_clicked()
{
    // TODO: Реализовать
}

void MainWindow::on_saveButton_clicked()
{
    // TODO: Реализовать
}

void MainWindow::on_exitButton_clicked()
{
    close();
}

void MainWindow::on_copyToClipboardButton_clicked()
{
    // TODO: Реализовать
}

void MainWindow::updateUI()
{
    // Изначально кнопки действий отключены
    ui->saveButton->setEnabled(false);
    ui->copyToClipboardButton->setEnabled(false);
    ui->captureAreaButton->setEnabled(false);
    
    // Устанавливаем начальный текст
    ui->screenshotLabel->setText("Нажмите 'Весь экран' для захвата");
    ui->screenshotLabel->setAlignment(Qt::AlignCenter);
    
    updateInfoLabel();
}

void MainWindow::updateScreenshotDisplay()
{
    if (m_currentScreenshot.isNull()) {
        return;
    }

    // Получаем размер метки
    QSize labelSize = ui->screenshotLabel->size();

    // Масштабируем изображение, сохраняя пропорции
    QPixmap scaledPixmap = m_currentScreenshot.scaled(
        labelSize,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
        );

    // Отображаем скриншот
    ui->screenshotLabel->setPixmap(scaledPixmap);
    ui->screenshotLabel->setText("");
}

void MainWindow::updateInfoLabel()
{
    ui->infoLabel->setText(m_capturer.getScreenshotInfo(m_currentScreenshot));
}

// Добавляем обработчик изменения размера окна
void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    updateScreenshotDisplay();
}
