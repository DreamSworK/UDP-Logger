#include "application.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(application);
    QApplication app(argc, argv);
    app.setOrganizationName(ORGANIZATION);
    app.setApplicationName(APPLICATION);
    app.setWindowIcon(ICON("logger"));
    MainWindow udpLogger;
    udpLogger.show();
    return app.exec();
}