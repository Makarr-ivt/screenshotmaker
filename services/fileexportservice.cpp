#include "fileexportservice.h"
#include <QFileInfo>
#include <QMap>

bool FileExportService::saveToFile(const QPixmap &image, const QString &filePath)
{
    if (image.isNull() || filePath.isEmpty())
        return false;

    QString format = getFormatFromExtension(filePath);
    return image.save(filePath, format.toUtf8().constData());
}

QStringList FileExportService::supportedFormats() const
{
    return {"png", "jpg", "jpeg", "bmp"};
}

QString FileExportService::formatFilter() const
{
    QStringList filters;
    filters << "PNG (*.png)";
    filters << "JPEG (*.jpg *.jpeg)";
    filters << "BMP (*.bmp)";
    filters << "All Files (*.*)";
    
    return filters.join(";;");
}

QString FileExportService::getFormatFromExtension(const QString &filePath) const
{
    QFileInfo info(filePath);
    QString extension = info.suffix().toLower();
    
    static const QMap<QString, QString> formatMap = {
        {"jpg", "JPG"},
        {"jpeg", "JPG"},
        {"png", "PNG"},
        {"bmp", "BMP"}
    };
    
    return formatMap.value(extension, "PNG");
}