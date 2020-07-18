/***************************************************************************
 *   Copyright (C) 2005 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "prog_config_widget.h"
#include "prog_config_widget.moc"

#include <qlabel.h>

#include "progs/base/prog_config.h"
#include "progs/base/prog_group.h"

Programmer::ConfigWidget::ConfigWidget(const Group &group, QWidget *parent)
  : ::ConfigWidget(parent), _group(group)
{
  if ( group.properties() & NeedDeviceSpecificFirmware ) {
    uint row = numRows();
    QLabel *label = new QLabel(i18n("Firmware directory:"), this);
    addWidget(label, row,row, 0,0);
    _firmwareDir = new KUrlRequester(this);
    _firmwareDir->setMode(KFile::Directory | KFile::ExistingOnly);
    addWidget(_firmwareDir, row,row, 1,1);
  } else _firmwareDir = 0;
}

void Programmer::ConfigWidget::loadConfig()
{
  if (_firmwareDir) _firmwareDir->setUrl(GroupConfig::firmwareDirectory(_group));
}

void Programmer::ConfigWidget::saveConfig()
{
  if (_firmwareDir) GroupConfig::writeFirmwareDirectory(_group, _firmwareDir->url().path());
}

bool Programmer::ConfigWidget::setPort(const HardwareDescription &)
{
  return true;//_group.checkConnection(hd);
}
