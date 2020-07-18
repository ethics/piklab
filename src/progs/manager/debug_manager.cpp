/***************************************************************************
 *   Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "debug_manager.h"
#include "debug_manager.moc"

#include <qtimer.h>
#include <qeventloop.h>

#include "coff/base/text_coff.h"
#include "coff/base/cdb_parser.h"
#include "progs/base/generic_prog.h"
#include "progs/base/generic_debug.h"
#include "progs/base/prog_group.h"
#include "devices/base/register.h"
#include "progs/base/debug_config.h"
#include "prog_manager.h"

Debugger::Manager *Debugger::manager = NULL;

Debugger::Manager::Manager()
: QObject(Programmer::manager, "debug_manager"),
  Log::Base(Programmer::manager),
  GenericView(Breakpoint::list()),
  _coff(NULL),
  _inited(false),
  _data(NULL)
{
  connect(&_runTimer, SIGNAL(timeout()), SLOT(slotRunTimeout()));
  connect(&_stepTimer, SIGNAL(timeout()), SLOT(doStep()));
}

Debugger::Manager::~Manager()
{
  delete _coff;
  _coff = NULL;
}

bool Debugger::Manager::isStepping() const
{
  return (programmer() ? programmer()->state()==Programmer::Halted : false) && _stepTimer.isActive();
}

Debugger::Base *Debugger::Manager::debugger() const
{
  return (programmer() ? programmer()->debugger() : NULL);
}

void Debugger::Manager::updateDevice()
{
  const Device::Data *data = deviceData();
  if ( data==_data ) return;
  _data = data;
  Register::list().clearWatched();
  clear();
}

bool Debugger::Manager::checkState(bool &first)
{
  if ( programmer()->state()==Programmer::NotConnected ) {
    if ( !prepareDebugging() ) return false;
    if ( !programmer()->connectHardware() ) return false;
  }
  first = ( debugger()->hasError() || programmer()->state()==Programmer::Stopped );
  if (first) {
    log(Log::LineType::Normal, "--------------------------------------------------");
    debugger()->setCoff(_coff);
    if ( !debugger()->init() ) return false;
  }
  return true;
}

bool Debugger::Manager::init()
{
  if ( !internalInit() ) return false;
  return update(true);
}

bool Debugger::Manager::internalInit()
{
  clear();
  _inited = true;
  if ( !coffUrl().exists() ) return false;
  Log::Base log;
  log.setView(compileView());
  log.log(Log::LineType::Information, i18n("Parsing COFF file: %1").arg(coffUrl().pretty()));
  _coff = new Coff::TextObject(_data, coffUrl());
  if ( !_coff->parse(log) ) {
    delete _coff;
    _coff = NULL;
    return false;
  }
/*
  PURL::Url url = coffUrl().toExtension("adb");
  qDebug("%s", url.pretty().latin1());
  if ( url.exists() ) {
    log.log(Log::LineType::Information, i18n("Parsing CDB file: %1").arg(url.pretty()));
    CDB::Object *cdb = new CDB::Object(url, log);
    bool ok = cdb->parse();
    qDebug("parse ok: %i symbols=%i modules=%i", ok, cdb->nbRecords(CDB::Record::Symbol), cdb->nbRecords(CDB::Record::Module));
    delete cdb;
  }
*/
  computeBreakpointAddresses();
  return true;
}

void Debugger::Manager::clearBreakpoints()
{
  Breakpoint::list().clear();
}

void Debugger::Manager::freeActiveBreakpoint()
{
  uint nb = 0;
  Breakpoint::Data last;
  for (uint i=0; i<Breakpoint::list().count(); i++) {
    const Breakpoint::Data &data = Breakpoint::list().data(i);
    if ( Breakpoint::list().state(data)!=Breakpoint::Active ) continue;
    nb++;
    last = data;
  }
  uint max = programmerGroup()->maxNbBreakpoints(deviceData());
  ASSERT( nb<=max && max!=0 );
  if ( nb==max ) {
    log(Log::LineType::Warning, i18n("The number of active breakpoints is higher than the maximum for the current debugger (%1): disabling the last breakpoint.").arg(max));
    Breakpoint::list().setState(last, Breakpoint::Disabled);
  }
}

BitValue Debugger::Manager::pc() const
{
  return (debugger() ? debugger()->pc() : BitValue());
}

Breakpoint::MarkType Debugger::Manager::breakpointType(const Breakpoint::Data &data) const
{
  if ( _coff==NULL ) return Breakpoint::BreakpointDisabled;
  Address address = Breakpoint::list().address(data);
  if ( !address.isValid() ) return Breakpoint::BreakpointInvalid;
  if ( Breakpoint::list().state(data)!=Breakpoint::Active ) return Breakpoint::BreakpointDisabled;
  if ( address==pc() ) {
    if ( programmer()->state()==::Programmer::Halted ) return Breakpoint::BreakpointReached;
    return Breakpoint::ProgramCounterDisabled;
  }
  return Breakpoint::BreakpointActive;
}

bool Debugger::Manager::checkBreakpoint(const Breakpoint::Data &bdata, bool onlyWarn, Address &address)
{
  address = Address();
  if ( _coff==NULL ) return true;
  QValueVector<Address> addresses = _coff->addresses(bdata.url, bdata.line);
  if ( addresses.isEmpty() ) {
    QString s = i18n("Breakpoint at non-code line.");
    if (onlyWarn) log(Log::LineType::Warning, s);
    else sorry(s);
    return false;
  }
  if ( addresses.count()>1 ) log(Log::LineType::Warning, i18n("Breakpoint corresponds to several addresses. Using the first one."));
  address = addresses[0];
  return true;
}

void Debugger::Manager::computeBreakpointAddresses()
{
  if ( programmerGroup()==NULL ) return;
  int nb = programmerGroup()->maxNbBreakpoints(deviceData());
  for (int i=Breakpoint::list().count()-1; i>=0; i--) {
    const Breakpoint::Data &data = Breakpoint::list().data(i);
    Address address;
    checkBreakpoint(data, true, address);
    Breakpoint::list().setAddress(data, address);
    if ( _coff==NULL ) Breakpoint::list().setState(data, Breakpoint::Unknown);
    else if ( Breakpoint::list().address(data).isValid() && nb>0 ) {
      Breakpoint::list().setState(data, Breakpoint::Active);
      nb--;
    }
  }
}

QValueList<Address> Debugger::Manager::activeBreakpointAddresses() const
{
  QValueList<Address> addresses;
  for (uint i=0; i<Breakpoint::list().count(); i++) {
    const Breakpoint::Data &data = Breakpoint::list().data(i);
    if ( Breakpoint::list().state(data)==Breakpoint::Active ) addresses.append(Breakpoint::list().address(data));
  }
  return addresses;
}

void Debugger::Manager::clear()
{
  _runTimer.stop();
  _stepTimer.stop();
  if ( programmer() != NULL ) programmer()->clear();
  delete _coff;
  _coff = NULL;
  _inited = false;
  _currentSourceLines.clear();
  computeBreakpointAddresses();
  update(true);
}

bool Debugger::Manager::update(bool gotoPC)
{
  _readRegisters.clear();
  if ( !updateRegisters() ) return false;
  if ( debugger() ) emit statusChanged(debugger()->statusString());
  else emit statusChanged(QString::null);
  _currentSourceLines.clear();
  if (_coff) _currentSourceLines = _coff->sourceLinesForAddress(pc().toUInt());
  internalUpdateView(gotoPC);
  return true;
}

bool Debugger::Manager::updateRegister(const Register::TypeData &data)
{
  // read related registers
  const Device::RegistersData *rdata = deviceData()->registersData();
  ASSERT(rdata);
  QValueList<Register::TypeData> related = rdata->relatedRegisters(data);
  for (uint k=0; k<uint(related.count()); k++)
    if ( !readRegister(related[k]) ) return false;
  // read port status
  if ( data.type()==Register::Regular ) {
    int index = rdata->portIndex(data.address());
    if ( index!=-1 ) {
      QMap<uint, Device::PortBitData> data;
      if ( !debugger()->updatePortStatus(index, data) ) return false;
      Register::list().setPortData(index, data);
    }
  }
  return true;
}

bool Debugger::Manager::updateRegisters()
{
  if ( programmer()==NULL || programmer()->state()!=Programmer::Halted ) return true;
  QValueList<Register::TypeData> watched = Register::list().watched();
  for (uint i=0; i<uint(watched.count()); i++)
    if ( !updateRegister(watched[i]) ) return false;
  return true;
}

bool Debugger::Manager::run()
{
  _stepTimer.stop();
  _runTimer.stop();
  bool first;
  if ( !checkState(first) ) return false;
  if ( !debugger()->setBreakpoints(activeBreakpointAddresses()) ) return false;
  if ( !debugger()->run() ) return false;
  log(Log::LineType::Information, i18n("Running..."));
  if ( !update(true) ) return false;
  _runTimer.start(programmer()->runUpdateWait());
  return true;
}

bool Debugger::Manager::halt()
{
  _stepTimer.stop();
  _runTimer.stop();
  if ( !debugger()->halt() ) return false;
  return update(true);
}

bool Debugger::Manager::reset()
{
  _stepTimer.stop();
  _runTimer.stop();
  log(Log::LineType::Normal, "--------------------------------------------------");
  if ( !debugger()->reset() ) return false;
  return doStep(true, false);
}

bool Debugger::Manager::checkIfContinueStepping(bool &continueStepping)
{
  continueStepping = false;
  if ( !update(false) ) return false;
  if ( !readConfigEntry(Config::OnlyStopOnSourceLine).toBool() ) return true;
  QMap<PURL::Url, uint>::const_iterator it;
  for (it=_currentSourceLines.begin(); it!=_currentSourceLines.end(); ++it) {
    PURL::FileGroup group = it.key().fileType().data().group;
    if ( group!=PURL::Source && group!=PURL::Header ) continue;
    if ( !it.key().exists() ) continue;
    if ( readConfigEntry(Config::OnlyStopOnProjectSourceLine).toBool() && !isProjectSource(it.key()) ) continue;
    QValueVector<Address> addresses = _coff->addresses(it.key(), it.data());
    qSort(addresses);
    ASSERT( addresses.count()!=0 );
    if ( pc()!=addresses[0] ) continue; // we only break if pc is on the first instruction of the source line
    break;
  }
  continueStepping = ( it==_currentSourceLines.end() );
  return true;
}

bool Debugger::Manager::doStep(bool first, bool continued)
{
  if ( programmer()->state()!=Programmer::Halted ) return true; // has been stopped
  if ( continued && !_stepTimer.isActive() ) return true; // has been stopped
  _stepTimer.stop();
  if ( !first && !debugger()->step() ) return false;
  bool continueStepping;
  if ( !checkIfContinueStepping(continueStepping) ) return false;
  if (continueStepping) _stepTimer.start(0);
  else internalUpdateView(true);
  return true;
}

bool Debugger::Manager::step()
{
  bool first;
  if ( !checkState(first) ) return false;
  if ( !debugger()->setBreakpoints(activeBreakpointAddresses()) ) return false;
  log(Log::LineType::Information, i18n("Step"));
  programmer()->setState(Programmer::Halted);
  return doStep(first, false);
}

void Debugger::Manager::slotRunTimeout()
{
  if ( programmer()->state()!=Programmer::Running ) return; // has been stopped
  if ( !_runTimer.isActive() ) return; // has been stopped
  _runTimer.stop();
  if ( !debugger()->update() ) return;
  if ( programmer()->state()==Programmer::Running ) {
    _runTimer.start(programmer()->runUpdateWait());
    return;
  }
  log(Log::LineType::Information, i18n("Reached breakpoint."));
  update(true);
  emit targetStateChanged();
}

void Debugger::Manager::setRegisterWatched(const Register::TypeData &data, bool watched)
{
  if (watched) {
    if ( Register::list().isWatched(data) ) return;
    Register::list().setWatched(data, true);
    if ( programmer() && programmer()->state()==Programmer::Halted ) updateRegister(data);
  } else Register::list().setWatched(data, false);
}

bool Debugger::Manager::readRegister(const Register::TypeData &data)
{
  ASSERT( data.type()==Register::Regular || data.type()==Register::Special );
  if ( _readRegisters.contains(data) ) return true;
  BitValue value;
  if ( !debugger()->readRegister(data, value) ) return false;
  Register::list().setValue(data, value);
  _readRegisters.append(data);
  return true;
}

bool Debugger::Manager::writeRegister(const Register::TypeData &data, BitValue value)
{
  ASSERT( data.type()==Register::Regular || data.type()==Register::Special );
  if ( !debugger()->writeRegister(data, value) ) return false;
  _readRegisters.clear();
  if ( !updateRegister(data) ) return false;
  emit statusChanged(debugger()->statusString());
  return true;
}

bool Debugger::Manager::readAllRegisters()
{
  const Device::RegistersData *rdata = _data->registersData();
  for (uint i=0; i<rdata->nbRegisters(); i++) {
    Register::TypeData rtd(rdata->addressFromIndex(i), rdata->nbChars());
    if ( !updateRegister(rtd) ) return false;
  }
  return true;
}

void Debugger::Manager::stopWatchAll()
{
  Register::list().clearWatched();
}

bool Debugger::Manager::prepareDebugging()
{
  if ( programmerGroup()->isSoftware() && programmerGroup()->isDebugger() ) {
    PURL::Url curl = coffUrl();
    if ( curl.isEmpty() ) {
      log(Log::LineType::Error, i18n("Cannot start debugging session without input file (not specified)."));
      return false;
    }
    PURL::Url first;
    uint i = 0;
    for (; i<Programmer::Nb_InputFileTypes; i++) {
      PURL::FileType type = Programmer::INPUT_FILE_TYPE_DATA[i];
      if ( !programmerGroup()->isInputFileTypeSupported(type) ) continue;
      PURL::Url url = curl.toFileType(type);
      if ( first.isEmpty() ) first = url;
      if ( !url.exists() ) continue;
      debugger()->setupInput(type, url.directory().path(), url.filename());
      break;
    }
    if ( i==Programmer::Nb_InputFileTypes ) {
      log(Log::LineType::Error, i18n("Cannot start debugging session without input file (%1).").arg(first.pretty()));
      return false;
    }
  }
  return true;
}
