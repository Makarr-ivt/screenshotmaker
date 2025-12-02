#include "screenshotcapturer.h"
#include <QGuiApplication>
#include <QScreen>
#include <QFileInfo>
#include <QClipboard>

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

    static QMap<QString, const char*> formatMap = {
        {"jpg", "JPG"},
        {"jpeg", "JPG"},
        {"png", "PNG"},
        {"bmp", "BMP"}
    };

    QString suffix = QFileInfo(filePath).suffix().toLower();
    const char* format = formatMap.value(suffix, "PNG");

    return screenshot.save(filePath, format);
}

void ScreenshotCapturer::copyToClipboard(const QPixmap& screenshot)
{
    if (screenshot.isNull()) {
        return;
    }

    // Получаем глобальный буфер обмена
    QClipboard *clipboard = QGuiApplication::clipboard();

    // Копируем изображение в буфер обмена
    clipboard->setPixmap(screenshot);
}

QString ScreenshotCapturer::getScreenshotInfo(const QPixmap& screenshot) const
{
    if (screenshot.isNull()) {
        return "Размер: 0×0";
    }
    return QString("Размер: %1×%2").arg(screenshot.width()).arg(screenshot.height());
}
