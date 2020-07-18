/***************************************************************************
 *   Copyright (C) 2005-2006 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef CUSTOM_PROCESS_H
#define CUSTOM_PROCESS_H

#include "tools/list/compile_process.h"

namespace Compile
{

//-----------------------------------------------------------------------------
class CustomProcess : public BaseProcess
{
Q_OBJECT
public:
  CustomProcess(const QString &command) : _command(command) {}
  virtual bool check() const { return true; }
  virtual FileData::List files(bool *ok) const { if (ok) *ok = true; return FileData::List(); }

protected:
  virtual void setupProcess();
  virtual QString outputFiles() const { return QString::null; }
  virtual QString deviceName() const;

protected slots:
  virtual void logLine(::Process::OutputType type, const QString &line);

private:
  QString _command;
};

} // namespace

#endif
