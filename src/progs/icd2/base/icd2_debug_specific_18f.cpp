/***************************************************************************
 *   Copyright (C) 2005-2007 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "icd2_debug_specific_18f.h"

#include "devices/pic/base/pic_register.h"
#include "icd2_data.h"

//----------------------------------------------------------------------------
Icd2::P18FDebuggerSpecific::P18FDebuggerSpecific(::Debugger::Base &base)
  : DebuggerSpecific(base)
{
  const Pic::RegistersData &rdata = device()->registersData();
  // find last used bank (but not #15)
  _reservedBank = rdata.nbBanks - 1; // #14
  for (uint i=1; i<rdata.nbBanks-1; i++) {
    if ( rdata.isBankUsed(i) ) continue;
    _reservedBank = i - 1;
    break;
  }
  // check it's not a bank for CAN
  for (; _reservedBank>0; _reservedBank--)
    if ( !rdata.bankHasSfrs(_reservedBank) ) break;
  // also take care of USB RAM
  if ( (device()->architecture()==Pic::Architecture::P18F || device()->architecture()==Pic::Architecture::P18J)
       && device()->hasFeature(Pic::Feature::USB) ) {
    if ( _reservedBank==7 ) _reservedBank = 3; // 18F2455 family: 4 USB RAM banks
    // 18F87J50 family ?
  }
}

Address Icd2::P18FDebuggerSpecific::addressWREG()const
{
  return reservedRegisterOffset() | 0x0FF;
}

Address Icd2::P18FDebuggerSpecific::addressRegister(Address address) const
{
  QString name = device()->registersData().sfrNames[address];
  if ( name=="PCLATU" ) return reservedRegisterOffset() | 0x0F4;
  if ( name=="PCLATH" ) return reservedRegisterOffset() | 0x0F5;
  if ( name=="FSR0H" ) return reservedRegisterOffset() | 0x0FB;
  if ( name=="FSR0L" ) return reservedRegisterOffset() | 0x0FC;
  if ( name=="BSR" ) return reservedRegisterOffset() | 0x0FD;
  if ( name=="STATUS" ) return reservedRegisterOffset() | 0x0FE;
  return address;
}

bool Icd2::P18FDebuggerSpecific::setBreakpoint(Address address)
{
  BitValue value = 0x0FFFFF00;
  if (address.isValid()) {
     value = address.toUInt() >> 1;
     value = value.maskWith(0x0000FF00) | (value.maskWith(0x000000FF) << 16) | 0x00000040;
  }
  return hardware()->writeRegister(0xFB6, value, 4);
}

bool Icd2::P18FDebuggerSpecific::readBreakpoint(BitValue &value)
{
  if ( !hardware()->readRegister(0xFB6, value, 4) ) return false;
  value = value.maskWith(0x0000FF00) | (value.maskWith(0x00FF0000) >> 16);
  value = value << 1;
  return true;
}

bool Icd2::P18FDebuggerSpecific::reset()
{
  if ( !hardware()->writeRegister(0xFB5, 0x00, 1) ) return false; // #### ??
  if ( !hardware()->writeRegister(0xFB5, 0x01, 1) ) return false; // #### ??
  if ( !hardware()->command("2D", 0) ) return false; // reset
  if ( !hardware()->writeRegister(0xFB5, 0x00, 1) ) return false; // #### ??
  if ( !debugger().update() ) return false;
  BitValue expectedPC = 0x0000;
  if ( debugger().pc()==0x0001) {
    expectedPC = 0x0001;
    log(Log::LineType::Information, i18n("Detected custom ICD2"));
  }
  if ( debugger().pc()!=expectedPC ) {
    log(Log::LineType::Error, i18n("  PC is not at address %1 (%2)").arg(toHexLabel(expectedPC, 4)).arg(toHexLabel(debugger().pc(), 4)));
    return false;
  }
  return true;
}

bool Icd2::P18FDebuggerSpecific::init(bool)
{
  if ( !hardware()->setTargetReset(Device::ResetMode::Released) ) return false;
  if ( !debugger().waitForTargetMode(Device::TargetMode::Stopped) ) return false;
  return reset();
}

QString Icd2::P18FDebuggerSpecific::debugExecutiveFilename() const
{
  return QString("de18F_BANK%1.hex").arg(QString(toString(NumberBase::Dec, _reservedBank, 2)));
}

void Icd2::P18FDebuggerSpecific::initDebugExecutiveArray(Pic::Memory& memory)
{
  // that's a bit ugly but it cannot be guessed for 18F2455 family...
  uint size = 0x120;
  switch (_reservedBank) {
    case 0:  size = 0x0E0; break;
    case 12:
    case 14: size = 0x140; break;
    default: break;
  }
  const uint nbWords = device()->nbWords(Pic::MemoryRangeType::Code);
  _deStart = nbWords - size;
  _deEnd = nbWords - 1;
  for (uint i=0; i<size; i++) {
    const BitValue v = memory.word(Pic::MemoryRangeType::Code, i);
    memory.setWord(Pic::MemoryRangeType::Code, i, BitValue());
    memory.setWord(Pic::MemoryRangeType::Code, _deStart+i, v);
  }
  _deArray = memory.arrayForWriting(Pic::MemoryRangeType::Code);
}
