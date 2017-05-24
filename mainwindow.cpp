#include "application.h"
#include "mainwindow.h"

MainWindow::MainWindow()
{
    udpSocket = new QUdpSocket(this);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));

    textEdit = new QPlainTextEdit;
    textEdit->setCenterOnScroll(true);
    textEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
    textEdit->setUndoRedoEnabled(false);
    textEdit->setReadOnly(true);
    setCentralWidget(textEdit);
    setUnifiedTitleAndToolBarOnMac(true);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    setWindowTitle(APPLICATION);

    settings = new SettingsWindow(this);
    resize(settings->values["mainWindowSize"].toSize());
    move(settings->values["mainWindowPosition"].toPoint());
    if (settings->values["autoStart"].toBool()) {
        start();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveChanges() ? event->accept() : event->ignore();
    settings->saveSettings();
    udpSocket->close();
}

bool MainWindow::save()
{
    QString fileName = QFileDialog::getSaveFileName(this, APPLICATION, settings->values["lastDir"].toString(),
                                                    "Logs (*.log);;Text files (*.txt);;Any files (*)");
    if (fileName.isEmpty())
        return false;

    settings->values["lastDir"] = QFileInfo(fileName).absoluteDir().absolutePath();
    return saveFile(fileName);
}

void MainWindow::createActions()
{
    saveAction = new QAction(ICON("save"), "Save", this);
    saveAction->setShortcuts(QList<QKeySequence>() << QKeySequence::Save << QKeySequence::SaveAs);
    saveAction->setStatusTip("Save the log output");
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

    exitAction = new QAction(ICON("exit"), "Exit", this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip("Exit the application");
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    copyAction = new QAction(ICON("copy"), "Copy", this);
    copyAction->setShortcuts(QKeySequence::Copy);
    copyAction->setStatusTip("Copy the current selection to the clipboard");
    copyAction->setEnabled(false);
    connect(copyAction, SIGNAL(triggered()), textEdit, SLOT(copy()));
    connect(textEdit, SIGNAL(copyAvailable(bool)), copyAction, SLOT(setEnabled(bool)));

    clearAction = new QAction(ICON("clear"), "Clear", this);
    clearAction->setShortcuts(QKeySequence::Delete);
    clearAction->setStatusTip("Clear the log window");
    connect(clearAction, SIGNAL(triggered()), textEdit, SLOT(clear()));

    startAction = new QAction(ICON("start"), "Start", this);
    startAction->setStatusTip(QString("Start ") + APPLICATION);
    connect(startAction, SIGNAL(triggered()), this, SLOT(start()));

    stopAction = new QAction(ICON("stop"), "Stop", this);
    stopAction->setStatusTip(QString("Stop ") + APPLICATION);
    stopAction->setEnabled(false);
    connect(stopAction, SIGNAL(triggered()), this, SLOT(stop()));

    settingsAction = new QAction(ICON("settings"), "Settings", this);
    settingsAction->setShortcuts(QKeySequence::Preferences);
    settingsAction->setStatusTip("Show the settings window");
    connect(settingsAction, SIGNAL(triggered()), this, SLOT(showSettings()));

    aboutAction = new QAction(ICON("about"), "About", this);
    aboutAction->setStatusTip("Show the information about the program");
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction(saveAction);
    fileMenu->addAction(settingsAction);
    fileMenu->addAction(exitAction);

    editMenu = menuBar()->addMenu("Edit");
    editMenu->addAction(copyAction);
    editMenu->addAction(clearAction);

    runMenu = menuBar()->addMenu("Run");
    runMenu->addAction(startAction);
    runMenu->addAction(stopAction);

    helpMenu = menuBar()->addMenu("Help");
    helpMenu->addAction(aboutAction);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar("File");
    fileToolBar->addAction(saveAction);

    editToolBar = addToolBar("Edit");
    editToolBar->addAction(copyAction);
    editToolBar->addAction(clearAction);

    runToolBar = addToolBar("Run");
    runToolBar->addAction(startAction);
    runToolBar->addAction(stopAction);

    configToolBar = addToolBar("Config");
    configToolBar->addAction(settingsAction);
    configToolBar->addAction(exitAction);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage("Ready");
}

bool MainWindow::saveChanges()
{
    if (textEdit->document()->isModified()) {
        QMessageBox::StandardButton button;
        button = QMessageBox::warning(this, APPLICATION, "The log output has been modified.\n" "Do you want to save changes?",
                                      QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (button == QMessageBox::Save)
            return save();
        else if (button == QMessageBox::Cancel)
            return false;
    }
    return true;
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, APPLICATION, QString("Cannot write file %1:\n%2.").arg(fileName).arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << textEdit->toPlainText();
    QApplication::restoreOverrideCursor();

    statusBar()->showMessage("File saved", 2000);
    return true;
}

void MainWindow::start() {
    udpSocket->bind((quint16) settings->values["udpPort"].toInt(), QUdpSocket::ShareAddress);
    statusBar()->showMessage(QString("%1 UDP port is listening").arg(settings->values["udpPort"].toInt()), 2000);
    setWindowTitle(QString("%1 is running").arg(APPLICATION));
    startAction->setEnabled(false);
    stopAction->setEnabled(true);
}

void MainWindow::stop() {
    udpSocket->close();
    statusBar()->showMessage(QString("%1 is stopped").arg(APPLICATION), 2000);
    startAction->setEnabled(true);
    stopAction->setEnabled(false);
    setWindowTitle(APPLICATION);
}

void MainWindow::processPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize((int) udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
        QString message = datagram.data();
        textEdit->appendPlainText(message);
    }
}

void MainWindow::showSettings()
{
    settings->show();
}

void MainWindow::about() {
    QMessageBox::about(this, "About",
                       QString("Application: %1\nAuthor: %2\nVersion: %3")
                               .arg(APPLICATION).arg(ORGANIZATION).arg(VERSION));
}