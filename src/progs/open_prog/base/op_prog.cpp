/***************************************************************************
 *   Copyright (C) 2010 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "op_prog.h"

#include "devices/base/device_group.h"
#include "devices/list/device_list.h"
#include "op_mem24.h"

//-----------------------------------------------------------------------------
::Programmer::Base *OpenProg::Group::createBase(const Device::Data *data) const
{
  if ( data==NULL || data->group().name()=="pic" ) return new PicBase(*this, static_cast<const Pic::Data *>(data));
  return new Mem24Base(*this, static_cast<const Mem24::Data *>(data));
}

::Programmer::Hardware *OpenProg::Group::createHardware(::Programmer::Base &base, const ::Programmer::HardwareDescription &) const
{
  return new Hardware(base);
}

//-----------------------------------------------------------------------------
::Programmer::DeviceSpecific *OpenProg::DGroup::createDeviceSpecific(::Programmer::Base &base) const
{
  if ( base.device()->group().name()=="pic" ) {
    return Group::createDeviceSpecific(base);
  }
  return new Mem24DeviceSpecific(base);
}

void OpenProg::DGroup::initSupported()
{
  Group::initSupported();
  const ::Group::Base& gmem24 = *Device::Lister::instance().group("mem24");
  for (::Group::Base::const_iterator pit = gmem24.begin(); pit != gmem24.end(); ++pit) {
    addDevice(pit->first, pit->second.data, pit->second.support);
  }
}
