/***************************************************************************
 *   Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "synchronous.h"
#include "synchronous.moc"

#include "common/common/misc.h"
#include "global.h"

//------------------------------------------------------------------------------
Synchronous::Synchronous(uint timeout)
: _state(Stopped),
  _timer(NULL)
{
  if (timeout) {
    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), SLOT(done()));
    _timer->start(timeout, true);
  }
  _loop = new QEventLoop(this);
}

bool Synchronous::enterLoop()
{
  ASSERT( _state!=Running );
  if ( _state==Done ) return true;
  _state = Running;
  _loop->exec();
  return (_timer != NULL ? _timer->isActive() : true);
}

void Synchronous::done()
{
  if ( _state==Running ) {
    _loop->exit();
  }
  _state = Done;
}
