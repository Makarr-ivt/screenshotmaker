#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ScreenshotController;
class ScreenshotViewer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    // Эти слоты теперь делегируют работу контроллеру
    void on_captureFullScreenButton_clicked();
    void on_captureAreaButton_clicked();
    void on_saveButton_clicked();
    void on_copyToClipboardButton_clicked();

    // Сигналы от контроллера
    void onControllerImageChanged();
    void onControllerError(const QString &msg);
    void onControllerInfo(const QString &msg);

private:
    Ui::MainWindow *ui;

    ScreenshotController *m_controller;
    ScreenshotViewer *m_viewer;

    // Удобные методы UI
    void updateUIStates(); // включает/отключает кнопки в зависимости от наличия изображения
    void showTemporaryButtonFeedback(QWidget *buttonWidget, const QString &text, int msec = 1000);

protected:
    void resizeEvent(QResizeEvent *event) override;
};
