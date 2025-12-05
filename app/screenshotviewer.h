#pragma once

#include <QObject>
#include <QPixmap>

class QLabel;

class ScreenshotViewer : public QObject
{
    Q_OBJECT
public:
    explicit ScreenshotViewer(QLabel *label, QObject *parent = nullptr);

    void setImage(const QPixmap &pix);
    QPixmap image() const;
    bool hasImage() const;

    void clear();

private:
    QLabel *m_label;
    QPixmap m_pixmap;
};
