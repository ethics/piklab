/***************************************************************************
 *   Copyright (C) 2005-2006 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "pic_prog_specific.h"

//-----------------------------------------------------------------------------
uint Programmer::PicSpecific::findNonMaskStart(Pic::MemoryRangeType type, const Device::Array &data) const
{
  uint start = 0;
  for (; start<data.count(); start++)
    if ( data[start]!=device().mask(type) ) break;
  const_cast<PicSpecific *>(this)->log(Log::DebugLevel::Extra, QString("start before align: %1").arg(start));
  uint align = device().nbWordsWriteAlignment(type);
  start -= start % align;
  const_cast<PicSpecific *>(this)->log(Log::DebugLevel::Extra, QString("start after align: %1 (align=%2)").arg(start).arg(align));
  return start;
}

uint Programmer::PicSpecific::findNonMaskEnd(Pic::MemoryRangeType type, const Device::Array &data) const
{
  ASSERT(data.count() > 0);
  uint end = data.count() - 1;
  for (; end>0; end--)
    if ( data[end]!=device().mask(type) ) break;
  const_cast<PicSpecific *>(this)->log(Log::DebugLevel::Extra, QString("end before align: %1").arg(end));
  uint align = device().nbWordsWriteAlignment(type);
  if ( (end+1) % align ) end += align - (end+1) % align;
  // this can happen when the last word is calibration...
  if (end >= data.count()) end = data.count() - 1;
  const_cast<PicSpecific *>(this)->log(Log::DebugLevel::Extra, QString("end after align: %1 (align=%2)").arg(end).arg(align));
  return end;
}

Device::Array Programmer::PicSpecific::prepareCodeRange(const Device::Array &data, bool force, uint &wordOffset)
{
  wordOffset = (force || !(capabilities() & VariableOffsetReadWrite) ? 0 : findNonMaskStart(Pic::MemoryRangeType::Code, data));
  uint nbWords = 0;
  if ( wordOffset!=data.count() ) {
    uint end = (force || !(capabilities() & VariableSizeReadWrite) ? data.count()-1 : findNonMaskEnd(Pic::MemoryRangeType::Code, data));
    nbWords = end - wordOffset + 1;
    log(Log::DebugLevel::Extra, QString("    start=%1 nbWords=%2 total=%3 force=%4 varOffset=%5 varSize=%6")
      .arg(toHexLabel(wordOffset, device().nbCharsAddress()))
      .arg(toHexLabel(nbWords, device().nbCharsAddress()))
      .arg(toHexLabel(data.count(), device().nbCharsAddress()))
      .arg(force ? "true" : "false")
      .arg(capabilities() & VariableOffsetReadWrite ? "true" : "false")
      .arg(capabilities() & VariableSizeReadWrite ? "true" : "false"));
  }
  _base.progressMonitor().addTaskProgress(data.count()-nbWords);
  return data.mid(wordOffset, nbWords);
}

bool Programmer::PicSpecific::read(Pic::MemoryRangeType type, Device::Array &data, const VerifyData *vdata)
{
  setPowerOn();
  bool ok = internalRead(type, data, vdata);
  setPowerOff();
  return ok;
}

bool Programmer::PicSpecific::internalRead(Pic::MemoryRangeType type, Device::Array &data, const VerifyData *vdata)
{
  if (vdata == NULL) {
    data.resize(device().nbWords(type));
    return doRead(type, 0, data, NULL);
  } else {
    Device::Array tmp = static_cast<const Pic::Memory &>(vdata->memory()).arrayForWriting(type);
    if ( type!=Pic::MemoryRangeType::Code ) return doRead(type, 0, tmp, vdata);
    bool only = ( vdata->actions & ::Programmer::OnlyProgrammedVerify );
    uint wordOffset = 0;
    Device::Array pdata = prepareCodeRange(tmp, !only, wordOffset);
    return doRead(type, wordOffset, pdata, vdata);
  }
}

bool Programmer::PicSpecific::write(Pic::MemoryRangeType type, const Device::Array &data, bool force)
{
  setPowerOn();
  const bool ok = internalWrite(type, data, force);
  setPowerOff();
  return ok;
}

bool Programmer::PicSpecific::internalWrite(Pic::MemoryRangeType type, const Device::Array &data, bool force)
{
  ASSERT(data.count() == device().nbWords(type));

  const bool configInCodeArea = device().architecture() == Pic::Architecture::P18J
    || device().architecture() == Pic::Architecture::P24F;
  if (configInCodeArea
      && !(capabilities() & VariableOffsetReadWrite)) {
    qFatal("Variable-offset-read/write capability is needed for 18J and 24F families...");
  }

  if (type == Pic::MemoryRangeType::Config
      && configInCodeArea) {
    const uint align = device().nbWordsWriteAlignment(Pic::MemoryRangeType::Code);
    int delta = align;
    if (device().architecture() == Pic::Architecture::P18J) {
      ASSERT(data.count()%2 == 0);
      delta -= data.count()/2; // config memory words contains 1 byte
    } else {
      delta -= data.count();
    }
    ASSERT(delta >= 0);
    Device::Array rdata(delta);
    const uint wordOffset = device().nbWords(Pic::MemoryRangeType::Code) - delta;
    _base.progressMonitor().addTaskTotalProgress(rdata.size());
    if ( !doRead(Pic::MemoryRangeType::Code, wordOffset, rdata, 0) ) return false;
    Device::Array pdata(align);
    for (uint i=0; i<uint(delta); i++) pdata[i] = rdata[i];
    for (uint i=delta; i<align; i++) {
      if (device().architecture() == Pic::Architecture::P18J) {
        pdata[i] = data[2*(i-delta)];
        pdata[i] |= data[2*(i-delta)+1] << 8;
      } else {
	pdata[i] = data[i-delta];
      }
    }
    _base.progressMonitor().addTaskTotalProgress(pdata.size());
    return doWrite(Pic::MemoryRangeType::Code, wordOffset, pdata, true);
  }

  if ( type!=Pic::MemoryRangeType::Code ) return doWrite(type, 0, data, true);

  uint wordOffset = 0;
  Device::Array pdata = prepareCodeRange(data, force, wordOffset);
  if (configInCodeArea) {
    const uint end = wordOffset + pdata.size();
    if (end >= device().nbWords(Pic::MemoryRangeType::Code)) {
      Device::Array rdata(device().nbWords(Pic::MemoryRangeType::Config));
      _base.progressMonitor().addTaskTotalProgress(rdata.size());
      if ( !doRead(Pic::MemoryRangeType::Code, device().nbWords(Pic::MemoryRangeType::Code), rdata, 0) ) return false;
      int delta = pdata.size();
      if (device().architecture() == Pic::Architecture::P18J) {
	ASSERT(data.count()%2 == 0);
	delta -= rdata.count()/2;
      } else {
	delta -= rdata.count();
      }
      ASSERT(delta >= 0);
      for (uint i=delta; i<pdata.size(); i++) {
	if (device().architecture() == Pic::Architecture::P18J) {
          pdata[i] = rdata[2*(i-delta)];
          pdata[i] |= rdata[2*(i-delta)+1] << 8;
        } else {
	  pdata[i] = rdata[i-delta];
	}
      }
    }
  }
  return doWrite(type, wordOffset, pdata, force);
}

bool Programmer::PicSpecific::erase(bool isProtected)
{
  setPowerOn();
  const bool ok = internalErase(isProtected);
  setPowerOff();
  return ok;
}

bool Programmer::PicSpecific::internalErase(bool isProtected)
{
  if ( device().architecture()==Pic::Architecture::P18J ) { // ### also true for others ?
    Device::Array data(device().nbWords(Pic::MemoryRangeType::Config));
    for (uint i=0; i<data.size(); i++) data[i] = device().config()._words[i].wmask;
    if ( !internalWrite(Pic::MemoryRangeType::Config, data, true) ) return false;
  }
  return doErase(isProtected);
}

bool Programmer::PicSpecific::eraseRange(Pic::MemoryRangeType type)
{
  setPowerOn();
  const bool ok = doEraseRange(type);
  setPowerOff();
  return ok;
}

bool Programmer::PicSpecific::doEmulatedEraseRange(Pic::MemoryRangeType type)
{
  Pic::Memory memory(device());
  if ( !doWrite(type, 0, memory.arrayForWriting(type), true) ) return false;
  if ( !canReadRange(type) ) return true;
  VerifyData vdata(BlankCheckVerify, memory);
  Device::Array data;
  return doRead(type, 0, data, &vdata);
}

//-----------------------------------------------------------------------------
bool Programmer::PicHardware::compareWords(Pic::MemoryRangeType type, uint index, BitValue v, BitValue d, Programmer::VerifyActions actions)
{
  if ( v==d ) return true;
  const uint inc = device().addressIncrement(type);
  Address address = device().range(type).start + inc * index;
  if ( actions & ::Programmer::BlankCheckVerify )
    log(Log::LineType::SoftError, i18n("Device memory is not blank (in %1 at address %2: reading %3 and expecting %4).")
      .arg(type.label()).arg(toHexLabel(address, device().nbCharsAddress()))
      .arg(toHexLabel(d, device().nbCharsWord(type))).arg(toHexLabel(v, device().nbCharsWord(type))));
  else log(Log::LineType::SoftError, i18n("Device memory does not match hex file (in %1 at address %2: reading %3 and expecting %4).")
      .arg(type.label()).arg(toHexLabel(address, device().nbCharsAddress()))
      .arg(toHexLabel(d, device().nbCharsWord(type))).arg(toHexLabel(v, device().nbCharsWord(type))));
  return false;
}

bool Programmer::PicHardware::verifyWord(uint i, BitValue word, Pic::MemoryRangeType type, const VerifyData &vdata)
{
  if ( !(vdata.actions & ::Programmer::IgnoreProtectedVerify) && vdata.protectedRanges.contains(i) ) return true; // protected
  BitValue v = static_cast<const Pic::Memory &>(vdata.memory()).normalizedWord(type, i);
  BitValue d = static_cast<const Pic::Memory &>(vdata.memory()).normalizeWord(type, i, word);
  if ( type==Pic::MemoryRangeType::Config ) {
    const BitValue pmask = device().config()._words[i].pmask;
    v = v.clearMaskBits(pmask);
    d = d.clearMaskBits(pmask);
  }
  return compareWords(type, i, v, d, vdata.actions);
}
