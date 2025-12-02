#pragma once

#include <QPixmap>
#include <QString>

class FileExportService
{
public:
    FileExportService() = default;
    
    bool saveToFile(const QPixmap &image, const QString &filePath);
    QStringList supportedFormats() const;
    QString formatFilter() const;
    
private:
    QString getFormatFromExtension(const QString &filePath) const;
};