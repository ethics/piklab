/***************************************************************************
 *   Copyright (C) 2010 Nicolas Hadacek <hadacek@kde.org>                  *
 *   Copyright (C) 2009-2010 Alberto Maccioni                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "op.h"

#include <sys/ioctl.h>
#include <linux/hiddev.h>

#include "progs/base/generic_prog.h"
#include "op_code.h"

using namespace std;

//-----------------------------------------------------------------------------
namespace OpenProg
{
  const uint BUF_SIZE = 64;
} // namespace

//-----------------------------------------------------------------------------
OpenProg::Array::Array()
: _data(BUF_SIZE)
{
  reset();
}

void OpenProg::Array::reset()
{
  _data.fill(0x0);
  _index = 1; // first char is always 0x0
}

uint OpenProg::Array::available() const
{
  return length() - _index - 1; // keep 1 for FLUSH
}

bool OpenProg::Array::empty() const
{
  return _index == 1;
}

OpenProg::Array& OpenProg::Array::operator+=(uchar c)
{
  ASSERT(_index < _data.count());
  _data[_index] = c;
  ++_index;
  return *this;
}

void OpenProg::Array::set(const uchar* array, uint size)
{
  reset();
  append(array, size);
}

void OpenProg::Array::append(const uchar* array, uint size)
{
  ASSERT(size <= available());
  for (uint i = 0; i < size; ++i) {
    *this += array[i];
  }
}

QString OpenProg::Array::pretty() const
{
  int end = _data.count() - 1;
  for (; end>=0; end--)
    if ( _data[end]!=0x0 ) break;
  QString s;
  for (int i=0; i<=end; i++) s += toPrintable(_data[i], PrintEscapeAll);
  return s;
}

//-----------------------------------------------------------------------------
class OpenProg::HIDPort::Private
{
public:
  Private();
  bool write(int handle, const Array& array);
  bool read(int handle, Array& array);

private:
  hiddev_report_info     _repInfoIn;
  hiddev_report_info     _repInfoOut;
  hiddev_usage_ref_multi _refMultiIn;
  hiddev_usage_ref_multi _refMultiOut;
};

OpenProg::HIDPort::Private::Private()
{
  _repInfoOut.report_type = HID_REPORT_TYPE_OUTPUT;
  _repInfoOut.report_id = HID_REPORT_ID_FIRST;
  _repInfoOut.num_fields = 1;

  _repInfoIn.report_type = HID_REPORT_TYPE_INPUT;
  _repInfoIn.report_id = HID_REPORT_ID_FIRST;
  _repInfoIn.num_fields = 1;

  _refMultiOut.uref.report_type = HID_REPORT_TYPE_OUTPUT;
  _refMultiOut.uref.report_id = HID_REPORT_ID_FIRST;
  _refMultiOut.uref.field_index = 0;
  _refMultiOut.uref.usage_index = 0;
  _refMultiOut.num_values = BUF_SIZE;

  _refMultiIn.uref.report_type = HID_REPORT_TYPE_INPUT;
  _refMultiIn.uref.report_id = HID_REPORT_ID_FIRST;
  _refMultiIn.uref.field_index = 0;
  _refMultiIn.uref.usage_index = 0;
  _refMultiIn.num_values = BUF_SIZE;
}

bool OpenProg::HIDPort::Private::write(int handle, const Array& array)
{
  for (uint i=0; i<BUF_SIZE; ++i) _refMultiOut.values[i] = array[i];
  return ioctl(handle, HIDIOCSUSAGES, &_refMultiOut) >= 0
         && ioctl(handle, HIDIOCSREPORT, &_repInfoOut) >= 0;
}

bool OpenProg::HIDPort::Private::read(int handle, Array& array)
{
  if (ioctl(handle, HIDIOCGUSAGES, &_refMultiIn) < 0
      || ioctl(handle, HIDIOCGREPORT, &_repInfoIn) < 0) {
    return false;
  }
  array.reset();
  for (uint i=1; i<BUF_SIZE; ++i) array += _refMultiIn.values[i];
  return true;
}

//-----------------------------------------------------------------------------
OpenProg::HIDPort::HIDPort(Log::Base &log)
: Port::HID(log, 0x4D8, 0x100),
  _private(new Private)
{}

OpenProg::HIDPort::~HIDPort()
{
  delete _private;
  _private = NULL;
}

bool OpenProg::HIDPort::send(const Array &cmd)
{
  log(Log::DebugLevel::Extra, QString("send command: \"%1\"").arg(cmd.pretty()));
  return _private->write(handle(), cmd);
}

bool OpenProg::HIDPort::receive(Array &array)
{
  if (!_private->read(handle(), array)) {
    return false;
  }
  log(Log::DebugLevel::Max, QString("received: \"%1\"").arg(array.pretty()));
  return true;
}

//-----------------------------------------------------------------------------
OpenProg::Hardware::Hardware(::Programmer::Base &base)
  : ::Programmer::PicHardware(base, new HIDPort(base), QString::null)
{
}

bool OpenProg::Hardware::internalConnectHardware()
{
  if (!port().open()) {
    return false;
  }


  Array array;
/*  array += PROG_RST;
  if (!command(array, 2)) {
    return false;
  }
  QString fid = toHex(array[2], 2) + "." + toHex(array[3], 2) + "." + toHex(array[4], 2);
  QString hid = toHex(array[5], 2) + "." + toHex(array[6], 2) + "." + toHex(array[7], 2);
  log(Log::LineType::Information, QString("firmware: \"%1\" hardware: \"%2\"").arg(fid).arg(hid));
*/
  // ### REMOVE ME
  const uchar cmd[39] = { 0x07, 0x00, 0x01, 0x64, 0x07, 0x01, 0x61, 0xA8, 0x0E, 0x00,
                          0x0F, 0x00, 0x0E, 0x01, 0x0A, 0x0A, 0x0E, 0x05, 0x00, 0x11,
                          0xFF, 0xFF, 0x17, 0x06, 0x14, 0x16, 0x14, 0x00, 0x0E, 0x01,
                          0x0E, 0x00, 0x0F, 0x00, 0x0A, 0x0E, 0x01, 0x0E, 0x05 };
  array.set(cmd, 39);
  if (!command(array, 50)) {
      return false;
  }

  return true;
}

bool OpenProg::Hardware::command(Array& data, uint wait)
{
  if (data.empty()) {
    return true;
  }
  data += FLUSH;
  if (!port().send(data)) {
    return false;
  }
  Port::msleep(wait);
  data.reset();
  if (!port().receive(data)) {
     return false;
  }
  Port::msleep(2); // needed ?
  return true;
}
