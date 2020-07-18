/***************************************************************************
 *   Copyright (C) 2005-2009 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "cli_log.h"

#include "cli_main.h"

//-----------------------------------------------------------------------------
CLI::View::View()
: _file(*this)
{
}

void CLI::View::logInFile(Log::LineType type, const QString &text)
{
  if (_file.isOpen()) {
    if (type == Log::LineType::Input) _file.appendText("o\n");
    QString prefix;
    switch (type.type()) {
      case Log::LineType::Error:
        case Log::LineType::SoftError: prefix = "e"; break;
        case Log::LineType::Warning:   prefix = "w"; break;
        case Log::LineType::Input:     prefix = "i"; break;
        case Log::LineType::Comment:   prefix = "#"; break;
        default:                       prefix = "o"; break;
    }
    _file.appendText(prefix + " " + text + "\n");
    _file.flush();
  }
}

void CLI::View::doLog(Log::LineType type, const QString &text, Log::Action action)
{
  // log file
  logInFile(type, text);

  // command-line output
  if (type.type() != Log::LineType::Input
      && action != Log::LogFileOnly) {
    QString prefix;
    switch (type.type()) {
      case Log::LineType::Error:
      case Log::LineType::SoftError: prefix = "Error: ";   break;
      case Log::LineType::Warning:   prefix = "Warning: "; break;
      default: break;
    }
    const char* cs = stringToChar(prefix + text + "\n");
    if ( type==Log::LineType::Error || type==Log::LineType::SoftError ) fprintf(stderr, "%s", cs);
    else fprintf(stdout, "%s", cs);
  }
}

void CLI::View::doLog(Log::DebugLevel, const QString &text, Log::Action action)
{
  // log file
  if (_file.isOpen()) {
    _file.appendText("d " + text + "\n");
    _file.flush();
  }

  // command-line output
  if (action != Log::LogFileOnly) {
    QString s = text + "\n";
    const char* cs = stringToChar(s);
    fprintf(stdout, "%s", cs);
  }
}

void CLI::View::sorry(const QString &message, const QString &details)
{
  if ( details.isEmpty() ) log(Log::LineType::Error, message);
  else log(Log::LineType::Error, message + " (" + details + ")");
}

bool CLI::View::askContinue(const QString &message)
{
  bool force = MainBase::instance().force();
  log(Log::LineType::Warning, message + " " + (force ? i18n("*yes*") : i18n("*no*")));
  if (force) return true;
  // #### TODO
  return false;
}

void CLI::View::logUserAbort()
{
  //Log::View::logUserAbort();
}

bool CLI::View::setLogFile(const PURL::Url& url)
{
  _file.setUrl(url);
  return _file.openForWrite();
}
