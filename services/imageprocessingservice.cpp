#include "imageprocessingservice.h"
#include <QPainter>

QPixmap ImageProcessingService::crop(const QPixmap &image, const QRect &rect)
{
    if (image.isNull() || !rect.isValid())
        return QPixmap();
        
    return image.copy(rect);
}

QPixmap ImageProcessingService::scale(const QPixmap &image, const QSize &size, bool keepAspectRatio)
{
    if (image.isNull())
        return QPixmap();
        
    Qt::AspectRatioMode mode = keepAspectRatio ? Qt::KeepAspectRatio : Qt::IgnoreAspectRatio;
    return image.scaled(size, mode, Qt::SmoothTransformation);
}

QPixmap ImageProcessingService::addText(const QPixmap &image, const QString &text, const QPoint &position)
{
    if (image.isNull() || text.isEmpty())
        return image;
        
    QPixmap result = image;
    QPainter painter(&result);
    painter.setPen(Qt::red);
    painter.setFont(QFont("Arial", 14));
    painter.drawText(position, text);
    
    return result;
}

QPixmap ImageProcessingService::drawLine(const QPixmap &image, const QPoint &from, const QPoint &to)
{
    if (image.isNull())
        return QPixmap();
        
    QPixmap result = image;
    QPainter painter(&result);
    painter.setPen(QPen(Qt::red, 2));
    painter.drawLine(from, to);
    
    return result;
}

QString ImageProcessingService::getImageInfo(const QPixmap &image) const
{
    if (image.isNull())
        return QString("Размер: 0×0");
        
    return QString("Размер: %1×%2").arg(image.width()).arg(image.height());
}