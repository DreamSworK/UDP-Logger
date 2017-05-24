#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QtNetwork>
#include <QtGui>

#define ORGANIZATION "DreamSworK"
#define APPLICATION "UDP Logger"
#define VERSION "0.0.5"

#define ICON(name) QIcon(":/images/"name".png")

#if defined(_MSC_VER)
    #pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#endif