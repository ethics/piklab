/***************************************************************************
 *   Copyright (C) 2005-2006 Nicolas Hadacek <hadacek@kde.org>             *
 *   Copyright (C) 2004 Alain Gibaud <alain.gibaud@free.fr>                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef TOOLS_CONFIG_WIDGET_H
#define TOOLS_CONFIG_WIDGET_H

#include <map>
#include <memory>

#include <QStackedWidget>

#include "common/gui/config_widget.h"
#include "common/gui/dialog.h"
#include "common/gui/key_gui.h"
#include "common/gui/editlistbox.h"
#include "tools/base/generic_tool.h"
#include "tools/list/compile_process.h"
#include "tools/list/tool_list.h"
class Project;
class ToolConfigWidget;

//----------------------------------------------------------------------------
class HelpDialog : public Dialog
{
Q_OBJECT
public:
  HelpDialog(QWidget *parent, const CustomArgument::ArgList &arguments);
};

//----------------------------------------------------------------------------
class ToolsConfigWidget : public ConfigWidget
{
Q_OBJECT
public:
  ToolsConfigWidget(Project& project, QWidget *parent);
  ~ToolsConfigWidget();
  virtual void loadConfig();

public slots:
  virtual void saveConfig();
  void customDisplayHelp();
  void displayHelp(Tool::Category category);

private slots:
  void toolChanged();

private:
  Project&       _project;
  std::auto_ptr<ListerComboBox<Tool::Lister> > _tool;
  std::auto_ptr<KeyComboBox<Tool::OutputType> > _output;
  QStackedWidget *_mainStack;
  QWidget       *_customWidget;
  EditListBox   *_commandsEditor;
  QTabWidget    *_tabWidget;
  typedef std::map<Tool::Category, KeyWidgetStack<QString>*> StackMap;
  StackMap       _stacks;
  QPointer<Dialog> _helpDialog;
  QPointer<Dialog> _customHelpDialog;
};

#endif
