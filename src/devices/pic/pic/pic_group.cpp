/***************************************************************************
 *   Copyright (C) 2005 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "pic_group.h"

#if !defined(NO_KDE)
# include <qpainter.h>
#endif

#include "pic_memory.h"
#include "devices/pic/base/pic_register.h"

std::auto_ptr<Device::Memory> Pic::Group::createMemory(const Device::Data &data) const
{
  return std::auto_ptr<Device::Memory>(new Memory(static_cast<const Pic::Data &>(data)));
}

QString Pic::Group::informationHtml(const Device::Data &data) const
{
  const Pic::Data &pdata = static_cast<const Pic::Data &>(data);
  // memory type
  QString s = htmlTableRow(i18n("Memory Type"), data.memoryTechnology().label());
  if ( pdata.isPresent(MemoryRangeType::Code) ) {
    uint nbw = pdata.nbWords(MemoryRangeType::Code);
    QString tmp = i18n("%1 words").arg(formatNumber(nbw));
    tmp += i18n(" (%2 bits)").arg(pdata.nbBitsWord(MemoryRangeType::Code));
    s += htmlTableRow(MemoryRangeType(MemoryRangeType::Code).label(), tmp);
  }
  if ( pdata.isPresent(MemoryRangeType::Eeprom) ) {
    uint nbw = pdata.nbWords(MemoryRangeType::Eeprom);
    QString tmp = i18n("%1 bytes").arg(formatNumber(nbw));
    tmp += i18n(" (%2 bits)").arg(pdata.nbBitsWord(MemoryRangeType::Eeprom));
    if ( !(pdata.range(MemoryRangeType::Eeprom).properties & Programmable) ) tmp += i18n(" (not programmable)");
    s += htmlTableRow(MemoryRangeType(MemoryRangeType::Eeprom).label(), tmp);
  }

  // io ports
  const Pic::RegistersData &rdata = pdata.registersData();
  QString tmp;
  if ( rdata.nbBanks!=0 ) {
    uint nb = 0;
    for (uint i=0; i<Device::MAX_NB_PORTS; i++) {
      if ( !rdata.hasPort(i) ) continue;
      uint nbBits = 0;
      for (uint k=0; k<Device::MAX_NB_PORT_BITS; k++) if ( rdata.hasPortBit(i, k) ) nbBits++;
      tmp += rdata.portName(i) + "[" + QString::number(nbBits) + "] ";
      nb++;
    }
    if ( nb==0 ) tmp = i18n("<none>");
    s += htmlTableRow(i18n("IO Ports"), tmp);
  }

  // features
  tmp = QString::null;
  FOR_EACH(Feature, feature) {
    if ( !pdata.hasFeature(feature) ) continue;
    if ( !tmp.isEmpty() ) tmp += ", ";
    tmp += feature.label();
  }
  if ( !tmp.isEmpty() ) s += htmlTableRow(i18n("Features"), tmp);

  return s;
}

QPixmap Pic::Group::memoryGraph(const Device::Data &data) const
{
  const Pic::Data &pdata = static_cast<const Pic::Data &>(data);
  std::list<Device::MemoryGraphData> ranges;
  FOR_EACH(Pic::MemoryRangeType, type) {
    if ( type==Pic::MemoryRangeType::Eeprom || !pdata.isPresent(type) ) continue;
    Device::MemoryGraphData data;
    data.startAddress = pdata.range(type).start;
    data.start = toHexLabel(pdata.range(type).start, pdata.nbCharsAddress());
    data.endAddress = pdata.range(type).end;
    data.end = toHexLabel(pdata.range(type).end, pdata.nbCharsAddress());
    data.label = type.label();
    ranges.push_back(data);
  }
  return Device::memoryGraph(ranges);
}
