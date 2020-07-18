/***************************************************************************
 *   Copyright (C) 2005-2010 Nicolas Hadacek <hadacek@kde.org>             *
 *   Copyright (C) 2002-2003 Stephen Landamore <stephen@landamore.com>     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "progs/icd2/base/icd2_data.h"

// last updated for MPLAB v8.50

const Icd2::FamilyData Icd2::FAMILY_DATA[] = {
  //                              HEX_OFFSET WREG   FSR    STATUS
  { "16F62XA",      1, "16F648A",  0x0F00,   0x14F, 0x14E, 0x1F0   }, // 16F627A/628A
  { "16F648A",      1, "16F648A",  0x0F00,   0x16F, 0x16E, 0x1F0   }, // 16F648A
  { "16F716",       1, "16F716",   0x0700,   0x06F, 0x06E, 0x0F0   },
  { "16F7X7",       1, "16F7X7",   0x1F00,   0x16F, 0x16E, 0x1F0   }, // 16F737/747/767/777
  { "16F819",       1, "16F819",   0x0700,   0x16F, 0x16E, 0x1F0   }, // 16F818/819
  { "16F88",        1, "16F88",    0x0F00,   0x1EF, 0x1EE, 0x1F0   }, // 16F87/88
  { "16F872",       1, "16F872",   0x0700,   0x1BF, 0x1BE, 0x1F0   }, // 16F870/871/872
  { "16F874",       1, "16F874",   0x0F00,   0x1FF, 0x1FE, 0x17F   }, // 16F873/873A/874/874A
  { "16F877",       1, "16F877",   0x1F00,   0x1EF, 0x1EE, 0x1F0   }, // 16F876/876A/877/877A
  { "16F7X",        1, NULL,       0,        0,     0,     0       }, // 16F73/74/76/77 (not debuggable)

  { NULL,           2, NULL,       0,        0,     0,     0       }, // 30F for revision A2 (legacy: not in MPLAB 7)
  { NULL,           3, "30F",      0,        0,     0,     0       }, // 30F for revision A3/B0 (legacy: not in MPLAB 7)

  { "18F_4",        4, "",         0,        0,     0,     0       }, // debug executive filename is computed at runtime
  { "18F_5",        5, "",         0,        0,     0,     0       }, // debug executive filename is computed at runtime
  { "18CX01",       6, NULL/*"18CX01"*/, 0,  0,     0,     0       }, // 16C601/801

  { "16F5X",        7, NULL,       0,        0,     0,     0       }, // 16F54/57/59 (not debuggable)
  { "16F505",       7, NULL/*"16F505"*/, 0,  0,     0,     0       }, // 10F200/202/204/206 12F508/509 16F505
  { "16F506",       7, NULL/*"16F506"*/, 0,  0,     0,     0       }, // 10F220/222 12F510 16F506/16F526

  { "12F635",       8, "12F635",   0x0700, 0x06F, 0x06E, 0x0F0     }, // 12F635/683 16F631/636/639
  { "16F684",       8, "16F684",   0x0700, 0x06F, 0x06E, 0x0F0     }, // 16F677/684/687/785
  { "16F688",       8, "16F688",   0x0F00, 0x16F, 0x16E, 0x1F0     }, // 16F685/688/689/690
  { "16F722",       8, "72X_2K_B1",0x0700, 0x0BF, 0x0BE, 0x1F0     }, // 16F722
  { "16F723",       8, "72X_4K_B2",0x0F00, 0x12F, 0x12E, 0x1F0     }, // 16F723/724
  { "16F726",       8, "72X_8K_B3",0x1F00, 0x1EF, 0x1EE, 0x1F0     }, // 16F726/727
  { "16F882",       8, "16F882",   0x0670, 0x0BF, 0x0BE, 0x1F0     }, // 16F882
  { "16F884",       8, "16F884",   0x0F00, 0x16F, 0x16E, 0x1F0     }, // 16F883/884
  { "16F886",       8, "16F886",   0x1F00, 0x1EF, 0x1EE, 0x1F0     }, // 16F886/887
  { "16F916",       8, "16F916",   0x1F00, 0x16F, 0x16E, 0x1F0     }, // 16F913/914/916/917/946

  { "12F629",       9, "12F629",   0x0300, 0x0DF, 0x0DE, 0x0D4     }, // 12F629/675
  { "16F676",       9, "16F676",   0x0300, 0x0DF, 0x0DE, 0x0D4     }, // 16F630/676

  { "30F",         10, "30f_REVB", 0,      0,     0,     0         }, // 30F revision B1 and above
  { "18F_J",       11, NULL,       0,      0,     0,     0         }, // 18J
  { "16F72",       12, NULL,       0,      0,     0,     0         }, // 16F627/628/72/83/84/84A (not debuggable)

  { "24F",         14, "24F",      0,      0,     0,     0         }, // 24F
  { "33F",         14, "33F",      0,      0,     0,     0         }, // 33F/24H

  { "16F610",      15, "PIC16F616-ICD_1K_B0", 0x0300, 0x06F, 0x06E, 0x0F0 }, // 12F609/615 16F610
  { "16F616",      15, "PIC16F616-ICD_1K_B1", 0x0700, 0x0BF, 0x0BE, 0x0F0 }, // 16F616

  { "18F_K",       16, "",         0,        0,     0,     0       }, // debug executive filename is computed at runtime

  { "16F1826",     22, NULL,       0,      0,     0,     0         }, // 16F1826/1827

  { NULL,           0, NULL,       0,      0,     0,     0         }
};
