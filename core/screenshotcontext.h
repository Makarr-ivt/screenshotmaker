#pragma once

#include <QPixmap>
#include <QObject>
#include <memory>
#include <stack>

class EditCommand;

class ScreenshotContext : public QObject
{
    Q_OBJECT

public:
    explicit ScreenshotContext(QObject *parent = nullptr);
    ~ScreenshotContext();

    void setImage(const QPixmap &image);
    const QPixmap& image() const;
    QPixmap& imageRef();
    bool hasImage() const;

    void applyEdit(std::unique_ptr<EditCommand> command);
    bool canUndo() const;
    bool canRedo() const;
    void undo();
    void redo();
    void clearHistory();

signals:
    void imageChanged(const QPixmap &newImage);
    void canUndoChanged(bool canUndo);
    void canRedoChanged(bool canRedo);

private:
    QPixmap m_image;
    std::stack<std::unique_ptr<EditCommand>> m_undoStack;
    std::stack<std::unique_ptr<EditCommand>> m_redoStack;
};