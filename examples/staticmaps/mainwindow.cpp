/*
    Copyright (C) 2012  Jan Grulich <jgrulich@redhat.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#include "mainwindow.h"
#include "ui_main.h"
#include <libkgapi/objects/staticmapurl.h>
#include <QDebug>

MainWindow::MainWindow(QWidget * parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow),
    m_map(new KGAPI::Services::StaticMap(this))
{
    m_ui->setupUi(this);

    connect(m_ui->get, SIGNAL(clicked(bool)),
            this, SLOT(getImage()));
    connect(m_ui->addMarker, SIGNAL(clicked(bool)),
            this, SLOT(addMarker()));
    connect(m_map, SIGNAL(tileFetched(QPixmap)),
            this, SLOT(showImage(QPixmap)));
}

MainWindow::~MainWindow()
{ }

void MainWindow::addMarker()
{
    KGAPI::Objects::StaticMapMarker marker;

    if (!m_ui->markerLocation->text().isEmpty()) {

        if (!m_ui->markerLabel->text().isEmpty())
            marker.setLabel(QChar(m_ui->markerLabel->text().at(0)));

        marker.setSize((KGAPI::Objects::StaticMapMarker::MarkerSize)m_ui->markerSize->currentIndex());
        marker.setColor(m_ui->markerColor->color());
        marker.setLocation(m_ui->markerLocation->text());

        m_ui->markers->addItem(marker.toString());
        m_markers << marker;
    }
}

void MainWindow::getImage()
{
    KGAPI::Objects::StaticMapUrl map;
    KABC::Address addr;

    switch (m_ui->locationType->currentIndex()) {
	case KGAPI::Objects::StaticMapUrl::String:
	    map.setLocation(m_ui->locationString->text());
	    break;
	case KGAPI::Objects::StaticMapUrl::KABCAddress:
	    addr.setLocality(m_ui->locationCity->text());
	    addr.setStreet(m_ui->locationStreet->text());
	    map.setLocation(addr);
	    break;
	case KGAPI::Objects::StaticMapUrl::KABCGeo:
	    map.setLocation(KABC::Geo(m_ui->locationLatitude->value(),
				      m_ui->locationLongitude->value()));
	    break;
    }

    map.setZoomLevel(m_ui->zoom->value());
    map.setSize(QSize(m_ui->width->value(), m_ui->height->value()));
    map.setScale(KGAPI::Objects::StaticMapUrl::Scale(m_ui->scale->currentIndex()+1));
    map.setFormat((KGAPI::Objects::StaticMapUrl::ImageFormat)(m_ui->format->currentIndex()));
    map.setMapType((KGAPI::Objects::StaticMapUrl::MapType)(m_ui->map->currentIndex()));
    map.setSensorUsed(m_ui->sensor->currentIndex());
    map.setMarkers(m_markers);

    qDebug() << "Size: " << map.size();
    qDebug() << "Format: " << map.format();
    qDebug() << "Scale: " << map.scale();
    qDebug() << "Zoom: " << map.zoomLevel();
    qDebug() << "Sensor: " << map.sensorUsed();
    qDebug() << "Map type: " << map.mapType();
    qDebug() << "Valid: " << map.isValid();
    qDebug() << "Url: " << map.url();

    m_map->fetchTile(map.url());
}

void MainWindow::showImage(const QPixmap & pixmap)
{
    m_ui->label->setPixmap(pixmap);
}
