/*
    SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/


#pragma once

#include <QMainWindow>

#include <types.h>
#include <drive/permission.h>

namespace Ui {
    class MainWindow;
}

namespace KGAPI2 {
  class Job;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

  private Q_SLOTS:
    /**
     * Retrieves tokens from Google that we will use to authenticate
     * further requests
     */
    void authenticate();

    /**
     * Authentication has finished
     */
    void slotAuthJobFinished(KGAPI2::Job *job);

    /**
     * Retrieves list of all permissions for file id
     */
    void slotFetchPermissions();

    /**
     * Permissions listing was fetched.
     */
    void slotPermissionsFetchJobFinished(KGAPI2::Job *job);

    /**
     * A specific permission in permissions list has been selected. Sends a request
     * to Google to retrieve the permission details
     */
    void permissionSelected();

    /**
     * Permissions listing was fetched.
     */
    void slotPermissionFetchJobFinished(KGAPI2::Job *job);

  private:
    Ui::MainWindow *m_ui;

    KGAPI2::AccountPtr m_account;

    QString roleToName(KGAPI2::Drive::Permission::Role role);
    QString typeToName(KGAPI2::Drive::Permission::Type type);
    QString permissionTypeToName(KGAPI2::Drive::Permission::PermissionDetails::PermissionType permissionType);
};

