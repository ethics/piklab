/***************************************************************************
 *   Copyright (C) 2012 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "progs/pickit3/base/pickit3_data.h"

const Pickit3::FamilyData Pickit3::FAMILY_DATA[] = {
  //               MinorAlgo DebugAlgo EraseAlgo AlgoPlugin
  { "18FXX2_XX8",      0x01,     0x05,     0x01, 5 },
  { "18F2XXX_4XXX",    0x02,     0x05,     0x07, 5 },
  {         NULL,         0,        0,        0, 0 }
};

const Pickit3::FamilyData* Pickit3::findFamilyData(const QString& name)
{
  for (uint i = 0; FAMILY_DATA[i].name != NULL ; ++i) {
    if (name == FAMILY_DATA[i].name) return &FAMILY_DATA[i];
  }
  return NULL;
}
