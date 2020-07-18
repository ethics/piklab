/***************************************************************************
 *   Copyright (C) 2010 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef OP_16_H
#define OP_16_H

#include "op_pic.h"

namespace OpenProg
{
//----------------------------------------------------------------------------
class pic16 : public PicSpecific
{
public:
  pic16(::Programmer::Base &base) : PicSpecific(base) {}
  virtual bool doRead(Pic::MemoryRangeType type, uint wordOffset, Device::Array &data, const ::Programmer::VerifyData *vdata);
  virtual bool doWrite(Pic::MemoryRangeType type, uint wordOffset, const Device::Array &data, bool force);

protected:
  virtual bool setPowerOn() { return setPowerOnVddFirst(true); }
  virtual bool gotoMemory(Pic::MemoryRangeType type) = 0;
  virtual bool incrementPC(uint steps);
  void incrementPCArray(Array& cmd, uint steps) const;
  virtual uint nbWordsCodeProg() const { return 1; }
  virtual void startProgArray(Array&, Pic::MemoryRangeType) const;
  virtual uint waitProgTime(Pic::MemoryRangeType type) const = 0;
  virtual void endProgArray(Array&, Pic::MemoryRangeType) const {}
  virtual bool writeWords(Pic::MemoryRangeType type, const Device::Array &data, uint &i, bool force);
  virtual bool doWriteWords(Pic::MemoryRangeType type, const Device::Array &data, uint i);
  virtual bool skipMaskWords(Pic::MemoryRangeType type, const Device::Array &data, uint &i, bool force);
};

} // namespace

#endif
