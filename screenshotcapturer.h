#pragma once

#include <QPixmap>
#include <QRect>

class ScreenshotCapturer
{
public:
    ScreenshotCapturer() = default;
    
    QPixmap captureFullScreen();
    QPixmap captureArea(const QRect& area);
    bool saveToFile(const QPixmap& screenshot, const QString& filePath);
    void copyToClipboard(const QPixmap& screenshot);
    QString getScreenshotInfo(const QPixmap& screenshot) const;
};
