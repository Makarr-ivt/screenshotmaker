#include "screenshotcapturer_adapter.h"
#include "capture/fullscreencapturestrategy.h"
#include "capture/rectanglecapturestrategy.h"

ScreenshotCapturerAdapter::ScreenshotCapturerAdapter()
{
}

QPixmap ScreenshotCapturerAdapter::captureFullScreen()
{
    m_captureManager.setStrategy(std::make_unique<FullScreenCaptureStrategy>());
    return m_captureManager.capture();
}

QPixmap ScreenshotCapturerAdapter::captureArea(const QRect& area)
{
    m_captureManager.setStrategy(std::make_unique<RectangleCaptureStrategy>(area));
    return m_captureManager.capture();
}

bool ScreenshotCapturerAdapter::saveToFile(const QPixmap& screenshot, const QString& filePath)
{
    return m_fileExportService.saveToFile(screenshot, filePath);
}

void ScreenshotCapturerAdapter::copyToClipboard(const QPixmap& screenshot)
{
    m_clipboardService.copyToClipboard(screenshot);
}

QString ScreenshotCapturerAdapter::getScreenshotInfo(const QPixmap& screenshot) const
{
    return m_imageProcessingService.getImageInfo(screenshot);
}