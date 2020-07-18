/***************************************************************************
 *   Copyright (C) 2005 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "main_global.h"

#include <kxmlguifactory.h>

#include "toplevel.h"
#include "common/global/about.h"
#include "global_config.h"
#include "devices/list/device_list.h"
#include "project_manager.h"
#include "editor_manager.h"
#include "tools/list/compile_config.h"
#include "gui_prog_manager.h"
#include "device_editor.h"

//----------------------------------------------------------------------------
const Piklab::CmdLineOption OPTIONS[] = {
  { "+[file]", I18N_NOOP("Optional filenames to be opened upon startup."), NULL },
  { NULL, NULL, NULL}
};

//----------------------------------------------------------------------------
Main::State           Main::_state = Main::Idle;
EditorManager*        Main::_editorManager = NULL;
ProjectManager::View* Main::_projectManager = NULL;
Breakpoint::View*     Main::_breakpointsView = NULL;
Register::WatchView*  Main::_watchView = NULL;
Compile::LogWidget*   Main::_compileLog = NULL;
Compile::Manager*     Main::_compileManager = NULL;
ConsoleView*          Main::_consoleView = NULL;

//----------------------------------------------------------------------------
void Main::setState(State state)
{
  _state = state;
  MainWindow::self().updateGUI();
}

Programmer::Generic* Main::programmer()
{
  return Programmer::manager->programmer();
}

const Programmer::Group& Main::programmerGroup()
{
  return Compile::Config::programmerGroup(Main::project());
}

Programmer::State Main::programmerState()
{
  return (programmer() != NULL ? programmer()->state() : Programmer::NotConnected);
}

QAction* Main::action(const char* name)
{
  return MainWindow::self().action(name);
}

QAction* Main::action(KStandardAction::StandardAction action)
{
  return MainWindow::self().action(KStandardAction::name(action));
}

QMenu* Main::popup(const char* name)
{
  return dynamic_cast<QMenu *>(MainWindow::self().guiFactory()->container(name, &MainWindow::self()));
}

const Device::Data& Main::deviceData()
{
  return Compile::Config::device(project());
}

Editor* Main::currentEditor()
{
  return _editorManager->currentEditor();
}

Project* Main::project()
{
  return _projectManager->project();
}

const Tool::Group& Main::toolGroup()
{
  return Compile::Config::toolGroup(project());
}

void Main::setupApplication(int argc, char** argv)
{
  Piklab::AboutData about("piklab", I18N_NOOP("Piklab"),
                          I18N_NOOP( "Graphical development environment for applications based on PIC and dsPIC microcontrollers."));
  Piklab::OptionList options(OPTIONS);
  Piklab::initArgs(about, argc, argv, options);
  const bool gui = true;
  Piklab::createApplication(about.appName(), argc, argv, gui, NULL);
  if (kapp->isSessionRestored() && KMainWindow::canBeRestored(1)) MainWindow::self().restore(1);
  else MainWindow::self().show();
}
