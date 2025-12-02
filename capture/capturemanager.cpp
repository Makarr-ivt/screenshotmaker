#include "capturemanager.h"

CaptureManager::CaptureManager()
{
}

void CaptureManager::setStrategy(std::unique_ptr<ICaptureStrategy> strategy)
{
    m_strategy = std::move(strategy);
}

QPixmap CaptureManager::capture()
{
    if (m_strategy) {
        return m_strategy->capture();
    }
    return QPixmap();
}

QString CaptureManager::currentStrategyName() const
{
    if (m_strategy) {
        return m_strategy->name();
    }
    return "Не выбрана";
}