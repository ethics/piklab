/***************************************************************************
 *   Copyright (C) 2012 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "pickit3.h"

#include "progs/base/generic_prog.h"
#include "progs/icd2/base/microchip.h"
#include "coff/base/coff_data.h"
#include "progs/pickit3/base/pickit3_data.h"
#include "devices/pic/pic/pic_group.h"

//-----------------------------------------------------------------------------
const Pickit3::ProgRegion::Data Pickit3::ProgRegion::DATA[Nb_Types] = {
  { Pic::MemoryRangeType::Code,             0x01,  0x01  },
  { Pic::MemoryRangeType::Eeprom,           0x02,  0x02  },
  { Pic::MemoryRangeType::UserId,           0x04,  0x03  },
  { Pic::MemoryRangeType::Config,           0x08,  0x04  },
  { Pic::MemoryRangeType::Nb_Types,         0x10,  0x05  },
  { Pic::MemoryRangeType::Cal,              0x20,  0x09  },
  { Pic::MemoryRangeType::ProgramExecutive, 0x40,  0x08  },
  { Pic::MemoryRangeType::Nb_Types,         0x80,  0x07  },
  { Pic::MemoryRangeType::Nb_Types,         0x100, 0x0F }
};

Pickit3::ProgRegion Pickit3::getProgRegion(Pic::MemoryRangeType type)
{
  ProgRegion progRegion;
  for (; progRegion < ProgRegion::Nb_Types; ++progRegion) {
    if (progRegion.data().type == type) {
      break;
    }
  }
  return progRegion;
}

Pickit3::SystemStatus::SystemStatus()
: _major(0), _init0(0), _init1(0), _warn0(0), _warn1(0), _mode0(0), _mode1(0)
{
}

QString Pickit3::SystemStatus::pretty() const
{
  return QString("status: major=%1 init0=%2 init1=%3 warn0=%4 warn1=%5 mode0=%6 mode1=%7")
  .arg(_major).arg(_init0).arg(_init1).arg(_warn0).arg(_warn1).arg(_mode0).arg(_mode1);
}

//-----------------------------------------------------------------------------
Pickit3::Port::Port(Log::Base &log)
: ::Port::USB(log, Microchip::VENDOR_ID, 0x900A, 1, 0)
{}

bool Pickit3::Port::send(const ByteArray &data)
{
  ASSERT(data.size() == BUF_SIZE);
  log(Log::DebugLevel::Extra, QString("> \"%1\"").arg(data.pretty(0, 64)));
  ::Port::msleep(5);
  return write(0x01, (const char*)data.data(), BUF_SIZE);
}

bool Pickit3::Port::receive(ByteArray &data)
{
  ASSERT(data.size() == BUF_SIZE);
  ::Port::msleep(5);
  if (!read(0x81, (char*)data.data(), BUF_SIZE)) {
    return false;
  }
  log(Log::DebugLevel::Max, QString("< \"%1\"").arg(data.pretty(0, 64)));
  return true;
}

//-----------------------------------------------------------------------------
Pickit3::Hardware::Hardware(::Programmer::Base &base)
: ::Programmer::PicHardware(base, new Port(base), QString::null),
  _apId(0), _firmwareId(0),
  _powerOptions(NoPowerOption)
{
}

bool Pickit3::Hardware::command(uchar cmd, const ByteArray* out)
{
  ByteArray data = createArray();
  data.setValue(cmd, 0, 1);
  data.setValue(0, 1, 1);
  uint size = out != NULL ? out->size() : 0;
  uint len = 2 + size;
  for (uint i = 0; i < size && i < 58; ++i) data[2+i] = (*out)[i];
  data.setValue(len, 60, 4);

  if (!port().send(data)) return false;
  for (uint k = 58; k < size; k += 64) {
    data.clear();
    for (uint i = 0; i < 64 && k+i < size; ++i) data[i] = (*out)[k+i];
    if (!port().send(data)) return false;
  }
  return true;
}

bool Pickit3::Hardware::commandWithSimpleResponse(uchar cmd, ByteArray& in, const ByteArray* out)
{
  if (!command(cmd, out)) return false;

  in = createArray();
  if (!port().receive(in)) return false;
  uint receivedCmd = in.getValue(0, 2);
  if (receivedCmd != cmd) {
    ByteArray cmdArray(2, 0x00, PrintEscapeAll);
    cmdArray[0] = cmd;
    cmdArray[1] = 0x00;
    log(Log::LineType::Error, i18n("Wrong return command \"%1\" (was expecting \"%2\")")
      .arg(in.pretty(0, 2)).arg(cmdArray.pretty(0, 2)));
    return false;
  }
  return true;
}

bool Pickit3::Hardware::commandWithComplexResponse(uchar cmd, const char* cmdName, ByteArray& in, const ByteArray* out)
{
  ByteArray data;
  if (!commandWithSimpleResponse(cmd, data, out)) return false;

  uint offset = 4;
  uint size = data.getValue(60, 4);
  log(Log::DebugLevel::Max, QString("Receiving %1 bytes").arg(size));
  uint receivedSubCmd = data.getValue(2, 2);
  if (receivedSubCmd == 0x3F) {
    for (;;) {
      if (!port().receive(data)) return false;
      int receivedCmd = data.getValue(0, 2);
      if (receivedCmd == 0x00) break;
      if (receivedCmd == 0xFF) {
        ByteArray cmdArray(2, 0x00, PrintEscapeAll);
        cmdArray[0] = cmd;
        cmdArray[1] = 0x00;
        log(Log::LineType::Error, i18n("Command \"%1\" failed (%2)")
          .arg(cmdName).arg(cmdArray.pretty(0, 2)));
        return false;
      }
      if (receivedCmd != 0x3F) {
        ByteArray cmdArray(2, 0x00, PrintEscapeAll);
        cmdArray[0] = 0x3F;
        cmdArray[1] = 0x00;
        log(Log::LineType::Error, i18n("Wrong return command \"%1\" (was expecting \"%2\")")
          .arg(data.pretty(0, 2)).arg(cmdArray.pretty(0, 2)));
        return false;
      }
    }
    offset = 2;
  }

  in.resize(size);
  for (uint i = 0; i < 60-offset && i < size; ++i) in[i] = data[i+offset];
  for (uint i = 60-offset; i < size; i += 64) {
    if (!port().receive(data)) return false;
    for (uint k = 0; k < 64 && i+k < size; ++k) in[i+k] = data[k];
  }
  return true;
}

uint Pickit3::Hardware::computeChecksum(const ByteArray& data) const
{
  uint len = data.size();
  BitValue cs = 0;
  for (uint k = 0; k < len; k += 2) {
    BitValue v1 = data[k];
    BitValue v2 = data[k + 1];
    cs += (v1 << 8) | v2;
  }
  return cs.complementInMask(0xFFFF).toUInt() + 1;
}

bool Pickit3::Hardware::sendBulk(const ByteArray& data)
{
  // array to send
  uint len = data.size();
  uint nb = (len+2+4) / 64;
  if ((len+2+4) % 64 != 0) ++nb;
  ByteArray array(64*nb, 'Z', PrintEscapeAll);
  for (uint i = 0; i < 60; ++i) array[i] = data[i];
  array.setValue(len+2, 60, 4);
  for (uint i = 60; i < len; ++i) array[4+i] = data[i];

  // checksum
  array.setValue(computeChecksum(data), 4+len, 2);

  // send
  ByteArray out = createArray();
  for (uint i = 0; i < array.size(); ++i) {
    out[i%64] = array[i];
    if (i%64 == 63 || i == array.size()-1) {
      if (!port().send(out)) return false;
      out.clear();
    }
  }

  ByteArray in = createArray();
  return port().receive(in);
}

bool Pickit3::Hardware::internalConnectHardware()
{
  if (!port().open()) return false;
  if (!command(0x29)) return false;
  SystemStatus status;
  if (!getStatus(status)) return false;
  if (!selfTest()) return false;
  if (!readSerialNumber()) return false;
  if (!queryProgToGoConfig()) return false;
  if (!getProtocolVersion()) return false;
  return true;
}

bool Pickit3::Hardware::getStatus(SystemStatus& status)
{
  ByteArray in;
  if (!commandWithSimpleResponse(0x3F, in)) return false;
  status._major = in.getValue( 6, 2);
  status._init0 = in.getValue(10, 2);
  status._init1 = in.getValue(14, 2);
  status._warn0 = in.getValue(18, 2);
  status._warn1 = in.getValue(22, 2);
  status._mode0 = in.getValue(26, 2);
  status._mode1 = in.getValue(30, 2);
  log(Log::DebugLevel::Extra, status.pretty());
  return true;
}

bool Pickit3::Hardware::selfTest()
{
  ByteArray in;
  return commandWithComplexResponse(0x26, "bist", in);
}

bool Pickit3::Hardware::readSerialNumber()
{
  ByteArray in;
  if (!commandWithSimpleResponse(0x47, in)) return false;
  QString serial;
  for (uint i = 6; i < 64; ++i) {
    if (in[i] == 0x0) break;
    serial += in[i];
  }
  log(Log::DebugLevel::Normal, QString("  Serial number: %1").arg(serial));
  return true;
}

bool Pickit3::Hardware::getVersions()
{
  ByteArray in;
  if (!commandWithSimpleResponse(0x41, in)) return false;
  const uint size = in.getValue(2, 2);
  if (size != 0x16) {
    log(Log::LineType::Error, i18n("Wrong size \"%1\" (was expecting \"%2\")").arg(size).arg(0x16));
    return false;
  }
  const uint status = in.getValue(4, 2);
  if (status != 0x00) {
    log(Log::LineType::Error, i18n("Wrong status \"%1\" (was expecting \"%2\")").arg(status).arg(0x00));
    return false;
  }
  _osVersion = VersionData(in.getValue(7, 1), in.getValue(8, 1), in.getValue(9, 1));
  log(Log::DebugLevel::Extra, QString("OS Version: %1").arg(_osVersion.pretty()));
  _apId = in.getValue(10, 1);
  _apVersion = VersionData(in.getValue(11, 1), in.getValue(12, 1), in.getValue(13, 1));
  log(Log::DebugLevel::Extra, QString("AP: %1 Version: %2").arg(_apId).arg(_apVersion.pretty()));
  _hardwareRevision = in.getValue(18, 2);
  log(Log::DebugLevel::Extra, QString("Hardware Revision: %1").arg(toHexLabel(_hardwareRevision, 2)));
  _firmwareId = in.getValue(24, 1);
  _firmwareVersion = VersionData(in.getValue(25, 1), in.getValue(26, 1), in.getValue(27, 1));
  log(Log::DebugLevel::Extra, QString("Firmware: %1 Version: %2").arg(_firmwareId).arg(_firmwareVersion.pretty()));
  return true;
}

bool Pickit3::Hardware::queryProgToGoConfig()
{
  ByteArray in;
  if (!commandWithComplexResponse(0xA1, "queryProgToGoConfig", in)) return false;
  // TODO
  return true;
}

bool Pickit3::Hardware::getProtocolVersion()
{
  ByteArray in;
  if (!commandWithSimpleResponse(0x44, in)) return false;
  // TODO
  return true;
}

ByteArray Pickit3::Hardware::createConfigPacket() const
{
  ByteArray data(334, 0x0, PrintEscapeAll);
  //processor id
  uint id = Coff::data(device().name()).ids[0];
  data.setValue(id, 0, 2);
  // operation mode
  data.setValue(_base.group().isDebugger() ? 1 : 2, 2, 2);
  // device id start and mask
  Address a = device().range(Pic::MemoryRangeType::DeviceId).start;
  data.setValue(a, 4, 4);
  BitValue v = device().devIdMask();
  data.setValue(v, 8, 4);
  // device id
  v = device().ids()[Device::Special::Normal];
  data.setValue(v, 12, 4);
  // 8 other possible device ids
  data.setValue(0, 16, 4);
  data.setValue(0, 20, 4);
  data.setValue(0, 24, 4);
  data.setValue(0, 28, 4);
  data.setValue(0, 32, 4);
  data.setValue(0, 36, 4);
  data.setValue(0, 40, 4);
  data.setValue(0, 44, 4);
  // code wait time
  const Pic::ProgFamilyData* progFamily = device().progFamilyData();
  data.setValue(progFamily->codeWaitTime, 48, 2);
  // low voltage code wait time
  data.setValue(progFamily->lowVoltageCodeWaitTime, 50, 2);
  // eeprom wait time
  data.setValue(progFamily->eepromWaitTime, 52, 2);
  // config wait time
  data.setValue(progFamily->configWaitTime, 54, 2);
  // user ids wait time
  data.setValue(progFamily->userIdsWaitTime, 56, 2);
  // erase wait time
  data.setValue(progFamily->eraseWaitTime, 58, 2);
  // low voltage erase wait time
  data.setValue(progFamily->lowVoltageEraseWaitTime, 60, 2);
  // low voltage code wait time
  // erase algo
  const FamilyData& family = FAMILY_DATA[Pickit3::family(device().name())];
  data.setValue(family.eraseAlgo, 62, 2);
  // code latches
  data.setValue(progFamily->codeLatches, 64, 1);
  // eeprom latches
  data.setValue(progFamily->eepromLatches, 65, 1);
  // config latches
  data.setValue(progFamily->configLatches, 66, 1);
  // user ids latches
  data.setValue(progFamily->userIdsLatches, 67, 1);
  // row erase latches
  data.setValue(progFamily->rowEraseLatches, 68, 1);
  // vdd min, max, and default
  uint vddMin = progFamily->vddMin;
  if (vddMin == 2000) vddMin = 2125; // FIXME
  data.setValue((vddMin * 8) / 1000, 69, 1);
  data.setValue((progFamily->vddMax * 8) / 1000, 70, 1);
  data.setValue((progFamily->vddDef * 8) / 1000, 71, 1);
  // vpp min, max, and default
  data.setValue((progFamily->vppMin * 8) / 1000, 72, 1);
  data.setValue((progFamily->vppMax * 8) / 1000, 73, 1);
  data.setValue((progFamily->vppDef * 8) / 1000, 74, 1);
  // default vdd min and max
  data.setValue((progFamily->vddNominalMin * 8) / 1000, 75, 1);
  data.setValue((progFamily->vddNominalMax * 8) / 1000, 76, 1);
  // low voltage threshold
  data.setValue((progFamily->eraseMin * 8) / 1000, 77, 1);
  // code start and size
  a = device().range(Pic::MemoryRangeType::Code).start;
  data.setValue(a, 78, 4);
  uint codeSize = device().nbWords(Pic::MemoryRangeType::Code)
    * device().addressIncrement(Pic::MemoryRangeType::Code);
  data.setValue(codeSize, 82, 4);
  // eeprom start and size
  a = device().range(Pic::MemoryRangeType::Eeprom).start;
  data.setValue(a, 86, 4);
  uint size = device().nbWords(Pic::MemoryRangeType::Eeprom)
    * device().addressIncrement(Pic::MemoryRangeType::Eeprom);
  data.setValue(size, 90, 4);
  // user ids start and size
  a = device().range(Pic::MemoryRangeType::UserId).start;
  data.setValue(a, 94, 4);
  size = device().nbWords(Pic::MemoryRangeType::UserId)
    * device().addressIncrement(Pic::MemoryRangeType::UserId);
  data.setValue(size, 98, 4);
  // config start and size
  a = device().range(Pic::MemoryRangeType::Config).start;
  data.setValue(a, 102, 4);
  size = device().nbWords(Pic::MemoryRangeType::Config)
    * device().addressIncrement(Pic::MemoryRangeType::Config);
  data.setValue(size, 106, 4);
  // test memory start and size
  data.setValue(progFamily->testStart, 110, 4);
  data.setValue(progFamily->testSize, 114, 4);
  // test app start, size
  data.setValue(0, 118, 4); // FIXME
  data.setValue(0, 122, 4); // FIXME
  // panel count, size
  uint panelCount = 1;
  if (progFamily->panelSize != 0) panelCount = (codeSize - progFamily->bootSize) / progFamily->panelSize;
  data.setValue(panelCount, 126, 2);
  data.setValue(progFamily->panelSize, 128, 2);
  // config masks
  const Pic::Config& config = device().config();
  for (uint k = 0; k < 20; ++k) {
    if (k < (uint)config._words.size()) data.setValue(config._words[k].bvalue, 130+k, 1);
    else data.setValue(0, 130+k, 1);
  }
  // config masks and blank values
  for (uint k = 0; k < 20; ++k) {
    if (k < (uint)config._words.size() && config._words[k].bvalue != 0) data.setValue(config._words[k].wmask, 150+k, 1);
    else data.setValue(0, 150+k, 1);
  }
  // calibration start
  a = device().range(Pic::MemoryRangeType::Cal).start;
  data.setValue(a, 170, 4);
  // debug config offset, mask, and true value
  uint wordIndex = 0;
  const Pic::Config::Mask* mask = config.findMask("DEBUG", &wordIndex);
  if (mask != NULL) {
    a = wordIndex * device().addressIncrement(Pic::MemoryRangeType::Config);
    data.setValue(a, 174, 4);
    data.setValue(mask->value, 178, 4);
    data.setValue(0, 182, 4);
  } else {
    data.setValue(0, 174, 4);
    data.setValue(0, 178, 4);
    data.setValue(0, 182, 4);
  }
  // minor algo
  data.setValue(family.minorAlgo, 186, 2);
  // config boot start and size
  //QString configBootMaskName = config.protection().maskName(
  //  Pic::Protection::WriteProtected, Pic::MemoryRangeType::Config);
  //mask = config.findMask(configBootMaskName, &wordIndex);
  //if (mask != NULL) {
  //  a += wordIndex * device().addressIncrement(Pic::MemoryRangeType::Config);
  //  data.setValueInc(a, i, 4);
  //  data.setValueInc(0, i, 4); // FIXME
  //} else {
    data.setValue(0, 188, 4);
    data.setValue(0, 192, 4);
  //}
  // debug algo
  data.setValue(family.debugAlgo, 196, 2);
  // header only
  data.setValue(0, 198, 2); // FIXME
  // header device id, select address, and select value
  data.setValue(0, 200, 4); // FIXME
  data.setValue(0, 204, 4); // FIXME
  data.setValue(0, 208, 4); // FIXME
  // header config address
  data.setValue(0, 212, 4); // FIXME
  // header calibration address
  data.setValue(0, 216, 4); // FIXME
  // header debug exec address
  data.setValue(0, 220, 4); // FIXME
  // erase algo 2 (?)
  const Data& ddata = Pickit3::data(device().name());
  data.setValue(ddata.eraseAlgo, 224, 2);
  // device specific flags (?)
  data.setValue(ddata.flags, 226, 4);

  return data;
}

bool Pickit3::Hardware::sendConfigPacket()
{
  ByteArray out = createConfigPacket();
  ByteArray in;
  ByteArray data(2, 0x0, PrintAlphaNum);
  data.setValue(out.size(), 0, 2);
  return commandWithSimpleResponse(0x40, in, &data) && sendBulk(out);
}

bool Pickit3::Hardware::setPower(bool on, bool targetSelfPowered)
{
  if (on) _powerOptions |= PowerOn;
  else _powerOptions &= ~PowerOn;
  if (!on || targetSelfPowered) _powerOptions &= ~UseProgrammerPower;
  else _powerOptions |= UseProgrammerPower;
  return setPower(_powerOptions);
}

bool Pickit3::Hardware::setTargetReset(Device::ResetMode mode)
{
  if (mode == Device::ResetMode::Held) _powerOptions |= HoldReset;
  else _powerOptions &= ~HoldReset;
  return setPower(_powerOptions);
}

bool Pickit3::Hardware::setPower(PowerOptions options)
{
  ByteArray in;
  ByteArray out(6, 0x0, PrintEscapeAll);
  out.setValue(0x04, 0, 2);
  out.setValue(uint(options), 2, 2);
  double vdd = device().architecture().data().nominalVdd;
  out.setValue(vdd * 8, 4, 2);
  return commandWithComplexResponse(0x63, "setPower", in, &out);
}

bool Pickit3::Hardware::connectToDevice(BitValue& deviceId)
{
  if (!beginBracket()) return false;
  ByteArray in;
  if (!commandWithComplexResponse(0x70, "connectToDevice", in)) return false;
  deviceId = in.getValue(0, 4);
  return endBracket();
}

bool Pickit3::Hardware::beginBracket()
{
  ByteArray in;
  ByteArray out(2, 0x0, PrintEscapeAll);
  out[0] = 0x01;
  return commandWithComplexResponse(0xA0, "beginBracket", in, &out);
}

bool Pickit3::Hardware::endBracket()
{
  ByteArray in;
  ByteArray out(2, 0x0, PrintEscapeAll);
  return commandWithComplexResponse(0xA0, "endBracket", in, &out);
}

bool Pickit3::Hardware::readVoltages(Device::VoltageValues& voltages)
{
  ByteArray in;
  if (!commandWithSimpleResponse(0x20, in)) return false;
  voltages[Device::VoltageType::ProgrammerVpp] = Device::VoltageValue(Device::VoltageValue::Normal, double(in.getValue(4, 2)) / 1000);
  //voltages[Device::VoltageType::ProgrammerVdd] = Device::VoltageValue(Device::VoltageValue::Normal, double(in.getValue(6, 2)) / 1000);
  voltages[Device::VoltageType::TargetVdd] = Device::VoltageValue(Device::VoltageValue::Normal, double(in.getValue(8, 2)) / 1000);
  return true;
}

bool Pickit3::Hardware::setDebugOption()
{
  ByteArray out(10, 0x00, PrintEscapeAll);
  out.setValue(0x08, 0, 2);
  ByteArray in;
  return commandWithSimpleResponse(0x93, in, &out);
}

ByteArray Pickit3::Hardware::createOpDescriptor(ProgOptions options) const
{
  ByteArray data(0x42, 0x00, PrintEscapeAll);
  data.setValue(0x40, 0, 2);
  data.setValue(uint(options), 6, 4);
  return data;
}

void Pickit3::Hardware::addMemRangeToOpDescriptor(Pic::MemoryRangeType type, uint offset, uint size, ByteArray& data)
{
  ProgRegion progRegion = getProgRegion(type);
  BitValue v = data.getValue(2, 4);
  v |= progRegion.data().opType;
  data.setValue(v, 2, 4);
  uint i = 10 + progRegion.type() * 8;
  data.setValue(offset, i, 4);
  data.setValue(offset + size - 1, i+4, 4);
}

void Pickit3::Hardware::addMemRangeToOpDescriptor(Pic::MemoryRangeType type, ByteArray& data)
{
  uint size = device().nbWords(type) * device().nbBytesWord(type);
  addMemRangeToOpDescriptor(type, 0, size, data);
}

bool Pickit3::Hardware::eraseRange(Pic::MemoryRangeType type)
{
  ProgOptions options = NoProgOption;
  ByteArray out = createOpDescriptor(options);
  addMemRangeToOpDescriptor(type, out);
  ByteArray in;
  if (!commandWithSimpleResponse(0x50, in, &out)) return false;
  if (!beginBracket()) return false;
  if (!commandWithComplexResponse(0x53, "erase", in)) return false;
  return endBracket();
}

bool Pickit3::Hardware::eraseAll()
{
  ProgOptions options = NoProgOption;
  ByteArray out = createOpDescriptor(options);
  addMemRangeToOpDescriptor(Pic::MemoryRangeType::Code, out);
  addMemRangeToOpDescriptor(Pic::MemoryRangeType::Eeprom, out);
  addMemRangeToOpDescriptor(Pic::MemoryRangeType::UserId, out);
  addMemRangeToOpDescriptor(Pic::MemoryRangeType::Config, out);
  ByteArray in;
  if (!commandWithSimpleResponse(0x50, in, &out)) return false;
  if (!beginBracket()) return false;
  if (!commandWithComplexResponse(0x53, "erase", in)) return false;
  return endBracket();
}

bool Pickit3::Hardware::getMem(Pic::MemoryRangeType type, uint offset, uint size, ByteArray& in)
{
  ByteArray out(0x0A, 0x00, PrintEscapeAll);
  ProgRegion progRegion = getProgRegion(type);
  out.setValue(progRegion.data().memType, 0, 2);
  out.setValue(offset, 2, 4);
  out.setValue(size, 6, 4);
  in.resize(size);
  return commandWithComplexResponse(0x61, "getMem", in, &out);
}

bool Pickit3::Hardware::read(Pic::MemoryRangeType type, uint wordOffset, Device::Array& data, const ::Programmer::VerifyData *vdata)
{
  if (!setDebugOption()) return false;
  if (!beginBracket()) return false;
  uint nbBytesWord = device().nbBytesWord(type);
  uint offset = wordOffset * nbBytesWord;
  uint nbBytes = data.size() * nbBytesWord;
  for (uint i = offset; i < nbBytes; i += 0x600) {
    uint size = std::min(uint(0x600), nbBytes - i);
    ProgOptions options = NoProgOption;
    ByteArray out = createOpDescriptor(options);
    addMemRangeToOpDescriptor(type, i, size, out);
    ByteArray in;
    if (!commandWithSimpleResponse(0x50, in, &out)) return false;
    if (!commandWithComplexResponse(0x54, "read", in)) return false;
    if (!getMem(type, i, size, in)) return false;
    for (uint k = 0; k < size; k += nbBytesWord) {
      uint index = (i + k) / nbBytesWord;
      data[index] = in.getValue(k, nbBytesWord);
      if (vdata != NULL) {
        if ( !verifyWord(index, data[index], type, *vdata) ) return false;
      }
    }
    if ( type==Pic::MemoryRangeType::Code || type==Pic::MemoryRangeType::Eeprom )
      _base.progressMonitor().addTaskProgress(size / nbBytesWord);
  }
  return endBracket();
}

bool Pickit3::Hardware::setMem(Pic::MemoryRangeType type, uint offset, const ByteArray& out)
{
  ByteArray in;
  ByteArray data(0x0A, 0x00, PrintEscapeAll);
  ProgRegion progRegion = getProgRegion(type);
  data.setValue(progRegion.data().memType, 0, 2);
  data.setValue(offset, 2, 4);
  data.setValue(out.size(), 6, 4);
  return commandWithSimpleResponse(0x60, in, &data) && sendBulk(out);
}

bool Pickit3::Hardware::write(Pic::MemoryRangeType type, uint wordOffset, const Device::Array &data)
{
  if (!setDebugOption()) return false;
  if (!beginBracket()) return false;
  uint nbBytesWord = device().nbBytesWord(type);
  uint offset = wordOffset * nbBytesWord;
  uint nbBytes = data.size() * nbBytesWord;
  for (uint i = offset; i < nbBytes; i += 0x600) {
    uint size = std::min(uint(0x600), nbBytes - i);
    ProgOptions options = DoNotVerify;
    ByteArray out = createOpDescriptor(options);
    addMemRangeToOpDescriptor(type, i, size, out);
    ByteArray in;
    if (!commandWithSimpleResponse(0x50, in, &out)) return false;
    out.resize(size);
    for (uint k = 0; k < size; k += nbBytesWord) {
      uint index = (i + k) / nbBytesWord;
      out.setValue(data[index], k, nbBytesWord);
    }
    if (!setMem(type, i, out)) return false;
    if ( type==Pic::MemoryRangeType::Code || type==Pic::MemoryRangeType::Eeprom )
      _base.progressMonitor().addTaskProgress(size / nbBytesWord);
    ByteArray dummy;
    if (!commandWithComplexResponse(0x51, "program", dummy)) return false;
  }
  return endBracket();
}

bool Pickit3::Hardware::uploadAP(const Pic::Memory&)
{
  // ### FIXME
  log(Log::LineType::Error, "Uploading firmware not supported yet");
  return false;
}
