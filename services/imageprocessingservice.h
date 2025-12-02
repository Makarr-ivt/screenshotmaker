#pragma once

#include <QPixmap>
#include <QRect>

class ImageProcessingService
{
public:
    ImageProcessingService() = default;
    
    QPixmap crop(const QPixmap &image, const QRect &rect);
    QPixmap scale(const QPixmap &image, const QSize &size, bool keepAspectRatio = true);
    QPixmap addText(const QPixmap &image, const QString &text, const QPoint &position);
    QPixmap drawLine(const QPixmap &image, const QPoint &from, const QPoint &to);
    
    QString getImageInfo(const QPixmap &image) const;
};