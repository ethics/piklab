/***************************************************************************
 *   Copyright (C) 2010 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "xml_to_data/prog_xml_to_data.h"
#include "devices/pic/base/pic.h"
#include "progs/open_prog/base/op_data.h"

namespace OpenProg
{
class XmlToData : public ::Programmer::XmlToData<Data>
{
public:
  XmlToData() : ::Programmer::XmlToData<Data>("op", "OpenProg") {}

private:
  virtual void parseData(QDomElement element, Data &data);
  virtual void includes(QTextStream &s) const;
  virtual void outputFunctions(QTextStream &s) const;
};

void OpenProg::XmlToData::parseData(QDomElement element, Data &)
{
  const Device::Data* d = Device::Lister::instance().data(currentDevice());
  if (d == NULL) qFatal("Invalid device name");
}

void OpenProg::XmlToData::includes(QTextStream &s) const
{
  Programmer::XmlToData<Data>::includes(s);
  s << "#include \"op_16F.h\"" << endl;
}

void OpenProg::XmlToData::outputFunctions(QTextStream &s) const
{
  Programmer::XmlToData<Data>::outputFunctions(s);
  s << "::Programmer::DeviceSpecific *OpenProg::Group::createDeviceSpecific(::Programmer::Base &base) const" << endl;
  s << "{" << endl;
  s << "  uint i = family(static_cast<PicBase &>(base).device()->name());" << endl;
  s << "  switch(i) {" << endl;
  for (uint i=0; i<uint(families().count()); i++) {
    s << "    case " + QString::number(i) + ": return new P" + families()[i] + "(base);" << endl;
  }
  s << "  }" << endl;
  s << "  ASSERT(false);" << endl;
  s << "  return NULL;" << endl;
  s << "}" << endl;
}

} // namespace

//-----------------------------------------------------------------------------
XML_MAIN(OpenProg::XmlToData)
