/***************************************************************************
 *   Copyright (C) 2005-2006 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "prog_group.h"

#include "generic_prog.h"
#include "prog_config.h"
#include "generic_debug.h"
#include "devices/base/device_group.h"

// order is important
const PURL::FileType Programmer::INPUT_FILE_TYPE_DATA[Nb_InputFileTypes] = {
  PURL::Coff, PURL::Cod, PURL::Hex
};

QString Programmer::Group::statusLabel(PortType type) const
{
  uint nb = 0;
  FOR_EACH(PortType, ptype) if ( isPortSupported(ptype) ) nb++;
  if ( nb<=0 ) return label();
  return label() + " (" + type.label() + ")";
}

Programmer::Generic *Programmer::Group::createProgrammer(bool targetSelfPowered, const Device::Data *data,
                                                         const HardwareDescription &hd, Log::Base* log) const
{
  Generic *generic = createBase(data);
  generic->Log::Base::setParent(log);
  if ( !(generic->group().properties() & Custom) ) {
    typedef ::Programmer::Base Base; // for G++ 3.3.6
    Base *base = static_cast<Base*>(generic);
    Hardware *hardware = createHardware(*base, hd);
    DeviceSpecific *ds = (data != NULL ? createDeviceSpecific(*base) : NULL);
    base->init(targetSelfPowered, hardware, ds);
  }
  return generic;
}

Debugger::Base *Programmer::Group::createDebugger(::Programmer::Base &base) const
{
  ::Debugger::Base *dbase = createDebuggerBase(base);
  if (dbase != NULL) {
    ::Debugger::DeviceSpecific *dspecific = createDebuggerDeviceSpecific(*dbase);
    ::Debugger::Specific *specific = createDebuggerSpecific(*dbase);
    dbase->init(dspecific, specific);
  }
  return dbase;
}

bool Programmer::Group::checkConnection(const HardwareDescription &hd) const
{
  Generic *generic = createProgrammer(false, NULL, hd, NULL);
  typedef ::Programmer::Base Base; // for G++ 3.3.6
  bool ok = (generic->group().properties() & Custom
             ? true : static_cast<Base*>(generic)->simpleConnectHardware());
  delete generic;
  return ok;
}

bool Programmer::Group::isSoftware() const
{
  FOR_EACH(PortType, type) if ( isPortSupported(type) ) return false;
  return true;
}

bool Programmer::Group::canReadVoltages() const
{
  FOR_EACH(Device::VoltageType, type)
    if ( canReadVoltage(type) ) return true;
  return false;
}
