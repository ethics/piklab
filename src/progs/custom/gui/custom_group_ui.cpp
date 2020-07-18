/***************************************************************************
 *   Copyright (C) 2007 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "custom_group_ui.h"
#include "custom_group_ui.moc"

#include "progs/base/prog_group.h"
#include "tools/list/tools_config_widget.h"

//----------------------------------------------------------------------------
CustomProgrammer::ConfigWidget::ConfigWidget(const ::Programmer::Group &group, QWidget *parent)
: ::Programmer::ConfigWidget(group, parent)
{
  Config config;
  uint row = numRows();
  KPushButton *button = new KPushButton(KGuiItem(QString::null, "help-about"), this);
  connect(button, SIGNAL(clicked()), SLOT(displayHelpSlot()));
  addWidget(button, row,row, 2,2);
  ++row;

  FOR_EACH(Action, action) {
    QLabel *label = new QLabel(action.label(), this);
    addWidget(label, row,row, 0,0);
    _commands[action] = new KLineEdit(this);
    _commands[action]->setText(config.command(action));
    addWidget(_commands[action], row,row, 1,1);
    ++row;
  }
}

void CustomProgrammer::ConfigWidget::loadConfig()
{
  ::Programmer::ConfigWidget::loadConfig();
  Config config;
  FOR_EACH(Action, action) {
    _commands[action]->setText(config.command(action));
  }
}

void CustomProgrammer::ConfigWidget::saveConfig()
{
  ::Programmer::ConfigWidget::saveConfig();
  Config config;
  FOR_EACH(Action, action) {
    config.writeCommand(action, _commands[action]->text());
  }
}

void CustomProgrammer::ConfigWidget::displayHelpSlot()
{
  if ( _helpDialog.isNull() ) {
    CustomProgrammer::Base base(group(), NULL);
    CustomProgrammer::ProcessManager manager(base);
    CustomArgument::ArgList args = manager.argList();
    _helpDialog = new HelpDialog(this, args);
  }
  _helpDialog->show();
}

//----------------------------------------------------------------------------
::Programmer::ConfigWidget *CustomProgrammer::GroupUI::createConfigWidget(QWidget *parent) const
{
  return new ConfigWidget(static_cast<const ::Programmer::Group &>(group()), parent);
}
