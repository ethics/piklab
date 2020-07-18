/***************************************************************************
 *   Copyright (C) 2010 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "op_pic.h"

#include "common/common/misc.h"
#include "op_code.h"

//-----------------------------------------------------------------------------
OpenProg::PicSpecific::PicSpecific(::Programmer::Base &base)
  : ::Programmer::PicSpecific(base)
{
}

void OpenProg::PicSpecific::setT3Array(Array& cmd, uint us) const
{
  ASSERT(us < 0xFFFF);
  cmd += SET_PARAMETER;
  cmd += SET_T3;
  cmd += us >> 8;
  cmd += us & 0xFF;
}

void OpenProg::PicSpecific::powerOnArray(Array& cmd) const
{
  cmd += SET_PARAMETER;
  cmd += SET_T1T2;
  cmd += 1;              // T1 = 1 us
  cmd += 100;            // T2 = 100 us
  cmd += EN_VPP_VCC;  //enter program mode
  cmd += 0x0;
  cmd += SET_CK_D;
  cmd += 0x0;
}

bool OpenProg::PicSpecific::setPowerOnVddFirst(
  bool withDelay)
{
  setPowerOff();
  Array cmd;
  powerOnArray(cmd);
  cmd += EN_VPP_VCC;
  cmd += 0x1;        // VDD
  if (withDelay) { // 50 ms delay
    setT3Array(cmd, 25000); // T3 = 25 ms
    cmd += WAIT_T3;
    cmd += WAIT_T3;
  }
  cmd += EN_VPP_VCC;
  cmd += 0x5;        // VDD+VPP
  return hardware().command(cmd, 50); // 50 ms
}

bool OpenProg::PicSpecific::setPowerOnVppFirst()
{
  setPowerOff();
  Array cmd;
  powerOnArray(cmd);
  cmd += EN_VPP_VCC;
  cmd += 0x4;        // VPP
  cmd += NOP;
  cmd += EN_VPP_VCC;
  cmd += 0x5;        // VDD+VPP
  return hardware().command(cmd);
}

bool OpenProg::PicSpecific::setPowerOff()
{
  Array cmd;
  cmd += NOP;
  cmd += EN_VPP_VCC;
  cmd += 0x1;         // VDD
  cmd += EN_VPP_VCC;
  cmd += 0x0;
  cmd += SET_CK_D;
  cmd += 0x0;
  return hardware().command(cmd);
}
