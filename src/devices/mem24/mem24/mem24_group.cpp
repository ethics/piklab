/***************************************************************************
 *   Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "mem24_group.h"

#include "mem24_memory.h"

std::auto_ptr<Device::Memory> Mem24::Group::createMemory(const Device::Data &data) const
{
  return std::auto_ptr<Device::Memory>(new Memory(static_cast<const Mem24::Data &>(data)));
}

QString Mem24::Group::informationHtml(const Device::Data &data) const
{
  const Mem24::Data &mdata = static_cast<const Mem24::Data &>(data);
  QString tmp = i18n("%1 bytes").arg(formatNumber(mdata.nbBytes()));
  return htmlTableRow(i18n("Memory Size"), tmp);
}

QPixmap Mem24::Group::memoryGraph(const Device::Data &data) const
{
  const Mem24::Data &mdata = static_cast<const Mem24::Data &>(data);
  uint offset = 0x0;
  std::list<Device::MemoryGraphData> ranges;
  for (uint i=0; i<mdata.nbBlocks(); i++) {
    Device::MemoryGraphData data;
    data.startAddress = offset;
    offset += mdata.nbBytes() / mdata.nbBlocks();
    data.endAddress = offset - 1;
    data.start = toHexLabel(data.startAddress, mdata.nbCharsAddress());
    data.end = toHexLabel(data.endAddress, mdata.nbCharsAddress());
    data.label = i18n("Block #%1").arg(i+1);
    ranges.push_back(data);
  }
  return Device::memoryGraph(ranges);
}
