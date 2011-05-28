/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Dan Vratil <dan@progdan.cz>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef AUTHDIALOG_H
#define AUTHDIALOG_H

#include <QtCore/QUrl>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QProgressBar>
#include <QtGui/QLabel>
#include <QtNetwork/QNetworkReply>

#include <Akonadi/ResourceBase>

#include <KDE/KDialog>
#include <KDE/KWebView>
#include <KDE/KPushButton>
#include <kdemacros.h>

class KDE_EXPORT AuthDialog: public KDialog
{
  Q_OBJECT
  public:
    AuthDialog(Akonadi::ResourceBase *parent, WId windowId);
    virtual ~AuthDialog();

    void setScopes (const QStringList &scopes);
    void auth (const QString &m_clientId, const QString &m_clientSecret);
    
    QString accessToken() { return m_accessToken; }
    QString refreshToken() { return m_refreshToken; }
    
  signals:
    void finished();
    
  private slots:
    void webviewUrlChanged(const QUrl &url);
    void webviewFinished();
    
    void networkRequestFinished(QNetworkReply *reply);
    
  private:
    void setError(const QString &error);
    
    /* GUI */
    QWidget *m_widget;
    QProgressBar *m_progressbar;
    QHBoxLayout *m_hbox;
    QVBoxLayout *m_vbox;
    KWebView *m_webiew;
    QLabel *m_label;
    
    QStringList m_scopes;
    QString m_clientId;
    QString m_clientSecret;    
    QString m_accessToken;
    QString m_refreshToken;
};

#endif // AUTHDIALOG_H
