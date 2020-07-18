/***************************************************************************
 *   Copyright (C) 2005-2006 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef COMPILE_MANAGER_H
#define COMPILE_MANAGER_H

#include "compile_process.h"

namespace Compile
{

class Manager : public QObject, public Log::Base
{
Q_OBJECT
public:
  Manager(QObject *parent);
  QString label() const { return _label; }
  bool compileOnly() const { return _compileOnly; }
  bool compileFile(const Item &item);
  void cleanFile(const PURL::Url &url);
  bool buildProject();
  void cleanProject();
  void kill();
  void processDone();
  void processFailed();

signals:
  void success();
  void failure();
  void updateFile(const Compile::FileData &fdata);

private slots:
  bool execute();

private:
  bool _compileOnly;
  QValueList<Data> _todo;
  Data _next;
  std::auto_ptr<BaseProcess> _base;
  QString _label;
  uint _customCommandIndex;

  bool clearAll();
  bool addCompileItem(const QValueList<Item> &items, Operation operation);
  bool addAssembleItem(const QValueList<Item> &items, Operation operation);
  void addLinkItem(const QValueList<Item> &items, Operation operation);
  bool addProjectSources(Operation operation, uint &nb);
  bool executeNext();
  bool executeCustom();
  LinkingType linkingType() const;
};

} // namespace

#endif
