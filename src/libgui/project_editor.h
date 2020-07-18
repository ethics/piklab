/***************************************************************************
 *   Copyright (C) 2005-2009 Nicolas Hadacek <hadacek@kde.org>             *
 *   Copyright (C) 2003 Alain Gibaud <alain.gibaud@free.fr>                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef PROJECT_EDITOR_H
#define PROJECT_EDITOR_H

#include "tools/list/tools_config_widget.h"
#include "common/gui/misc_gui.h"
#include "progs/list/prog_list.h"
class Project;
namespace DeviceChooser { class Button; }

class ProjectEditor : public Dialog
{
Q_OBJECT
public:
  enum PageType { Main = 0, Toolchain };
  ProjectEditor(Project &project, PageType type, QWidget *parent);

private slots:
  virtual void slotOk();

private:
  Project &_project;
  Q3TextEdit *_description;
  QLineEdit *_version;
  DeviceChooser::Button *_device;
  std::auto_ptr<ListerComboBox<Programmer::Lister> > _prog;
  ToolsConfigWidget *_tools;
};

#endif
