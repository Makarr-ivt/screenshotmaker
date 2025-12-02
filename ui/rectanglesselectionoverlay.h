#pragma once

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>
#include <QScreen>

class RectangleSelectionOverlay : public QWidget
{
    Q_OBJECT

public:
    explicit RectangleSelectionOverlay(QWidget *parent = nullptr);
    ~RectangleSelectionOverlay() override = default;

    QRect selectedRect() const { return m_selectionRect.normalized(); }

signals:
    void selectionCompleted(const QRect &rect);
    void selectionCancelled();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    void updateCursor();
    void captureSelection();

    QRect m_selectionRect;
    QPoint m_startPoint;
    bool m_isSelecting = false;
    
    const QColor m_selectionColor = QColor(0, 120, 215, 100);
    const QColor m_borderColor = QColor(0, 120, 215);
    const int m_borderWidth = 2;
};