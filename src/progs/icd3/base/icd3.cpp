/***************************************************************************
 *   Copyright (C) 2012 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "icd3.h"

#include "common/common/misc.h"
#include "common/port/port_base.h"
#include "icd3_data.h"
#include "icd3_usb.h"

//-----------------------------------------------------------------------------
const Icd3::TARGET_MODE_VALUES::Data Icd3::TARGET_MODE_VALUES::DATA[Nb_Types] = {
  0x00, // stopped
  0x01, // running
  0x02  // in programming
};

const Icd3::RESET_MODE_VALUES::Data Icd3::RESET_MODE_VALUES::DATA[Nb_Types] = {
  "00", // reset held
  "01"  // reset released
};

//-----------------------------------------------------------------------------
const Icd3::RESULT_TYPE_VALUES::Data Icd3::RESULT_TYPE_VALUES::DATA[Nb_Types] = {
  0x00, // pass
  0x01, // low160
  0x80, // high
  -2,   // fail (unused)
};

const Icd3::TestData::VoltageType::Data Icd3::TestData::VoltageType::DATA[Nb_Types] = {
  { I18N_NOOP("Target Vdd")   },
  { I18N_NOOP("Module Vpp")   },
  { I18N_NOOP("MCLR ground") },
  { I18N_NOOP("MCLR Vdd")     },
  { I18N_NOOP("MCLR Vpp")     }
};

Icd3::TestData::TestData()
{
  FOR_EACH(VoltageType, type) _voltages[type] = -1;
}

Icd3::TestData::TestData(const QString &rx)
{
  FOR_EACH(VoltageType, type) _voltages[type] = fromHex(rx.mid(5 + 2*type.type(), 2), 0);
}

bool Icd3::TestData::pass() const
{
  FOR_EACH(VoltageType, type)
    if ( _voltages[type]!=RESULT_TYPE_VALUES::data(::Programmer::ResultType::Pass) ) return false;
  return true;
}

QString Icd3::TestData::result(VoltageType type) const
{
  if ( _voltages[type]==-1 ) return "---";
  FOR_EACH(::Programmer::ResultType, rtype)
    if ( _voltages[type]==RESULT_TYPE_VALUES::data(rtype) ) return type.label();
  return toHex(_voltages[type], 2);
}

QString Icd3::TestData::pretty(VoltageType type) const
{
  return type.label() + "=" + result(type);
}

//-----------------------------------------------------------------------------
bool Icd3::Hardware::uploadFirmware(const Pic::Memory &memory)
{
  // ### TODO
  return false;
}

bool Icd3::Hardware::setTarget()
{
  // ### TODO
  return false;
}

bool Icd3::Hardware::setup()
{
  QMemArray<uchar> data;
  if (!command(0x29, "", data, -1)) return false;
  if (!command(0x3F, "", data, 34)) return false;
  if (!command(0x3F, "", data, 34)) return false;
  if (!command(0x47, "", data, 17)) return false;
  // ...
  return !hasError();
}

bool Icd3::Hardware::command(char cmd, const char* cdata, QMemArray<uchar>& rdata, int rsize)
{
  if ( hasError() ) return false;

  QMemArray<uchar> sdata(2 + strlen(cdata));
  sdata[0] = cmd;
  sdata[1] = 0x00;
  for (uint i = 2; i<sdata.size(); ++i) sdata[i] = cdata[i-2];
  log(Log::DebugLevel::Extra, QString("send command: '%1'").arg(toPrintable(sdata, PrintAlphaNum)));
  if (!_port->send(sdata)) return false;
  if (rsize < 0) {
    return true;
  }

  if (!_port->receive(2+rsize, rdata)) return false;
  log(Log::DebugLevel::Extra, QString("received answer: '%1'").arg(toPrintable(rdata, PrintAlphaNum)));
  if (rdata[0] != cmd) {
    log(Log::LineType::Error, i18n("Unexpected answer (received %1).").arg(rdata[0]));
    return false;
  }
  if (rdata[1] != 0x00) {
    log(Log::LineType::Error, i18n("Unexpected answer (received %1).").arg(rdata[1]));
    return false;
  }
  for (uint i = 2; i<rdata.size(); ++i) rdata[i-2] = rdata[i];
  rdata.resize(rsize);
  return true;
}

bool Icd3::Hardware::getFirmwareVersion(VersionData &version)
{
  // ### TODO
  return false;
}

uint Icd3::Hardware::getFirmwareId()
{
  // ### TODO
  return 0;
}

bool Icd3::Hardware::getDebugExecVersion(VersionData &version)
{
  // ### TODO
  return false;
}

bool Icd3::Hardware::setTargetPowerOn(bool on)
{
  // ### TODO
  return false;
}

bool Icd3::Hardware::readVoltage(Device::VoltageType type, double &value)
{
  // ### TODO
  return false;
}

bool Icd3::Hardware::readVoltages(Device::VoltageValues &voltages)
{
  // ### TODO
  return false;
}

bool Icd3::Hardware::getTargetMode(Device::TargetMode &tmode)
{
  // ### TODO
  return false;
}

bool Icd3::Hardware::setTargetReset(Device::ResetMode mode)
{
  // ### TODO
  return false;
}

bool Icd3::Hardware::selfTest(TestData &test)
{
  // ### TODO
  return false;
}

bool Icd3::Hardware::readBlock(uint nbBytesWord, uint nbWords, Device::Array &data)
{
  // ### TODO
  return false;
}

bool Icd3::Hardware::readMemory(Pic::MemoryRangeType type, uint wordOffset,
                                Device::Array &data, const ::Programmer::VerifyData *vdata)
{
  // ### TODO
  return false;
}

bool Icd3::Hardware::writeBlock(uint nbBytesWord, const Device::Array &data, uint wordIndex, uint nbWords)
{
  // ### TODO
  return false;
}

bool Icd3::Hardware::writeMemory(Pic::MemoryRangeType type, uint wordOffset, const Device::Array &data)
{
  // ### TODO
  return false;
}

bool Icd3::Hardware::eraseAll()
{
  // ### TODO
  return false;
}

bool Icd3::Hardware::haltRun()
{
  // ### TODO
  return false;
}

bool Icd3::Hardware::step()
{
  // ### TODO
  return false;
}

bool Icd3::Hardware::resumeRun()
{
  // ### TODO
  return false;
}

bool Icd3::Hardware::setWriteMode(Device::WriteMode mode)
{
  // ### TODO
  return false;
}

bool Icd3::Hardware::writeRegister(Address address, BitValue value, uint nbBytes)
{
  // ### TODO
  return false;
}

bool Icd3::Hardware::readRegister(Address address, BitValue &value, uint nbBytes)
{
  // ### TODO
  return false;
}

BitValue Icd3::Hardware::getProgramCounter()
{
  // ### TODO
  return false;
}
