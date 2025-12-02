#include "screenshotcapturer.h"

QPixmap ScreenshotCapturer::captureFullScreen() 
{ 
    return QPixmap(); // TODO: Реализовать
}

QPixmap ScreenshotCapturer::captureArea(const QRect& area) 
{ 
    Q_UNUSED(area); 
    return QPixmap(); // TODO: Реализовать
}

bool ScreenshotCapturer::saveToFile(const QPixmap& screenshot, const QString& filePath) 
{ 
    Q_UNUSED(screenshot); 
    Q_UNUSED(filePath); 
    return false; // TODO: Реализовать
}

void ScreenshotCapturer::copyToClipboard(const QPixmap& screenshot) 
{ 
    Q_UNUSED(screenshot); 
    // TODO: Реализовать
}

QString ScreenshotCapturer::getScreenshotInfo(const QPixmap& screenshot) 
{ 
    Q_UNUSED(screenshot); 
    return QString(); // TODO: Реализовать
}