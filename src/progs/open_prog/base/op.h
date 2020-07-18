/***************************************************************************
 *   Copyright (C) 2010 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef OP_H
#define OP_H

#include "common/port/hid_port.h"
#include "devices/pic/prog/pic_prog.h"

namespace OpenProg
{
//-----------------------------------------------------------------------------
class Array
{
public:
  Array();
  void reset();
  uint length() const { return _data.count(); }
  uint available() const;
  bool empty() const;
  QString pretty() const;
  uchar operator[](uint i) const { return _data[i]; }
  Array& operator+=(uchar c);
  void set(const uchar* array, uint size);
  void append(const uchar* array, uint size);

private:
  QMemArray<uchar> _data;
  uint             _index;

friend class Port;
};

//-----------------------------------------------------------------------------
class HIDPort : public Port::HID
{
public:
  HIDPort(Log::Base& log);
  virtual ~HIDPort();

  bool send(const Array& cmd);
  bool receive(Array& array);

private:
  class Private;
  Private* _private;
};

//-----------------------------------------------------------------------------
class Hardware : public ::Programmer::PicHardware
{
public:
  Hardware(::Programmer::Base &base);

  bool command(Array& data, uint msWait = 5);

private:
  virtual bool internalConnectHardware();
  HIDPort& port() { return static_cast<HIDPort&>(*_port); }

  friend class Programmer;
};

} // namespace

#endif
