#pragma once

#include <QObject>
#include <QPixmap>
#include <memory>

#include "icapturestrategy.h"

class QWidget;
class RectangleSelectionOverlay;

class ScreenshotController : public QObject
{
    Q_OBJECT
public:
    explicit ScreenshotController(QObject *parent = nullptr);

    // Захват
    void captureFullScreen(QWidget *parentWidget);
    void startAreaSelection(QWidget *parentWidget);

    // Работа с изображением
    bool hasImage() const;
    QPixmap currentImage() const;

    // Экспорт
    bool saveImageToFile(QWidget *parentWidget);
    void copyImageToClipboard();

signals:
    void imageChanged();
    void infoMessage(const QString &message);
    void errorOccured(const QString &message);

private:
    void setImage(const QPixmap &pixmap);
    void onAreaSelected(const QRect &rect);

    QString fileDialogFilter() const;
    QString formatFromExtension(const QString &filePath) const;

private:
    QPixmap m_currentImage;
    std::unique_ptr<ICaptureStrategy> m_fullScreenStrategy;
    RectangleSelectionOverlay *m_overlay = nullptr;
};
