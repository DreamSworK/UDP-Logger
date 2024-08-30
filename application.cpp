#include "application.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(application);
    QApplication app(argc, argv);
    QApplication::setOrganizationName(ORGANIZATION);
    QApplication::setApplicationName(APPLICATION);
    QApplication::setWindowIcon(ICON("logger"));
    MainWindow udpLogger;
    udpLogger.show();
    return QApplication::exec();
}