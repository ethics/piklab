/***************************************************************************
 *   Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "xml_to_data/prog_xml_to_data.h"
#include "progs/icd2/base/icd2_data.h"

#include "devices/base/device_group.h"
#include "devices/pic/base/pic.h"

//-----------------------------------------------------------------------------
namespace Icd2
{

class XmlToData : public Programmer::XmlToData<Data>
{
public:
  XmlToData() : Programmer::XmlToData<Data>("icd2", "Icd2") {}
  virtual void parse();

private:
  virtual uint familyIndex(const QString &family) const;
  virtual void parseData(QDomElement element, Data &data);
  virtual void outputData(const Data &data, QTextStream &s) const;
};

} // namespace

void Icd2::XmlToData::parse()
{
  Programmer::XmlToData<Data>::parse();

  const Device::GroupBase* group = Device::Lister::instance().group("pic");
  for(Group::Base::const_iterator it = group->begin(); it != group->end(); ++it) {
    const Pic::Data* data = static_cast<const Pic::Data*>(it->second.data);
    QString name = it->first;
    if (_map.contains(name)) continue;
    PData pdata;
    switch (data->architecture().type()) {
      case Pic::Architecture::P18J:
        pdata.family = 0x11;
        pdata.data.famid = 0x40;
        break;
      case Pic::Architecture::P24F:
        pdata.family = 0x14;
        pdata.data.famid = 0x41;
        break;
      case Pic::Architecture::P30F:
        pdata.family = 0x10;
        pdata.data.famid = 0xBB;
        break;
      case Pic::Architecture::P24H:
      case Pic::Architecture::P33F:
        pdata.family = 0x14;
        pdata.data.famid = 0x40;
        break;
      default:
        continue;
    }
    pdata.data.debugSupport = Group::Support::Untested;
    _map[name] = pdata;
  }
}

uint Icd2::XmlToData::familyIndex(const QString &family) const
{
  uint i = 0;
  for (; Icd2::FAMILY_DATA[i].efid!=0; i++)
    if ( family==Icd2::FAMILY_DATA[i].name ) break;
  if ( Icd2::FAMILY_DATA[i].efid==0 ) qFatal(QString("Family \"%1\" is unknown").arg(family));
  return i;
}

void Icd2::XmlToData::parseData(QDomElement element, Data &data)
{
  bool ok;
  data.famid = fromHexLabel(element.attribute("famid"), 2, &ok);
  if ( !ok ) qFatal("Missing or malformed famid attribute");
  data.debugSupport = extractSupport(element.attribute("debug_support_type"));
}

void Icd2::XmlToData::outputData(const Data &data, QTextStream &s) const
{
  s << toHexLabel(data.famid, 2) << ", ";
  s << "::Group::Support::Type(" << data.debugSupport.type() << ")";
}

//-----------------------------------------------------------------------------
XML_MAIN(Icd2::XmlToData)
