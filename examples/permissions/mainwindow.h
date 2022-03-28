/*
    SPDX-FileCopyrightText: 2019 David Barchiesi <david@barchie.si>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#pragma once

#include "ui_mainwindow.h"
#include <QMainWindow>

#include "core/types.h"
#include "drive/permission.h"

namespace KGAPI2
{
class Job;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private Q_SLOTS:
    /**
     * Retrieves tokens from Google that we will use to authenticate
     * further requests
     */
    void authenticate();

    /**
     * Retrieves list of all permissions for file id
     */
    void slotFetchPermissions();

    /**
     * A specific permission in permissions list has been selected. Sends a request
     * to Google to retrieve the permission details
     */
    void permissionSelected();

private:
    Ui::MainWindow ui;

    KGAPI2::AccountPtr m_account;

    static QString roleToName(KGAPI2::Drive::Permission::Role role);
    static QString typeToName(KGAPI2::Drive::Permission::Type type);
    static QString permissionTypeToName(KGAPI2::Drive::Permission::PermissionDetails::PermissionType permissionType);
};
