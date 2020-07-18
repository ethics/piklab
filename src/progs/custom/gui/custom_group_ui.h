/***************************************************************************
 *   Copyright (C) 2007 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef CUSTOM_GROUP_UI_H
#define CUSTOM_GROUP_UI_H

#include "progs/gui/prog_group_ui.h"
#include "progs/gui/prog_config_widget.h"
#include "progs/custom/base/custom_prog.h"
class HelpDialog;

namespace CustomProgrammer
{
//----------------------------------------------------------------------------
class ConfigWidget : public ::Programmer::ConfigWidget
{
Q_OBJECT
public:
  ConfigWidget(const ::Programmer::Group &group, QWidget *parent);
  virtual void loadConfig();
  virtual void saveConfig();

private slots:
  void displayHelpSlot();

private:
  EnumVector<Action, KLineEdit *> _commands;
  QPointer<HelpDialog>            _helpDialog;
};

//----------------------------------------------------------------------------
class GroupUI : public ::Programmer::GroupUI
{
public:
  virtual ::Programmer::ConfigWidget *createConfigWidget(QWidget *parent) const;
  virtual bool hasAdvancedDialog() const { return false; }
  virtual ::Programmer::AdvancedDialog *createAdvancedDialog(::Programmer::Base &, QWidget *) const { return 0; }
};

} // namespace

#endif
