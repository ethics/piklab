/***************************************************************************
 *   Copyright (C) 2012 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef PICKIT3_DATA_H
#define PICKIT3_DATA_H

#include "common/common/global.h"
#include "devices/pic/base/pic_prog_family.h"

namespace Pickit3
{

//-----------------------------------------------------------------------------
  struct FamilyData {
    const char* name;
    uint minorAlgo, debugAlgo, eraseAlgo;
    uint apId;
  };

  extern const FamilyData FAMILY_DATA[];
  extern const FamilyData* findFamilyData(const QString& family);

//-----------------------------------------------------------------------------
  struct Data {
    uint eraseAlgo, flags;
  };
  extern bool isSupported(const QString &device);
  extern uint family(const QString &device);
  extern const Data &data(const QString &device);

} // namespace

#endif
