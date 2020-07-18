/***************************************************************************
 *   Copyright (C) 2005-2007 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "icd2_debug_specific_16f.h"

#include "devices/pic/base/pic_register.h"
#include "icd2_data.h"

//----------------------------------------------------------------------------
Address Icd2::P16FDebuggerSpecific::addressWREG() const
{
  return FAMILY_DATA[family(device()->name())].wreg;
}

Address Icd2::P16FDebuggerSpecific::addressRegister(Address address) const
{
  QString name = device()->registersData().sfrNames[address];
  if ( name=="FSR" ) return FAMILY_DATA[family(device()->name())].fsr;
  if ( name=="STATUS" ) return FAMILY_DATA[family(device()->name())].status;
  return address;
}

bool Icd2::P16FDebuggerSpecific::reset()
{
  if ( !hardware()->setTargetReset(Device::ResetMode::Held) ) return false;
  return init(true);
}

bool Icd2::P16FDebuggerSpecific::setBreakpoint(Address address)
{
  if ( !address.isValid() ) address = 0x1FFF; // outside memory range
  BitValue value = address.toUInt() | 0x8000; // read-only INBUG bit
  return hardware()->writeRegister(0x18E, value, 2);
}

bool Icd2::P16FDebuggerSpecific::readBreakpoint(BitValue &value)
{
  if ( !hardware()->readRegister(0x18E, value, 2) ) return false;
  value = value.maskWith(0x1FFF);
  return true;
}

bool Icd2::P16FDebuggerSpecific::beginInit(Device::Array *saved)
{
  if ( !hardware()->setTargetReset(Device::ResetMode::Held) ) return false;
  double vdd;
  if ( !hardware()->readVoltage(Device::VoltageType::TargetVdd, vdd) ) return false;
  log(Log::DebugLevel::Normal, QString("  Target Vdd: %1 V").arg(vdd));

  if (saved) {
    uint nbWords = device()->nbWordsRowEraseAlignment(Pic::MemoryRangeType::Code);
    saved->resize(nbWords);
    if ( !hardware()->readMemory(Pic::MemoryRangeType::Code, 0, *saved, 0) ) return false; // save first instruction
    if ( (*saved)[0]!=device()->nopInstruction() ) log(Log::LineType::Warning, i18n("  According to ICD2 manual, instruction at address 0x0 should be \"nop\"."));
  }

  return true;
}

bool Icd2::P16FDebuggerSpecific::writeGotoAtResetVector(const Device::Array &saved)
{
  Device::Array data = saved;
  Device::Array gotoData = device()->gotoInstruction(0x0000, false); // loop at reset vector
  for (uint i=0; i<gotoData.size(); ++i) {
    data[i] = gotoData[i];
  }
  return hardware()->writeMemory(Pic::MemoryRangeType::Code, 0, data);
}

bool Icd2::P16FDebuggerSpecific::endInit(BitValue expectedPC)
{
  if ( !hardware()->setTargetReset(Device::ResetMode::Released) ) return false;
  if ( !debugger().waitForTargetMode(Device::TargetMode::Stopped) ) return false;
  BitValue value;
  if ( !readBreakpoint(value) ) return false;
  if ( value==expectedPC+1 ) {
    // #### happen for some custom icd2 or sometimes when we had to force a halt (?)
    expectedPC = expectedPC+1;
    //log(Log::LineType::Information, i18n("Detected custom ICD2"));
  }
  if ( value!=expectedPC ) {
    log(Log::LineType::Error, i18n("  PC is not at address %1 (%2)").arg(toHexLabel(expectedPC, 4)).arg(toHexLabel(value, 4)));
    return false;
  }
  if ( !setBreakpoint(0x0000) ) return false;

  if ( !debugger().update() ) return false;
  if ( debugger().pc()!=expectedPC ) {
    log(Log::LineType::Error, i18n("  PC is not at address %1 (%2)").arg(toHexLabel(expectedPC, 4)).arg(toHexLabel(debugger().pc(), 4)));
    return false;
  }
  return true;
}

QString Icd2::P16FDebuggerSpecific::debugExecutiveFilename() const
{
  const FamilyData &fdata = FAMILY_DATA[family(device()->name())];
  return QString("de%1.hex").arg(fdata.debugExec);
}

//----------------------------------------------------------------------------
bool Icd2::P16F872DebuggerSpecific::init(bool)
{
  Device::Array saved;
  if ( !beginInit(&saved) ) return false;

  // this seems to be needed
  if ( !hardware()->setTargetReset(Device::ResetMode::Released) ) return false;
  Device::TargetMode mode;
  if ( !programmer().getTargetMode(mode) ) return false;

  if ( !hardware()->setTargetReset(Device::ResetMode::Held) ) return false;
  if ( !hardware()->setWriteMode(Device::WriteMode::EraseWrite) ) return false;
  log(Log::DebugLevel::Normal, "  Write \"goto 0x0\" at reset vector and run target.");
  if ( !writeGotoAtResetVector(saved) ) return false;
  if ( !hardware()->setTargetReset(Device::ResetMode::Released) ) return false; // run device
  if ( !debugger().waitForTargetMode(Device::TargetMode::Running) ) return false;
  log(Log::DebugLevel::Normal, "  Try to halt target.");
  if ( !hardware()->haltRun() ) return false;
  if ( !debugger().waitForTargetMode(Device::TargetMode::Stopped) ) return false;
  if ( !programmer().readDebugExecutiveVersion() ) return false;
  log(Log::DebugLevel::Normal, "  Set breakpoint at reset vector.");
  if ( !setBreakpoint(0x0000) ) return false;
  if ( !hardware()->setTargetReset(Device::ResetMode::Held) ) return false;
  if ( !debugger().waitForTargetMode(Device::TargetMode::InProgramming) ) return false;
  log(Log::DebugLevel::Normal, "  Restore instruction at reset vector and run target.");
  if ( !hardware()->writeMemory(Pic::MemoryRangeType::Code, 0, saved) ) return false; // restore instruction
  if ( !hardware()->setWriteMode(Device::WriteMode::WriteOnly) ) return false;

  return endInit(0x0001);
}

//----------------------------------------------------------------------------
bool Icd2::P16F87XDebuggerSpecific::init(bool)
{
  Device::Array saved;
  if ( !beginInit(&saved) ) return false;

  if ( !hardware()->setTargetReset(Device::ResetMode::Held) ) return false;
  if ( !hardware()->setWriteMode(Device::WriteMode::EraseWrite) ) return false;
  log(Log::DebugLevel::Normal, "  Write \"goto 0x0\" at reset vector and run target.");
  if ( !writeGotoAtResetVector(saved) ) return false;
  if ( !hardware()->setTargetReset(Device::ResetMode::Released) ) return false; // run device
  Device::TargetMode mode;
  if ( !programmer().getTargetMode(mode) ) return false;
  if ( mode==Device::TargetMode::Running && !hardware()->haltRun() ) return false;
  if ( !debugger().waitForTargetMode(Device::TargetMode::Stopped) ) return false;
  if ( !programmer().readDebugExecutiveVersion() ) return false;
  log(Log::DebugLevel::Normal, "  Set breakpoint at reset vector.");
  if ( !setBreakpoint(0x0000) ) return false;
  if ( !hardware()->setTargetReset(Device::ResetMode::Held) ) return false;
  if ( !debugger().waitForTargetMode(Device::TargetMode::InProgramming) ) return false;
  log(Log::DebugLevel::Normal, "  Restore instruction at reset vector and run target.");
  if ( !hardware()->writeMemory(Pic::MemoryRangeType::Code, 0, saved) ) return false; // restore instruction
  if ( !hardware()->setWriteMode(Device::WriteMode::WriteOnly) ) return false;

  return endInit(0x0001);
}

//----------------------------------------------------------------------------
bool Icd2::P16F7X7DebuggerSpecific::init(bool last)
{
  Device::Array saved;
  if ( !beginInit(last ? &saved : 0) ) return false;

  log(Log::DebugLevel::Normal, "  Run target.");
  if ( !hardware()->setTargetReset(Device::ResetMode::Released) ) return false;
  if ( !debugger().waitForTargetMode(Device::TargetMode::Stopped) ) return false;
  if ( !programmer().readDebugExecutiveVersion() ) return false;

  BitValue value;
  if ( !readBreakpoint(value) ) return false;
  BitValue expectedPC = (last ? 0x0001 : 0x0000);
  if ( value==expectedPC+1 ) {
    expectedPC = expectedPC+1;
    log(Log::DebugLevel::Normal, "Probably detected custom ICD2");
  }
  if ( value!=expectedPC )
    log(Log::DebugLevel::Normal, i18n("  PC is not at address %1 (%2)").arg(toHexLabel(expectedPC, 4)).arg(toHexLabel(value, 4)));
  if ( !setBreakpoint(0x0000) ) return false;

  if ( !debugger().update() ) return false;
  // #### not sure if there is a better way to get initial values (we are stopped here...)
  Register::list().setValue(debugger().pcTypeData(), value);
  Register::list().setValue(debugger().registerTypeData("STATUS"), 0x0);
  Register::list().setValue(deviceSpecific()->wregTypeData(), 0x0);

  return true;
}
