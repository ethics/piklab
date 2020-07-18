/***************************************************************************
 *   Copyright (C) 2005-2006 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef DEVICE_GROUP_H
#define DEVICE_GROUP_H

#include "common/gui/global_ui.h"
#include "generic_device.h"
#include "common/common/group.h"
#include "common/common/streamer.h"
namespace Debugger { class DeviceSpecific; class Base; }

namespace Device
{
class Memory;

//----------------------------------------------------------------------------
class MemoryRange {
public:
  MemoryRange() {}
  virtual ~MemoryRange() {}
  virtual bool all() const { return true; }
};

//----------------------------------------------------------------------------
class GroupBase : public ::Group::Base
{
public:
  virtual std::auto_ptr<Memory> createMemory(const Device::Data &data) const = 0;
  virtual QString informationHtml(const Device::Data &data) const = 0;
  virtual QPixmap memoryGraph(const Device::Data &data) const = 0;

protected:
  virtual void addDevice(const QString &name, const Device::Data *data, ::Group::Support support) {
    const_cast<Device::Data *>(data)->_group = this;
    ::Group::Base::addDevice(name, data, support);
  }
};

template <class DataType>
class Group : public GroupBase, public DataStreamer<DataType>
{
protected:
  virtual void initSupported() {
    QValueList<DataType *> list = this->fromCppString(dataStream(), dataSize());
    for (uint i=0; i<uint(list.count()); i++) addDevice(list[i]->name(), list[i], ::Group::Support::Tested);
  }
  virtual uint dataSize() const = 0;
  virtual const char *dataStream() const = 0;
};

//----------------------------------------------------------------------------
QColor statusColor(Status status);
QPixmap vddGraph(const QString &xLabel, const QString &yLabel, const QValueVector<RangeBox> &boxes);
const Package *barPackage(const char *name, const Data &data);
QPixmap pinsGraph(const Package &package);

QString htmlInfo(const Data &data, const QString &deviceHref, const QString &documentHtml);
const Package* preferredPackage(const Device::Data &data);
QString pinsHtml(const Device::Data &data, const QString &imagePrefix);
QString pinsPixmapLabel(const Device::Data &data);
QPixmap pinsPixmap(const Device::Data &data);
QString voltageFrequencyHtml(const Device::Data &data, const QString &imagePrefix);
QPixmap voltageFrequencyPixmap(const Device::Data &data, const QString& label);
QString voltagePixmapLabel(const Device::Data &data, const Device::FrequencyRange &fr);
QPixmap voltagePixmap(const Device::FrequencyRange& fr);

struct MemoryGraphData
{
  Address startAddress, endAddress;
  QString start, end, label;
  int height;

  bool operator<(const Device::MemoryGraphData &data) const;
};

extern QPixmap memoryGraph(const std::list<MemoryGraphData> &ranges);

} // namespace

#endif
