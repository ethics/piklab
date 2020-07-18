/***************************************************************************
 *   Copyright (C) 2007-2008 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "custom_prog.h"
#include "custom_prog.moc"

#include "devices/base/device_group.h"
#include "devices/list/device_list.h"
#include "libgui/main_global.h"
#include "libgui/project_manager.h"

//-----------------------------------------------------------------------------
const CustomProgrammer::Action::Data CustomProgrammer::Action::DATA[Nb_Types] = {
  { "read",        I18N_NOOP("Read")        },
  { "erase",       I18N_NOOP("Erase")       },
  { "program",     I18N_NOOP("Program")     },
  { "verify",      I18N_NOOP("Verify")      },
  { "blank_check", I18N_NOOP("Blank Check") },
  { "run",         I18N_NOOP("Run")         },
  { "stop",        I18N_NOOP("Stop")        }
};

//-----------------------------------------------------------------------------
QString CustomProgrammer::Config::command(Action action)
{
  Config config;
  return config.readEntry(action.key(), QString::null);
}

void CustomProgrammer::Config::writeCommand(Action action, const QString &command)
{
  Config config;
  config.writeEntry(action.key(), command);
}

//-----------------------------------------------------------------------------
CustomProgrammer::ProcessManager::ProcessManager(::CustomProgrammer::Base& base)
: Log::Base(&base),
  _base(base)
{
}

QString CustomProgrammer::ProcessManager::deviceName() const
{
  return Main::deviceData().name();
}

void CustomProgrammer::ProcessManager::logLine(::Process::OutputType type, const QString &s)
{
  Log::LineType ltype = (type==::Process::OutputType::Stderr ? Log::LineType::SoftError : Log::LineType::Normal);
  log(ltype, s, Log::Delayed); // Delayed => critical!!
}

bool CustomProgrammer::ProcessManager::execute(const QString &command, const QString& action)
{
  _process = new ::Process::LineSignal;
  _process->setUseShell(true);
  connect(_process, SIGNAL(logLine(::Process::OutputType, const QString &)),
          SLOT(logLine(::Process::OutputType, const QString &)));
  QStringList commands;
  commands += command;
  QStringList args = process(commands);
  QString cmd = args.join(" ");
  _process->setup(cmd, QStringList(), false);
  PURL::Url url = Main::projectManager().projectUrl();
  if ( !url.isEmpty() ) {
    _process->setWorkingDirectory(url.directory());
  }
  log(Log::LineType::Command, _process->arguments().join(" "));
  Process::State state = Process::runSynchronouslyDialog(*_process, i18n("Executing '%1' command...").arg(action));
  if (state == Process::Error) {
    log(Log::LineType::Error, i18n("*** Error executing command ***"));
    return false;
  }
  if (state == Process::Killed) {
    log(Log::LineType::Error, i18n("*** Aborted ***"));
    return false;
  }
  int code = _process->exitCode();
  if ( code == 0 ) {
    log(Log::LineType::Information, i18n("*** Success ***"), Log::Delayed);
  } else {
    log(Log::LineType::Error, i18n("*** Exited with status: %1 ***").arg(code), Log::Delayed);
  }
  return true;
}

//-----------------------------------------------------------------------------
CustomProgrammer::Base::Base(const ::Programmer::Group &group, const Device::Data *data)
 : Programmer::Generic(group, data),
   _manager(*this)
{
}

bool CustomProgrammer::Base::execute(Action action, Device::Memory *memory, const Device::MemoryRange *range)
{
  Config config;
  QString cmd = config.command(action).stripWhiteSpace();
  if (cmd.isEmpty()) {
    log(Log::LineType::Error, i18n("There is no command specified for the \"%1\" action.").arg(action.label()));
    return false;
  }
  return _manager.execute(cmd, action.label());
}

//-----------------------------------------------------------------------------
void CustomProgrammer::Group::initSupported()
{
  QStringList names = Device::Lister::instance().supportedDevices();
  for (QStringList::const_iterator it = names.begin(); it != names.end(); ++it)
    addDevice(*it, Device::Lister::instance().data(*it), ::Group::Support::Tested);
}
