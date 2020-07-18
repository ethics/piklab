/***************************************************************************
 *   Copyright (C) 2010 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef OP_PROG_H
#define OP_PROG_H

#include "progs/base/prog_group.h"
#include "devices/pic/prog/pic_prog.h"
#include "devices/mem24/prog/mem24_prog.h"
#include "op.h"

namespace OpenProg
{
  extern bool isSupported(const QString &device);

//----------------------------------------------------------------------------
class PicBase : public ::Programmer::PicBase
{
public:
  PicBase(const ::Programmer::Group &group, const Pic::Data *data)
    : ::Programmer::PicBase(group, data, "pic_op_programmer") {}

private:
  Hardware &hardware() { return static_cast<Hardware &>(*_hardware); }
};

//----------------------------------------------------------------------------
class Mem24Base : public ::Programmer::Mem24Base
{
public:
  Mem24Base(const ::Programmer::Group &group, const Mem24::Data *data)
    : ::Programmer::Mem24Base(group, data, "mem24_op_programmer") {}

private:
  Hardware &hardware() { return static_cast<Hardware &>(*_hardware); }
};

//----------------------------------------------------------------------------
class Group : public ::Programmer::PicGroup
{
public:
  virtual QString name() const { return "open_prog"; }
  virtual QString label() const { return i18n("Open Programmer"); }
  virtual ::Programmer::Properties properties() const { return ::Programmer::Programmer | ::Programmer::CanReadMemory | ::Programmer::HasConnectedState; }
  virtual ::Programmer::TargetPowerMode targetPowerMode() const { return ::Programmer::TargetExternallyPowered; }
  virtual bool isPortSupported(PortType type) const { return ( type==PortType::HID ); }
  virtual bool canReadVoltage(Device::VoltageType) const { return false; }

protected:
  virtual void initSupported();
  virtual ::Programmer::DeviceSpecific *createDeviceSpecific(::Programmer::Base &base) const;
  virtual ::Programmer::Base *createBase(const Device::Data *data) const;
  virtual ::Programmer::Hardware *createHardware(::Programmer::Base &base, const ::Programmer::HardwareDescription &hd) const;
};

class DGroup : public Group // add mem24 devices to methods defined in "op_data.cpp"
{
protected:
  virtual void initSupported();
  virtual ::Programmer::DeviceSpecific *createDeviceSpecific(::Programmer::Base &base) const;
};

} // namespace

#endif
