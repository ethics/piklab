/***************************************************************************
 *   Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "picdev.h"
#include "devices/pic/base/pic.h"
#include "devices/base/device_group.h"
#include "devices/list/device_list.h"
#include "progs/psp/base/psp.h"

int main(int, char **)
{
  for (uint i=0; deviceList[i]; i++) {
    const Device::Data *data = Device::Lister::instance().data(deviceList[i]->name);
    if ( data==0 ) {
      qDebug("%s: not found", deviceList[i]->name);
      continue;
    }
    if ( data->group().name()!="pic" ) {
      qDebug("%s: not pic", deviceList[i]->name);
      continue;
    }
    const Pic::Data &pdata = static_cast<const Pic::Data &>(*data);
    if ( !Psp::isSupported(data->name()) ) {
      qDebug("%s: not supported", deviceList[i]->name);
      continue;
    }
    qDebug("%s:", deviceList[i]->name);

    // check normal info
    QMemArray<uchar> array = Psp::createDeviceInfo(pdata);
    for (uint k=0; k<array.count()-1; k++)
      if ( array[k]!=deviceList[i]->def[k] ) qDebug("  dinfo different at %i: %s (%s)", k, toHex(array[k], 2).latin1(), toHex(deviceList[i]->def[k], 2).latin1());

    // check extra info
    array = Psp::createConfigInfo(pdata);
    for (uint k=0; k<array.count()-1; k++)
      if ( array[k]!=deviceList[i]->defx[k] ) qDebug(" cinfo different at %i: %s (%s)", k, toHex(array[k], 2).latin1(), toHex(deviceList[i]->def[k], 2).latin1());
  }

  return 0;
}
