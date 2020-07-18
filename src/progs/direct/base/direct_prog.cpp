/***************************************************************************
 *   Copyright (C) 2005-2006 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "direct_prog.h"

#include "devices/base/device_group.h"
#include "devices/list/device_list.h"
#include "direct_prog_config.h"
#include "direct_mem24.h"
//#include "direct_30.h"

//-----------------------------------------------------------------------------
Hardware::Config *Direct::Group::hardwareConfig() const
{
  return new Config;
}

::Programmer::Base *Direct::Group::createBase(const Device::Data *data) const
{
  if ( data==0 || data->group().name()=="pic" ) return new PicBase(*this, static_cast<const Pic::Data *>(data));
  return new Mem24Base(*this, static_cast<const Mem24::Data *>(data));
}

::Programmer::Hardware *Direct::Group::createHardware(::Programmer::Base &base, const ::Programmer::HardwareDescription &hd) const
{
  Config config;
  HardwareData *hdata = static_cast<HardwareData *>(config.hardwareData(hd.name));
  ASSERT( hdata->portType==hd.port.type );
  ::Programmer::Hardware *hardware = 0;
  if ( hd.port.type==PortType::Serial ) hardware = new SerialHardware(base, hd.port.device, *hdata);
  else hardware = new ParallelHardware(base, hd.port.device, *hdata);
  delete hdata;
  return hardware;
}

//-----------------------------------------------------------------------------
::Programmer::DeviceSpecific *Direct::DGroup::createDeviceSpecific(::Programmer::Base &base) const
{
  if ( base.device()->group().name()=="pic" ) {
    //    if ( static_cast<const Pic::Data *>(base.device())->architecture()==Pic::Architecture::P30X ) return new Pic30(base);
    return Group::createDeviceSpecific(base);
  }
  return new Mem24DeviceSpecific(base);
}

void Direct::DGroup::initSupported()
{
  Group::initSupported();
  //  const ::Group::Base *gpic = Device::lister().group("pic");
  //  for (pit=gpic->begin(); pit!=gpics->end(); ++pit) {
  //    ::Group::DeviceData data = pit.data();
  //    if ( static_cast<const Pic::Data *>(data.data)->architecture()!=Pic::Architecture::P30X ) continue;
  //    data.supportType = ::Group::Untested;
  //    addDevice(data);
  //  }
  const ::Group::Base& gmem24 = *Device::Lister::instance().group("mem24");
  for (::Group::Base::const_iterator pit = gmem24.begin(); pit != gmem24.end(); ++pit) {
    addDevice(pit->first, pit->second.data, pit->second.support);
  }
}
