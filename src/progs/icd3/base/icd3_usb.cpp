/***************************************************************************
 * Copyright (C) 2012 Nicolas Hadacek <hadacek@kde.org>                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "icd3_usb.h"

#include "progs/icd2/base/microchip.h"
#include "common/common/misc.h"

//------------------------------------------------------------------------------
Icd3::USBPort::USBPort(uint deviceId, Log::Base &log)
  : Port::USB(log, Microchip::VENDOR_ID, deviceId, 1, 0), _dataSend(false)
{}

bool Icd3::USBPort::connectDevice(const QString &mode)
{
  return true;
}

bool Icd3::USBPort::internalReceive(uint size, char *data, uint)
{
  return read(0x81, data, size);
}

bool Icd3::USBPort::internalSend(const char *data, uint size, uint)
{
  return write(0x02, data, size);
}

//------------------------------------------------------------------------------
Icd3::USBHardware::USBHardware(::Programmer::Base &base)
  : Hardware(base, new USBPort(ID_CLIENT, base))
{}

bool Icd3::USBHardware::internalConnect(const QString &mode)
{
  // load control messages for USB device if needed
  log(Log::DebugLevel::Extra, QString("need firmware ? %1").arg(USBPort::findDevice(Microchip::VENDOR_ID, ID_FIRMWARE)!=0));
  if ( Port::USB::findDevice(Microchip::VENDOR_ID, ID_FIRMWARE) ) {
    USBPort port(ID_FIRMWARE, *this);
    if ( !port.open() ) return false;
    //uint i = 0;
    //    while ( CONTROL_MESSAGE_DATA[i].bytes!=0 ) {
    //      if ( !port.sendControlMessage(CONTROL_MESSAGE_DATA[i]) ) return false;
    //      i++;
    //}
    port.close();
    for (uint i=0; i<10; i++) {
      log(Log::DebugLevel::Extra, QString("client here ? %1").arg(USBPort::findDevice(Microchip::VENDOR_ID, ID_CLIENT)!=0));
      if ( Port::USB::findDevice(Microchip::VENDOR_ID, ID_CLIENT) ) break;
      Port::msleep(1000);
    }
  }

  if ( !_port->open() ) return false;
  return static_cast<USBPort *>(_port)->connectDevice(mode);
}
