#include "TextEdit.h"

TextEdit::TextEdit(QWidget *parent) : QPlainTextEdit(parent)
{
    setCenterOnScroll(true);
    setLineWrapMode(QPlainTextEdit::NoWrap);
    setUndoRedoEnabled(false);
    setReadOnly(true);
}

void TextEdit::addContextMenu(QAction *action) {
    contextMenuActions.append(action);
}

void TextEdit::contextMenuEvent(QContextMenuEvent *event) {
    QMenu *menu = createStandardContextMenu();
    if (contextMenuActions.length()) {
        menu->addSeparator();
        foreach(QAction* action, contextMenuActions) {
            menu->addAction(action);
        }
    }
    menu->exec(event->globalPos());
    delete menu;
}