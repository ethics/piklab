/***************************************************************************
 *   Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "dev_parser.h"

#include <qdir.h>
#include <qstringlist.h>
#include <qregexp.h>
#include <qtextstream.h>

#include "common/common/misc.h"
#include "devices/list/device_list.h"

QString DevParser::findDevice(const QStringList &lines)
{
  QRegExp reg("#\\s*device\\s*=([^()]+).*");
  QStringList::const_iterator it = lines.begin();
  for (; it!=lines.end(); ++it)
    if ( reg.exactMatch(*it) ) break;
  if ( it==lines.end() ) return QString::null;
  QString name = reg.cap(1).stripWhiteSpace();
  if ( name.startsWith("PIC") ) return name.mid(3);
  if ( name.startsWith("dsPIC") ) return name.mid(5);
  return name;
}

QString DevParser::parseAttribute(QString &todo, QString &value) const
{
  QRegExp reg("([\\w]+)=(.*)");
  if ( !reg.exactMatch(todo) ) { // attribute with no value ?
    reg.setPattern("([\\w-]+).*");
    if ( !reg.exactMatch(todo) ) qFatal(QString("Could not recognize attribute: \"%1\"").arg(todo));
    int end = reg.cap(1).length();
    if ( end==todo.length() ) todo = QString::null;
    else {
      if ( todo[end]!=' ' ) qFatal(QString("Error parsing for attribute: \"%1\"").arg(todo));
      todo = todo.mid(end+1).stripWhiteSpace();
    }
    value = QString::null;
    return reg.cap(1);
  }
  uint start = reg.cap(1).length()+1;
  int end = start+1;
  //qDebug("start: %s", todo.mid(start).latin1());
  if ( todo[start]=='\'' || todo[start]=='\"' ) {
    QChar c = todo[start];
    start += 1;
    end = todo.find(c, start);
    if ( end==-1 ) qFatal(QString("Unterminated string: %1").arg(todo));
  } else {
    end = todo.find(' ', start);
    if ( end==-1 ) end = todo.length();
  }
  //qDebug("add attribute %s: %s", reg.cap(1).latin1(), todo.mid(start, end-start).latin1());
  value = todo.mid(start, end-start);
  todo = todo.mid(end+1).stripWhiteSpace();
  return reg.cap(1);
}

DevParser::Section DevParser::parseSection(QStringList::const_iterator &it,
                     const QStringList::const_iterator &end, uint nbTabs) const
{
  Section section;
  QRegExp reg("([\\w]+)\\s*\\((.*)\\)");
  if ( !reg.exactMatch((*it).stripWhiteSpace()) ) qFatal(QString("Could not recognize section start: %1").arg((*it).stripWhiteSpace()));
  section.name = reg.cap(1);
  QString todo = reg.cap(2);
  for (;;) {
    QString value;
    QString name = parseAttribute(todo, value);
    if ( value.isNull() ) section.unamedAttributes.append(name);
    else {
      if ( section.attributes.contains(name) && section.attributes[name]!=value ) qFatal(QString("Attribute \"%1\" duplicated [%2].").arg(name).arg(*it));
      section.attributes[name] = value;
    }
    if ( todo.isEmpty() ) break;
  }
  ++it;
  if ( it==end ) return section;
  for (;;) {
    if ( !(*it).startsWith(repeat(" ", 4*(nbTabs+1)))
         && !(*it).startsWith(repeat("\t", nbTabs+1)) ) break;
    Section sub = parseSection(it, end, nbTabs+1);
    section.sections.append(sub);
    if ( it==end ) break;
  }
  return section;
}

void DevParser::parseFile(const QStringList &lines)
{
  QStringList::const_iterator it = lines.begin();
  for (; it!=lines.end();) {
    QRegExp reg("([\\w]+)=(.+)");
    if ( reg.exactMatch(*it) ) {
      if ( _fields.contains(reg.cap(1)) ) qWarning(QString("Duplicated field \"%1\".").arg(reg.cap(1)));
      _fields[reg.cap(1)] = reg.cap(2).stripWhiteSpace();
      ++it;
      continue;
    }
    Section section = parseSection(it, lines.end(), 0);
    _sections.append(section);
  }
}

const DevParser::Section *DevParser::findUniqueSection(const QString &name) const
{
  int k = -1;
  for (int i=0; i<_sections.count(); i++) {
    if ( _sections[i].name!=name ) continue;
    if ( k!=-1 ) qWarning(QString("Section \"%1\" duplicated").arg(name));
    k = i;
  }
  if ( k==-1 ) return 0;
  return &_sections[k];
}

const DevParser::Section *DevParser::findSection(const QVector<Section> &sections,
                                                 const QString &name, const QStringList &keys) const
{
  int k = -1;
  for (int i=0; i<sections.count(); i++) {
    if ( sections[i].name!=name || keys.find(sections[i].attributes["key"])==keys.end() ) continue;
    if ( k!=-1 ) qWarning(QString("Section \"%1\" with keys \"%2\" duplicated").arg(name).arg(keys.join("/")));
    k = i;
  }
  if ( k==-1 ) return 0;
  return &sections[k];
}

void DevParser::extractRange(const QString &s, Address &start, Address &end) const
{
  QRegExp reg("0x([\\w]+)-0x([\\w]+)");
  if ( reg.exactMatch(s) ) {
    bool ok1, ok2;
    start = fromHex(reg.cap(1), &ok1);
    end = fromHex(reg.cap(2), &ok2);
    if ( ok1 && ok2 ) return;
  }
  qFatal(QString("Could not recognize range %1").arg(s));
}

//-----------------------------------------------------------------------------
void DevParser::loopFiles(int argc, char **argv, QStringList &devices)
{
  if ( argc!=2 ) ::qFatal("Need directory or file as argument.\n");

  // find files
  QStringList files;
  QDir dir(argv[1]);
  if ( !dir.exists() ) {
    QFile file(argv[1]);
    if ( !file.exists() ) ::qFatal("Directory or file \"%s\" does not exists.", argv[1]);
    files.append(argv[1]);
  } else {
    QStringList list = dir.entryList("*.dev", QDir::Files);
    QStringList::const_iterator it;
    for (it=list.begin(); it!=list.end(); ++it)
      files.append(dir.absFilePath(*it));
  }

  // process each file
  QStringList::const_iterator it;
  for (it=files.begin(); it!=files.end(); ++it) {
    QFile file(*it);
    if ( !file.open(IO_ReadOnly) ) {
      ::qWarning("  Could not open file \"%s\".", file.name().latin1());
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
    QString fname = (*it).section('/', -1, -1).section('.', 0, 0).upper();
    if ( fname.startsWith("PIC") ) fname = fname.mid(3);
    else if ( fname.startsWith("DSPIC") ) fname = fname.mid(5);
    if ( Device::Lister::instance().data(fname)==0 ) {
      //qDebug("%s not supported: skipped", fname.latin1());
      continue;
    }
    devices.append(fname);
    qDebug("%s", fname.latin1());
    QString name = DevParser::findDevice(lines).upper();
    if ( !name.isEmpty() && name!=fname ) {
      ::qWarning("Device name is different from filename \"%s\".", file.name().latin1());
      continue;
    }
    // remove comments
    QStringList cleanedLines;
    QStringList::const_iterator it = lines.begin();
    for (; it!=lines.end(); ++it) {
      if ( (*it).stripWhiteSpace().startsWith("#") ) continue;
      int i = (*it).find('#');
      if ( i==-1 ) cleanedLines.append(*it);
      else cleanedLines.append((*it).mid(0, i));
    }
    // parse
    DevParser *parser = DevParser::factory(fname);
    if ( parser==0 ) continue;
    parser->parseFile(cleanedLines);
    parser->doAction();
    delete parser;
  }
}
