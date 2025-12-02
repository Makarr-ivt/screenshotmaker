#include "screenshotcontext.h"
#include "editcommand.h"

ScreenshotContext::ScreenshotContext(QObject *parent)
    : QObject(parent)
{
}

ScreenshotContext::~ScreenshotContext()
{
    clearHistory();
}

void ScreenshotContext::setImage(const QPixmap &image)
{
    if (m_image.cacheKey() == image.cacheKey())
        return;

    m_image = image;
    clearHistory();
    emit imageChanged(m_image);
}

const QPixmap &ScreenshotContext::image() const
{
    return m_image;
}

QPixmap &ScreenshotContext::imageRef()
{
    return m_image;
}

bool ScreenshotContext::hasImage() const
{
    return !m_image.isNull();
}

void ScreenshotContext::applyEdit(std::unique_ptr<EditCommand> command)
{
    if (!command)
        return;

    command->execute(m_image);
    m_undoStack.push(std::move(command));
    m_redoStack = std::stack<std::unique_ptr<EditCommand>>();

    emit canUndoChanged(true);
    emit canRedoChanged(false);
    emit imageChanged(m_image);
}

bool ScreenshotContext::canUndo() const
{
    return !m_undoStack.empty();
}

bool ScreenshotContext::canRedo() const
{
    return !m_redoStack.empty();
}

void ScreenshotContext::undo()
{
    if (m_undoStack.empty())
        return;

    auto command = std::move(m_undoStack.top());
    m_undoStack.pop();
    command->undo(m_image);
    m_redoStack.push(std::move(command));

    emit canUndoChanged(!m_undoStack.empty());
    emit canRedoChanged(true);
    emit imageChanged(m_image);
}

void ScreenshotContext::redo()
{
    if (m_redoStack.empty())
        return;

    auto command = std::move(m_redoStack.top());
    m_redoStack.pop();
    command->execute(m_image);
    m_undoStack.push(std::move(command));

    emit canUndoChanged(true);
    emit canRedoChanged(!m_redoStack.empty());
    emit imageChanged(m_image);
}

void ScreenshotContext::clearHistory()
{
    while (!m_undoStack.empty())
        m_undoStack.pop();
    while (!m_redoStack.empty())
        m_redoStack.pop();

    emit canUndoChanged(false);
    emit canRedoChanged(false);
}