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

#include "devices/list/device_list.h"
#include "devices/base/device_group.h"
#include "devices/pic/base/pic_config.h"
#include "coff/base/disassembler.h"

class IncParser
{
public:
  IncParser(const Pic::Data &data) : _data(data) {}
  static QString findDevice(const QStringList &lines);
  void parseFile(const QStringList &lines);
  void checkDeviceData() const;

private:
  const Pic::Data &_data;
  QMap<QString, uint> _configNames;

  void qWarning(const QString &msg) const { ::qWarning("Warning [%s]: %s", _data.name().latin1(), msg.latin1()); }
  void qFatal(const QString &msg) const { ::qFatal("FATAL [%s]: %s", _data.name().latin1(), msg.latin1()); }
  bool checkMaskName(uint wordIndex, const QString &name, BitValue value) const;
  bool findName(const QString &name, BitValue value) const;
};


QString IncParser::findDevice(const QStringList &lines)
{
  QRegExp reg("\\s*IFNDEF\\s+__(.*)");
  QStringList::const_iterator it = lines.begin();
  for (; it!=lines.end(); ++it)
    if ( reg.exactMatch(*it) ) return reg.cap(1).stripWhiteSpace();
  // try harder
  reg.setPattern(";\\s*P([A-Z0-9]+)\\.INC.*");
  for (it=lines.begin(); it!=lines.end(); ++it)
    if ( reg.exactMatch(*it) ) return reg.cap(1).stripWhiteSpace();
  return QString::null;
}

void IncParser::parseFile(const QStringList &lines)
{
  //qDebug("Parse file for %s", _data.name().latin1());
  QRegExp reg("(_[\\w_]+)\\s+EQU\\s+H'([A-F0-9]+)'.*");
  QStringList::const_iterator it = lines.begin();
  for (; it!=lines.end(); ++it)
    if ( reg.exactMatch(*it) ) _configNames[reg.cap(1)] = fromHex(reg.cap(2), 0);
}

bool IncParser::checkMaskName(uint wordIndex, const QString &name, BitValue value) const
{
  if ( _data.name()=="14000" && ( name=="_CPP_ON" || name=="_CPP_OFF" || name=="_CPU_ON" || name=="_CPU_OFF" ) )
    return true; // #### FIXME
  const Pic::Config::Word &cw = _data.config()._words[wordIndex];
  for (int k=0; k<cw.masks.count(); k++) {
    const Pic::Config::Mask &cm = cw.masks[k];
    if ( SourceLine::ignoredConfigNames(_data, wordIndex).contains(name) ) return true;
    for (int l=0; l<cm.values.count(); l++) {
      const QStringList &cnames = cm.values[l].configNames[Pic::ConfigNameType::Default];
      for (int m=0; m<cnames.count(); m++) {
        if ( SourceLine::transformConfigName(_data, wordIndex, cnames[m])!=name
          && !SourceLine::extraConfigNames(Pic::ConfigNameType::Default, _data, wordIndex, cm.values[l]).contains(name) ) continue;
        BitValue mask = _data.mask(Pic::MemoryRangeType::Config);
        if ( _data.name()=="16C62" || _data.name()=="16C64" || _data.name()=="16C65"
             || _data.name()=="16C73" || _data.name()=="16C74" ) mask = 0x3FBF;
        if ( _data.name()=="14000" && name=="_CPC_ON" ) mask = 0x3EFF; // ??
        if ( _data.name()=="16F505" && name=="_CP_ON" ) mask = 0x003F; // ??
        BitValue cv = cm.value.complementInMask(mask) | cm.values[l].value;
        if ( value!=cv ) qWarning(QString("Mask name %1 does not have the correct value %2 (%3)").arg(name).arg(toHex(value, 4)).arg(toHex(cv, 4)));
        return true;
      }
    }
  }
  return false;
}

bool IncParser::findName(const QString &name, BitValue value) const
{
  if ( name.startsWith("_DEVID") || name.startsWith("_IDLOC") ) return true;
  for (int i=0; i<_data.config()._words.count(); i++) {
    const Pic::Config::Word &cw = _data.config()._words[i];
    QString cname;
    if ( !cw.name.isEmpty() ) cname = "_" + cw.name;
    if ( name==cname ) {
      Address address = _data.range(Pic::MemoryRangeType::Config).start + i;
      if ( value!=address ) qFatal(QString("Config name %1 does not have the correct address %2 (%3)").arg(name).arg(toHex(value, 8)).arg(toHex(address, 8)));
      return true;
    }
    if ( checkMaskName(i, name, value) ) return true;
  }
  return false;
}

void IncParser::checkDeviceData() const
{
  // check .inc config names
  QMap<QString, uint>::const_iterator it = _configNames.begin();
  for (; it!=_configNames.end(); ++it) {
    if ( findName(it.key(), it.data()) ) continue;
    qWarning(QString("Could not recognize name %1").arg(it.key()));
  }

  // check our config names
  for (int i=0; i<_data.config()._words.count(); i++) {
    const Pic::Config::Word &cw = _data.config()._words[i];
    QString cname;
    if ( !cw.name.isEmpty() ) cname = "_" + cw.name;
    if ( !cname.isEmpty() && cw.masks.count()!=0 && !_configNames.contains(cname) )
      qFatal(QString("Config name %1 not in include file").arg(cname));
    for (int k=0; k<cw.masks.count(); k++) {
      const Pic::Config::Mask &cm = cw.masks[k];
      if ( cm.name=="DEBUG" && (_data.name()=="18C601" || _data.name()=="18C801") ) continue;
      for (int l=0; l<cm.values.count(); l++) {
        const QStringList &cnames = cm.values[l].configNames[Pic::ConfigNameType::Default];
        for (int m=0; m<cnames.count(); m++) {
          QString name = SourceLine::transformConfigName(_data, i, cnames[m]);
          if ( !_configNames.contains(name) ) qWarning(QString("Config name %2 not in include file").arg(name));
        }
      }
    }
  }
}

//-----------------------------------------------------------------------------
int main(int argc, char **argv)
{
  if ( argc!=2 ) qFatal("Need directory as argument.\n");

  QDir dir(argv[1]);
  if ( !dir.exists() ) qFatal("Directory \"%s\" does not exists.", argv[1]);

  QStringList devices;
  QStringList files = dir.entryList("*.inc;*.INC", QDir::Files);
  QStringList::const_iterator it = files.begin();
  for (; it!=files.end(); ++it) {
    QFile file(dir.absFilePath(*it));
    if ( !file.open(IO_ReadOnly) ) {
      qWarning("  Could not open file \"%s\".", file.name().latin1());
      continue;
    }
    QTextStream stream(&file);
    QStringList lines;
    for (;;) {
      QString s = stream.readLine();
      if ( s.isNull() ) break;
      if ( s.stripWhiteSpace().isEmpty() ) continue;
      lines.append(s);
    }
    // find device
    QString name = IncParser::findDevice(lines).upper();
    if ( name.isEmpty() ) {
      qWarning("Could no find device name in file \"%s\".", file.name().latin1());
      continue;
    }
    devices.append(name);
    const Device::Data *data = Device::Lister::instance().data(name);
    if ( data==0 ) {
      qWarning("Device \"%s\" not supported.", name.latin1());
      continue;
    }
    if ( data->group().name()!="pic" ) continue;
    // parse
    IncParser parser(static_cast<const Pic::Data &>(*data));
    parser.parseFile(lines);
    parser.checkDeviceData();
  }

  QStringList all = Device::Lister::instance().supportedDevices();
  for (int i=0; i<all.count(); i++)
    if ( devices.find(all[i])==devices.end() ) qWarning("Device \"%s\" not in any .inc file.", all[i].latin1());

  return 0;
}
