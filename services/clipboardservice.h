#pragma once

#include <QPixmap>
#include <QObject>

class ClipboardService : public QObject
{
    Q_OBJECT

public:
    explicit ClipboardService(QObject *parent = nullptr);
    
    void copyToClipboard(const QPixmap &image);
    bool hasImage() const;
    QPixmap pasteFromClipboard() const;
    
signals:
    void clipboardChanged();
};