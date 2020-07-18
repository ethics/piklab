/***************************************************************************
 *   Copyright (C) 2005-2009 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "prog_list.h"

const Programmer::Lister& Programmer::Lister::instance()
{
  static Lister* lister = NULL;
  if (lister == NULL) lister = new Lister;
  return *lister;
}
