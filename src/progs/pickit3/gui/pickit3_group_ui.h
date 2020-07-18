/***************************************************************************
 *   Copyright (C) 2012 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef PICKIT3_GROUP_UI_H
#define PICKIT3_GROUP_UI_H

#include "devices/pic/gui/pic_prog_group_ui.h"
#include "progs/pickit3/base/pickit3_prog.h"

namespace Pickit3
{
//----------------------------------------------------------------------------
class AdvancedDialog : public ::Programmer::PicAdvancedDialog
{
Q_OBJECT
public:
  AdvancedDialog(PicBase &base, QWidget *parent);
  virtual void updateDisplay();

private:
  QLabel *_osVersionLabel, *_apVersionLabel, *_apIdLabel;
  PicBase &base() { return static_cast<PicBase &>(_base); }
};

//----------------------------------------------------------------------------
class GroupUI : public ::Programmer::GroupUI
{
public:
  virtual ::Programmer::ConfigWidget *createConfigWidget(QWidget *parent) const;
  virtual bool hasAdvancedDialog() const { return true; }
  virtual ::Programmer::AdvancedDialog *createAdvancedDialog(::Programmer::Base &base, QWidget *parent) const;
};

} // namespace

#endif
