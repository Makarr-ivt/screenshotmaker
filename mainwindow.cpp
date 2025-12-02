#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QThread>
#include <QTimer>
#include <QResizeEvent>
#include <QGuiApplication>

#include <QFileDialog>
#include <QMessageBox>
#include <QDateTime>
#include <QDir>

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
    // Проверяем, есть ли скриншот для сохранения
    if (m_currentScreenshot.isNull()) {
        QMessageBox::warning(this,
                             "Нет скриншота",
                             "Сначала сделайте скриншот, чтобы сохранить его.");
        return;
    }

    // Генерируем имя файла по умолчанию с датой и временем
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss");
    QString defaultFileName = QDir::homePath() + "/Снимок_" + timestamp + ".png";

    // Открываем диалог сохранения файла
    QString fileName = QFileDialog::getSaveFileName(
        this,                                      // родительское окно
        "Сохранить скриншот",                     // заголовок
        defaultFileName,                          // начальный путь
        "Изображения (*.png *.jpg *.jpeg *.bmp);;PNG (*.png);;JPEG (*.jpg *.jpeg);;BMP (*.bmp)" // фильтры
        );

    // Если пользователь выбрал файл (не нажал "Отмена")
    if (!fileName.isEmpty()) {
        // Пытаемся сохранить
        bool success = m_capturer.saveToFile(m_currentScreenshot, fileName);

        if (success) {
            // Успех - показываем сообщение
            QMessageBox::information(this, "Успешно", QString("Скриншот сохранен в:\n%1").arg(fileName));

            // Можно добавить визуальную обратную связь
            QString originalText = ui->saveButton->text();
            ui->saveButton->setText("Сохранено!");

            // Через секунду возвращаем оригинальный текст
            QTimer::singleShot(1000, this, [this, originalText]() {
                ui->saveButton->setText(originalText);
            });
        }
    }
}

void MainWindow::on_exitButton_clicked()
{
    close();
}

void MainWindow::on_copyToClipboardButton_clicked()
{
    // Упрощенная проверка
    if (m_currentScreenshot.isNull()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("Нет скриншота");
        msgBox.setText("Сначала сделайте скриншот.");
        msgBox.exec();
        return;
    }

    // Копируем
    m_capturer.copyToClipboard(m_currentScreenshot);

    // Простая обратная связь
    ui->copyToClipboardButton->setText("Скопировано!");
    QTimer::singleShot(1000, this, [this]() {
        ui->copyToClipboardButton->setText("Копировать в буфер");
    });
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
