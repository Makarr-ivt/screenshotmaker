#include "mainwindow.h"
#include "ui_mainwindow.h"

// Стандартные Qt заголовки
#include <QThread>
#include <QTimer>
#include <QResizeEvent>
#include <QGuiApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QDateTime>
#include <QDir>
#include <QScreen>

// Заголовки новой архитектуры
#include "capture/fullscreencapturestrategy.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Подключаем сигналы новой архитектуры
    connect(&m_screenshotContext, &ScreenshotContext::imageChanged,
            this, &MainWindow::onScreenshotContextChanged);

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

    // Захватываем экран с использованием новой архитектуры
    m_captureManager.setStrategy(std::make_unique<FullScreenCaptureStrategy>());
    QPixmap screenshot = m_captureManager.capture();
    m_screenshotContext.setImage(screenshot);

    // Показываем окно обратно
    this->show();

    // Восстанавливаем состояние окна
    if (wasMaximized) {
        this->showMaximized();
    } else {
        this->setGeometry(originalGeometry);
    }

    // Если скриншот удался, обновляем UI
    if (m_screenshotContext.hasImage()) {
        // Временно меняем заголовок для обратной связи
        this->setWindowTitle("ScreenshotMaker - Захвачено!");
        QTimer::singleShot(1500, this, [this]() {
            this->setWindowTitle("ScreenshotMaker");
        });
    } else {
        // Обработка ошибки
        ui->screenshotLabel->setText("Ошибка захвата экрана");
        ui->screenshotLabel->setAlignment(Qt::AlignCenter);
    }
}

void MainWindow::on_captureAreaButton_clicked()
{
    // TODO: Будет реализовано в Фазе 2
    QMessageBox::information(this, "В разработке",
                             "Выбор области будет реализован в следующей версии.");
}

void MainWindow::on_saveButton_clicked()
{
    // Проверяем, есть ли скриншот для сохранения
    if (!m_screenshotContext.hasImage()) {
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
        m_fileExportService.formatFilter()        // фильтры из сервиса
        );

    // Если пользователь выбрал файл (не нажал "Отмена")
    if (!fileName.isEmpty()) {
        // Пытаемся сохранить
        bool success = m_fileExportService.saveToFile(m_screenshotContext.image(), fileName);

        if (success) {
            // Успех - показываем сообщение
            QMessageBox::information(this, "Успешно", QString("Скриншот сохранен в:\n%1").arg(fileName));

            // Визуальная обратная связь
            QString originalText = ui->saveButton->text();
            ui->saveButton->setText("Сохранено!");

            // Через секунду возвращаем оригинальный текст
            QTimer::singleShot(1000, this, [this, originalText]() {
                ui->saveButton->setText(originalText);
            });
        } else {
            QMessageBox::critical(this, "Ошибка", "Не удалось сохранить файл.");
        }
    }
}

void MainWindow::on_exitButton_clicked()
{
    close();
}

void MainWindow::on_copyToClipboardButton_clicked()
{
    // Проверяем, есть ли скриншот
    if (!m_screenshotContext.hasImage()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle("Нет скриншота");
        msgBox.setText("Сначала сделайте скриншот.");
        msgBox.exec();
        return;
    }

    // Копируем с использованием сервиса
    m_clipboardService.copyToClipboard(m_screenshotContext.image());

    // Визуальная обратная связь
    ui->copyToClipboardButton->setText("Скопировано!");
    QTimer::singleShot(1000, this, [this]() {
        ui->copyToClipboardButton->setText("Копировать в буфер");
    });
}

void MainWindow::onScreenshotContextChanged()
{
    updateScreenshotDisplay();
    updateInfoLabel();
    updateButtonStates();
}

void MainWindow::updateUI()
{
    // Изначально кнопки действий отключены
    ui->saveButton->setEnabled(false);
    ui->copyToClipboardButton->setEnabled(false);
    ui->captureAreaButton->setEnabled(false); // Пока не реализовано

    // Устанавливаем начальный текст
    ui->screenshotLabel->setText("Нажмите 'Весь экран' для захвата");
    ui->screenshotLabel->setAlignment(Qt::AlignCenter);

    updateInfoLabel();
    updateButtonStates();
}

void MainWindow::updateScreenshotDisplay()
{
    if (!m_screenshotContext.hasImage()) {
        return;
    }

    // Получаем размер метки
    QSize labelSize = ui->screenshotLabel->size();

    // Масштабируем изображение, сохраняя пропорции
    QPixmap scaledPixmap = m_screenshotContext.image().scaled(
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
    if (m_screenshotContext.hasImage()) {
        QString info = QString("Размер: %1×%2")
                           .arg(m_screenshotContext.image().width())
                           .arg(m_screenshotContext.image().height());
        ui->infoLabel->setText(info);
    } else {
        ui->infoLabel->setText("Размер: 0×0");
    }
}

void MainWindow::updateButtonStates()
{
    bool hasImage = m_screenshotContext.hasImage();
    ui->saveButton->setEnabled(hasImage);
    ui->copyToClipboardButton->setEnabled(hasImage);

    // TODO: Активировать кнопку выбора области, когда она будет реализована
    ui->captureAreaButton->setEnabled(false);
}

// Добавляем обработчик изменения размера окна
void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    updateScreenshotDisplay();
}
