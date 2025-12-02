#pragma once

#include "capture/capturemanager.h"
#include "services/fileexportservice.h"
#include "services/clipboardservice.h"
#include "services/imageprocessingservice.h"

class ScreenshotCapturerAdapter
{
public:
    ScreenshotCapturerAdapter();
    
    QPixmap captureFullScreen();
    QPixmap captureArea(const QRect& area);
    bool saveToFile(const QPixmap& screenshot, const QString& filePath);
    void copyToClipboard(const QPixmap& screenshot);
    QString getScreenshotInfo(const QPixmap& screenshot) const;
    
private:
    CaptureManager m_captureManager;
    FileExportService m_fileExportService;
    ClipboardService m_clipboardService;
    ImageProcessingService m_imageProcessingService;
};