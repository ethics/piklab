/***************************************************************************
 *   Copyright (C) 2005-2006 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "gui_prog_manager.h"
#include "gui_prog_manager.moc"

#include "progs/base/generic_prog.h"
#include "progs/base/prog_group.h"
#include "progs/base/prog_config.h"
#include "progs/base/hardware_config.h"
#include "progs/base/generic_debug.h"
#include "progs/gui/prog_group_ui.h"
#include "toplevel.h"
#include "project.h"
#include "project_manager.h"
#include "progs/manager/debug_manager.h"

//----------------------------------------------------------------------------
bool Programmer::GuiManager::internalInitProgramming(ProgramAction action)
{
  if ( !Manager::internalInitProgramming(action) ) return false;
  if (group()->isDebugger()
      && (action == ProgramAction::Run
	  || action == ProgramAction::Step)) {
    if ( _programmer->isActive() ) return true;
    if ( ::Debugger::manager->coff()==0 && !::Debugger::manager->init() ) {
      KGuiItem item(i18n("Recompile"));
      if ( !MessageBox::askContinue(i18n("Cannot start debugging without a COFF file."), item) ) return false;
      MainWindow::self().buildProject(action);
      return false;
    }
    if ( !group()->isSoftware() ) {
      KGuiItem item1(i18n("Compile and Program First"));
      KGuiItem item2(i18n("Continue Anyway"));
      MessageBox::Result res = MessageBox::questionYesNoCancel(i18n("The device memory is in an unknown state. You may want to reprogram the device. Continue anyway?"), item1, item2);
      switch (res) {
        case MessageBox::Cancel: return false;
        case MessageBox::Yes: MainWindow::self().buildProject(action); return false;
        case MessageBox::No: break;
      }
    }
    return true;
  } else if (_programmer->isActive() && action != ProgramAction::Halt) {
    if ( group()->isDebugger() && !askContinue(i18n("The selected operation will stop the current debugging session. Continue anyway?")) ) return false;
    if ( !halt() ) return false;
  }
  return true;
}

void Programmer::GuiManager::toggleDevicePower()
{
  bool on = _programmer->isTargetPowerOn();
  setDevicePower(!on);
}

void Programmer::GuiManager::showAdvancedDialog()
{
  const ::Programmer::GroupUI *groupui = static_cast<const ::Programmer::GroupUI *>(group()->gui());
  const Device::Data* data = &Main::deviceData();
  if ( !group()->isSupported(data->name()) ) data = NULL;
  ::Programmer::Generic *generic = createProgrammer(data);
  ASSERT( !(generic->group().properties() & ::Programmer::Custom) );
  typedef ::Programmer::Base Base; // for g++ 3.3.6
  Base* base = static_cast<Base*>(generic);
  std::auto_ptr< ::Programmer::AdvancedDialog> dialog(groupui->createAdvancedDialog(*base, &MainWindow::self()));
  if (dialog.get() != NULL) {
    dialog->updateDisplay();
    dialog->exec();
  }
}

Programmer::Generic *Programmer::GuiManager::createProgrammer(const Device::Data *data)
{
  HardwareDescription hd;
  hd.port = GroupConfig::portDescription(*group());
  std::auto_ptr< ::Hardware::Config> hconfig(group()->hardwareConfig());
  if (hconfig.get() != NULL) hd.name = hconfig->currentHardware(hd.port.type);
  internalCreateProgrammer(data, hd);
  return _programmer.get();
}
