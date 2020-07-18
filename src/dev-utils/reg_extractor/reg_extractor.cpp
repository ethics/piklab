/***************************************************************************
 *   Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include <qdir.h>
#include <qstringlist.h>
#include <qregexp.h>
#include <qtextstream.h>
#include <qdom.h>

#include "dev-utils/dev_parser/dev_parser.h"
#include "common/common/misc.h"
#include "devices/pic/base/pic.h"
#include "devices/pic/base/pic_register.h"

class RegExtractor : public DevParser
{
public:
  RegExtractor(const QString &name) : DevParser(name) {}
  virtual void doAction() const;
  static QDomDocument _doc;
};

DevParser *DevParser::factory(const QString &name) { return new RegExtractor(name); }

void RegExtractor::doAction() const
{
  QDomElement device = _doc.createElement("device");
  device.setAttribute("name", _name);

  // banks
  bool ok;
  uint nb = _fields["NumBanks"].toUInt(&ok);
  if ( !ok ) {
    qWarning(QString("NumBanks is malformed \"%1\": skipping device").arg(_fields["NumBanks"]));
    return;
  }
  if ( nb==0 ) nb = 1;
  device.setAttribute("nb_banks", QString::number(nb));
  if ( !_fields["AccessBankSplitOffset"].isEmpty() ) {
    uint offset = fromHexLabel(_fields["AccessBankSplitOffset"], &ok);
    if ( !ok) qWarning(QString("AccessBankSplitOffset is malformed \"%1\"").arg(_fields["AccessBankSplitOffset"]));
    device.setAttribute("access_bank_split_offset", toHexLabel(offset, 2));
  }
  if (!_fields["UnusedBankMask"].isEmpty()) {
    if (_fields["UnusedBankMask"] == "0x0") nb = 0;
    else {
      nb = fromHexLabel(_fields["UnusedBankMask"], 4, &ok);
      if (!ok) {
        qWarning(QString("UnusedBankMask is malformed \"%1\": skipping device").arg(_fields["UnusedBankMask"]));
        return;
      }
    }
    device.setAttribute("unused_bank_mask", toHexLabel(nb, 4));
  }

  // mirror regs
  for (int i=0; i<_sections.count(); i++) {
    if ( _sections[i].name!="MirrorRegs" ) continue;
    QDomElement e = _doc.createElement("mirror");
    for (int k=0; k<_sections[i].unamedAttributes.count(); k++) {
      Address start, end;
      extractRange(_sections[i].unamedAttributes[k], start, end);
      QDomElement r = _doc.createElement("range");
      r.setAttribute("start", toHexLabel(start, 4));
      r.setAttribute("end", toHexLabel(end, 4));
      e.appendChild(r);
    }
    device.appendChild(e);
  }

  // unused regs
  for (int i=0; i<_sections.count(); i++) {
    if ( _sections[i].name!="UnusedRegs" ) continue;
    QDomElement e = _doc.createElement("unused");
    if ( _sections[i].unamedAttributes.count()!=1 ) qFatal("Unused have more or less than 1 range");
    Address start, end;
    extractRange(_sections[i].unamedAttributes[0], start, end);
    e.setAttribute("start", toHexLabel(start, 4));
    e.setAttribute("end", toHexLabel(end, 4));
    device.appendChild(e);
  }

  // sfr
  for (int i=0; i<_sections.count(); i++) {
    if (_sections[i].name != "sfr") continue;
    QDomElement e;
    QString key = _sections[i].attributes["key"];
    bool ok;
    int size = _sections[i].attributes["size"].toUInt(&ok);
    if ( !ok || size==0 ) qFatal("Malformed or null size attribute");
    if ( _sections[i].attributes["flags"]=="j" ) {
      e = _doc.createElement("combined");
      e.setAttribute("size", QString::number(size));
    } else {
      e = _doc.createElement("sfr");
      // bit access
      QString s = _sections[i].attributes["access"];
      if ( s.isEmpty() ) qFatal("Missing access attribute");
      QStringList list = QStringList::split(' ', s);
      if ( list.count()!=8*size ) qFatal("Access attribute has wrong number of fields");
      QValueVector<Pic::RegisterBitProperties> bits(list.count());
      for (int j=0; j<bits.count(); j++) {
        if ( list[j]=="r" ) bits[j] = Pic::RegisterBitRead;
        else if ( list[j]=="w" ) bits[j] = Pic::RegisterBitWrite;
        else if ( list[j]=="rw" ) bits[j] = Pic::RegisterBitRead | Pic::RegisterBitWrite;
        else if ( list[j]=="u" || list[j]=="-" ) bits[j] = Pic::RegisterBitUnused;
        else if ( list[j]=="rc" ) bits[j] = Pic::RegisterBitRead | Pic::RegisterBitOnlySoftwareClear;
        else if ( list[j]=="rs" ) bits[j] = Pic::RegisterBitRead | Pic::RegisterBitOnlySoftwareSet;
        else qFatal(QString("Unknown register bit %1:%2").arg(key).arg(list[j]));
      }
      for (int j=0; j<bits.count(); j++) list[j] = toHex(bits[j], 1);
      e.setAttribute("access", list.join(""));
      // bit por
      QValueVector<Pic::RegisterBitState> states(list.count());
      int k = 0;
      for (; k<_sections[i].sections.count(); k++)
        if ( _sections[i].sections[k].name=="reset" ) break;
      if ( k==_sections[i].sections.count() ) {
        qFatal(QString("No reset section (%1)").arg(key));
        continue;
      }
      s = _sections[i].sections[k].attributes["por"];
      if ( s.length()!=states.count() ) qFatal(QString("POR of incorrect length (%1)").arg(key));
      for (int j=0; j<states.count(); j++) {
        states[j] = Pic::RegisterBitUnknown;
        if ( s[j]=='0' ) states[j] = Pic::RegisterBitLow;
        else if ( s[j]=='1' ) states[j] = Pic::RegisterBitHigh;
        else if ( s[j]=='u' ) states[j] = Pic::RegisterBitUnchanged;
        else if ( s[j]=='q' ) states[j] = Pic::RegisterBitDepends;
        else if ( s[j]=='y' ) states[j] = Pic::RegisterBitDependsConfig;
        else if ( s[j]!='-' && s[j]!='x' ) qFatal(QString("Unknown POR bit '%1' (%2)").arg(s[j]).arg(key));
      }
      for (int j=0; j<bits.count(); j++) list[j] = toHex(states[j], 1);
      e.setAttribute("por", list.join(""));
      // bit mclr
      s = _sections[i].sections[k].attributes["mclr"];
      if ( s.length()!=states.count() ) qFatal(QString("MCLR of incorrect length (%1)").arg(key));
      for (int j=0; j<states.count(); j++) {
        states[j] = Pic::RegisterBitUnknown;
        if ( s[j]=='0' ) states[j] = Pic::RegisterBitLow;
        else if ( s[j]=='1' ) states[j] = Pic::RegisterBitHigh;
        else if ( s[j]=='u' ) states[j] = Pic::RegisterBitUnchanged;
        else if ( s[j]=='q' ) states[j] = Pic::RegisterBitDepends;
        else if ( s[j]=='y' ) states[j] = Pic::RegisterBitDependsConfig;
        else if ( s[j]!='-' && s[j]!='x' ) qFatal(QString("Unknown MCLR bit (%2) '%1'").arg(s[j]).arg(key));
      }
      for (int j=0; j<bits.count(); j++) list[j] = toHex(states[j], 1);
      e.setAttribute("mclr", list.join(""));
    }
    e.setAttribute("name", key);
    uint address = fromHexLabel(_sections[i].attributes["addr"], &ok);
    if ( !ok ) address = fromHex(_sections[i].attributes["addr"], &ok);
    if ( !ok ) qWarning(QString("Address \"%1\" malformed").arg(_sections[i].attributes["addr"]));
    e.setAttribute("address", toHexLabel(address, 4));
    device.appendChild(e);
  }

  _doc.documentElement().appendChild(device);
}

QDomDocument RegExtractor::_doc;

//-----------------------------------------------------------------------------
int main(int argc, char **argv)
{
  QDomImplementation di = RegExtractor::_doc.implementation();
  QDomDocumentType dtype = di.createDocumentType("piklab", QString::null, QString::null);
  QDomDocument doc = di.createDocument(QString::null, "registers", dtype);
  QDomComment comment = doc.createComment("This file is generated. Do not edit.");
  doc.insertBefore(comment, doc.documentElement());
  RegExtractor::_doc = doc;

  QStringList devices;
  DevParser::loopFiles(argc, argv, devices);

  QFile output("registers.xml");
  if ( !output.open(IO_WriteOnly) ) ::qFatal("  Could not open file \"%s\".", output.name().latin1());
  QTextStream stream(&output);
  stream << RegExtractor::_doc.toString(2);

  return 0;
}
