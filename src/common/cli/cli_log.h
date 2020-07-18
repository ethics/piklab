/***************************************************************************
 *   Copyright (C) 2005-2009 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef CLI_LOG_H
#define CLI_LOG_H

#include "common/global/log.h"
#include "common/global/pfile.h"

namespace CLI
{

class View : public Log::View
{
public:
  View();
  virtual void clear() {}
  virtual void sorry(const QString &message, const QString &details);
  virtual bool askContinue(const QString &message);
  virtual void logUserAbort();

  bool setLogFile(const PURL::Url& url);
  void logInFile(Log::LineType type, const QString& text);

private:
  PURL::File _file;

  virtual void doLog(Log::LineType type, const QString &text, Log::Action action);
  virtual void doLog(Log::DebugLevel level, const QString &text, Log::Action action);
};

} // namespace

#endif
