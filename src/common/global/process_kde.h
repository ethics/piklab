/***************************************************************************
 *   Copyright (C) 2008 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef PROCESS_KDE_H
#define PROCESS_KDE_H

#include "common/common/global.h"

namespace Process
{
class Base;

class ProcessIO : public QObject
{
Q_OBJECT
public:
  ProcessIO(QObject *parent = 0);
  void setArguments(const QStringList &args);
  void start(QStringList *env = 0);
  QByteArray readStdout();
  QByteArray readStderr();

private slots:
  void receivedStandardOutput();
  void receivedStandardError();

signals:
  void readyReadStdout();
  void readyReadStderr();
  void error();
  void finished();

private:
  std::auto_ptr<KProcess> _kproc;
  QByteArray              _stdout, _stderr;

friend class Base;
};

} // namespace

#endif
