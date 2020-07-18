/***************************************************************************
 *   Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "gpsim_group_ui.h"
#include "gpsim_group_ui.moc"

#include <qtimer.h>

#include "common/gui/misc_gui.h"
#include "progs/base/prog_group.h"
#include "progs/gpsim/base/gpsim_debug.h"

//----------------------------------------------------------------------------
GPSim::ConfigWidget::ConfigWidget(const ::Programmer::Group &group, QWidget *parent)
: ::Programmer::ConfigWidget(group, parent)
{
  uint row = numRows();

  QLabel* label = new QLabel(i18n("Executable directory:"), this);
  addWidget(label, row,row, 0,0);
  _dir = new PURL::DirectoryWidget(this);
  connect(_dir, SIGNAL(changed()), SLOT(updateStatus()));
  addWidget(_dir, row,row, 1,2);
  ++row;

  label = new QLabel(i18n("Status:"), this);
  addWidget(label, row,row, 0,0);
  _status = new QLabel(this);
  addWidget(_status, row,row, 1,1);
  KPushButton* button = new KPushButton(KGuiItem(QString::null, "zoom-in"), this);
  addWidget(button, row,row, 2,2);
  connect(button, SIGNAL(clicked()), SLOT(showVersionDetails()));
  ++row;

  label = new QLabel(i18n("Device Detection:"), this);
  addWidget(label, row,row, 0,0);
  _devicesLabel = new QLabel(this);
  addWidget(_devicesLabel, row,row, 1,1);
  button = new KPushButton(KGuiItem(QString::null, "zoom-in"), this);
  addWidget(button, row,row, 2,2);
  connect(button, SIGNAL(clicked()), SLOT(showDevicesDetails()));
  ++row;
}

void GPSim::ConfigWidget::loadConfig()
{
    ::Programmer::ConfigWidget::loadConfig();
    _dir->blockSignals(true);
    _dir->setDirectory(GPSim::Config::executableDirectory());
    _dir->blockSignals(false);
}

void GPSim::ConfigWidget::saveConfig()
{
    ::Programmer::ConfigWidget::saveConfig();
    GPSim::Config::writeExecutableDirectory(_dir->directory());
}

void GPSim::ConfigWidget::updateStatus()
{
  _status->setText("Detecting \"gpsim\"...");
  _versionLines.clear();
  _devicesLines.clear();
  _devicesLabel->setText("");

  ProcessManager manager(0);
  bool ok = manager.start(_dir->directory());
  _arguments = manager.process().arguments();
  _versionLines = manager.outputLines();
  if (!ok) {
    _status->setText(i18n("Could not start \"gpsim\""));
    return;
  }
  VersionData version;
  ok = ( manager.getVersion(version) && version.isValid() );
  _versionLines += manager.outputLines();
  if (!ok) {
    _status->setText(i18n("Could not detect \"gpsim\" version"));
    return;
  }
  _status->setText(i18n("Found version \"%1\"").arg(version.pretty()));

  QStringList devices;
  ok = manager.getSupportedDevices(version, devices);
  _devicesLines = manager.outputLines();
  if (!ok) {
    _devicesLabel->setText(i18n("Could not detect supported devices"));
    return;
  }
  _devicesLabel->setText(i18n("Detected (%1)").arg(devices.count()));
}

void GPSim::ConfigWidget::showVersionDetails()
{
  QString s = "<qt>" + i18n("<b>Command: </b>%1<br>").arg(_arguments.join(" "));
  s += _versionLines.join("<br>");
  MessageBox::text(s, Log::Show);
}

void GPSim::ConfigWidget::showDevicesDetails()
{
  QString s = "<qt>";
  s += _devicesLines.join("<br>");
  MessageBox::text(s, Log::Show);
}

//----------------------------------------------------------------------------
::Programmer::ConfigWidget *GPSim::GroupUI::createConfigWidget(QWidget *parent) const
{
  return new ConfigWidget(static_cast<const ::Programmer::Group &>(group()), parent);
}
