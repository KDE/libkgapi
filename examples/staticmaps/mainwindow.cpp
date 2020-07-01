/*
    SPDX-FileCopyrightText: 2012 Jan Grulich <jgrulich@redhat.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/


#include "mainwindow.h"
#include "ui_main.h"

#include <libkgapi2/staticmaps/staticmapurl.h>

using namespace KGAPI2;

MainWindow::MainWindow(QWidget * parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);

    connect(m_ui->get, SIGNAL(clicked(bool)),
            this, SLOT(getImage()));
    connect(m_ui->addMarker, SIGNAL(clicked(bool)),
            this, SLOT(addMarker()));
}

MainWindow::~MainWindow()
{ }

void MainWindow::addMarker()
{
    StaticMapMarker marker;

    if (!m_ui->markerLocation->text().isEmpty()) {

        if (!m_ui->markerLabel->text().isEmpty())
            marker.setLabel(QChar(m_ui->markerLabel->text().at(0)));

        marker.setSize((StaticMapMarker::MarkerSize) m_ui->markerSize->currentIndex());
        marker.setColor(m_ui->markerColor->color());
        marker.setLocation(m_ui->markerLocation->text());

        m_ui->markers->addItem(marker.toString());
        m_markers << marker;
    }
}

void MainWindow::getImage()
{
    StaticMapUrl map;
    KContacts::Address addr;

    switch (m_ui->locationType->currentIndex()) {
	case StaticMapUrl::String:
	    map.setLocation(m_ui->locationString->text());
	    break;
	case StaticMapUrl::KABCAddress:
	    addr.setLocality(m_ui->locationCity->text());
	    addr.setStreet(m_ui->locationStreet->text());
	    map.setLocation(addr);
	    break;
	case StaticMapUrl::KABCGeo:
	    map.setLocation(KContacts::Geo(m_ui->locationLatitude->value(),
				      m_ui->locationLongitude->value()));
	    break;
    }

    map.setZoomLevel(m_ui->zoom->value());
    map.setSize(QSize(m_ui->width->value(), m_ui->height->value()));
    map.setScale(static_cast<StaticMapUrl::Scale>(m_ui->scale->currentIndex() + 1));
    map.setFormat(static_cast<StaticMapUrl::ImageFormat>(m_ui->format->currentIndex()));
    map.setMapType(static_cast<StaticMapUrl::MapType>(m_ui->map->currentIndex()));
    map.setSensorUsed(m_ui->sensor->currentIndex());
    map.setMarkers(m_markers);

    StaticMapTileFetchJob *fetchJob = new StaticMapTileFetchJob(map.url(), this);
    connect(fetchJob, SIGNAL(finished(KGAPI2::Job*)),
            this, SLOT(slotTileFetched(KGAPI2::Job*)));
}

void MainWindow::slotTileFetched(KGAPI2::Job *job)
{
    StaticMapTileFetchJob *fetchJob = qobject_cast<StaticMapTileFetchJob*>(job);
    Q_ASSERT(fetchJob);
    fetchJob->deleteLater();

    const QPixmap pixmap = fetchJob->tilePixmap();
    m_ui->label->setPixmap(pixmap);
}
