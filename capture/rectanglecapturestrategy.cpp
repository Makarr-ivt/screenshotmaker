#include "rectanglecapturestrategy.h"
#include <QGuiApplication>
#include <QScreen>

RectangleCaptureStrategy::RectangleCaptureStrategy(const QRect &area)
    : m_area(area)
{
}

QPixmap RectangleCaptureStrategy::capture()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen && m_area.isValid()) {
        QPixmap fullScreen = screen->grabWindow(0);
        return fullScreen.copy(m_area);
    }
    return QPixmap();
}