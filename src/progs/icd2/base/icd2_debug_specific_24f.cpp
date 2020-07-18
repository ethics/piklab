/***************************************************************************
 *   Copyright (C) 2010 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "icd2_debug_specific_24f.h"

#include "devices/pic/base/pic_register.h"
#include "icd2_data.h"


//----------------------------------------------------------------------------
Icd2::P24FDebuggerSpecific::P24FDebuggerSpecific(::Debugger::Base &base)
  : DebuggerSpecific(base)
{
}

Address Icd2::P24FDebuggerSpecific::addressWREG()const
{
  // ### FIXME
  return 0x0;
}

Address Icd2::P24FDebuggerSpecific::addressRegister(Address) const
{
  // ### FIXME
  return 0x0;
}

bool Icd2::P24FDebuggerSpecific::setBreakpoint(Address)
{
  // ### FIXME
  return false;
}

bool Icd2::P24FDebuggerSpecific::readBreakpoint(BitValue&)
{
  // ### FIXME
  return false;
}

bool Icd2::P24FDebuggerSpecific::reset()
{
  return true;
}

bool Icd2::P24FDebuggerSpecific::init(bool)
{
  return reset();
}

QString Icd2::P24FDebuggerSpecific::debugExecutiveFilename() const
{
  // ### there is another one too...
  return "DE_PIC24FJ64GA004.hex";
}
