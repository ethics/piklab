/***************************************************************************
 *   Copyright (C) 2012 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "pickit3_group_ui.h"
#include "pickit3_group_ui.moc"

#include "common/gui/misc_gui.h"
#include "progs/gui/prog_config_widget.h"
#include "progs/base/prog_group.h"

//----------------------------------------------------------------------------
Pickit3::AdvancedDialog::AdvancedDialog(PicBase &base, QWidget *parent)
: ::Programmer::PicAdvancedDialog(base, parent, "pickit3_advanced_dialog")
{
  uint row = _firmwareContainer->numRows();
  QLabel *label = new QLabel(i18n("OS Version:"), _firmwareContainer);
  _firmwareContainer->addWidget(label, row,row, 0,0);
  _osVersionLabel = new QLabel(_firmwareContainer);
  _firmwareContainer->addWidget(_osVersionLabel, row,row, 1,1);
  row++;

  label = new QLabel(i18n("AP Version:"), _firmwareContainer);
  _firmwareContainer->addWidget(label, row,row, 0,0);
  _apVersionLabel = new QLabel(_firmwareContainer);
  _firmwareContainer->addWidget(_apVersionLabel, row,row, 1,1);
  row++;

  label = new QLabel(i18n("AP Id:"), _firmwareContainer);
  _firmwareContainer->addWidget(label, row,row, 0,0);
  _apIdLabel = new QLabel(_firmwareContainer);
  _firmwareContainer->addWidget(_apIdLabel, row,row, 1,1);
  row++;
}

void Pickit3::AdvancedDialog::updateDisplay()
{
  ::Programmer::PicAdvancedDialog::updateDisplay();
  const VersionData& osVersion = base().getOSVersion();
  _osVersionLabel->setText(osVersion.pretty());
  const VersionData& apVersion = base().getAPVersion();
  _apVersionLabel->setText(apVersion.pretty());
  uint apId = base().getAPId();
  _apIdLabel->setText(apId != 0 ? QString::number(apId) : "---");
}

//----------------------------------------------------------------------------
::Programmer::ConfigWidget *Pickit3::GroupUI::createConfigWidget(QWidget *parent) const
{
  return new ::Programmer::ConfigWidget(static_cast<const Group &>(group()), parent);
}

::Programmer::AdvancedDialog *Pickit3::GroupUI::createAdvancedDialog(::Programmer::Base &base, QWidget *parent) const
{
  return new AdvancedDialog(static_cast<PicBase &>(base), parent);
}
