/***************************************************************************
 *   Copyright (C) 2005-2009 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "process.h"
#include "process.moc"

#include "unistd.h"

#include "purl.h"

extern char **environ; // FreeBSD

//----------------------------------------------------------------------------
Process::Base::Base(QObject *parent, const char *name)
  : QObject(parent, name),
    _useShell(false),
    _state(Stopped),
    _stopExecute(false)
{
  _process = new ProcessIO(this);
  connect(_process, SIGNAL(finished()), SLOT(finishedSlot()));
  connect(_process, SIGNAL(error()), SLOT(errorSlot()));
  connect(_process, SIGNAL(readyReadStdout()), SLOT(readyReadStdoutSlot()));
  connect(_process, SIGNAL(readyReadStderr()), SLOT(readyReadStderrSlot()));
  connect(&_timer, SIGNAL(timeout()), SLOT(timeoutSlot()));
}

Process::Base::~Base()
{
  kill();
}

void Process::Base::setup(const QString &executable, const QStringList &options, bool withWine)
{
  _arguments.clear();
  _environment.clear();
  if (withWine) {
    _environment += "WINEDEBUG=-all";
    _arguments += "wine";
  }
  _arguments += executable;
  _arguments += options;
}

void Process::Base::clearOutputs()
{
  FOR_EACH(OutputType, type) _outputs[type] = QString::null;
}

void Process::Base::start(int msec)
{
  _state = Stopped;
  clearOutputs();
  QStringList args;
  if (_useShell) {
    args += "/bin/sh";
    args += "-c";
    args += _arguments.join(" ");
  } else args = _arguments;
  _process->setArguments(args);
  QStringList env;
  if ( !_environment.isEmpty() ) {
    for (uint i=0; environ[i]; i++) env += environ[i];
    env += _environment;
  }

  _process->start(env.isEmpty() ? 0 : &env);
  if (msec >=0 ) _timer.start(msec);
  _state = Running;
}

int Process::Base::exitCode() const
{
  return _process->_kproc->exitCode();
}

void Process::Base::finishedSlot()
{
  _timer.stop();
  _state = Exited;
  FOR_EACH(OutputType, type) readyRead(type);
  emit done();
}

void Process::Base::errorSlot()
{
  _timer.stop();
  _state = Error;
  emit error();
}

void Process::Base::timeoutSlot()
{
  kill();
  _state = Timedout;
  emit timeout();
}

bool Process::Base::isRunning() const
{
  return _process->_kproc->state() != QProcess::NotRunning;
}

void Process::Base::writeToStdin(const QString &s)
{
  const char *cs = s.latin1();
  QByteArray a;
  a.duplicate(cs, strlen(cs));
  _process->_kproc->write(a);
}

void Process::Base::kill()
{
  _process->_kproc->kill();
  _process->_kproc->waitForFinished(-1);
  _state = Killed;
}

bool Process::Base::signal(int n)
{
#if defined(Q_OS_UNIX)
  return ( ::kill(_process->_kproc->pid(), n)!=-1 );
#elif defined(Q_OS_WIN)
  // #### impossible to do ??
  return false;
#endif
}

void Process::Base::setWorkingDirectory(const PURL::Directory &dir)
{
  _process->_kproc->setWorkingDirectory(dir.path());
}

void Process::Base::setUseShell(bool useShell)
{
  _useShell = useShell;
}

bool Process::Base::isFilteredLine(const QString &line)
{
  // "wine" returns all those "libGL warning" that mess up the output...
  return line.startsWith("libGL warning");
}

void Process::Base::readyRead(OutputType type)
{
  QString s;
  for (;;) {
    QByteArray a;
    switch (type.type()) {
      case OutputType::Stdout: a = _process->readStdout(); break;
      case OutputType::Stderr: a = _process->readStderr(); break;
      case OutputType::Nb_Types: ASSERT(false); break;
    }
    if (a.count() == 0) break;
    s += QString::fromLatin1(a.data(), a.count());
  }
  if (s.length() != 0) {
    emit received(type, s);
    emit dataReceived(type);
  }
}

bool Process::Base::execute(int msec)
{
  if (_state != Running) start();

  while (msec != 0) {
    int step = 100;
    if (msec >= 0) {
      step = std::min(msec, 100);
      msec -= step;
    }
    _process->_kproc->waitForFinished(step);
    if (_state != Running) {
      return _state == Exited;
    }
    if (_stopExecute) {
      _stopExecute = false;
      return true;
    }
  }

  _state = Timedout;
  emit timeout();
  return false;
}

//----------------------------------------------------------------------------
QString Process::StringOutput::allOutputs()
{
  QString s;
  FOR_EACH(OutputType, type) s += _outputs[type];
  return s;
}

//----------------------------------------------------------------------------
void filterControlSequence(QString &s)
{
    for (uint i=0; i<uint(s.length()); ++i) {
        if (s[i] == char(0x1B) // ESC
            && (i+1)<uint(s.length())
            && s[i+1] == '[') {
            // terminal control sequence
            for (uint k=i+2; k<uint(s.length()); ++k) {
                if (s[k]>=char(0x40) && s[k]<=char(0x7E)) { // end of sequence character
                    s.remove(i, k-i+1);
                    break;
                }
            }
        }
    }
}

QStringList received(const QString &s, QString &buffer)
{
  buffer += s;
  buffer.remove('\r');
  QStringList lines = QStringList::split('\n', buffer, true);
  if ( lines.isEmpty() ) buffer = QString::null;
  else {
    for (QStringList::iterator it=lines.begin(); it!=lines.end(); ++it) {
      filterControlSequence(*it);
    }
    buffer = lines.last();
    lines.pop_back();
  }
  return lines;
}

void Process::LineBase::received(OutputType type, const QString &s)
{
  QString& sout  = _outputs[type];
  QStringList lines = ::received(s, sout);
  for (QStringList::const_iterator it=lines.begin(); it!=lines.end(); ++it) {
    if ( !isFilteredLine(*it) ) {
      addLine(type, *it);
    }
  }
  if ( !isRunning() && !isFilteredLine(sout) ) addLine(type, sout);
}

//----------------------------------------------------------------------------
QStringList Process::LineOutput::allOutputs()
{
  QStringList lines;
  FOR_EACH(OutputType, type) lines += _lines[type];
  return lines;
}

void Process::LineOutput::clearOutputs()
{
  Process::LineBase::clearOutputs();
  FOR_EACH(OutputType, type) _lines[type].clear();
}

void Process::LineOutput::addLine(OutputType type, const QString &line)
{
    _lines[type] += line;
}
