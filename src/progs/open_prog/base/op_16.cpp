/***************************************************************************
 *   Copyright (C) 2010 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "op_16.h"

#include "op_code.h"

//----------------------------------------------------------------------------
bool OpenProg::pic16::doRead(Pic::MemoryRangeType type, uint wordOffset, Device::Array &data, const ::Programmer::VerifyData *vdata)
{
  gotoMemory(type);
  // ### FIXME
  //incrementPC(wordOffset);
  data.resize(16);

  Array array;
  const uchar readCmd = type == Pic::MemoryRangeType::Eeprom ? READ_DATA_DATA : READ_DATA_PROG;
  const uint neededPerCmd = type == Pic::MemoryRangeType::Eeprom ? 3 : 4; // 2 for command + 1 or 2 for data
  const uint maxNb = array.available() / neededPerCmd;
  uint size = data.count();
  const BitValue mask = device().mask(type);
  uint i = 0;
  uint nb = 0;
  while (size > 0) {
    array += readCmd;
    array += INC_ADDR;
    --size;
    ++nb;
    if (nb >= maxNb
        || size == 0) {
      if (!hardware().command(array)) {
        return false;
      }
      for (uint k=1; k<array.length(); ++k) {
        if (array[k] == readCmd) {
          if (type == Pic::MemoryRangeType::Eeprom) {
            data[i] = array[k+1];
            ++k;
          } else {
            data[i] = (array[k+1]<<8) + array[k+2];
            k += 2;
          }
          data[i] = data[i].maskWith(mask);
          if (vdata != NULL && !hardware().verifyWord(wordOffset+i, data[i], type, *vdata)) return false;
          k += 2;
        }
      }
      if ( type == Pic::MemoryRangeType::Code || type==Pic::MemoryRangeType::Eeprom )
        _base.progressMonitor().addTaskProgress(nb);
      nb = 0;
      array.reset();
    }
  }
  return true;
}

bool OpenProg::pic16::doWrite(Pic::MemoryRangeType type, uint wordOffset, const Device::Array &data, bool force)
{
  gotoMemory(type);
  incrementPC(wordOffset);
  Array cmd;
  setT3Array(cmd, waitProgTime(type));

  for (uint i=0; i<data.count(); ) {
    if ( !writeWords(type, data, i, force) ) {
      log(Log::LineType::Error, i18n("Error programming %1 at %2.").arg(type.label()).arg(toHexLabel(i, 8)));
      return false;
    }
  }
  return true;
}

bool OpenProg::pic16::skipMaskWords(Pic::MemoryRangeType type, const Device::Array &data, uint &i, bool force)
{
  if (force) return false;
  const uint nb = (type==Pic::MemoryRangeType::Code ? nbWordsCodeProg() : 1);
  for (uint k=0; k<nb; k++)
    if ( data[i+k]!=device().mask(type) ) return false;
  incrementPC(nb);
  i += nb;
  return true;
}

bool OpenProg::pic16::doWriteWords(Pic::MemoryRangeType type, const Device::Array &data, uint i)
{
  Array cmd;
  if (type==Pic::MemoryRangeType::Eeprom) {
    cmd += LOAD_DATA_DATA;
    cmd += data[i].byte(1);
    cmd += data[i].byte(0);
  } else {
    const uint nb = (type==Pic::MemoryRangeType::Code ? nbWordsCodeProg() : 1);
    for (uint k=0; k<nb; k++) {
      if ( k!=0 ) incrementPCArray(cmd, 1);
      cmd += LOAD_DATA_PROG;
      cmd += data[i+k].byte(1);
      cmd += data[i+k].byte(0);
    }
  }
  startProgArray(cmd, type);
  cmd += WAIT_T3;
  endProgArray(cmd, type);
  incrementPCArray(cmd, 1);
  return hardware().command(cmd);
}

bool OpenProg::pic16::writeWords(Pic::MemoryRangeType type, const Device::Array &data, uint &i, bool force)
{
  if ( skipMaskWords(type, data, i, force) ) return true;
  if ( !doWriteWords(type, data, i) ) return false;
  i += (type==Pic::MemoryRangeType::Code ? nbWordsCodeProg() : 1);
  return true;
}

void OpenProg::pic16::incrementPCArray(Array&cmd, uint steps) const
{
  ASSERT(steps <= 0xFF);
  if (steps == 1) {
    cmd += INC_ADDR;
  } else if (steps > 1) {
    cmd += INC_ADDR_N;
    cmd += steps;
  }
}

bool OpenProg::pic16::incrementPC(uint steps)
{
  Array cmd;
  while (steps > 0) {
    const uchar nb = steps % 0xFF;
    steps -= nb;
    incrementPCArray(cmd, nb);
    if (cmd.available() < 2
        || steps == 0) {
      if (!hardware().command(cmd)) {
        return false;
      }
      cmd.reset();
    }
  }
  return true;
}

void OpenProg::pic16::startProgArray(Array& cmd, Pic::MemoryRangeType) const
{
  cmd += BEGIN_PROG;
}
