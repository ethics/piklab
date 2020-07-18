/***************************************************************************
 * Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef PICKIT_PROG_H
#define PICKIT_PROG_H

#include "progs/icd2/base/microchip.h"
#include "progs/base/prog_group.h"
#include "pickit.h"

namespace Pickit
{
class Hardware;

//----------------------------------------------------------------------------
class Base : public Programmer::PicBase
{
public:
  Base(const Programmer::Group &group, const Pic::Data *data);
  DeviceSpecific* deviceSpecific() const { return static_cast<DeviceSpecific*>(_deviceSpecific.get()); }
  virtual bool deviceHasOsccalRegeneration() const = 0;
  bool regenerateOsccal(const PURL::Url &url);
  virtual bool readFirmwareVersion();

protected:
  Hardware &hardware();
};

//----------------------------------------------------------------------------
class Group : public Programmer::PicGroup
{
public:
  virtual ::Programmer::TargetPowerMode targetPowerMode() const { return ::Programmer::TargetExternallyPowered; }
  virtual bool isPortSupported(PortType type) const { return ( type==PortType::USB ); }
};

} // namespace

#endif
