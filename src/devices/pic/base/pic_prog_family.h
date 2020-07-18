/***************************************************************************
 *   Copyright (C) 2012 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef PIC_PROG_FAMILY_H
#define PIC_PROG_FAMILY_H

#include "common/common/global.h"

namespace Pic
{

  struct ProgFamilyData {
    const char *name;
    uint codeWaitTime, lowVoltageCodeWaitTime, eepromWaitTime, configWaitTime, userIdsWaitTime; // ms
    uint eraseWaitTime, lowVoltageEraseWaitTime; // ms
    uint codeLatches, eepromLatches, configLatches, userIdsLatches, rowEraseLatches;
    uint vddMin, vddMax, vddDef; // mV
    uint vppMin, vppMax, vppDef; // mV
    uint vddNominalMin, vddNominalMax, eraseMin; // mV
    uint testStart, testSize; // test memory
    uint panelSize;
    uint bootSize;
  };
  extern const ProgFamilyData PROG_FAMILY_DATA[];
  extern int findProgFamilyIndex(const QString& family);
  extern const ProgFamilyData* findProgFamilyData(const QString& family);

} // namespace

#endif
