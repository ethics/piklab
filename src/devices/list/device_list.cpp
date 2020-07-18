/***************************************************************************
 *   Copyright (C) 2005-2006 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "device_list.h"

#include "devices/base/device_group.h"

Device::Lister* Device::Lister::instance_ = NULL;

const Device::Lister& Device::Lister::instance()
{
  if (instance_ == NULL) {
    instance_ = new Lister;
  }
  return *instance_;
}

const Device::Data& Device::Lister::defaultData() const
{
  return *data("16F871");
}

const Device::Data* Device::Lister::data(const QString& device) const
{
  for (ConstIterator it=begin(); it!=end(); it++) {
    const Data* data = static_cast<const Data*>(it.data()->deviceData(device).data);
    if (data != NULL) return data;
  }
  return NULL;
}

const Device::Data& Device::Lister::checkedData(const QString& device) const
{
  if ( isSupported(device) ) return *data(device);
  if ( device.startsWith("p") ) return checkedData(device.mid(1)); // compat mode for PiKdev
  return defaultData();
}
