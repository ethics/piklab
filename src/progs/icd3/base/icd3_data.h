/***************************************************************************
 *   Copyright (C) 2012 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef ICD3_DATA_H
#define ICD3_DATA_H

#include "common/common/group.h"

namespace Icd3
{
  struct FamilyData {
    const char *name;
  };
  extern const FamilyData FAMILY_DATA[];
  struct Data {
        uint famid;
    //    ::Group::Support debugSupport;
  };
  extern const Data &data(const QString &device);
  extern uint family(const QString &device);
} // namespace

#endif
