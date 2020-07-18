/***************************************************************************
 *   Copyright (C) 2005-2006 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef DEVICE_LIST_H
#define DEVICE_LIST_H

#include "devices/base/generic_device.h"
#include "common/common/lister.h"

namespace Device
{

class Lister : public ::Group::Lister<GroupBase>
{
public:
  static const Lister& instance();

  const Data& defaultData() const;
  const Data *data(const QString &device) const;
  const Data& checkedData(const QString &device) const;

private:
  Lister();

  static Lister* instance_;
};

} // namespace

#endif
