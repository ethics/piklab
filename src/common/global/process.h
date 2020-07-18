/***************************************************************************
 *   Copyright (C) 2005-2008 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef PROCESS_H
#define PROCESS_H

#include <signal.h>

#include <QTimer>

#include "common/common/global.h"
#include "common/common/key_enum.h"

#include "common/global/process_kde.h"

namespace PURL { class Directory; }
//class Q3Process;

namespace Process
{
//----------------------------------------------------------------------------
//typedef Q3Process ProcessIO;

//----------------------------------------------------------------------------
enum State { Stopped, Error, Running, Exited, Timedout, Killed, };

BEGIN_DECLARE_ENUM(OutputType)
  Stdout = 0, Stderr
END_DECLARE_ENUM_NO_DATA(OutputType)

//----------------------------------------------------------------------------
class Base : public QObject
{
Q_OBJECT
public:
  Base(QObject *parent, const char *name);
  virtual ~Base();
  void setup(const QString &executable, const QStringList &options, bool withWine);
  QStringList arguments() const { return _arguments; }
  QString prettyCommand() const { return arguments().join(" "); }
  void setWorkingDirectory(const PURL::Directory &dir);
  void setUseShell(bool useShell);
  void start(int msec = -1); // -1 == no timeout
  void writeToStdin(const QString &s);
  bool signal(int n);
  bool isRunning() const;
  State state() const { return _state; }
  int exitCode() const;
  virtual void clearOutputs();
  bool execute(int msec); // -1 == no timeout

signals:
  void done();
  void error();
  void timeout();
  void dataReceived(::Process::OutputType type);

public slots:
  void kill();

protected slots:
  void finishedSlot();
  void errorSlot();
  void timeoutSlot();
  void readyReadStdoutSlot() { readyRead(OutputType::Stdout); }
  void readyReadStderrSlot() { readyRead(OutputType::Stderr); }

protected:
  ProcessIO   *_process;
  QStringList  _arguments, _environment;
  bool         _useShell;
  State        _state;
  EnumVector<OutputType, QString> _outputs;
  bool         _stopExecute;
  QTimer       _timer;

  void readyRead(OutputType type);
  virtual void received(OutputType type, const QString &s) = 0;
  static bool isFilteredLine(const QString &line);
};

//----------------------------------------------------------------------------
class StringOutput : public Base
{
Q_OBJECT
public:
  StringOutput(QObject *parent = 0, const char *name = 0) : Base(parent, name) {}
  QString string(OutputType type) const { return _outputs[type]; }
  QString allOutputs();

private:
  virtual void received(OutputType type, const QString &s) { _outputs[type] += s; }
};

//----------------------------------------------------------------------------
class LineBase : public Base
{
Q_OBJECT
public:
  LineBase(QObject *parent = 0, const char *name = 0) : Base(parent, name) {}

private:
  virtual void addLine(OutputType type, const QString &line) = 0;
  virtual void received(OutputType type, const QString &s);
};

//----------------------------------------------------------------------------
class LineOutput : public LineBase
{
Q_OBJECT
public:
  LineOutput(QObject *parent = 0, const char *name = 0) : LineBase(parent, name) {}
  QStringList lines(OutputType type) const { return _lines[type]; }
  QStringList allOutputs();
  virtual void clearOutputs();

protected:
  EnumVector<OutputType, QStringList> _lines;

  virtual void addLine(OutputType type, const QString &line);
};

//----------------------------------------------------------------------------
class LineSignal : public LineBase
{
Q_OBJECT
public:
  LineSignal(QObject *parent = 0, const char *name = 0) : LineBase(parent, name) {}

signals:
  void logLine(::Process::OutputType type, const QString &line);

private:
  virtual void addLine(OutputType type, const QString &line) { emit logLine(type, line); }
};

} // namespace

#endif
