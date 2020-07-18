/***************************************************************************
 *   Copyright (C) 2005-2007 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "icd2_debug.h"

#include "common/global/pfile.h"
#include "progs/base/prog_config.h"
#include "devices/pic/base/pic_register.h"
#include "devices/pic/pic/pic_group.h"
#include "icd2_data.h"
#include "icd2_debug_specific_16f.h"
#include "icd2_debug_specific_18f.h"
#include "icd2_debug_specific_24f.h"

//-----------------------------------------------------------------------------
Icd2::Debugger::Debugger(DebugProgrammer& programmer)
: ::Debugger::PicBase(programmer)
{}

Icd2::DebugProgrammer& Icd2::Debugger::programmer() const
{
  return static_cast<DebugProgrammer&>(_programmer);
}

Icd2::DebuggerSpecific *Icd2::Debugger::specific() const
{
  return static_cast<Icd2::DebuggerSpecific *>(_specific);
}

bool Icd2::Debugger::waitForTargetMode(Device::TargetMode mode)
{
  Device::TargetMode rmode;
  for (uint i=0; i<20; i++) {
    if ( !programmer().getTargetMode(rmode) ) return false;
    if ( rmode==mode ) return true;
    Port::msleep(200);
  }
  log(Log::LineType::Error, QString("Timeout waiting for mode: %1 (target is in mode: %2).").arg(mode.label()).arg(rmode.label()));
  return false;
}

bool Icd2::Debugger::init(bool last)
{
  _initLast = last;
  return ::Debugger::PicBase::init();
}

bool Icd2::Debugger::internalInit()
{
  return specific()->init(_initLast);
}

bool Icd2::Debugger::updateState()
{
  Device::TargetMode mode;
  if ( !programmer().getTargetMode(mode) ) return false;
  switch (mode.type()) {
    case Device::TargetMode::Stopped: _programmer.setState(::Programmer::Halted); break;
    case Device::TargetMode::Running: _programmer.setState(::Programmer::Running); break;
    case Device::TargetMode::InProgramming: _programmer.setState(::Programmer::Stopped); break;
    case Device::TargetMode::Nb_Types: ASSERT(false); break;
  }
  return true;
}

bool Icd2::Debugger::setBreakpoints(const QValueList<Address> &addresses)
{
  if ( addresses.count()==0 ) return specific()->setBreakpoint(Address());
  for (uint i=0; i<uint(addresses.count()); i++) {
    log(Log::DebugLevel::Normal, QString("Set breakpoint at %1").arg(toHexLabel(addresses[i], device()->nbCharsAddress())));
    if ( !specific()->setBreakpoint(addresses[i]) ) return false;
  }
  return true;
}

bool Icd2::Debugger::internalRun()
{
  return hardware()->resumeRun();
}

bool Icd2::Debugger::hardHalt()
{
  log(Log::LineType::Warning, i18n("Failed to halt target: try a reset."));
  return reset();
}

bool Icd2::Debugger::softHalt(bool &success)
{
  if ( !hardware()->haltRun() ) return false;
  success = waitForTargetMode(Device::TargetMode::Stopped);
  return true;
}

bool Icd2::Debugger::internalReset()
{
  return specific()->reset();
}

bool Icd2::Debugger::internalStep()
{
  return hardware()->step();
}

bool Icd2::Debugger::readRegister(const Register::TypeData &data, BitValue &value)
{
  if ( data.type()==Register::Special ) {
    if ( data.name()=="WREG" ) return hardware()->readRegister(specific()->addressWREG(), value, 1);
    if ( data.name()=="PC" ) { value = hardware()->getProgramCounter().maskWith(specific()->maskPC()); return !hasError(); }
    ASSERT(false);
    return true;
  }
  QString name = device()->registersData().sfrNames[data.address()];
  if ( name=="WREG" ) return hardware()->readRegister(specific()->addressWREG(), value, 1);
  if ( name=="PCL" ) { value = hardware()->getProgramCounter().maskWith(specific()->maskPC()).byte(0); return !hasError(); }
  if ( name=="PCLATH" ) { value = hardware()->getProgramCounter().maskWith(specific()->maskPC()).byte(1); return !hasError(); }
  return hardware()->readRegister(specific()->addressRegister(data.address()), value, 1);
}

bool Icd2::Debugger::writeRegister(const Register::TypeData &data, BitValue value)
{
  if ( data.type()==Register::Special ) {
    if ( data.name()=="WREG" ) return hardware()->writeRegister(specific()->addressWREG(), value, 1);
    ASSERT(false);
    return true;
  }
  QString name = device()->registersData().sfrNames[data.address()];
  if ( name=="WREG" ) return hardware()->writeRegister(specific()->addressWREG(), value, 1);
  return hardware()->writeRegister(specific()->addressRegister(data.address()), value, 1);
}

//-----------------------------------------------------------------------------
Icd2::DebugProgrammer::DebugProgrammer(const ::Programmer::Group &group, const Pic::Data *data)
  : Icd2::ProgrammerBase(group, data, "icd2_programmer")
{}

void Icd2::DebugProgrammer::clear()
{
  Icd2::ProgrammerBase::clear();
  _debugExecutiveVersion.clear();
}

bool Icd2::DebugProgrammer::internalSetupHardware()
{
  if (deviceSpecific() == NULL) return true;
  return debugger()->specific()->setupHardware()
         && Icd2::ProgrammerBase::internalSetupHardware();
}

bool Icd2::DebugProgrammer::doProgram(const Device::Memory &memory, const Device::MemoryRange &range)
{
  if ( !checkProgram(memory) ) return false;
  if ( !doConnectDevice() ) return false;
  _progressMonitor.startNextTask();
   // probably needed for all devices that don't have a "erase and write" mode
  if ( range.all() && FAMILY_DATA[family(device()->name())].debugExec==QString("16F7X7") ) {
     Pic::Memory dmemory(*device());
     dmemory.setWord(Pic::MemoryRangeType::Code, 0, 0x0028);
     dmemory.setWord(Pic::MemoryRangeType::Code, 1, 0x0030);
     log(Log::LineType::Information, i18n("Programming device for debugging test..."));
     if ( !internalProgram(dmemory, range) ) return false;
     if ( !debugger()->init(false) ) return false;
     log(Log::LineType::Information, i18n("Debugging test successful"));
  }
  log(Log::LineType::Information, i18n("Programming device memory..."));
  if ( !internalProgram(memory, range) ) return false;
  log(Log::LineType::Information, i18n("Programming successful"));
  return debugger()->init(true);
}

bool Icd2::DebugProgrammer::programAll(const Pic::Memory &mem)
{
  std::auto_ptr<Pic::Memory> memory = mem.clone();
  debugger()->specific()->toDebugMemory(*memory, false);
  if ( !programAndVerifyRange(Pic::MemoryRangeType::Code, *memory) ) return false;
  if ( !debugger()->specific()->writeDebugExecutive() ) return false;
  if ( !programAndVerifyRange(Pic::MemoryRangeType::DebugVector, *memory) ) return false;
  if ( !programAndVerifyRange(Pic::MemoryRangeType::Eeprom, *memory) ) return false;
  if ( !programAndVerifyRange(Pic::MemoryRangeType::UserId, *memory) ) return false;
  if ( device()->is18Family() ) {
    if ( !hardware()->command("0C00", 0) ) return false; // #### ??
    QString com = "42" + toHex(0xFB5, 8) + toHex(1, 8); // write RSBUG (?)
    if ( !hardware()->command(com, 0) ) return false;
    if ( !hardware()->command("0C01", 0) ) return false; // #### ??
  }
  if ( !programAndVerifyRange(Pic::MemoryRangeType::Config, *memory) ) return false;
  return true;
}

bool Icd2::DebugProgrammer::internalRead(Device::Memory *mem, const Device::MemoryRange &range, const ::Programmer::VerifyData *vd)
{
  if ( vd==0 || (vd->actions & ::Programmer::BlankCheckVerify) ) return Icd2::ProgrammerBase::internalRead(mem, range, vd);
  ::Programmer::VerifyData vdata(*vd);
  debugger()->specific()->toDebugMemory(static_cast<Pic::Memory&>(vdata.memory()), true);
  if ( !Icd2::ProgrammerBase::internalRead(0, range, &vdata) ) return false;
  if ( range.all() && !readRange(Pic::MemoryRangeType::DebugVector, 0, &vdata) ) return false;
  return true;
}

bool Icd2::DebugProgrammer::readDebugExecutiveVersion()
{
  if ( !hardware()->getDebugExecVersion(_debugExecutiveVersion) ) return false;
  log(Log::LineType::Information, i18n("  Debug executive version: %1").arg(_debugExecutiveVersion.pretty()));
  return true;
}

//----------------------------------------------------------------------------
void Icd2::DebuggerGroup::addDevice(const QString &name, const Device::Data *ddata, ::Group::Support)
{
  if ( FAMILY_DATA[family(name)].debugExec==NULL ) return;
  Group::addDevice(name, ddata, data(name).debugSupport);
}

::Debugger::Specific *Icd2::DebuggerGroup::createDebuggerSpecific(::Debugger::Base &base) const
{
  const Pic::Data *data = static_cast< ::Debugger::PicBase &>(base).device();
  if ( data==0 ) return 0;
  QString debugExec = FAMILY_DATA[family(data->name())].debugExec;
  switch (data->architecture().type()) {
    case Pic::Architecture::MidRange:
    case Pic::Architecture::EMidRange:
      if ( debugExec=="16F872" ) return new P16F872DebuggerSpecific(base);
      if ( debugExec=="16F7X7" ) return new P16F7X7DebuggerSpecific(base);
      return new P16F87XDebuggerSpecific(base);
    case Pic::Architecture::P17C:
    case Pic::Architecture::P18C:
    case Pic::Architecture::P18F:
    case Pic::Architecture::P18J: return new P18FDebuggerSpecific(base);
    case Pic::Architecture::P24F:
    case Pic::Architecture::P24H:
    case Pic::Architecture::P33F: return new P24FDebuggerSpecific(base);
    case Pic::Architecture::Baseline:
    case Pic::Architecture::P30F:
    case Pic::Architecture::Nb_Types: break;
  }
  ASSERT(false);
  return NULL;
}
