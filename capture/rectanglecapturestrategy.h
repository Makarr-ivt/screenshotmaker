#pragma once

#include "icapturestrategy.h"
#include <QRect>

class RectangleCaptureStrategy : public ICaptureStrategy
{
public:
    RectangleCaptureStrategy(const QRect &area);
    QPixmap capture() override;
    QString name() const override { return "Прямоугольная область"; }
    
private:
    QRect m_area;
};