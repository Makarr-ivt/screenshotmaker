#include "screenshotcapturer.h"
#include <QGuiApplication>
#include <QScreen>

QPixmap ScreenshotCapturer::captureFullScreen()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        return screen->grabWindow(0);
    }
    return QPixmap();
}

QPixmap ScreenshotCapturer::captureArea(const QRect& area)
{
    Q_UNUSED(area);
    return QPixmap(); // TODO: реализовать позже
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

QString ScreenshotCapturer::getScreenshotInfo(const QPixmap& screenshot) const
{
    if (screenshot.isNull()) {
        return "Размер: 0×0";
    }
    return QString("Размер: %1×%2").arg(screenshot.width()).arg(screenshot.height());
}
