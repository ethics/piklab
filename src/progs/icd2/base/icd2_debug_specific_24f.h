/***************************************************************************
 *   Copyright (C) 2010 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef ICD2_DEBUG_SPECIFIC_24F_H
#define ICD2_DEBUG_SPECIFIC_24F_H

#include "icd2_debug_specific.h"

namespace Icd2
{
//-----------------------------------------------------------------------------
class P24FDebuggerSpecific : public DebuggerSpecific
{
public:
  P24FDebuggerSpecific(::Debugger::Base &base);
  virtual Address addressWREG() const;
  virtual BitValue maskPC() const { return 0xFFFF; }
  virtual Address addressRegister(Address address) const;
  virtual bool setBreakpoint(Address address);
  virtual bool readBreakpoint(BitValue &value);
  virtual bool init(bool last);
  virtual bool reset();
  
protected:
  virtual QString debugExecutiveFilename() const;
};

} // namespace

#endif
