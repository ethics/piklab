/***************************************************************************\
 *   Copyright (C) 2010 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef OP_MEM24_H
#define OP_MEM24_H

#include "devices/mem24/prog/mem24_prog.h"
#include "op.h"

namespace OpenProg
{

class Mem24DeviceSpecific : public ::Programmer::Mem24DeviceSpecific
{
public:
  Mem24DeviceSpecific(::Programmer::Base &base);
  virtual Hardware &hardware() { return static_cast<Hardware &>(*_base.hardware()); }
  const Mem24::Data &device() const { return static_cast<const Mem24::Data &>(*::Programmer::DeviceSpecific::_base.device()); }
  virtual bool init() { return true; }
  virtual bool setPowerOff();
  virtual bool setPowerOn();
  virtual bool setTargetPowerOn(bool) { return true; }
  virtual bool verifyPresence();

private:
  virtual bool doRead(Device::Array &data, const ::Programmer::VerifyData *vdata);
  virtual bool doWrite(const Device::Array &data);
};

} // namespace

#endif
