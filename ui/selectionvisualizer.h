#pragma once

#include <QWidget>
#include <QPainter>

class SelectionVisualizer : public QWidget
{
    Q_OBJECT

public:
    explicit SelectionVisualizer(QWidget *parent = nullptr);
    
    void setSelectionRect(const QRect &rect);
    QRect selectionRect() const { return m_selectionRect; }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QRect m_selectionRect;
    const QColor m_selectionColor = QColor(0, 120, 215, 50);
    const QColor m_borderColor = QColor(0, 120, 215);
};