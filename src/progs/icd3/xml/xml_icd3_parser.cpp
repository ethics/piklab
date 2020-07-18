/***************************************************************************
 *   Copyright (C) 2012 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "xml_to_data/prog_xml_to_data.h"
#include "progs/icd3/base/icd3_data.h"

//-----------------------------------------------------------------------------
namespace Icd3
{

class XmlToData : public Programmer::XmlToData<Data>
{
public:
  XmlToData() : Programmer::XmlToData<Data>("icd3", "Icd3") {}

private:
  virtual uint familyIndex(const QString &family) const;
  virtual void parseData(QDomElement element, Data &data);
  virtual void outputData(const Data &data, QTextStream &s) const;
};

uint Icd3::XmlToData::familyIndex(const QString &family) const
{
  uint i = 0;
  for (; Icd3::FAMILY_DATA[i].name!=NULL; i++)
    if ( family==Icd3::FAMILY_DATA[i].name ) break;
  //  if ( Icd3::FAMILY_DATA[i].efid==0 ) qFatal(QString("Family \"%1\" is unknown").arg(family));
  return i;
}

void Icd3::XmlToData::parseData(QDomElement element, Data &data)
{
    bool ok;
    data.famid = fromHexLabel(element.attribute("famid"), 2, &ok);
    if ( !ok ) qFatal("Missing or malformed famid attribute");
  //  data.debugSupport = extractSupport(element.attribute("debug_support_type"));
}

void Icd3::XmlToData::outputData(const Data &data, QTextStream &s) const
{
    s << toHexLabel(data.famid, 2) << ", ";
  //  s << "::Group::Support::Type(" << data.debugSupport.type() << ")";
}

} // namespace

//-----------------------------------------------------------------------------
XML_MAIN(Icd3::XmlToData)
