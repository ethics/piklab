/***************************************************************************
 *   Copyright (C) 2012 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "xml_to_data/prog_xml_to_data.h"

#include "progs/pickit3/base/pickit3_data.h"
#include "devices/base/device_group.h"
#include "devices/pic/base/pic.h"
#include "devices/pic/base/pic_prog_family.h"
#include "coff/base/coff_data.h"

//-----------------------------------------------------------------------------
namespace Pickit3
{

class XmlToData : public Programmer::XmlToData<Data>
{
public:
  XmlToData() : Programmer::XmlToData<Data>("pickit3", "Pickit3") {}

private:
  virtual uint familyIndex(const QString &family) const;
  virtual void parseData(QDomElement element, Data& data);
  virtual void outputData(const Data &data, QTextStream &s) const;
};

} // namespace

uint Pickit3::XmlToData::familyIndex(const QString &family) const
{
  uint i = 0;
  for (; FAMILY_DATA[i].name != NULL; i++)
    if (family == FAMILY_DATA[i].name) break;
  if (FAMILY_DATA[i].name == NULL) qFatal(QString("Family \"%1\" is unknown").arg(family));
  return i;
}

void Pickit3::XmlToData::parseData(QDomElement element, Data& data)
{
  const Device::Data* ddata = Device::Lister::instance().data(currentDevice());
  if (ddata == NULL || ddata->group().name() != "pic") qFatal("non-pic device not supported");
  const Pic::Data& pdata = static_cast<const Pic::Data&>(*ddata);
  if (pdata.progFamilyData() == NULL) qFatal("no prog family");
  if (!Coff::isSupported(currentDevice())) qFatal("no coff");

  bool ok;
  data.eraseAlgo = fromHexLabel(element.attribute("erase_algo"), 4, &ok);
  if (!ok) qFatal(QString("Incorrect erase algo: %1").arg(element.attribute("erase_algo")));
  data.flags = fromHexLabel(element.attribute("flags"), 4, &ok);
  if (!ok) qFatal(QString("Incorrect flags: %1").arg(element.attribute("flags")));
}

void Pickit3::XmlToData::outputData(const Data &data, QTextStream &s) const
{
  s << toHexLabel(data.eraseAlgo, 4) << ", ";
  s << toHexLabel(data.flags, 4);
}

//-----------------------------------------------------------------------------
XML_MAIN(Pickit3::XmlToData)
