/***************************************************************************
 *   Copyright (C) 2010 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "op_mem24.h"

#include "common/common/misc.h"

OpenProg::Mem24DeviceSpecific::Mem24DeviceSpecific(::Programmer::Base &base)
  : ::Programmer::Mem24DeviceSpecific(base)
{}

bool OpenProg::Mem24DeviceSpecific::setPowerOn()
{
  // TODO
  return false;
}

bool OpenProg::Mem24DeviceSpecific::setPowerOff()
{
  // TODO
  return false;
}

bool OpenProg::Mem24DeviceSpecific::verifyPresence()
{
  // TODO
  return false;
}

bool OpenProg::Mem24DeviceSpecific::doRead(Device::Array &data, const ::Programmer::VerifyData *vdata)
{
  // TODO
  return false;
}

bool OpenProg::Mem24DeviceSpecific::doWrite(const Device::Array &data)
{
  // TODO
  return false;
}
