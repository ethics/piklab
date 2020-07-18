/***************************************************************************
 *   Copyright (C) 2012 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef PICKIT3_PROG_H
#define PICKIT3_PROG_H

#include "progs/base/prog_group.h"
#include "devices/pic/prog/pic_prog.h"
#include "devices/mem24/prog/mem24_prog.h"
#include "pickit3.h"

namespace Pickit3
{
  extern bool isSupported(const QString &device);

//----------------------------------------------------------------------------
class PicBase : public ::Programmer::PicBase
{
public:
  PicBase(const ::Programmer::Group &group, const Pic::Data *data);
  virtual bool readFirmwareVersion();
  const VersionData& getOSVersion() const;
  const VersionData& getAPVersion() const;
  uint getAPId() const;

private:
  Hardware &hardware() { return static_cast<Hardware &>(*_hardware); }
  const Hardware &hardware() const { return static_cast<const Hardware &>(*_hardware); }
  virtual BitValue readDeviceId();
  virtual bool setTarget();
  virtual bool setupFirmware();
};

//-----------------------------------------------------------------------------
class DeviceSpecific : public ::Programmer::PicSpecific
{
public:
  DeviceSpecific(::Programmer::Base &base) : ::Programmer::PicSpecific(base) {}

  virtual ::Programmer::Capabilities capabilities() const { return ::Programmer::CanEraseAll; }
  virtual bool canEraseRange(Pic::MemoryRangeType) const { return true; }
  virtual bool canReadRange(Pic::MemoryRangeType) const { return true; }
  virtual bool canWriteRange(Pic::MemoryRangeType) const { return true; }
  Hardware& hardware() const { return static_cast<Hardware&>(*_base.hardware()); }
  virtual bool setPowerOn();
  virtual bool setPowerOff();
  virtual bool setTargetPowerOn(bool on);
  virtual bool doEraseRange(Pic::MemoryRangeType type);
  virtual bool doErase(bool);
  virtual bool doRead(Pic::MemoryRangeType type, uint wordOffset, Device::Array &data, const ::Programmer::VerifyData *vdata);
  virtual bool doWrite(Pic::MemoryRangeType type, uint wordOffset, const Device::Array &data, bool force);
};

//----------------------------------------------------------------------------
class Group : public ::Programmer::PicGroup
{
public:
  virtual QString name() const { return "pickit3"; }
  virtual QString label() const { return i18n("Pickit3"); }
  virtual ::Programmer::Properties properties() const { return ::Programmer::Programmer | ::Programmer::CanReadMemory | ::Programmer::HasConnectedState
      | ::Programmer::CanReleaseReset | ::Programmer::HasFirmware | ::Programmer::CanUploadFirmware | ::Programmer::NeedDeviceSpecificFirmware; }
  virtual ::Programmer::TargetPowerMode targetPowerMode() const { return ::Programmer::TargetPowerModeFromConfig; }
  virtual bool isPortSupported(PortType type) const { return type == PortType::USB; }
  virtual bool canReadVoltage(Device::VoltageType) const;

protected:
  virtual void initSupported();
  virtual ::Programmer::DeviceSpecific *createDeviceSpecific(::Programmer::Base &base) const;
  virtual ::Programmer::Base *createBase(const Device::Data *data) const;
  virtual ::Programmer::Hardware *createHardware(::Programmer::Base &base, const ::Programmer::HardwareDescription &hd) const;
};

} // namespace

#endif
