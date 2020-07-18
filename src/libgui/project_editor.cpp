/***************************************************************************
 *   Copyright (C) 2005-2006 Nicolas Hadacek <hadacek@kde.org>             *
 *   Copyright (C) 2003 Alain Gibaud <alain.gibaud@free.fr>                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "project_editor.h"
#include "project_editor.moc"

#include "project.h"
#include "tools/list/compile_config.h"
#include "device_gui.h"
#include "devices/list/device_list.h"
#include "progs/list/prog_list.h"

//----------------------------------------------------------------------------
ProjectEditor::ProjectEditor(Project &project, PageType type, QWidget *parent)
: Dialog(parent, "project_options", true, i18n("Project Options"), Ok|Cancel, Ok, false),
  _project(project)
{
   QVBoxLayout *top = new QVBoxLayout(mainWidget(), 0, 10);
  TabWidget *tabWidget = new TabWidget(mainWidget());
  top->addWidget(tabWidget);

// global
  QWidget *tab = new QWidget(tabWidget);
  tabWidget->addTab(tab, i18n("General"));
  QGridLayout *grid = new QGridLayout(tab, 0, 0, 10, 10);
  QLabel *label = new QLabel(i18n("Name:"), tab);
  grid->addWidget(label, 0, 0);
  label = new QLabel(project.name(), tab);
  grid->addWidget(label, 0, 1);
  label = new QLabel(i18n("Description:"), tab);
  grid->addWidget(label, 1, 0);
  _description = new Q3TextEdit(tab);
  _description->setText(_project.description());
  grid->addMultiCellWidget(_description, 1,1, 1,2);
  label = new QLabel(i18n("Version:"), tab);
  grid->addWidget(label, 2, 0);
  _version = new QLineEdit(tab);
  _version->setText(_project.version());
  grid->addWidget(_version, 2, 1);
  label = new QLabel(i18n("Device:"), tab);
  grid->addWidget(label, 3, 0);
  _device = new DeviceChooser::Button(tab);
  _device->setDevice(Compile::Config::device(&_project).name());
  grid->addWidget(_device, 3, 1);
  label = new QLabel(i18n("Programmer:"), tab);
  grid->addWidget(label, 4, 0);
  _prog.reset(new ListerComboBox<Programmer::Lister>(tab));
  _prog->setGroup(Compile::Config::programmerGroup(&_project));
  grid->addWidget(_prog->widget(), 4, 1);

  grid->setRowStretch(5, 1);
  grid->setColStretch(2, 1);

// toochain
  tab = new QWidget(tabWidget);
  tabWidget->addTab(tab, i18n("Toochain"));
  grid = new QGridLayout(tab, 0, 0, 10, 10);
  _tools = new ToolsConfigWidget(project, tab);
  _tools->loadConfig();
  grid->addMultiCellWidget(_tools, 0,0, 0,2);
  grid->setRowStretch(1, 1);
  grid->setColStretch(2, 1);

  tabWidget->setCurrentPage(type);

  connect(this, SIGNAL(okClicked()), SLOT(slotOk()));
}

void ProjectEditor::slotOk()
{
  _project.setDescription(_description->text());
  _project.setVersion(_version->text());
  const Device::Data& data = Device::Lister::instance().checkedData(_device->device());
  Compile::Config::setDevice(&_project, data);
  Compile::Config::setProgrammerGroup(&_project, _prog->group());
  _tools->saveConfig();
  accept();
}
