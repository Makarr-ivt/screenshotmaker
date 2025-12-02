#include "selectionvisualizer.h"
#include <QPainter>

SelectionVisualizer::SelectionVisualizer(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_TranslucentBackground);
}

void SelectionVisualizer::setSelectionRect(const QRect &rect)
{
    m_selectionRect = rect;
    update();
}

void SelectionVisualizer::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    if (m_selectionRect.isNull()) {
        return;
    }
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Заливка
    painter.setBrush(m_selectionColor);
    painter.setPen(Qt::NoPen);
    painter.drawRect(m_selectionRect);
    
    // Рамка
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(m_borderColor, 2));
    painter.drawRect(m_selectionRect);
    
    // Размеры
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 10));
    QString sizeText = QString("%1 × %2")
        .arg(m_selectionRect.width())
        .arg(m_selectionRect.height());
    
    QRect textRect = m_selectionRect;
    textRect.setTop(textRect.top() - 25);
    textRect.setHeight(20);
    
    painter.drawText(textRect, Qt::AlignCenter, sizeText);
}