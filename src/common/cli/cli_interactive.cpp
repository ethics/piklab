/***************************************************************************
 *   Copyright (C) 2006-2009 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "cli_interactive.h"
#include "cli_interactive.moc"

#if defined(READLINE_FOUND)
#  include <readline/readline.h>
#  include <readline/history.h>
#else
#  include <stdio.h>
#endif
#include <signal.h>
#include <qtimer.h>
#include <stdlib.h>

#include "cli_log.h"

//-----------------------------------------------------------------------------
CLI::Interactive* CLI::Interactive::_instance = NULL;

CLI::Interactive::Interactive(Log::Base& log)
: QObject(NULL, "interactive"),
  Log::Base(&log)
{
  ASSERT(_instance == NULL);
  _instance = this;

  ::signal(SIGINT, signalHandler);
#if defined(READLINE_FOUND)
  using_history();
#else
  _stdin.open(IO_ReadOnly, stdin);
#endif
  QTimer::singleShot(0, this, SLOT(displayPrompt()));
}

CLI::Interactive::~Interactive()
{
  _instance = NULL;
}

void CLI::Interactive::redisplayPrompt()
{
#if defined(READLINE_FOUND)
  rl_forced_update_display();
#else
  fprintf(stdout, "> ");
  fflush(stdout);
#endif
}

void CLI::Interactive::displayPrompt()
{
#if defined(READLINE_FOUND)
  char *line = readline("> ");
  _input = QString(line);
  if ( !_input.isEmpty() ) add_history(line);
  free(line);
#else
  fprintf(stdout, "> ");
  fflush(stdout);
  char buffer[1000];
  _stdin.readLine(buffer, 1000);
  _input = QString(buffer);
#endif
  lineRead();
}

void CLI::Interactive::lineRead()
{
  QString line = _input.stripWhiteSpace();
  _input = QString::null;
  if (MainBase::instance().processLine(line) == EXITING) {
    QCoreApplication::exit(OK);
    return;
  }
  QTimer::singleShot(0, this, SLOT(displayPrompt()));
}

void CLI::Interactive::processSignal(int sig)
{
  if (sig == SIGINT) {
    log(Log::LineType::Normal, "<CTRL C> Break");
    fflush(stdout);
    MainBase::instance().processInterrupt();
    redisplayPrompt();
  }
}

void CLI::Interactive::signalHandler(int sig)
{
  Interactive::_instance->processSignal(sig);
}
