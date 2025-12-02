#include "screenshotcapturer.h"
#include <QGuiApplication>
#include <QScreen>
#include <QFileInfo>

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
    if (screenshot.isNull() || filePath.isEmpty()) {
        return false;
    }

    // Определяем формат по расширению файла
    QString format = "PNG"; // формат по умолчанию
    QString suffix = QFileInfo(filePath).suffix().toLower();

    if (suffix == "jpg" || suffix == "jpeg") {
        format = "JPG";
    } else if (suffix == "bmp") {
        format = "BMP";
    }
    // Пытаемся сохранить изображение
    bool success = screenshot.save(filePath, format.toUtf8().constData());
    return success;
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
