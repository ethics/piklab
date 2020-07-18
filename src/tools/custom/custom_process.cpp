/***************************************************************************
 *   Copyright (C) 2005-2010 Nicolas Hadacek <hadacek@kde.org>             *
 *   Copyright (C) 2003-2004 Alain Gibaud <alain.gibaud@free.fr>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "custom_process.h"
#include "custom_process.moc"

#include "tools/list/compile_manager.h"

//-----------------------------------------------------------------------------
void Compile::CustomProcess::setupProcess()
{
  _process->setUseShell(true);
  QStringList args = QStringList::split(" ", _command);
  args = process(args);
  _process->setup(args.join(" "), QStringList(), false);
}

void Compile::CustomProcess::logLine(::Process::OutputType, const QString &line)
{
  _manager->log(Log::LineType::Normal, line);
}

QString Compile::CustomProcess::deviceName() const
{
  return GPUtils::toDeviceName(_data.device);
}
