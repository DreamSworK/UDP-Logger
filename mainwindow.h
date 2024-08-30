#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "settings.h"
#include "TextEdit.h"

QT_BEGIN_NAMESPACE
class QMenu;
class QAction;
class QUdpSocket;
class QPlainTextEdit;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    SettingsWindow *settings;

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    bool save();
    void start();
    void stop();
    void about();
    void showSettings() const;
    void processPendingDatagrams() const;

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar() const;
    bool saveChanges();
    bool saveFile(const QString &fileName);

    QUdpSocket *udpSocket;
    TextEdit *textEdit;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *runMenu;
    QMenu *helpMenu;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QToolBar *runToolBar;
    QToolBar *configToolBar;

public:
    QAction *saveAction;
    QAction *exitAction;
    QAction *copyAction;
    QAction *clearAction;
    QAction *startAction;
    QAction *stopAction;
    QAction *settingsAction;
    QAction *aboutAction;
};

#endif