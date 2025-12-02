#pragma once

#include "icapturestrategy.h"

class FullScreenCaptureStrategy : public ICaptureStrategy
{
public:
    QPixmap capture() override;
    QString name() const override { return "Полный экран"; }
};