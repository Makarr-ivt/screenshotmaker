#pragma once

#include <QMainWindow>
#include "screenshotcapturer.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_captureFullScreenButton_clicked();
    void on_captureAreaButton_clicked();
    void on_saveButton_clicked();
    void on_exitButton_clicked();
    void on_copyToClipboardButton_clicked();

private:
    Ui::MainWindow *ui;
    ScreenshotCapturer m_capturer;
    QPixmap m_currentScreenshot;
    
    void updateUI();
    void updateScreenshotDisplay();
    void updateInfoLabel();

protected:
    void resizeEvent(QResizeEvent *event) override;
};
