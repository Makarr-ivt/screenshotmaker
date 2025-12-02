#pragma once

#include <QPixmap>
#include <memory>

class EditCommand
{
public:
    virtual ~EditCommand() = default;
    
    virtual void execute(QPixmap &image) = 0;
    virtual void undo(QPixmap &image) = 0;
    virtual QString description() const = 0;
    
protected:
    QPixmap m_originalState;
    QPixmap m_modifiedState;
};