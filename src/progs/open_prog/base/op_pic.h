/***************************************************************************
 *   Copyright (C) 2010 Nicolas Hadacek <hadacek@kce.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef OP_PIC_H
#define OP_PIC_H

#include "devices/pic/pic/pic_memory.h"
#include "op.h"

namespace OpenProg
{

//-----------------------------------------------------------------------------
class PicSpecific : public ::Programmer::PicSpecific
{
public:
  PicSpecific(::Programmer::Base &base);
  virtual ::Programmer::Capabilities capabilities() const { return ::Programmer::CanEraseAll | ::Programmer::VariableOffsetReadWrite | ::Programmer::VariableSizeReadWrite; }
  virtual bool canEraseRange(Pic::MemoryRangeType type) const { return ( type==Pic::MemoryRangeType::Code || type==Pic::MemoryRangeType::Eeprom ); }
  virtual bool canReadRange(Pic::MemoryRangeType) const { return true; }
  virtual bool canWriteRange(Pic::MemoryRangeType) const { return true; }
  virtual bool setPowerOff();
  virtual bool setTargetPowerOn(bool) { return true; }

protected:
  bool setPowerOnVddFirst(bool withDelay);
  bool setPowerOnVppFirst();
  void powerOnArray(Array& cmd) const;
  void setT3Array(Array& cmd, uint us) const;
  Hardware& hardware() { return static_cast<Hardware&>(*_base.hardware()); }
};

} // namespace

#endif
