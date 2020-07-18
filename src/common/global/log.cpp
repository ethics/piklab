/***************************************************************************
 *   Copyright (C) 2005 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "log.h"

#include <qeventloop.h>

//-----------------------------------------------------------------------------
const Log::LineType::Data Log::LineType::DATA[Nb_Types] = {
  { 0, 0, "red",       false }, // error
  { 0, 0, "red",       false }, // soft error
  { 0, 0, "orange",    false }, // warning
  { 0, 0, "black",     false }, // normal
  { 0, 0, "blue",      false }, // info
  { 0, 0, "black",     true  }, // command
  { 0, 0, "black",     true  }, // input
  { 0, 0, "lightGray", false } // comment
};

const Log::DebugLevel::Data Log::DebugLevel::DATA[Nb_Types] = {
  { "quiet",          I18N_NOOP("No debug message"),      0,           false },
  { "debug",          I18N_NOOP("Normal debug messages"), "darkGreen", false },
  { "extra-debug",    I18N_NOOP("Extra debug messages"),  "darkGreen", false },
  { "max-debug",      I18N_NOOP("Max debug messages"),    "darkGreen", false },
  { "lowlevel-debug", I18N_NOOP("All debug messages"),    "darkGreen", false }
};

//-----------------------------------------------------------------------------
Log::View::View()
{
  setDebugLevel(DebugLevel::Normal);
  FOR_EACH(LineType, type) _modes[type.type()] = Show;
}

void Log::View::setDebugLevel(DebugLevel level)
{
  _debugLevel = level;
}

void Log::View::log(LineType type, const QString &text, Action action)
{
  if ( _modes[type.type()]==Show ) doLog(type, text, action);
}

void Log::View::log(DebugLevel level, const QString &text, Action action)
{
  ASSERT( level!=DebugLevel::Quiet );
  updateDebugLevel();
  if ( level<=_debugLevel ) doLog(level, text, action);
}

void Log::View::logUserAbort()
{
  doLog(LineType::SoftError, i18n("Operation aborted by user."), Immediate);
}

//-----------------------------------------------------------------------------
void Log::StringView::sorry(const QString &message, const QString &details)
{
  if ( details.isEmpty() ) _s += message;
  else _s += message + ": " + details;
}

bool Log::StringView::askContinue(const QString &message)
{
  log(LineType::Warning, message, Immediate);
  return false; // always fail
}

//-----------------------------------------------------------------------------
Log::Base::Base(Base *parent)
: _parent(NULL),
  _data(NULL)
{
  setParent(parent);
}

void Log::Base::setParent(Base *parent)
{
  _parent = parent;
  _data.reset(parent != NULL ? NULL : new LogData);
}

void Log::Base::setView(View *view)
{
  ASSERT(_data.get() != NULL);
  _data->_view = view;
}

void Log::Base::logUserAbort()
{
  if (view() != NULL) view()->logUserAbort();
}

void Log::Base::log(LineType type, const QString &message, Action action)
{
  if (type == LineType::Error) setError(message);
  if (view() != NULL) view()->log(type, message, action);
}

void Log::Base::log(DebugLevel level, const QString &message, Action action)
{
  if (view() != NULL) view()->log(level, message, action);
}

void Log::Base::sorry(const QString &message, const QString &details)
{
  if (view() != NULL) view()->sorry(message, details);
}

bool Log::Base::askContinue(const QString &message)
{
  if (view() == NULL) return false;
  return view()->askContinue(message);
}

void Log::Base::clear()
{
  resetError();
  if (view() != NULL) view()->clear();
}

//-----------------------------------------------------------------------------
QString Log::KeyList::text() const
{
  QString text;
  if ( !_title.isEmpty() ) text += _title + "\n";
  uint nb = 0;
  for (uint i=0; i<uint(_keys.count()); i++) nb = qMax(nb, uint(_keys[i].length()));
  for (uint i=0; i<uint(_keys.count()); i++) text += " " + _keys[i].leftJustify(nb+2) + _labels[i] + "\n";
  return text;
}

void Log::KeyList::display(Generic &log) const
{
  if ( !_title.isEmpty() ) log.log(Log::LineType::Normal, _title);
  uint nb = 0;
  for (uint i=0; i<uint(_keys.count()); i++) nb = qMax(nb, uint(_keys[i].length()));
  for (uint i=0; i<uint(_keys.count()); i++) log.log(Log::LineType::Normal, " " + _keys[i].leftJustify(nb+2) + _labels[i]);
}
