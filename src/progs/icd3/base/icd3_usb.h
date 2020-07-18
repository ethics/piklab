/***************************************************************************
 * Copyright (C) 2012 Nicolas Hadacek <hadacek@kde.org>                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef ICD3_USB_H
#define ICD3_USB_H

#include "icd3.h"
#include "common/port/usb_port.h"

namespace Icd3
{

//-----------------------------------------------------------------------------
class USBPort : public Port::USB
{
public:
  USBPort(uint deviceId, Log::Base &log);
  bool connectDevice(const QString &mode);
  bool poll(uint size, QString &s);
  bool poll(uint size, QMemArray<uchar> &data);
  bool dataSend(const char *data, uint size);
  bool dataReceive(uint size, QString &s);

private:
  uchar _seqnum;
  bool  _poll;
  uint  _ctype;
  bool  _dataSend;

  enum SequenceType { Receive = 0, Send, Connect, Poll, Nb_SequenceTypes };
  struct SequenceData {
    char type;
  };
  static const SequenceData SEQUENCE_DATA[Nb_SequenceTypes];
  bool doSequence(SequenceType type, char *data, uint size);
  virtual bool internalSend(const char *data, uint size, uint timeout = 0);
  virtual bool internalReceive(uint size, char *data, uint timeout = 0);
};

//------------------------------------------------------------------------------
class USBHardware : public Hardware
{
public:
  USBHardware(::Programmer::Base &base);

private:
  //  static const Port::USB::ControlMessageData CONTROL_MESSAGE_DATA[];
  enum {
         ID_FIRMWARE = 0x9000, // ### FIXME ICD3 id before usb firmware is transmitted
         ID_CLIENT = 0x9009    // ICD3 id after firmware is transmitted
  };
  virtual bool internalConnect(const QString &mode);
};

} // namespace

#endif
