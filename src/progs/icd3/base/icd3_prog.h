/***************************************************************************
 *   Copyright (C) 2012 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef ICD3_PROG_H
#define ICD3_PROG_H

#include "progs/icd2/base/icd_prog.h"
#include "progs/base/prog_group.h"
#include "icd3.h"

namespace Icd3
{
class Hardware;

//-----------------------------------------------------------------------------
class ProgrammerBase : public Icd::ProgrammerBase
{
public:
  ProgrammerBase(const Programmer::Group &group, const Pic::Data *data, const char *name)
    : Icd::ProgrammerBase(group, data, name) {}
  Hardware *hardware() const { return static_cast<Hardware *>(_hardware.get()); }
  //const TestData &testData() const { return _testData; }
  virtual bool selfTest(bool ask);
  virtual bool readFirmwareVersion();
  uchar firmwareId() const { return _firmwareId; }
  virtual bool setTarget() { return hardware()->setTarget(); }

protected:
  virtual void clear();
  virtual bool setupFirmware();
  virtual bool internalSetupHardware();

private:
  uchar    _firmwareId;
  //  TestData _testData;
};

//-----------------------------------------------------------------------------
class Programmer : public ProgrammerBase
{
public:
  Programmer(const ::Programmer::Group &group, const Pic::Data *data);
};

//-----------------------------------------------------------------------------
class Group : public Icd::Group
{
public:
  virtual QString xmlName() const { return "icd3"; }
  virtual ::Programmer::Properties properties() const;
  virtual ::Programmer::TargetPowerMode targetPowerMode() const { return ::Programmer::TargetPowerModeFromConfig; }
  virtual bool isPortSupported(PortType type) const { return type==PortType::USB; }
  virtual bool canReadVoltage(Device::VoltageType type) const;

protected:
  virtual void initSupported();
  virtual ::Programmer::Hardware *createHardware(::Programmer::Base &base, const ::Programmer::HardwareDescription &hd) const;
  virtual ::Programmer::DeviceSpecific *createDeviceSpecific(::Programmer::Base &base) const;
};

//-----------------------------------------------------------------------------
class ProgrammerGroup : public Group
{
public:
  virtual QString name() const { return "icd3"; }
  virtual QString label() const { return i18n("ICD3 Programmer"); }

protected:
  virtual ::Programmer::Base *createBase(const Device::Data *data) const { return new Programmer(*this, static_cast<const Pic::Data *>(data)); }
};

} // namespace

#endif
