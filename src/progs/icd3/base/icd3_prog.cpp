/***************************************************************************
 *   Copyright (C) 2012 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "icd3_prog.h"

#include "common/global/pfile.h"
#include "progs/base/prog_config.h"
#include "devices/list/device_list.h"
#include "icd3_usb.h"
#include "icd3_data.h"

//-----------------------------------------------------------------------------
void Icd3::ProgrammerBase::clear()
{
  Icd::ProgrammerBase::clear();
  _firmwareId = 0;
  //  _testData = TestData();
}

bool Icd3::ProgrammerBase::readFirmwareVersion()
{
  if ( !hardware()->setup() ) return false;
  if ( !Icd::ProgrammerBase::readFirmwareVersion() ) return false;
  _firmwareId = hardware()->getFirmwareId();
  return !hasError();
}

bool Icd3::ProgrammerBase::internalSetupHardware()
{
  ::Programmer::Config config;
  if ( !_targetSelfPowered && device()->architecture()==Pic::Architecture::P30F
       && !askContinue(i18n("It is not recommended to power dsPICs from ICD. Continue anyway?")) ) {
    logUserAbort();
    return false;
  }
  return Icd::ProgrammerBase::internalSetupHardware();
}

bool Icd3::ProgrammerBase::selfTest(bool ask)
{
  // ### TODO
  return true;
}

bool Icd3::ProgrammerBase::setupFirmware()
{
  // ### TODO
  return false;
}

//-----------------------------------------------------------------------------
Icd3::Programmer::Programmer(const ::Programmer::Group &group, const Pic::Data *data)
  : Icd3::ProgrammerBase(group, data, "icd3_programmer")
{}

//----------------------------------------------------------------------------
Programmer::Properties Icd3::Group::properties() const
{
   return ::Programmer::Programmer | ::Programmer::HasFirmware | ::Programmer::CanUploadFirmware
     | ::Programmer::NeedDeviceSpecificFirmware | ::Programmer::CanReleaseReset
     | ::Programmer::HasSelfTest | ::Programmer::CanReadMemory | ::Programmer::HasConnectedState;
}

bool Icd3::Group::canReadVoltage(Device::VoltageType type) const
{
  return ( type==Device::VoltageType::ProgrammerVpp || type==Device::VoltageType::TargetVdd || type==Device::VoltageType::TargetVpp );
}

Programmer::Hardware *Icd3::Group::createHardware(::Programmer::Base &base, const ::Programmer::HardwareDescription &hd) const
{
  return new USBHardware(base);
}

Programmer::DeviceSpecific *Icd3::Group::createDeviceSpecific(::Programmer::Base &base) const
{
  return new Icd::DeviceSpecific(base);
}
