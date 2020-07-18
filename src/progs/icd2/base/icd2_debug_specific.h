/***************************************************************************
 *   Copyright (C) 2005-2007 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef ICD2_DEBUG_SPECIFIC_H
#define ICD2_DEBUG_SPECIFIC_H

#include "icd2_debug.h"

namespace Icd2
{
//-----------------------------------------------------------------------------
class DebuggerSpecific : public ::Debugger::Specific
{
public:
  DebuggerSpecific(::Debugger::Base &base) : ::Debugger::Specific(base) {}
  Debugger &debugger() const { return static_cast<Debugger&>(_base); }
  const Pic::Data *device() const { return debugger().device(); }
  Hardware *hardware() const { return debugger().hardware(); }
  DebugProgrammer &programmer() const { return debugger().programmer(); }
  ::Debugger::PicSpecific *deviceSpecific() const { return debugger().deviceSpecific(); }
  
  virtual Address addressWREG() const = 0;
  virtual BitValue maskPC() const = 0;
  virtual Address addressRegister(Address address) const = 0;
  virtual bool setBreakpoint(Address address) = 0;
  virtual bool readBreakpoint(BitValue &value) = 0;
  virtual bool init(bool last) = 0;
  virtual bool reset() = 0;
  virtual bool setupHardware();
  virtual bool writeDebugExecutive();
  virtual void toDebugMemory(Pic::Memory &memory, bool withDebugExecutive);
  
protected:
  Device::Array _deArray;
  uint          _deStart, _deEnd;
  
  virtual QString debugExecutiveFilename() const = 0;
  virtual void initDebugExecutiveArray(Pic::Memory&);
  Pic::MemoryRangeType debugExecutiveMemoryType() const;
};

} // namespace

#endif
