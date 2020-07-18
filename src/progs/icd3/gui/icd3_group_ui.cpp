/***************************************************************************
 *   Copyright (C) 2012 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "icd3_group_ui.h"
#include "icd3_group_ui.moc"

#include "common/gui/misc_gui.h"
#include "progs/gui/prog_config_widget.h"
#include "progs/base/prog_group.h"

//----------------------------------------------------------------------------
::Programmer::ConfigWidget *Icd3::GroupUI::createConfigWidget(QWidget *parent) const
{
  return new ::Programmer::ConfigWidget(static_cast<const Group &>(group()), parent);
}

::Programmer::AdvancedDialog *Icd3::GroupUI::createAdvancedDialog(::Programmer::Base &, QWidget *) const
{
  return NULL;
}
