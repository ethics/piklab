/***************************************************************************
 *   Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "generic_config.h"

#if defined(NO_KDE)
#  include <qsettings.h>
#else
#  include <kapplication.h>
#  include <kconfig.h>
#  include <kconfiggroup.h>
#endif

//----------------------------------------------------------------------------
class GenericConfigPrivate
{
public:
#if defined(NO_KDE)
  GenericConfigPrivate(const QString &group)
    : _group(group) { _settings.beginGroup("/piklab/" + group); }
  QSettings _settings;

#else
  GenericConfigPrivate(const QString &group)
    : _config(KGlobal::config()->group(group)) {}
  ~GenericConfigPrivate() { KGlobal::config()->sync(); }
  KConfigGroup _config;
#endif
  QString _group;
};

//----------------------------------------------------------------------------
GenericConfig::GenericConfig(const QString &group)
{
  _d = new GenericConfigPrivate(group);
}

GenericConfig::~GenericConfig()
{
  delete _d;
  _d = NULL;
}

QString GenericConfig::group() const
{
  return _d->_group;
}

QString GenericConfig::readEntry(const QString &key, const QString &def) const
{
#if defined(NO_KDE)
  return _d->_settings.value(key, def).toString();
#else
  return _d->_config.readEntry(key, def);
#endif
}
void GenericConfig::writeEntry(const QString &key, const QString &value)
{
#if defined(NO_KDE)
  _d->_settings.setValue(key, value);
#else
  _d->_config.writeEntry(key, value);
#endif
}

QStringList GenericConfig::readListEntry(const QString &key, const QStringList &defaultValues) const
{
#if defined(NO_KDE)
  return _d->_settings.value(key, defaultValues).toStringList();
#else
  return _d->_config.readEntry(key, defaultValues);
#endif
}
void GenericConfig::writeEntry(const QString &key, const QStringList &value)
{
#if defined(NO_KDE)
  _d->_settings.setValue(key, value);
#else
  _d->_config.writeEntry(key, value);
#endif
}

QList<int> GenericConfig::readIntListEntry(const QString &key) const
{
#if defined(NO_KDE)
  QList<int> ilist;
  QStringList list = readListEntry(key, QStringList());
  QStringList::const_iterator it;
  for (it=list.begin(); it!=list.end(); ++it) {
    bool ok;
    int v = (*it).toInt(&ok);
    if ( !ok ) return ilist;
    ilist.append(v);
  }
  return ilist;
#else
  QList<int> defaultValues;
  return _d->_config.readEntry(key, defaultValues);
#endif
}
void GenericConfig::writeEntry(const QString &key, const QList<int> &value)
{
#if defined(NO_KDE)
  QStringList list;
  QList<int>::const_iterator it;
  for (it=value.begin(); it!=value.end(); ++it)
    list.append(QString::number(*it));
  writeEntry(key, list);
#else
  _d->_config.writeEntry(key, value);
#endif
}

QSize GenericConfig::readSizeEntry(const QString &key, QSize def) const
{
#if defined(NO_KDE)
  QValueList<int> list = readIntListEntry(key);
  if ( list.count()!=2 ) return def;
  return QSize(list[0], list[1]);
#else
  return _d->_config.readEntry(key, def);
#endif
}
void GenericConfig::writeEntry(const QString &key, const QSize &value)
{
#if defined(NO_KDE)
  QValueList<int> ilist;
  ilist.append(value.width());
  ilist.append(value.height());
  writeEntry(key, ilist);
#else
  _d->_config.writeEntry(key, value);
#endif
}

bool GenericConfig::readBoolEntry(const QString &key, bool def) const
{
#if defined(NO_KDE)
  return _d->_settings.value(key, def).toBool();
#else
  return _d->_config.readEntry(key, def);
#endif
}
void GenericConfig::writeEntry(const QString &key, bool value)
{
#if defined(NO_KDE)
  _d->_settings.setValue(key, value);
#else
  _d->_config.writeEntry(key, value);
#endif
}

int GenericConfig::readIntEntry(const QString &key, int def) const
{
#if defined(NO_KDE)
  return _d->_settings.value(key, def).toInt();
#else
  return _d->_config.readEntry(key, def);
#endif
}
void GenericConfig::writeEntry(const QString &key, int value)
{
#if defined(NO_KDE)
  _d->_settings.setValue(key, value);
#else
  _d->_config.writeEntry(key, value);
#endif
}

void GenericConfig::deleteGroup(const QString &group)
{
#if defined(NO_KDE)
  Q_UNUSED(group);
  // #### cannot do that...
#else
  KGlobal::config()->deleteGroup(group);
#endif
}

QVariant GenericConfig::readVariantEntry(const QString &key, const QVariant &defValue) const
{
  switch (defValue.type()) {
    case QVariant::Bool: return QVariant(readBoolEntry(key, defValue.toBool()), 0);
    case QVariant::UInt: return readUIntEntry(key, defValue.toUInt());
    default: break;
  }
  ASSERT(false);
  return QVariant();
}

void GenericConfig::writeVariantEntry(const QString &key, const QVariant &v)
{
  switch (v.type()) {
    case QVariant::Bool: writeEntry(key, v.toBool()); break;
    case QVariant::UInt: writeEntry(key, v.toUInt()); break;
    default: ASSERT(false); break;
  }
}
