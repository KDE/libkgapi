#include "libkgoogle/authdialog.h"

#include <KDE/KApplication>
#include <KDE/KAboutData>
#include <KDE/KCmdLineArgs>

#include <QtCore/QObject>
#include <QtCore/QDebug>
#include <QtCore/QString>

int main (int argc, char **argv)
{
    KAboutData about(argv[0], 0,
		     ki18n("AuthDialog Test"), "1.0");
    KCmdLineArgs::init(argc, argv, &about);
    
    KApplication app;
    
    AuthDialog dialog(0, 0);
    dialog.setScopes(QStringList() << "https://www.google.com/calendar/feeds");
    dialog.auth("554041944266.apps.googleusercontent.com", "mdT1DjzohxN3npUUzkENT0gO");
    dialog.exec();
    
    return app.exec();
}

