/*
    Akonadi Google - Calendar Resource
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


#include "calendareditor.h"
#include "ui_calendar_editor.h"

#include <libkgoogle/services/calendar.h>

#include <kstandarddirs.h>
#include <ktimezone.h>
#include <qfile.h>

using namespace KGoogle::Objects;

CalendarEditor::CalendarEditor(Calendar *calendar) :
  QDialog(),
  m_calendar(calendar)
{
  m_ui = new Ui::CalendarEditor();
  m_ui->setupUi(this);

  initTimezones();
  initColors();

  if (calendar)
    initWidgets();

  connect(m_ui->buttons, SIGNAL(accepted()),
          this, SLOT(accepted()));
}

CalendarEditor::~CalendarEditor()
{
  delete m_ui;
}

void CalendarEditor::accepted()
{
  if (!m_calendar)
    m_calendar = new KGoogle::Objects::Calendar();

  m_calendar->setTitle(m_ui->nameEdit->text());
  m_calendar->setDetails(m_ui->descriptionEdit->toPlainText());
  m_calendar->setLocation(m_ui->locationEdit->text());
  m_calendar->setTimezone(m_ui->timezoneCombo->currentText());
  m_calendar->setColor(m_ui->colorCombo->color().name());

  emit accepted(m_calendar);
}


void CalendarEditor::initTimezones()
{
  KTimeZones timeZones;

  foreach(const KTimeZone tz, timeZones.zones())
  {
    QIcon icon;

    QString flag = KStandardDirs::locate("locale", QString("l10n/%1/flag.png").arg(tz.countryCode().toLower()));

    if (QFile::exists(flag))
      icon = QIcon(flag);

    m_ui->timezoneCombo->addItem(icon, tz.name());
  }
}

void CalendarEditor::initColors()
{
  /* http://code.google.com/apis/calendar/data/2.0/reference.html#gCalcolor */
  QList< QColor > colors;
  colors << QColor("#A32929") << QColor("#B1365F") << QColor("#7A367A")
         << QColor("#5229A3") << QColor("#29527A") << QColor("#2952A3")
         << QColor("#1B887A") << QColor("#28754E") << QColor("#0D7813")
         << QColor("#528800") << QColor("#88880E") << QColor("#AB8B00")
         << QColor("#BE6D00") << QColor("#B1440E") << QColor("#865A5A")
         << QColor("#705770") << QColor("#4E5D6C") << QColor("#5A6986")
         << QColor("#4A716C") << QColor("#6E6E41") << QColor("#8D6F47")
         << QColor("#853104") << QColor("#691426") << QColor("#5C1158")
         << QColor("#23164E") << QColor("#182C57") << QColor("#060D5E")
         << QColor("#125A12") << QColor("#2F6213") << QColor("#2F6309")
         << QColor("#5F6B02") << QColor("#8C500B") << QColor("#8C500B")
         << QColor("#754916") << QColor("#6B3304") << QColor("#5B123B")
         << QColor("#42104A") << QColor("#113F47") << QColor("#333333")
         << QColor("#0F4B38") << QColor("#856508") << QColor("#711616");

  m_ui->colorCombo->setColors(colors);
}

void CalendarEditor::initWidgets()
{

  m_ui->nameEdit->setText(m_calendar->title());
  m_ui->descriptionEdit->setText(m_calendar->details());
  m_ui->locationEdit->setText(m_calendar->location());

  int tzIndex = m_ui->timezoneCombo->findText(m_calendar->timezone(), Qt::MatchContains);

  if (tzIndex > -1)
    m_ui->timezoneCombo->setCurrentIndex(tzIndex);

  m_ui->colorCombo->setColor(QColor(m_calendar->color()));
}
