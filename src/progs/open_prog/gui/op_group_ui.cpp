/***************************************************************************
 *   Copyright (C) 2010 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "op_group_ui.h"
#include "op_group_ui.moc"

#include "progs/gui/prog_config_widget.h"

//----------------------------------------------------------------------------
::Programmer::ConfigWidget *OpenProg::GroupUI::createConfigWidget(QWidget *parent) const
{
  return new ::Programmer::ConfigWidget(static_cast<const Group &>(group()), parent);
}
