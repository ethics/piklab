/***************************************************************************
 *   Copyright (C) 2007 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "bootloader_prog.h"

#include "progs/base/prog_config.h"

//-----------------------------------------------------------------------------
Bootloader::ProgrammerBase::ProgrammerBase(const Programmer::Group &group, const Pic::Data *data, const char *name)
  : Programmer::PicBase(group, data, name)
{}

//-----------------------------------------------------------------------------
bool Bootloader::Group::checkConnection(const ::Programmer::HardwareDescription &hd) const
{
  Programmer::Generic *generic = createProgrammer(false, NULL, hd, NULL);
  bool ok = static_cast<Hardware *>(static_cast<Programmer::Base *>(generic)->hardware())->openPort();
  delete generic;
  return ok;
}
