#include "fullscreencapturestrategy.h"
#include <QGuiApplication>
#include <QScreen>

QPixmap FullScreenCaptureStrategy::capture()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        return screen->grabWindow(0);
    }
    return QPixmap();
}