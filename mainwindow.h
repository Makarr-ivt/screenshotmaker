#pragma once

#include <QMainWindow>
#include <QRect>

#include "core/screenshotcontext.h"
#include "capture/capturemanager.h"
#include "services/fileexportservice.h"
#include "services/clipboardservice.h"

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

    void onScreenshotContextChanged();

private:
    Ui::MainWindow *ui;

    // Новая архитектура - полная замена ScreenshotCapturer
    ScreenshotContext m_screenshotContext;
    CaptureManager m_captureManager;
    FileExportService m_fileExportService;
    ClipboardService m_clipboardService;

    // Для захвата области
    void startAreaSelection();
    void completeAreaSelection(const QRect &area);
    void showMainWindow();

    // Временное состояние для захвата области
    QRect m_lastWindowGeometry;
    bool m_wasMaximized = false;
    QPixmap m_fullScreenShot;  // Добавляем это поле для хранения скриншота ДО показа оверлея

    void updateUI();
    void updateScreenshotDisplay();
    void updateInfoLabel();
    void updateButtonStates();

protected:
    void resizeEvent(QResizeEvent *event) override;
};
