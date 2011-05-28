#include "authdialog_test.h"
#include "libkgoogle/authdialog.h"

#include <KDE/KApplication>
#include <KDE/KAboutData>
#include <KDE/KCmdLineArgs>

#include <QtCore/QObject>
#include <QtCore/QDebug>
#include <QtCore/QString>


AuthDialog *dialog;

void Auth::authorized()
{
  QString access_token = dialog->accessToken();
  QString refresh_token = dialog->refreshToken();
  
  qDebug() << "Access token:" << access_token;
  qDebug() << "Refresh token:" << refresh_token;
    
  dialog->close();
  delete dialog;
}


int main (int argc, char **argv)
{
    KAboutData about(argv[0], 0,
		     ki18n("AuthDialog Test"), "1.0");
    KCmdLineArgs::init(argc, argv, &about);
    
    KApplication app;
    
    Auth *auth = new Auth();
   
    dialog = new AuthDialog(0, 0);
    QObject::connect (dialog, SIGNAL(finished()),
		      auth, SLOT (authorized()));
    dialog->setScopes(QStringList() << "https://www.google.com/calendar/feeds");
    dialog->show();
    dialog->auth();
    
    return app.exec();
}

