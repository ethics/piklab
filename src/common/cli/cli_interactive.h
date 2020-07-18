/***************************************************************************
 *   Copyright (C) 2006-2009 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef CLI_INTERACTIVE_H
#define CLI_INTERACTIVE_H

#include <qfile.h>

#include "common/common/global.h"
#include "common/global/log.h"
#include "cli_main.h"

namespace CLI
{
//----------------------------------------------------------------------------
class Interactive : public QObject, public Log::Base
{
Q_OBJECT
public:
  Interactive(Log::Base& log);
  virtual ~Interactive();
  void redisplayPrompt();

private slots:
  void displayPrompt();

private:
  static Interactive* _instance;
  QFile   _stdin;
  QString _input;

  void lineRead();
  ExitCode start();
  void processSignal(int sig);

  static void signalHandler(int sig);
};

} // namespace

#endif
