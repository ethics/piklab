/***************************************************************************
 *   Copyright (C) 2012 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "pic_prog_family.h"

int Pic::findProgFamilyIndex(const QString& family)
{
  for (uint i = 0; PROG_FAMILY_DATA[i].name != NULL ; ++i) {
    if (PROG_FAMILY_DATA[i].name == family) return i;
  }
  return -1;
}

const Pic::ProgFamilyData* Pic::findProgFamilyData(const QString& family)
{
  int i = findProgFamilyIndex(family);
  if (i == -1) return NULL;
  return &PROG_FAMILY_DATA[i];
}

