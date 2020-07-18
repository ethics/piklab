/***************************************************************************
 *   Copyright (C) 2008 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef ARGS_H
#define ARGS_H

#include "global.h"
#include "key_enum.h"

//----------------------------------------------------------------------------
namespace CustomArgument
{
BEGIN_DECLARE_ENUM(ArgType)
  Replace = 0, ReplaceIf, ReplaceAll, Separator
END_DECLARE_ENUM_NO_DATA(ArgType)

struct Data
{
  ArgType     type;
  char const* key, *label;
};

typedef QValueList<Data> ArgList;

class Processor
{
public:
  Processor() {}
  virtual ~Processor() {}
  void addArg(uint id, uint index, const Data &data);
  QStringList processOne(const QString &arg) const;
  QStringList process(const QStringList &args) const;
  ArgList argList() const;

protected:
  struct IdData {
    uint id, index;
  };
  virtual QStringList replaceAllValue(const IdData &idata) const = 0;
  virtual bool replaceIfCondition(const IdData &idata) const = 0;
  virtual QString replaceValue(const IdData &idata) const = 0;
  virtual QString postReplaceValue(const IdData &idata, const QString &arg) const = 0;

private:
  struct ArgData {
    IdData id;
    Data   data;
  };
  // sorted by reversed alphabetical order: we want %A to be processed after %AB
  typedef std::map<QString, ArgData, std::greater<QString> > Map;
  Map _data;

  static bool checkIs(const QString &s, const QString &key);
  static QString replaceIf(const QString &s, const QString &key, bool condition);
};

} // namespace

#endif
