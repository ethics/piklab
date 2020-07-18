/***************************************************************************
 * Copyright (C) 2010 Nicolas Hadacek <hadacek@kde.org>                    *
 * Copyright (C) 2009-2010 Alberto Maccioni                                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "hid_port.h"

#ifdef Q_OS_UNIX
#  include <sys/types.h>
#  include <sys/stat.h>
#  include <sys/ioctl.h>
#  include <fcntl.h>
#  include <linux/hiddev.h>
#  include <errno.h>
#  include <unistd.h>
#  include <sys/ioctl.h>
#endif

#include "common/common/number.h"

//-----------------------------------------------------------------------------
bool Port::HID::isAvailable()
{
#ifdef Q_OS_UNIX
  return true;
#else
  return false;
#endif
}

QString Port::HID::devicePath(uint index)
{
  return QString("/dev/usb/hiddev%1").arg(index);
}

bool Port::HID::getDeviceIds(uint index, uint& vendorId, uint& productId)
{
#ifdef Q_OS_UNIX
  const QString path = devicePath(index);
  const int fd = ::open(path.latin1(), O_RDONLY);
  if (fd < 0) {
     return false;
  }
  hiddev_devinfo info;
  const bool ok = (ioctl(fd, HIDIOCGDEVINFO, &info) >= 0);
  if (ok) {
    vendorId = info.vendor & 0xFFFF;
    productId = info.product & 0xFFFF;
  }
  ::close(fd);
  return ok;
#else
  return false;
#endif
}

QStringList Port::HID::probedDeviceList()
{
  QStringList list;
#ifdef Q_OS_UNIX
  for (uint i=0; ; ++i) {
    uint vendorId = 0x0;
    uint productId = 0x0;
    if (getDeviceIds(i, vendorId, productId)) {
      const QString s = QString("%1 - Vendor Id: %2 - Product Id: %3").arg(devicePath(i))
                        .arg(toLabel(NumberBase::Hex, vendorId, 4)).arg(toLabel(NumberBase::Hex, productId, 4));
      list.append(s);
    } else if (i >= 10) { // check at least hiddev0 to hiddev9
      break;
    }
  }
#endif
  return list;
}

bool Port::HID::findDevice(uint vendorId, uint productId, uint& index)
{
#ifdef Q_OS_UNIX
  for (index=0; ; ++index) {
    uint vid = 0x0;
    uint pid = 0x0;
    if (getDeviceIds(index, vid, pid)) {
      if (vid == vendorId && pid == productId) {
        return true;
      }
    } else if (index >= 10) { // check at least hiddev0 to hiddev9
      break;
    }
  }
 #endif
  return false;
}

#include <iostream>

void Port::HID::listReports(int fd)
{
  listReports(fd, HID_REPORT_TYPE_INPUT, "input");
  listReports(fd, HID_REPORT_TYPE_INPUT, "output");
  listReports(fd, HID_REPORT_TYPE_INPUT, "feature");
}

void Port::HID::listReports(int fd, int type, const char* name)
{
  hiddev_report_info rinfo;
  rinfo.report_type = type;
  rinfo.report_id = HID_REPORT_ID_FIRST;
  int ret = ioctl(fd, HIDIOCGREPORTINFO, &rinfo);

  while (ret >= 0) {
    for (__u32 i = 0; i < rinfo.num_fields; i++) {
      hiddev_field_info finfo;
      finfo.report_type = rinfo.report_type;
      finfo.report_id = rinfo.report_id;
      finfo.field_index = i;
      ioctl(fd, HIDIOCGFIELDINFO, &finfo);
      std::cout << name << " " << i << " " << finfo.maxusage << std::endl;
    }
    rinfo.report_id |= HID_REPORT_ID_NEXT;
    ret = ioctl(fd, HIDIOCGREPORTINFO, &rinfo);
  }
}

//-----------------------------------------------------------------------------
#ifdef Q_OS_UNIX

Port::HID::HID(Log::Base &base, uint vendorId, uint productId, uint size)
: Base(base), _vendorId(vendorId), _productId(productId),
 _handle(-1)
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
  _refMultiOut.num_values = size;

  _refMultiIn.uref.report_type = HID_REPORT_TYPE_INPUT;
  _refMultiIn.uref.report_id = HID_REPORT_ID_FIRST;
  _refMultiIn.uref.field_index = 0;
  _refMultiIn.uref.usage_index = 0;
  _refMultiIn.num_values = size;
}

Port::HID::~HID()
{
  close();
}

void Port::HID::setSystemError(const QString &message)
{
  log(Log::LineType::Error, message + QString(" (err=%1).").arg(errno));
}

bool Port::HID::internalOpen()
{
  uint index = 0;
  if (!findDevice(_vendorId, _productId, index)) {
    log(Log::LineType::Error, i18n("Could not find HID device (vendor=%1 product=%2).")
             .arg(toLabel(NumberBase::Hex, _vendorId, 4)).arg(toLabel(NumberBase::Hex, _productId, 4)));
    return false;
  }
  QString path = devicePath(index);
  log(Log::DebugLevel::Extra, QString("found HID device as \"%1\"").arg(path));
  _handle = ::open(path.toAscii(), O_RDONLY);
  if (_handle < 0) {
    setSystemError(i18n("Error opening HID device."));
    return false;
  }
  listReports(_handle);
  return true;
}

void Port::HID::internalClose()
{
  if (_handle >= 0) {
    ::close(_handle);
    _handle = -1;
  }
}

bool Port::HID::write(const char *data, uint size)
{
  for (uint i=0; i<size; ++i) _refMultiOut.values[i] = data[i];
  return ioctl(_handle, HIDIOCSUSAGES, &_refMultiOut) >= 0
    && ioctl(_handle, HIDIOCSREPORT, &_repInfoOut) >= 0;
}

bool Port::HID::read(char *data, uint size)
{
  if (ioctl(_handle, HIDIOCGUSAGES, &_refMultiIn) < 0
      || ioctl(_handle, HIDIOCGREPORT, &_repInfoIn) < 0) {
    return false;
  }
  for (uint i=0; i<size; ++i) data[i] = _refMultiIn.values[i];
  return true;
}

#endif
