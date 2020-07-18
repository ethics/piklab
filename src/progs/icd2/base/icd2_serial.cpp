/***************************************************************************
 *   Copyright (C) 2005 Nicolas Hadacek <hadacek@kde.org>                  *
 *   Copyright (C) 2002-2003 Stephen Landamore <stephen@landamore.com>     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "icd2_serial.h"

#include "common/common/misc.h"

//-----------------------------------------------------------------------------
Icd2::SerialPort::SerialPort(const QString &device, Log::Base &log)
  : Port::Serial(device, NeedDrain | NeedFlush, log)
{}

bool Icd2::SerialPort::open(Speed speed)
{
  if ( !Port::Serial::open() ) return false;
  return setMode(NoInputFlag, ByteSize8 | EnableReceiver | HardwareFlowControl, speed, 0);
}

//-----------------------------------------------------------------------------
Icd2::SerialHardware::SerialHardware(::Programmer::Base &base, const QString &portDevice)
  : Hardware(base, new SerialPort(portDevice, base))
{}

bool Icd2::SerialHardware::internalConnect(const QString &mode)
{
  if ( !static_cast<SerialPort *>(_port)->open(Port::Serial::S19200) ) return false;
  if ( !reset() ) return false;
  log(Log::DebugLevel::Max, QString("Send: \"%1\"").arg("Z"));
  if ( !_port->send("Z", 1) ) return false;
  QString s;
  if ( !_port->receive(4, s) ) return false;
  log(Log::DebugLevel::Max, QString("Received: \"%1\"").arg(s));

  if ( !reset() ) return false;
  for (uint i = 0; ; ++i) {
    const QByteArray a = toAscii(mode);
    log(Log::DebugLevel::Max, QString("Send: \"%1\"").arg(mode));
    if ( !_port->send(a.data(), a.count()) ) return false;
    if ( !_port->receive(1, s) ) return false;
    log(Log::DebugLevel::Max, QString("Received: \"%1\"").arg(s));
    if ( s.upper()==mode ) {
      break;
    }
    if (i == 10) {
      log(Log::LineType::Error, i18n("Failed to set port mode to '%1'.").arg(mode));
      return false;
    }
    Port::msleep(100);
  }
  //log(Log::Debug, "set fast speed");
  //if ( !setFastSpeed() ) return false;
  return true;
}

bool Icd2::SerialHardware::reset()
{
  log(Log::DebugLevel::Max, QString("Reset icd2 serial port"));
  static_cast<Port::Serial *>(_port)->setPinOn(Port::Serial::DTR, false, Port::PositiveLogic); // Trigger DTR to reset icd2
  Port::msleep(10);
  static_cast<Port::Serial *>(_port)->setPinOn(Port::Serial::DTR, true, Port::PositiveLogic); // remove reset
  Port::msleep(10);
  return true;
}

bool Icd2::SerialHardware::setFastSpeed()
{
  if ( !command("4D", 0) ) return false; // go faster
  static_cast<SerialPort *>(_port)->open(Port::Serial::S57600);
  Port::msleep(100); // ...we do need to delay here
  return !hasError();
}
