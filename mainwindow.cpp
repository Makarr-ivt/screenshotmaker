#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    updateUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_captureFullScreenButton_clicked()
{
    // TODO: Реализовать
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
    // TODO: Реализовать
}

void MainWindow::updateInfoLabel()
{
    ui->infoLabel->setText("Размер: 0×0");
}