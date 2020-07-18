/***************************************************************************
 *   Copyright (C) 2005-2006 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef PIC_PROG_SPECIFIC_H
#define PIC_PROG_SPECIFIC_H

#include "progs/base/prog_specific.h"
#include "progs/base/generic_prog.h"
#include "devices/pic/pic/pic_memory.h"

namespace Programmer
{
//-----------------------------------------------------------------------------
enum Capability { NoCapability = 0, CanEraseAll = 1, EmulatedErase = 2,
                  VariableOffsetReadWrite = 4, VariableSizeReadWrite = 8 };
Q_DECLARE_FLAGS(Capabilities, Capability)
Q_DECLARE_OPERATORS_FOR_FLAGS(Capabilities)

class PicSpecific : public DeviceSpecific
{
public:
  PicSpecific(::Programmer::Base &base) : DeviceSpecific(base) {}
  const Pic::Data &device() const { return static_cast<const Pic::Data &>(*_base.device()); }
  virtual Capabilities capabilities() const = 0;
  virtual bool canEraseRange(Pic::MemoryRangeType type) const = 0;
  virtual bool canReadRange(Pic::MemoryRangeType type) const = 0;
  virtual bool canWriteRange(Pic::MemoryRangeType type) const = 0;
  bool eraseRange(Pic::MemoryRangeType type);
  bool erase(bool isProtected);
  bool read(Pic::MemoryRangeType type, Device::Array &data, const VerifyData *vdata);
  bool write(Pic::MemoryRangeType type, const Device::Array &data, bool force);
  uint findNonMaskStart(Pic::MemoryRangeType type, const Device::Array &data) const;
  uint findNonMaskEnd(Pic::MemoryRangeType type, const Device::Array &data) const;

protected:
  virtual bool internalErase(bool isProtected);
  virtual bool doErase(bool isProtected) = 0;
  virtual bool doEraseRange(Pic::MemoryRangeType type) = 0;
  bool doEmulatedEraseRange(Pic::MemoryRangeType type);
  Device::Array prepareCodeRange(const Device::Array &data, bool force, uint &wordOffset);
  virtual bool internalRead(Pic::MemoryRangeType type, Device::Array &data, const VerifyData *vdata);
  virtual bool doRead(Pic::MemoryRangeType type, uint wordOffset, Device::Array &data, const VerifyData *vdata) = 0;
  virtual bool internalWrite(Pic::MemoryRangeType type, const Device::Array &data, bool force);
  virtual bool doWrite(Pic::MemoryRangeType type, uint wordOffset, const Device::Array &data, bool force) = 0;
};

//-----------------------------------------------------------------------------
class PicHardware : public Hardware
{
public:
  PicHardware(::Programmer::Base &base, Port::Base *port, const QString &name) : Hardware(base, port, name) {}
  const Pic::Data &device() const { return static_cast<const Pic::Data &>(*_base.device()); }
  bool compareWords(Pic::MemoryRangeType type, uint index, BitValue v, BitValue d, VerifyActions actions);
  bool verifyWord(uint index, BitValue word, Pic::MemoryRangeType type, const VerifyData &vdata);
};

} // namespace

#endif
