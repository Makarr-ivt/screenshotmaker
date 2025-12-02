#include "clipboardservice.h"
#include <QGuiApplication>
#include <QClipboard>
#include <QMimeData>

ClipboardService::ClipboardService(QObject *parent)
    : QObject(parent)
{
    connect(QGuiApplication::clipboard(), &QClipboard::dataChanged,
            this, &ClipboardService::clipboardChanged);
}

void ClipboardService::copyToClipboard(const QPixmap &image)
{
    if (image.isNull())
        return;
        
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setPixmap(image);
}

bool ClipboardService::hasImage() const
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    return clipboard->mimeData()->hasImage();
}

QPixmap ClipboardService::pasteFromClipboard() const
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    return clipboard->pixmap();
}
