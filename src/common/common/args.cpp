/***************************************************************************
 *   Copyright (C) 2008 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "args.h"

#include <qregexp.h>

//----------------------------------------------------------------------------
void CustomArgument::Processor::addArg(uint id, uint index, const Data &data)
{
  ASSERT(data.type != ArgType::Nb_Types);
  ASSERT(data.key != NULL);
  QString key = data.key;
  if (data.type == ArgType::ReplaceIf) {
    QRegExp rexp("\\$(.*)\\(xxx\\)");
    bool ok = rexp.exactMatch(data.key);
    ASSERT(ok);
    key = rexp.cap(1);
  }
  IdData idata = { id, index };
  ArgData adata = { idata, data };
  bool ok = _data.insert(std::make_pair(key, adata)).second;
  ASSERT(ok);
}

QStringList CustomArgument::Processor::processOne(const QString &arg) const
{
  // process ReplaceAll
  for (Map::const_iterator it = _data.begin(); it!=_data.end(); ++it) {
    const Data &data = it->second.data;
    if (data.type == ArgType::ReplaceAll && checkIs(arg, it->first)) {
      return replaceAllValue(it->second.id);
    }
  }
  // process ReplaceIf
  QString tmp = arg;
  for (Map::const_iterator it = _data.begin(); it!=_data.end(); ++it) {
    const Data &data = it->second.data;
    if (data.type == ArgType::ReplaceIf)
      tmp = replaceIf(tmp, it->first, replaceIfCondition(it->second.id));
  }
  // process Replace
  for (Map::const_iterator it = _data.begin(); it!=_data.end(); ++it)  {
    const Data &data = it->second.data;
    if (data.type == ArgType::Replace) {
      tmp = tmp.replace(it->first, replaceValue(it->second.id));
      tmp = postReplaceValue(it->second.id, tmp);
    }
  }
  QStringList list;
  if (tmp.isEmpty()) return list;
  list.append(tmp);
  return list;
}

QStringList CustomArgument::Processor::process(const QStringList &args) const
{
  QStringList nargs;
  for (QStringList::const_iterator it = args.begin(); it!=args.end(); ++it) {
    QStringList list = processOne(*it);
    if (!list.empty()) nargs += list;
  }
  for (Map::const_iterator it = _data.begin(); it!=_data.end(); ++it)  {
    const Data &data = it->second.data;
    if (data.type == ArgType::Separator) {
      QStringList sargs;
      for (int i=0; i<nargs.count(); i++) sargs += QStringList::split(data.key, nargs[i]);
      nargs = sargs;
    }
  }
  return nargs;
}

bool CustomArgument::Processor::checkIs(const QString &s, const QString &key)
{
  if ( !s.contains(key) ) return false;
  if ( s!=key ) qWarning("Argument should be only %s, the rest will be ignored...", key.latin1());
  return true;
}

QString CustomArgument::Processor::replaceIf(const QString &s, const QString &key, bool condition)
{
  QRegExp rexp("(.*)\\$" + key + "\\(([^)]*)\\)(.*)");
  if ( !rexp.exactMatch(s) ) return s;
  return rexp.cap(1) + (condition ? rexp.cap(2) : QString::null) + rexp.cap(3);
}

CustomArgument::ArgList CustomArgument::Processor::argList() const
{
  ArgList args;
  for (Map::const_iterator it = _data.begin();
       it != _data.end();
       ++it) {
    args.push_back(it->second.data);
  }
  return args;
}
