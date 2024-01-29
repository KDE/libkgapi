/*
    SPDX-FileCopyrightText: 2012 Jan Grulich <jgrulich@redhat.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#pragma once

#include <QMainWindow>

#include "libkgapi2/staticmaps/staticmapmarker.h"
#include "libkgapi2/staticmaps/staticmaptilefetchjob.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

public Q_SLOTS:
    void getImage();
    void addMarker();
    void slotTileFetched(KGAPI2::Job *);

private:
    Ui::MainWindow *m_ui;

    QList<KGAPI2::StaticMapMarker> m_markers;
};
