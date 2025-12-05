#pragma once

#include <QObject>
#include <QPixmap>
#include <memory>
#include <QWidget>

#include "capturemanager.h"
#include "fileexportservice.h"
#include "clipboardservice.h"

class ScreenshotController : public QObject
{
    Q_OBJECT
public:
    explicit ScreenshotController(QObject *parent = nullptr);
    ~ScreenshotController() override;

    // Основные операции
    void captureFullScreen(QWidget *windowToHide = nullptr);
    void startAreaSelection(QWidget *windowToHide = nullptr);
    bool saveImageToFile(QWidget *parent = nullptr); // открывает диалог сохранения
    void copyImageToClipboard();

    QPixmap currentImage() const;
    bool hasImage() const;

signals:
    void imageChanged();
    void errorOccured(const QString &message);
    void infoMessage(const QString &message);

private:
    // внутренние помощники
    void setImage(const QPixmap &pix);

    // зависимости (встроены, как в старом MainWindow)
    // используем оригинальные классы, уже в проекте
    CaptureManager m_captureManager;
    FileExportService m_fileExportService;
    ClipboardService m_clipboardService;

    // локальное хранилище текущего изображения
    QPixmap m_currentImage;

    // для выбора области (временное хранение полного скриншота)
    QPixmap m_fullScreenShot;
};
