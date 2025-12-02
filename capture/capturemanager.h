#pragma once

#include "icapturestrategy.h"
#include <memory>

class CaptureManager
{
public:
    CaptureManager();
    
    void setStrategy(std::unique_ptr<ICaptureStrategy> strategy);
    QPixmap capture();
    QString currentStrategyName() const;
    
private:
    std::unique_ptr<ICaptureStrategy> m_strategy;
};