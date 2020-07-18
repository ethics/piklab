/***************************************************************************
 *   Copyright (C) 2005 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef DIRECT_PROG_H
#define DIRECT_PROG_H

#include "progs/base/prog_group.h"
#include "devices/pic/prog/pic_prog.h"
#include "devices/mem24/prog/mem24_prog.h"
#include "direct.h"

namespace Direct
{
  extern bool isSupported(const QString &device);
  class Hardware;

//----------------------------------------------------------------------------
class PicBase : public ::Programmer::PicBase
{
public:
  PicBase(const ::Programmer::Group &group, const Pic::Data *data)
    : ::Programmer::PicBase(group, data, "pic_direct_programmer") {}

private:
  Hardware &hardware() { return static_cast<Hardware &>(*_hardware); }
};

//----------------------------------------------------------------------------
class Mem24Base : public ::Programmer::Mem24Base
{
public:
  Mem24Base(const ::Programmer::Group &group, const Mem24::Data *data)
    : ::Programmer::Mem24Base(group, data, "mem24_direct_programmer") {}

private:
  Hardware &hardware() { return static_cast<Hardware &>(*_hardware); }
};

//----------------------------------------------------------------------------
class Group : public ::Programmer::PicGroup
{
public:
  virtual QString name() const { return "direct"; }
  virtual QString label() const { return i18n("Direct Programmer"); }
  virtual ::Hardware::Config *hardwareConfig() const;
  virtual ::Programmer::Properties properties() const { return ::Programmer::Programmer | ::Programmer::CanReadMemory | ::Programmer::HasConnectedState; }
  virtual ::Programmer::TargetPowerMode targetPowerMode() const { return ::Programmer::TargetExternallyPowered; }
  virtual bool isPortSupported(PortType type) const { return ( type==PortType::Serial || type==PortType::Parallel ); }
  virtual bool canReadVoltage(Device::VoltageType) const { return false; }

protected:
  virtual void initSupported();
  virtual ::Programmer::DeviceSpecific *createDeviceSpecific(::Programmer::Base &base) const;
  virtual ::Programmer::Base *createBase(const Device::Data *data) const;
  virtual ::Programmer::Hardware *createHardware(::Programmer::Base &base, const ::Programmer::HardwareDescription &hd) const;
};

class DGroup : public Group // add mem24 devices to methods defined in "direct_data.cpp"
{
protected:
  virtual void initSupported();
  virtual ::Programmer::DeviceSpecific *createDeviceSpecific(::Programmer::Base &base) const;
};

} // namespace

#endif
