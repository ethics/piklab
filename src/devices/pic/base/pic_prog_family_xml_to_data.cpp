/***************************************************************************
 *   Copyright (C) 2012 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include <map>
#include <qfile.h>
#include <qtextstream.h>

#include "common/common/number.h"
#include "xml_to_data/xml_to_data.h"
#include "pic_prog_family.h"

namespace Pic
{
class ProgXmlToData : public XmlToData
{
public:
  ProgXmlToData() : _current(NULL) {}

private:
  QString         _currentName;
  ProgFamilyData* _current;
  typedef std::map<QString, ProgFamilyData*> Map;
  Map _map;

  virtual QString namespaceName() const { return "Pic"; }
  virtual QString currentDevice() const { return _currentName; }

//-----------------------------------------------------------------------------
void processFamily(QDomElement family)
{
  QDomElement waitTimes = findUniqueElement(family, "waitTimes", QString::null, QString::null);
  if (waitTimes.isNull()) qFatal("Missing \"waitTimes\"");
  bool ok1, ok2, ok3, ok4, ok5, ok6, ok7, ok8, ok9;
  _current->codeWaitTime = waitTimes.attribute("code").toUInt(&ok1);
  _current->lowVoltageCodeWaitTime = waitTimes.attribute("lv_code").toUInt(&ok2);
  _current->eepromWaitTime = waitTimes.attribute("eeprom").toUInt(&ok3);
  _current->configWaitTime = waitTimes.attribute("config").toUInt(&ok4);
  _current->userIdsWaitTime = waitTimes.attribute("user_ids").toUInt(&ok5);
  _current->eraseWaitTime = waitTimes.attribute("erase").toUInt(&ok6);
  _current->lowVoltageEraseWaitTime = waitTimes.attribute("lv_erase").toUInt(&ok7);
  if (!ok1 || !ok2 || !ok3 || !ok4 || !ok5 || !ok6 || !ok7) qFatal("invalid or missing waitTime(s)");

  QDomElement latches = findUniqueElement(family, "latches", QString::null, QString::null);
  if (latches.isNull()) qFatal("Missing \"latches\"");
  _current->codeLatches = latches.attribute("code").toUInt(&ok1);
  _current->eepromLatches = latches.attribute("eeprom").toUInt(&ok2);
  _current->configLatches = latches.attribute("config").toUInt(&ok3);
  _current->userIdsLatches = latches.attribute("user_ids").toUInt(&ok4);
  _current->rowEraseLatches = latches.attribute("row_erase").toUInt(&ok5);
  if (!ok1 || !ok2 || !ok3 || !ok4 || !ok5) qFatal("invalid or missing latche(s)");

  QDomElement voltages = findUniqueElement(family, "voltages", QString::null, QString::null);
  if (voltages.isNull()) qFatal("Missing \"voltages\"");
  _current->vddMin = voltages.attribute("vdd_min").toUInt(&ok1);
  _current->vddMax = voltages.attribute("vdd_max").toUInt(&ok2);
  _current->vddDef = voltages.attribute("vdd_def").toUInt(&ok3);
  _current->vppMin = voltages.attribute("vpp_min").toUInt(&ok4);
  _current->vppMax = voltages.attribute("vpp_max").toUInt(&ok5);
  _current->vppDef = voltages.attribute("vpp_def").toUInt(&ok6);
  _current->vddNominalMin = voltages.attribute("vdd_nominal_min").toUInt(&ok7);
  _current->vddNominalMax = voltages.attribute("vdd_nominal_max").toUInt(&ok8);
  _current->eraseMin = voltages.attribute("erase_min").toUInt(&ok9);
  if (!ok1 || !ok2 || !ok3 || !ok4 || !ok5 || !ok6 || !ok7 || !ok8 || !ok9) qFatal("invalid or missing voltage(s)");

  QDomElement memory = findUniqueElement(family, "memory", QString::null, QString::null);
  if (memory.isNull()) qFatal("Missing \"memory\"");
  _current->testStart = fromHexLabel(memory.attribute("test_start"), &ok1);
  _current->testSize = fromHexLabel(memory.attribute("test_size"), &ok2);
  _current->panelSize = fromHexLabel(memory.attribute("panel_size"), &ok3);
  _current->bootSize = fromHexLabel(memory.attribute("boot_size"), &ok4);
  if (!ok1 || !ok2 || !ok3 || !ok4) qFatal("invalid or missing memory attribute(s)");
}

//-----------------------------------------------------------------------------
void parse()
{
  QDomDocument doc = parseFile("pic_prog_family.xml");
  QDomElement root = doc.documentElement();
  if ( root.nodeName()!="prog_families" ) qFatal("root node should be \"prog_families\"");
  for (QDomNode child=root.firstChild(); !child.isNull(); child = child.nextSibling()) {
    if ( child.isComment() ) qDebug("comment: %s", child.toComment().data().latin1());
    else {
      if ( !child.isElement() ) qFatal("\"prog_families\" child should be an element");
      if ( child.nodeName()!="family" ) qFatal("Family node should be named \"family\"");
      QDomElement family = child.toElement();
      QString name = family.attribute("name");
      if (_map.find(name) != _map.end()) qFatal(QString("Family already defined for %1").arg(name));
      _current = new ProgFamilyData;
      _map[name] = _current;
      processFamily(family);
    }
  }
}

//-----------------------------------------------------------------------------
void output()
{
  // write .cpp file
  QFile file("pic_prog_family_data.cpp");
  if ( !file.open(IO_WriteOnly) ) qFatal(QString("Cannot open output file \"%1\"").arg(file.name()));
  QTextStream s(&file);
  s << "// #### Do not edit: this file is autogenerated !!!" << endl << endl;
  s << "#include \"pic_prog_family.h\"" << endl;
  s << endl;
  s << "namespace " << namespaceName() << endl;
  s << "{" << endl;

  // data list
  s << "const ProgFamilyData PROG_FAMILY_DATA[] = {" << endl;
  for (Map::const_iterator it = _map.begin(); it != _map.end(); ++it) {
    s << "  {\"" << it->first << "\", ";
    const ProgFamilyData& data = *it->second;
    s << data.codeWaitTime << ", " << data.lowVoltageCodeWaitTime << ", " << data.eepromWaitTime << ", ";
    s << data.configWaitTime << ", " << data.userIdsWaitTime << ", ";
    s << data.eraseWaitTime << ", " << data.lowVoltageEraseWaitTime << ", ";
    s << data.codeLatches << ", " << data.eepromLatches << ", ";
    s << data.configLatches << ", " << data.userIdsLatches << ", " << data.rowEraseLatches << ", ";
    s << data.vddMin << ", " << data.vddMax << ", " << data.vddDef << ", ";
    s << data.vppMin << ", " << data.vppMax << ", " << data.vppDef << ", ";
    s << data.vddNominalMin << ", " << data.vddNominalMax << ", " << data.eraseMin << ", ";
    s << data.testStart << ", " << data.testSize << ", " << data.panelSize << ", " << data.bootSize;
    s << "}," << endl;
  }
  s << "  {NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} " << endl;
  s << "};" << endl;
  s << "}" << endl;
}

}; // class ProgXmlToData

} // namespace

//-----------------------------------------------------------------------------
XML_MAIN(Pic::ProgXmlToData)
