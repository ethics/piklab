/***************************************************************************
 *   Copyright (C) 2006-2008 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "gpsim.h"
#include "gpsim.moc"

#include <qregexp.h>

#include "devices/pic/base/pic.h"
#include "devices/list/device_list.h"
#include "progs/base/generic_prog.h"
#include "progs/base/generic_debug.h"
#include "progs/gpsim/base/gpsim_debug.h"

//-----------------------------------------------------------------------------
namespace GPSim
{
  const char* const GP_PROMPT = "**gpsim>";
  const char* const GP_ERROR = "***ERROR:";
} // namespace

GPSim::Process::Process(Log::Base *base)
: ::Process::LineOutput(0, "gpsim_process"),
  Log::Base(base),
  _ready(false)
{
  connect(this, SIGNAL(dataReceived(::Process::OutputType)), SLOT(dataReceivedSlot(::Process::OutputType)));
}

bool GPSim::Process::runSynchronously()
{
  uint retries = 0;
  for (;;) {
    execute(200);
    // this is because sometimes the synchronous process get stuck and timeout
    if ( state()!=::Process::Timedout || _ready ) return true;
    ++retries;
    if ( retries == 20 ) break;
  }
  kill();
  return false;
}

void GPSim::Process::timeoutSlot()
{
  // do not kill the process on timeout
  _state = ::Process::Timedout;
}

void GPSim::Process::dataReceivedSlot(::Process::OutputType type)
{
  if ( type==::Process::OutputType::Stdout) {
    QString s = _outputs[type].stripWhiteSpace();
    if (s.startsWith(GP_PROMPT)
          || _ready) {
      log(Log::DebugLevel::Extra, "received console prompt", Log::Delayed);
      _ready = true;
      _stopExecute = true;
    }
  }
}

void GPSim::Process::addLine(::Process::OutputType type, const QString &line)
{
  if ( type==::Process::OutputType::Stdout) {
    log(Log::DebugLevel::Extra, "  " + line, Log::Delayed);
    QString s = line.stripWhiteSpace();
    if (s.startsWith(GP_ERROR)) {
      log(Log::LineType::Error, line);
      return;
    }
    while (s.startsWith(GP_PROMPT)) {
      _ready = true;
      s = s.mid(strlen(GP_PROMPT)).stripWhiteSpace();
    }
    _lines[type] += s;
  }
}

//-----------------------------------------------------------------------------
GPSim::ProcessManager::ProcessManager(Log::Base *base)
  : Log::Base(base), _process(base)
{
}

bool GPSim::ProcessManager::start()
{
  Config config;
  return start(config.executableDirectory());
}

bool GPSim::ProcessManager::start(const PURL::Directory& execDir)
{
  QString exec = execDir.path();
  exec += "gpsim";
  _process._ready = false;
  _process.setup(exec, QStringList("-i"), false);
  _process.start();
  return runSynchronously();
}

bool GPSim::ProcessManager::runSynchronously()
{
  if (!_process.runSynchronously()) {
    log(Log::LineType::Error, i18n("Timeout waiting for \"gpsim\"."));
    return false;
  }
  if ( !_process.isRunning() ) {
    log(Log::LineType::Error, i18n("\"gpsim\" unexpectedly exited."));
    return false;
  }
  return true;
}

bool GPSim::ProcessManager::sendCommand(const QString &cmd, bool synchronous)
{
  _process._ready = false;
  _process.clearOutputs();
  _process.writeToStdin(cmd + '\n');
  if (synchronous) return runSynchronously();
  return true;
}

bool GPSim::ProcessManager::sendSignal(uint n, bool synchronous)
{
  _process._ready = false;
  _process.clearOutputs();
  if ( !_process.signal(n) ) {
    log(Log::LineType::Error, i18n("Error sending a signal to the subprocess."));
    return false;
  }
  if (synchronous) return runSynchronously();
  return true;
}

QStringList GPSim::ProcessManager::outputLines() const
{
  return _process.lines(::Process::OutputType::Stdout);
}

bool GPSim::ProcessManager::getVersion(VersionData &version)
{
  if (!sendCommand("version", true)) return false;
  version = VersionData();
  QRegExp reg("\\w*\\s*(\\d+\\.\\d+\\.\\d+).*");
  for (int i = 0; i<outputLines().count(); ++i) {
    if (reg.exactMatch(outputLines()[i]) ) {
      version = VersionData::fromString(reg.cap(1));
      break;
    }
  }
  return true;
}

bool GPSim::ProcessManager::getSupportedDevices(VersionData &version, QStringList &devices)
{
  bool oldGpsim = (version.isValid() ? version<VersionData(0, 21, 11) : false);
  if ( !sendCommand("processor list", true) ) return false;
  QStringList list = QStringList::split(" ", outputLines().join(" "));
  for (QStringList::const_iterator it = list.begin(); it != list.end(); ++it) {
    QString s = (*it).upper();
    if ( s.startsWith("PIC") ) s = s.mid(3);
    const Pic::Data* data = static_cast<const Pic::Data *>(Device::Lister::instance().data(s));
    if (data != NULL) {
      if ( data->architecture()==Pic::Architecture::P18F && oldGpsim ) continue;
      devices += data->name();
    }
  }
  return true;
}

//-----------------------------------------------------------------------------
bool GPSim::Hardware::internalConnectHardware()
{
  _manager.reset(new ProcessManager(this));
  _manager->process().setWorkingDirectory(_base.debugger()->directory());
  if ( !_manager->start() ) return false;
  if ( !_manager->getVersion(_version) ) return false;
  if ( !_version.isValid() ) {
    log(Log::LineType::Error, i18n("Could not recognize gpsim version."));
    return false;
  }
  return true;
}

void GPSim::Hardware::internalDisconnectHardware()
{
  _manager.reset();
}

bool GPSim::Hardware::execute(const QString &command, bool synchronous, QStringList *output)
{
  ASSERT(_manager.get() != NULL);
  log(Log::DebugLevel::Normal, QString("command: %1").arg(command));
  if (output) output->clear();
  if ( !_manager->sendCommand(command, synchronous) ) return false;
  if (output) *output = _manager->process().lines(::Process::OutputType::Stdout);
  return true;
}

bool GPSim::Hardware::signal(uint n, bool synchronous, QStringList *output)
{
  ASSERT(_manager.get() != NULL);
  log(Log::DebugLevel::Normal, QString("signal: %1").arg(n));
  if (output) output->clear();
  if ( !_manager->sendSignal(n, synchronous) ) return false;
  if (output) *output = _manager->process().lines(::Process::OutputType::Stdout);
  return true;
}
