/***************************************************************************
 *   Copyright (C) 2012 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "pickit3_prog.h"

#include "devices/base/device_group.h"
#include "devices/list/device_list.h"
#include "pickit3_data.h"

//-----------------------------------------------------------------------------
Pickit3::PicBase::PicBase(const ::Programmer::Group &group, const Pic::Data *data)
: ::Programmer::PicBase(group, data, "pic_pickit3_programmer")
{
}

bool Pickit3::PicBase::readFirmwareVersion()
{
  if (!hardware().getVersions()) return false;
  _firmwareVersion = hardware().getFirmwareVersion();
  return true;
}

const VersionData& Pickit3::PicBase::getOSVersion() const
{
    return hardware().getOSVersion();
}

const VersionData& Pickit3::PicBase::getAPVersion() const
{
  return hardware().getAPVersion();
}

uint Pickit3::PicBase::getAPId() const
{
  return hardware().getAPId();
}

BitValue Pickit3::PicBase::readDeviceId()
{
  BitValue v;
  hardware().connectToDevice(v);
  return v;
}

bool Pickit3::PicBase::setTarget()
{
  return hardware().sendConfigPacket();
}

bool Pickit3::PicBase::setupFirmware()
{
    const FamilyData& family = FAMILY_DATA[Pickit3::family(device()->name())];
    log(Log::DebugLevel::Normal, QString("  AP id is %1 and we want %2").arg(getAPId()).arg(family.apId));
    if (getAPId() == family.apId) return true;
    log(Log::LineType::Information, i18n("  Incorrect AP loaded."));

    // find AP file
    PURL::Directory dir = firmwareDirectory();
    if ( dir.isEmpty() ) return false;
    QString nameFilter = "PK3AP_" + QString::number(family.apId).rightJustify(2, '0') + "_??????.hex";
    QStringList files = dir.files(nameFilter);
    if ( files.isEmpty() ) {
      log(Log::LineType::Error, i18n("Could not find AP file \"%1\" in directory \"%2\".").arg(nameFilter).arg(dir.path()));
      return false;
    }

    // upload hex file
    PURL::Url url(dir, files[files.count()-1]);
    log(Log::DebugLevel::Normal, QString("  AP file: %1").arg(url.pretty()));
    Log::StringView sview;
    PURL::File file(url, sview);
    if ( !file.openForRead() ) {
      log(Log::LineType::Error, i18n("Could not open AP file \"%1\".").arg(url.pretty()));
      return false;
    }
    const Device::Data &data = *Device::Lister::instance().data("24FJ256GB106");
    Pic::Memory memory(static_cast<const Pic::Data &>(data));
    QStringList errors, warnings;
    Pic::Memory::WarningTypes warningTypes;
    if ( !memory.load(file.stream(), errors, warningTypes, warnings) ) {
      log(Log::LineType::Error, i18n("Could not read AP hex file \"%1\": %2.").arg(file.url().pretty()).arg(errors[0]));
    return false;
    }
    if ( warningTypes!=Pic::Memory::NoWarning ) {
      log(Log::LineType::Error, i18n("AP hex file seems incompatible with device 24FJ256GB106 inside Pickit3."));
      return false;
    }
    if ( !hardware().uploadAP(memory) ) {
      log(Log::LineType::Error, i18n("Failed to upload AP."));
      return false;
    }

    // check AP
    if ( !hardware().getVersions() ) return false;
    if (getAPId() != family.apId) {
      log(Log::LineType::Error, i18n("AP still incorrect after uploading."));
      return false;
    }
    log(Log::LineType::Information, i18n("  AP succesfully uploaded."));
    return true;
}

//-----------------------------------------------------------------------------
bool Pickit3::DeviceSpecific::setPowerOn()
{
  return hardware().setPower(true, _base.isTargetSelfPowered());
}

bool Pickit3::DeviceSpecific::setPowerOff()
{
  return hardware().setPower(false, _base.isTargetSelfPowered());
}

bool Pickit3::DeviceSpecific::setTargetPowerOn(bool on)
{
  return hardware().setPower(on, _base.isTargetSelfPowered());
}

bool Pickit3::DeviceSpecific::doEraseRange(Pic::MemoryRangeType type)
{
  return hardware().eraseRange(type);
}

bool Pickit3::DeviceSpecific::doErase(bool)
{
  return hardware().eraseAll();
}

bool Pickit3::DeviceSpecific::doRead(Pic::MemoryRangeType type, uint wordOffset, Device::Array &data, const ::Programmer::VerifyData *vdata)
{
  return hardware().read(type, wordOffset, data, vdata);
}

bool Pickit3::DeviceSpecific::doWrite(Pic::MemoryRangeType type, uint wordOffset, const Device::Array &data, bool)
{
  return hardware().write(type, wordOffset, data);
}

//-----------------------------------------------------------------------------
::Programmer::Base *Pickit3::Group::createBase(const Device::Data *data) const
{
  return new PicBase(*this, static_cast<const Pic::Data *>(data));
}

::Programmer::Hardware *Pickit3::Group::createHardware(::Programmer::Base &base, const ::Programmer::HardwareDescription &) const
{
  return new Hardware(base);
}

::Programmer::DeviceSpecific *Pickit3::Group::createDeviceSpecific(::Programmer::Base &base) const
{
  return new DeviceSpecific(base);
}

bool Pickit3::Group::canReadVoltage(Device::VoltageType type) const
{
  return ( type==Device::VoltageType::ProgrammerVpp || type==Device::VoltageType::TargetVdd );
}
