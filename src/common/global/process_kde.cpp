/***************************************************************************
 *   Copyright (C) 2008 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "process_kde.h"
#include "process_kde.moc"

//----------------------------------------------------------------------------
Process::ProcessIO::ProcessIO(QObject *parent)
  : QObject(parent)
{
  _kproc.reset(new KProcess);
  _kproc->setOutputChannelMode(KProcess::SeparateChannels);
  connect(_kproc.get(), SIGNAL(readyReadStandardOutput()), SLOT(receivedStandardOutput()));
  connect(_kproc.get(), SIGNAL(readyReadStandardError()), SLOT(receivedStandardError()));
  connect(_kproc.get(), SIGNAL(error(QProcess::ProcessError)), SIGNAL(error()));
  connect(_kproc.get(), SIGNAL(finished(int, QProcess::ExitStatus)), SIGNAL(finished()));
}

void Process::ProcessIO::setArguments(const QStringList &args)
{
  _kproc->clearProgram();
  for (QStringList::const_iterator it = args.begin();
       it!=args.end();
       ++it) {
      *_kproc << *it;
  }
}

void Process::ProcessIO::start(QStringList *env)
{
  if (env) {
    for (QStringList::const_iterator it = env->begin();
         it!=env->end();
         ++it) {
      QStringList list = QStringList::split('=', *it);
      if ( list.count()==2 ) _kproc->setEnv(list[0], list[1]);
    }
  }
  _kproc->setOutputChannelMode(KProcess::SeparateChannels);
  _kproc->start();
}

QByteArray Process::ProcessIO::readStdout()
{
  QByteArray tmp = _stdout;
  _stdout.resize(0);
  return tmp;
}

QByteArray Process::ProcessIO::readStderr()
{
  QByteArray tmp = _stderr;
  _stderr.resize(0);
  return tmp;
}

void Process::ProcessIO::receivedStandardOutput()
{
  _kproc->setReadChannel(QProcess::StandardOutput);
  QByteArray data = _kproc->readAll();
  uint n = _stdout.size();
  _stdout.resize(n + data.size());
  for (int i=0; i<data.size(); i++) _stdout[n+i] = data[i] != '\0' ? data[i] : ' ';
  emit readyReadStdout();
}

void Process::ProcessIO::receivedStandardError()
{
  _kproc->setReadChannel(QProcess::StandardError);
  QByteArray data = _kproc->readAll();
  uint n = _stderr.size();
  _stderr.resize(n + data.size());
  for (int i=0; i<data.size(); i++) _stderr[n+i] = data[i] != '\0' ? data[i] : ' ';
  emit readyReadStderr();
}
