/***************************************************************************
 * Copyright (C) 2010 Nicolas Hadacek <hadacek@kde.org>                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef HID_PORT_H
#define HID_PORT_H

#include "port_base.h"

#ifdef Q_OS_UNIX
#  include <linux/hiddev.h>
#endif


namespace Port
{

class HID : public Base
{
public:
  HID(Log::Base &base, uint vendorId, uint productId, uint size);
  virtual ~HID();
  virtual Description description() const { return Description(PortType::HID, QString::null); }

  bool write(const char *data, uint size);
  bool read(char *data, uint size);

  static QString devicePath(uint index);
  static bool getDeviceIds(uint index, uint& vendorId, uint& productId);
  static bool findDevice(uint vendorId, uint productId, uint& index);
  static bool isAvailable();
  static QStringList probedDeviceList();
  static void listReports(int fd);
  static void listReports(int fd, int type, const char* name);

protected:
  virtual bool internalOpen();
  virtual void internalClose();
  virtual void setSystemError(const QString &message);
  int handle() const { return _handle; }

private:
  uint _vendorId;
  uint _productId;
  int  _handle;

  hiddev_report_info     _repInfoIn;
  hiddev_report_info     _repInfoOut;
  hiddev_usage_ref_multi _refMultiIn;
  hiddev_usage_ref_multi _refMultiOut;
};

} // namespace

#endif
