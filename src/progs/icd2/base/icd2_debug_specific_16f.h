/***************************************************************************
 *   Copyright (C) 2005-2007 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef ICD2_DEBUG_SPECIFIC_16F_H
#define ICD2_DEBUG_SPECIFIC_16F_H

#include "icd2_debug_specific.h"

namespace Icd2
{
//-----------------------------------------------------------------------------
class P16FDebuggerSpecific : public DebuggerSpecific
{
public:
  P16FDebuggerSpecific(::Debugger::Base &base) : DebuggerSpecific(base) {}
  virtual Address addressBreakpointRegister() const { return 0x18E; }
  virtual BitValue writeMaskBreakpointRegister() const { return 0x8000; }
  virtual BitValue readMaskBreakpointRegister() const { return 0x1FFF; }
  virtual Address addressWREG() const;
  virtual BitValue maskPC() const { return 0x1FFF; }
  virtual Address addressRegister(Address address) const;
  virtual bool setBreakpoint(Address address);
  virtual bool readBreakpoint(BitValue &value);
  virtual bool reset();

protected:
  bool beginInit(Device::Array *saved);
  bool writeGotoAtResetVector(const Device::Array &saved);
  bool endInit(BitValue expectedPC);
  
  virtual QString debugExecutiveFilename() const;
};

//-----------------------------------------------------------------------------
class P16F872DebuggerSpecific : public P16FDebuggerSpecific
{
public:
  P16F872DebuggerSpecific(::Debugger::Base &base) : P16FDebuggerSpecific(base) {}
  virtual bool init(bool last);
};

//-----------------------------------------------------------------------------
class P16F87XDebuggerSpecific : public P16FDebuggerSpecific
{
public:
  P16F87XDebuggerSpecific(::Debugger::Base &base) : P16FDebuggerSpecific(base) {}
  virtual bool init(bool last);
};

//-----------------------------------------------------------------------------
class P16F7X7DebuggerSpecific : public P16FDebuggerSpecific
{
public:
  P16F7X7DebuggerSpecific(::Debugger::Base &base) : P16FDebuggerSpecific(base) {}
  virtual bool init(bool last);
};

} // namespace

#endif
