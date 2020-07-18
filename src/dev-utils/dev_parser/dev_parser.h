/***************************************************************************
 *   Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef DEV_PARSER_H
#define DEV_PARSER_H

#include <qstringlist.h>
#include <qmap.h>
#include <qvector.h>
#include "common/common/bitvalue.h"

#include "common/common/bitvalue.h"

class DevParser
{
public:
  DevParser(const QString &name) : _name(name) {}
  virtual ~DevParser() {}
  void parseFile(const QStringList &lines);
  virtual void doAction() const = 0;

  static QString findDevice(const QStringList &lines);
  static void loopFiles(int argc, char **argv, QStringList &devices);
  static DevParser *factory(const QString &name);

protected:
  QString _name;
  class Section {
  public:
    QString name;
    QStringList unamedAttributes;
    QMap<QString, QString> attributes;
    QVector<Section> sections;
  };
  QVector<Section>  _sections;
  QMap<QString, QString> _fields;

  Section parseSection(QStringList::const_iterator &it,
                       const QStringList::const_iterator &end, uint nbTabs) const;
  QString parseAttribute(QString &todo, QString &value) const;
  const Section *findUniqueSection(const QString &name) const;
  const Section *findSection(const QVector<Section> &sections,
                             const QString &name, const QStringList &keys) const;
  void extractRange(const QString &s, Address &start, Address &end) const;

  void qWarning(const QString &msg) const { ::qWarning("Warning [%s]: %s", _name.latin1(), msg.latin1()); }
  void qFatal(const QString &msg) const { ::qWarning("FATAL [%s]: %s", _name.latin1(), msg.latin1()); }
};

#endif
