#pragma once

#include <QPixmap>

class ICaptureStrategy
{
public:
    virtual ~ICaptureStrategy() = default;
    virtual QPixmap capture() = 0;
    virtual QString name() const = 0;
};