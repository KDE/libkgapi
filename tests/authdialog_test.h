#include <QtCore/QObject>
#include <QtCore/QString>

class Auth: public QObject 
{
  Q_OBJECT
  public slots:
    void authorized();
};