#ifndef TEXT_EDIT_H
#define TEXT_EDIT_H

#include <QPlainTextEdit>
#include <QMenu>

class TextEdit : public QPlainTextEdit
{
    Q_OBJECT

public:
    TextEdit(QWidget *parent);
    void addContextMenu(QAction *action);

protected:
    void contextMenuEvent(QContextMenuEvent *event);

private:
    QList<QAction*> contextMenuActions;
};

#endif
