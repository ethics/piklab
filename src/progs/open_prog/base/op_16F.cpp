/***************************************************************************
 *   Copyright (C) 2010 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "op_16F.h"

#include "op_code.h"

//-----------------------------------------------------------------------------
void OpenProg::P16F::gotoTestMemoryArray(Array& cmd) const
{
  // PC set at 0x2000
  cmd += LOAD_CONF;
  cmd += 0xFF; // 3F ?
  cmd += 0xFF;
}

bool OpenProg::P16F::gotoMemory(Pic::MemoryRangeType type)
{
  Array cmd;
  switch (type.type()) {
    case Pic::MemoryRangeType::Code:     return true;
    case Pic::MemoryRangeType::Eeprom:   return true;
    case Pic::MemoryRangeType::UserId:   gotoTestMemoryArray(cmd); break;
    case Pic::MemoryRangeType::DeviceId: gotoTestMemoryArray(cmd); incrementPCArray(cmd, 6); break;
    case Pic::MemoryRangeType::Config:   gotoTestMemoryArray(cmd); incrementPCArray(cmd, 7); break;
    case Pic::MemoryRangeType::Cal:
      if ( device().range(type).start==device().range(Pic::MemoryRangeType::Code).end+1 )
        return incrementPC(device().range(type).start.toUInt());
      gotoTestMemoryArray(cmd); incrementPCArray(cmd, 8); break;
    case Pic::MemoryRangeType::CalBackup:
    case Pic::MemoryRangeType::DebugVector:
    case Pic::MemoryRangeType::HardwareStack:
    case Pic::MemoryRangeType::ProgramExecutive:
    case Pic::MemoryRangeType::Nb_Types: break;
  }
  return hardware().command(cmd);
}

//-----------------------------------------------------------------------------
void OpenProg::P16F8X::eraseArray(Array& cmd) const
{
    setT3Array(cmd, 10); // 10 ms
    cmd += CUST_CMD;
    cmd += 0x1;
    cmd += CUST_CMD;
    cmd += 0x7;
    cmd += BEGIN_PROG;
    cmd += WAIT_T3;
    cmd += CUST_CMD;
    cmd += 0x1;
    cmd += CUST_CMD;
    cmd += 0x7;
}

bool OpenProg::P16F8X::doEraseRange(Pic::MemoryRangeType type)
{
  if ( type==Pic::MemoryRangeType::Code ) {
    Array cmd;
    cmd += LOAD_DATA_PROG;
    cmd += 0x3F;
    cmd += 0xFF;
    eraseArray(cmd);
    return hardware().command(cmd, 40); // 40 ms
  }
  if ( type==Pic::MemoryRangeType::Eeprom ) {
    Array cmd;
    cmd += LOAD_DATA_DATA;
    cmd += 0x3F;
    cmd += 0xFF;
    eraseArray(cmd);
    return hardware().command(cmd, 40); // 40 ms
  }
  return false;
}

bool OpenProg::P16F8X::doErase(bool isProtected)
{
  if (isProtected) { // disable code protection + erase code and data
    Array cmd;
    gotoTestMemoryArray(cmd);
    eraseArray(cmd);
    return hardware().command(cmd, 40); // 40 ms
  } else {
    doEraseRange(Pic::MemoryRangeType::Code);
    doEraseRange(Pic::MemoryRangeType::Eeprom);
  }
  doEmulatedEraseRange(Pic::MemoryRangeType::UserId);
  doEmulatedEraseRange(Pic::MemoryRangeType::Config);
  return true;
}

//-----------------------------------------------------------------------------
uint OpenProg::P16F84A::waitProgTime(Pic::MemoryRangeType type) const
{
  if ( type==Pic::MemoryRangeType::Config || type==Pic::MemoryRangeType::UserId ) return 4000 + 4000; // prog +erase
  return 4000;
}

void OpenProg::P16F84A::startProgArray(Array& cmd, Pic::MemoryRangeType type) const
{
  if ( type==Pic::MemoryRangeType::Code || type==Pic::MemoryRangeType::Eeprom ) cmd += BEGIN_PROG2;
  else cmd += BEGIN_PROG;
}

bool OpenProg::P16F84A::doEraseRange(Pic::MemoryRangeType type)
{
  if ( type==Pic::MemoryRangeType::Code ) {
    Array cmd;
    setT3Array(cmd, 10000); // T3 = 10 ms
    cmd += LOAD_DATA_PROG;
    cmd += 0x3F;
    cmd += 0xFF;
    cmd += BULK_ERASE_PROG;
    cmd += BEGIN_PROG;
    cmd += WAIT_T3;
    return hardware().command(cmd, 40); // 40 ms
  }
  if ( type==Pic::MemoryRangeType::Eeprom ) {
    Array cmd;
    setT3Array(cmd, 10000); // T3 = 10 ms
    cmd += LOAD_DATA_DATA;
    cmd += 0x3F;
    cmd += 0xFF;
    cmd += BULK_ERASE_PROG;
    cmd += BEGIN_PROG;
    cmd += WAIT_T3;
    return hardware().command(cmd, 40); // 40 ms
  }
  return false;
}

//-----------------------------------------------------------------------------
uint OpenProg::P16F7X::waitProgTime(Pic::MemoryRangeType) const
{
  if ( device().name()=="16F72" ) return 3000;
  return 1000;
}

void OpenProg::P16F7X::endProgArray(Array& cmd, Pic::MemoryRangeType) const
{
  cmd += END_PROG2;
}

bool OpenProg::P16F7X::doEraseRange(Pic::MemoryRangeType type)
{
  if ( type!=Pic::MemoryRangeType::Code ) return false;
  Device::Array array;
  if ( !doRead(Pic::MemoryRangeType::Config, 0, array, 0) ) return false;
  Array cmd;
  setT3Array(cmd, 40000); // 40 ms
  cmd += BULK_ERASE_PROG; // chip erase (should only need 30ms according to prog sheet)
  cmd += WAIT_T3;
  return hardware().command(cmd, 40) // 40 ms
         && doWrite(Pic::MemoryRangeType::Config, 0, array, true);
}

bool OpenProg::P16F7X::doErase(bool)
{
  Array cmd;
  setT3Array(cmd, 40000); // 40 ms
  cmd += BULK_ERASE_PROG; // chip erase (should only need 30ms according to prog sheet)
  cmd += WAIT_T3;
  return hardware().command(cmd, 40)
         && doEmulatedEraseRange(Pic::MemoryRangeType::UserId);
}

//-----------------------------------------------------------------------------
// 16F628 seems to have problems with the standard 16F84 bulk
// erase when disabling code protection : the data memory is not set to 0xFF.
// This code adds a erase/programming pass on the data memory
bool OpenProg::P16F62X::doErase(bool isProtected)
{
  P16F84A::doErase(isProtected);
  if (isProtected) return doEmulatedEraseRange(Pic::MemoryRangeType::Eeprom);
  return true;
}

//-----------------------------------------------------------------------------
void OpenProg::P16F81X::startProgArray(Array& cmd, Pic::MemoryRangeType) const
{
  cmd += BEGIN_PROG2;
}

void OpenProg::P16F81X::endProgArray(Array& cmd, Pic::MemoryRangeType) const
{
  cmd += CUST_CMD;
  cmd += 0x17;
}

bool OpenProg::P16F81X::doEraseRange(Pic::MemoryRangeType type)
{
  if ( type==Pic::MemoryRangeType::Code ) {
    Array cmd;
    setT3Array(cmd, 2000); // 2 ms
    cmd += BULK_ERASE_PROG;
    cmd += WAIT_T3;
    return hardware().command(cmd, 40); // 40 ms
  }
  if ( type==Pic::MemoryRangeType::Eeprom ) {
    Array cmd;
    setT3Array(cmd, 2000); // 2 ms
    cmd += BULK_ERASE_DATA;
    cmd += WAIT_T3;
    return hardware().command(cmd, 40); // 40 ms
  }
  return false;
}

bool OpenProg::P16F81X::doErase(bool)
{
    Array cmd;
    setT3Array(cmd, 2000); // 2 ms
    cmd += CUST_CMD;
    cmd += 0x1F; // CHIP_ERASE
    cmd += WAIT_T3;
    return hardware().command(cmd, 40); // 40 ms
}

//-----------------------------------------------------------------------------
uint OpenProg::P12F675::waitProgTime(Pic::MemoryRangeType type) const
{
  if ( type==Pic::MemoryRangeType::Eeprom ) return 6000;
  return 2500;
}

bool OpenProg::P12F675::doEraseRange(Pic::MemoryRangeType type)
{
  if ( type==Pic::MemoryRangeType::Code ) {
    Array cmd;
    setT3Array(cmd, 9000); // 9 ms
    cmd += BULK_ERASE_PROG;
    cmd += WAIT_T3;
    return hardware().command(cmd, 40); // 40 ms
  }
  if ( type==Pic::MemoryRangeType::Eeprom ) {
    Array cmd;
    setT3Array(cmd, 9000); // 9 ms
    cmd += BULK_ERASE_DATA;
    cmd += WAIT_T3;
    return hardware().command(cmd, 40); // 40 ms
  }
  return false;
}

bool OpenProg::P12F675::doErase(bool)
{
  Array cmd;
  gotoTestMemoryArray(cmd);
  return hardware().command(cmd, 40) // 40 ms
         && doEraseRange(Pic::MemoryRangeType::Code)
         && doEraseRange(Pic::MemoryRangeType::Eeprom);
}

//-----------------------------------------------------------------------------
uint OpenProg::P16F62XA::waitProgTime(Pic::MemoryRangeType type) const
{
  if ( type==Pic::MemoryRangeType::Eeprom ) return 6000;
  return 2500;
}

bool OpenProg::P16F62XA::doEraseRange(Pic::MemoryRangeType type)
{
  if ( type==Pic::MemoryRangeType::Code ) {
    Array cmd;
    setT3Array(cmd, 12000); // 12 ms
    cmd += BULK_ERASE_PROG;
    cmd += WAIT_T3;
    return hardware().command(cmd, 40);
  }
  if ( type==Pic::MemoryRangeType::Eeprom ) {
    Array cmd;
    setT3Array(cmd, 12000); // 12 ms
    cmd += BULK_ERASE_DATA;
    cmd += WAIT_T3;
    return hardware().command(cmd, 40);
  }
  return false;
}

bool OpenProg::P16F62XA::doErase(bool)
{
  Array cmd;
  gotoTestMemoryArray(cmd);
  cmd += WAIT_T2; // 100 us
  return hardware().command(cmd, 40)
         && doEraseRange(Pic::MemoryRangeType::Code)
         && doEraseRange(Pic::MemoryRangeType::Eeprom);
}

//-----------------------------------------------------------------------------
bool OpenProg::P16F87XA::doEraseRange(Pic::MemoryRangeType type)
{
  if ( type==Pic::MemoryRangeType::Code ) {
    Array cmd;
    setT3Array(cmd, 8000); // 8 ms
    cmd += BULK_ERASE_PROG;
    cmd += WAIT_T3;
    return hardware().command(cmd, 40);
  }
  if ( type==Pic::MemoryRangeType::Eeprom ) {
    Array cmd;
    setT3Array(cmd, 8000); // 8 ms
    cmd += BULK_ERASE_DATA;
    cmd += WAIT_T3;
    return hardware().command(cmd, 40);
  }
  return false;
}

bool OpenProg::P16F87XA::doErase(bool)
{
  // I use the command 31 twice because, sometimes, the prog
  // memory is not totally erased. Not very elegant, but it works.
  Array cmd;
  setT3Array(cmd, 8000); // 8 ms
  gotoTestMemoryArray(cmd);
  cmd += CUST_CMD;
  cmd += 0x1F; // CHIP_ERASE
  cmd += WAIT_T3;
  cmd += 0x1F; // CHIP_ERASE
  cmd += WAIT_T3;
  return hardware().command(cmd, 40)
         && doEmulatedEraseRange(Pic::MemoryRangeType::UserId);
}

//-----------------------------------------------------------------------------
bool OpenProg::P16F88X::doErase(bool)
{
  Array cmd;
  gotoTestMemoryArray(cmd);
  cmd += WAIT_T2; // 100 us
  return hardware().command(cmd, 40)
         && doEraseRange(Pic::MemoryRangeType::Code)
         && doEraseRange(Pic::MemoryRangeType::Eeprom);
}

//-----------------------------------------------------------------------------
void OpenProg::P16F913::startProgArray(Array& cmd, Pic::MemoryRangeType) const
{
  cmd += BEGIN_PROG2;
}

void OpenProg::P16F913::endProgArray(Array& cmd, Pic::MemoryRangeType) const
{
  cmd += END_PROG;
  cmd += WAIT_T2; // 100 us
}

bool OpenProg::P16F913::doEraseRange(Pic::MemoryRangeType type)
{
  if ( type==Pic::MemoryRangeType::Code ) {
    Array cmd;
    setT3Array(cmd, 6000); // 6 ms
    cmd += BULK_ERASE_PROG;
    cmd += WAIT_T3;
    return hardware().command(cmd, 40);
  }
  if ( type==Pic::MemoryRangeType::Eeprom ) {
    Array cmd;
    setT3Array(cmd, 6000); // 6 ms
    cmd += BULK_ERASE_DATA;
    cmd += WAIT_T3;
    return hardware().command(cmd, 40);
  }
  return false;
}

bool OpenProg::P16F913::doErase(bool)
{
  // flow chart of figure 3.21 of prog sheet
  doEraseRange(Pic::MemoryRangeType::Code);
  Array cmd;
  gotoTestMemoryArray(cmd);
  return hardware().command(cmd, 40)
         && doEraseRange(Pic::MemoryRangeType::Code)
         && doEraseRange(Pic::MemoryRangeType::Eeprom);
}

//-----------------------------------------------------------------------------
bool OpenProg::P16F785::doErase(bool)
{
  // flow chart of figure 3.20 of prog sheet
  Array cmd;
  gotoTestMemoryArray(cmd);
  return hardware().command(cmd, 40)
         && doEraseRange(Pic::MemoryRangeType::Code)
         && doEraseRange(Pic::MemoryRangeType::Eeprom);
}
