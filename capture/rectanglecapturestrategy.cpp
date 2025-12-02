#include "rectanglecapturestrategy.h"
#include <QGuiApplication>
#include <QScreen>

RectangleCaptureStrategy::RectangleCaptureStrategy(const QRect &area)
    : m_area(area)
{
}

QPixmap RectangleCaptureStrategy::capture()
{
        // Эта стратегия больше не используется напрямую
        // Вместо нее используем вырезание из m_fullScreenShot
        return QPixmap();
}
