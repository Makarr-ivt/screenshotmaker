#include "rectangleselectionoverlay.h"
#include <QPainter>
#include <QKeyEvent>
#include <QGuiApplication>

RectangleSelectionOverlay::RectangleSelectionOverlay(QWidget *parent)
    : QWidget(parent)
{
    // Настройки окна
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setMouseTracking(true);
    
    // Захватываем весь экран
    QScreen *primaryScreen = QGuiApplication::primaryScreen();
    if (primaryScreen) {
        setGeometry(primaryScreen->geometry());
    }
    
    setCursor(Qt::CrossCursor);
}

void RectangleSelectionOverlay::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Полупрозрачный темный фон
    painter.fillRect(rect(), QColor(0, 0, 0, 100));
    
    // Если идет выделение, рисуем прямоугольник
    if (m_isSelecting && !m_selectionRect.isNull()) {
        // Вырезаем выделенную область (прозрачная в центре)
        QRegion fullRegion(rect());
        QRegion selectedRegion(m_selectionRect);
        QRegion darkRegion = fullRegion.subtracted(selectedRegion);
        
        painter.setClipRegion(darkRegion);
        painter.fillRect(rect(), QColor(0, 0, 0, 150));
        painter.setClipping(false);
        
        // Рамка выделения
        painter.setPen(QPen(m_borderColor, m_borderWidth));
        painter.setBrush(m_selectionColor);
        painter.drawRect(m_selectionRect);
        
        // Размеры области
        QString sizeText = QString("%1×%2")
            .arg(m_selectionRect.width())
            .arg(m_selectionRect.height());
        
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 12, QFont::Bold));
        
        QRect textRect = m_selectionRect;
        textRect.setTop(textRect.top() - 30);
        textRect.setHeight(25);
        
        painter.drawText(textRect, Qt::AlignCenter, sizeText);
    }
}

void RectangleSelectionOverlay::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_startPoint = event->pos();  // Меняем globalPos() на pos()
        m_selectionRect = QRect(m_startPoint, QSize());
        m_isSelecting = true;
        update();
    }
}

void RectangleSelectionOverlay::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isSelecting) {
        m_selectionRect = QRect(m_startPoint, event->pos()).normalized();  // Меняем globalPos() на pos()
        updateCursor();
        update();
    }
}

void RectangleSelectionOverlay::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_isSelecting) {
        m_isSelecting = false;
        captureSelection();
    }
}

void RectangleSelectionOverlay::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        emit selectionCancelled();
        close();
    } else if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        if (!m_selectionRect.isNull()) {
            captureSelection();
        }
    }
}

void RectangleSelectionOverlay::updateCursor()
{
    if (m_selectionRect.isNull()) {
        setCursor(Qt::CrossCursor);
    } else {
        setCursor(Qt::ArrowCursor);
    }
}

void RectangleSelectionOverlay::captureSelection()
{
    if (!m_selectionRect.isNull()) {
        emit selectionCompleted(m_selectionRect.normalized());
    }
    close();
}
