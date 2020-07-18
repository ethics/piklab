/***************************************************************************
 *   Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef GROUP_H
#define GROUP_H

#include "global.h"
#include "key_enum.h"

namespace Device { class Data; }

namespace Group
{
//-----------------------------------------------------------------------------
class BaseGui;
BEGIN_DECLARE_ENUM(Support)
  None = 0, Untested, Tested
END_DECLARE_ENUM_KEY_LABEL(Support)

//-----------------------------------------------------------------------------
class Base : public NoCopyClass
{
public:
  class Data {
  public:
    Data() : data(NULL), support(Support::None) {}
    const Device::Data* data;
    Support             support;
  };
  typedef std::map<QString, Data> Map;
  typedef Map::const_iterator const_iterator;

  Base();
  virtual ~Base() {}
  virtual QString name() const = 0;
  virtual QString label() const = 0;
  const_iterator begin() const;
  const_iterator end() const;
  const Data& deviceData(const QString &device) const;
  bool isSupported(const QString &device) const { return deviceData(device).support!=Support::None; }
  QStringList supportedDevices() const;
  uint count() const;
  const BaseGui *gui() const { return _gui; }
  void checkInitSupported();

protected:
  virtual void init();
  virtual void addDevice(const QString &name, const Device::Data *data, Support support);
  virtual void initSupported() = 0;

  Map _devices;

private:
  const BaseGui *_gui;
  bool _initialized;

  template <class GroupType> friend class Lister;
};

//-----------------------------------------------------------------------------
class BaseGui
{
public:
  BaseGui() : _group(NULL) {}
  virtual ~BaseGui() {}
  Base &group() const { return *_group; }

private:
  Base *_group;

  template <class GroupType> friend class Lister;
};

} // namespace

#endif
