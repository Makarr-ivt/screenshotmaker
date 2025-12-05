#include "screenshotviewer.h"
#include <QLabel>
#include <QPixmap>

ScreenshotViewer::ScreenshotViewer(QLabel *label, QObject *parent)
    : QObject(parent),
    m_label(label)
{
}

void ScreenshotViewer::setImage(const QPixmap &pix)
{
    m_pixmap = pix;
    if (!m_label) return;

    if (m_pixmap.isNull()) {
        m_label->clear();
        m_label->setText("Нет изображения");
        m_label->setAlignment(Qt::AlignCenter);
        return;
    }

    QSize labelSize = m_label->size();
    QPixmap scaled = m_pixmap.scaled(labelSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_label->setPixmap(scaled);
    m_label->setText("");
}

QPixmap ScreenshotViewer::image() const
{
    return m_pixmap;
}

bool ScreenshotViewer::hasImage() const
{
    return !m_pixmap.isNull();
}

void ScreenshotViewer::clear()
{
    m_pixmap = QPixmap();
    if (m_label) {
        m_label->clear();
        m_label->setText("Нет изображения");
        m_label->setAlignment(Qt::AlignCenter);
    }
}
