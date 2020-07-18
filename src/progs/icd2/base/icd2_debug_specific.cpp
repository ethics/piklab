/***************************************************************************
 *   Copyright (C) 2010 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "icd2_debug_specific.h"

#include "devices/pic/base/pic_register.h"
#include "icd2_data.h"

//----------------------------------------------------------------------------
bool Icd2::DebuggerSpecific::setupHardware()
{
  // find debug executive file
  PURL::Directory dir = programmer().firmwareDirectory();
  QString filename = debugExecutiveFilename();
  PURL::Url url = dir.findMatchingFilename(filename);
  log(Log::DebugLevel::Normal, QString("  Debug executive file: %1").arg(url.pretty()));
  if ( !url.exists() ) {
    log(Log::LineType::Error, i18n("Could not find debug executive file \"%1\".").arg(url.pretty()));
    return false;
  }
  
  // upload hex file
  Log::StringView sview;
  PURL::File file(url, sview);
  if ( !file.openForRead() ) {
    log(Log::LineType::Error, i18n("Could not open firmware file \"%1\".").arg(url.pretty()));
    return false;
  }
  QStringList errors;
  HexBuffer hbuffer;
  if ( !hbuffer.load(file.stream(), errors) ) {
    log(Log::LineType::Error, i18n("Could not read debug executive file \"%1\": %2.").arg(url.pretty()).arg(errors[0]));
    return false;
  }
 
  // offset
  const uint nbWords = device()->nbWords(Pic::MemoryRangeType::Code);
  const uint offset = nbWords - 0x100;
  const FamilyData &fdata = FAMILY_DATA[family(device()->name())];
  if ( fdata.debugExecOffset!=0 && fdata.debugExecOffset!=offset )
    for (uint i=0; i<0x100; i++) hbuffer.insert(offset+i, hbuffer[fdata.debugExecOffset+i]);
  
  Pic::Memory::WarningTypes warningTypes;
  QStringList warnings;
  QMap<uint, bool> inRange;
  Pic::Memory memory(*device());
  const Pic::MemoryRangeType type = debugExecutiveMemoryType();
  memory.fromHexBuffer(type, hbuffer, warningTypes, warnings, inRange);
  initDebugExecutiveArray(memory);
  log(Log::DebugLevel::Extra, QString("debug executive: \"%1\" %2:%3")
      .arg(url.pretty()).arg(toHexLabel(_deStart, 4)).arg(toHexLabel(_deEnd, 4)));  
  return true;
}

void Icd2::DebuggerSpecific::toDebugMemory(Pic::Memory &memory, bool withDebugExecutive)
{
  //config bits
  // debug bit should have already been put to ON in internalProgram()
  if ( memory.hasWatchdogTimerOn() ) {
    log(Log::LineType::Warning, i18n("Disabling watchdog timer for debugging"));
    memory.setWatchdogTimerOn(false);
  }
  if ( memory.isProtected(Pic::Protection::ProgramProtected, Pic::MemoryRangeType::Code) ) {
    log(Log::LineType::Warning, i18n("Disabling code program protection for debugging"));
    memory.setProtection(false, Pic::Protection::ProgramProtected, Pic::MemoryRangeType::Code);
  }
  if ( memory.isProtected(Pic::Protection::WriteProtected, Pic::MemoryRangeType::Code) ) {
    log(Log::LineType::Warning, i18n("Disabling code write protection for debugging"));
    memory.setProtection(false, Pic::Protection::WriteProtected, Pic::MemoryRangeType::Code);
  }
  if ( memory.isProtected(Pic::Protection::ReadProtected, Pic::MemoryRangeType::Code) ) {
    log(Log::LineType::Warning, i18n("Disabling code read protection for debugging"));
    memory.setProtection(false, Pic::Protection::ReadProtected, Pic::MemoryRangeType::Code);
  }
  
  // debug vector
  const uint address = _deStart * device()->addressIncrement(Pic::MemoryRangeType::Code);
  const Device::Array data = device()->gotoInstruction(address, false);
  for (uint i=0; i<data.count(); i++) memory.setWord(Pic::MemoryRangeType::DebugVector, i, data[i]);
  if ( device()->is18Family() )
    memory.setWord(Pic::MemoryRangeType::DebugVector, data.count(), 0xFF00); // ??
    
  // debug executive
  if (withDebugExecutive) {
    const Pic::MemoryRangeType type = debugExecutiveMemoryType();
    bool ok = true;
    for (uint i=_deStart; i<=_deEnd; i++) {
      if ( memory.word(type, i).isInitialized() ) ok = false;
      memory.setWord(type, i, _deArray[i]);
    }
    if ( !ok ) log(Log::LineType::Warning, i18n("Memory area for debug executive was not empty. Overwrite it and continue anyway..."));
  }
}

bool Icd2::DebuggerSpecific::writeDebugExecutive()
{
  log(Log::LineType::Information, i18n("  Write debug executive"));
  const Pic::MemoryRangeType type = debugExecutiveMemoryType();
  Device::Array data = _deArray.mid(_deStart, _deEnd - _deStart + 1);
  if ( !hardware()->writeMemory(type, _deStart, data) ) return false;
  log(Log::LineType::Information, i18n("  Verify debug executive"));
  if ( !hardware()->readMemory(type, _deStart, data, 0) ) return false;
  for (uint i=0; i<data.count(); i++) {
    if ( _deArray[_deStart+i]==data[i] ) continue;
    const uint inc = device()->addressIncrement(type);
    const Address address = device()->range(type).start + inc * (_deStart + i);
    log(Log::LineType::Error, i18n("Device memory doesn't match debug executive (at address %1: reading %2 and expecting %3).")
                    .arg(toHexLabel(address, device()->nbCharsAddress()))
                    .arg(toHexLabel(data[i], device()->nbCharsWord(type)))
                    .arg(toHexLabel(_deArray[_deStart+i], device()->nbCharsWord(type))));
    return false;
  }
  return true;
}

Pic::MemoryRangeType Icd2::DebuggerSpecific::debugExecutiveMemoryType() const
{
  if (device()->isPresent(Pic::MemoryRangeType::ProgramExecutive)) {
    return Pic::MemoryRangeType::ProgramExecutive;
  }
  return Pic::MemoryRangeType::Code;
}

void Icd2::DebuggerSpecific::initDebugExecutiveArray(Pic::Memory& memory)
{
  const Pic::MemoryRangeType type = debugExecutiveMemoryType();
  _deArray = memory.arrayForWriting(type);
  _deStart = programmer().deviceSpecific()->findNonMaskStart(type, _deArray);
  _deEnd = programmer().deviceSpecific()->findNonMaskEnd(type, _deArray);
}
