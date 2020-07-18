/***************************************************************************
 *   Copyright (C) 2005-2010 Nicolas Hadacek <hadacek@kde.org>             *
 *   Copyright (C) 2002-2003 Stephen Landamore <stephen@landamore.com>     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "icd2.h"

#include "common/common/misc.h"
#include "common/port/port_base.h"
#include "icd2_data.h"
#include "icd2_usb.h"

//-----------------------------------------------------------------------------
const Icd2::TARGET_MODE_VALUES::Data Icd2::TARGET_MODE_VALUES::DATA[Nb_Types] = {
  0x00, // stopped
  0x01, // running
  0x02  // in programming
};

const Icd2::RESET_MODE_VALUES::Data Icd2::RESET_MODE_VALUES::DATA[Nb_Types] = {
  "00", // reset held
  "01"  // reset released
};

//-----------------------------------------------------------------------------
const Icd2::RESULT_TYPE_VALUES::Data Icd2::RESULT_TYPE_VALUES::DATA[Nb_Types] = {
  0x00, // pass
  0x01, // low
  0x80, // high
  -2,   // fail (unused)
};

const Icd2::TestData::VoltageType::Data Icd2::TestData::VoltageType::DATA[Nb_Types] = {
  { I18N_NOOP("Target Vdd")   },
  { I18N_NOOP("Module Vpp")   },
  { I18N_NOOP("MCLR ground") },
  { I18N_NOOP("MCLR Vdd")     },
  { I18N_NOOP("MCLR Vpp")     }
};

Icd2::TestData::TestData()
{
  FOR_EACH(VoltageType, type) _voltages[type] = -1;
}

Icd2::TestData::TestData(const QString &rx)
{
  FOR_EACH(VoltageType, type) _voltages[type] = fromHex(rx.mid(5 + 2*type.type(), 2), 0);
}

bool Icd2::TestData::pass() const
{
  FOR_EACH(VoltageType, type)
    if ( _voltages[type]!=RESULT_TYPE_VALUES::data(::Programmer::ResultType::Pass) ) return false;
  return true;
}

QString Icd2::TestData::result(VoltageType type) const
{
  if ( _voltages[type]==-1 ) return "---";
  FOR_EACH(::Programmer::ResultType, rtype)
    if ( _voltages[type]==RESULT_TYPE_VALUES::data(rtype) ) return rtype.label();
  return toHex(_voltages[type], 2);
}

QString Icd2::TestData::pretty(VoltageType type) const
{
  return type.label() + "=" + result(type);
}

//-----------------------------------------------------------------------------
const Icd2::Hardware::VOLTAGE_TYPE_DATA::Data Icd2::Hardware::VOLTAGE_TYPE_DATA::DATA[Nb_Types] = {
 { "37", 0.07988 }, // icd Vpp
 { "34", 0.03908 }, // target Vdd
 { "35", 0.07988 }, // target Vpp
};

const Icd2::Hardware::WRITE_MODE_VALUES::Data Icd2::Hardware::WRITE_MODE_VALUES::DATA[Nb_Types] = {
  "00", // write only
  "01"  // erase then write
};

//-----------------------------------------------------------------------------
const char *Icd2::Hardware::readCommand(Pic::MemoryRangeType type) const
{
  switch (type.type()) {
    case Pic::MemoryRangeType::Code:                           return "47";
    case Pic::MemoryRangeType::Eeprom:                         return "48";
    case Pic::MemoryRangeType::Config:
    case Pic::MemoryRangeType::CalBackup:
      if (device().architecture()==Pic::Architecture::P18J)    return "47";
      else                                                     return "49";
    case Pic::MemoryRangeType::UserId:
      if (device().architecture() == Pic::Architecture::P33F)  return "49";
      else                                                     return "4A";
    case Pic::MemoryRangeType::DeviceId:
      if ( device().is16bitFamily() )                          return "49";
      else                                                     return "4A";
    case Pic::MemoryRangeType::Cal:
      if (device().isCalibrationAtCodeEnd())                   return "47";
      else                                                     return "49";
    case Pic::MemoryRangeType::DebugVector:                    return "40";
    case Pic::MemoryRangeType::ProgramExecutive:
    case Pic::MemoryRangeType::HardwareStack:
    case Pic::MemoryRangeType::Nb_Types:                       break;
  }
  ASSERT(false);
  return NULL;
}

const char *Icd2::Hardware::writeCommand(Pic::MemoryRangeType type) const
{
  switch (type.type()) {
    case Pic::MemoryRangeType::Code:                           return "43";
    case Pic::MemoryRangeType::Eeprom:                         return "44";
    case Pic::MemoryRangeType::Config:
    case Pic::MemoryRangeType::CalBackup:
      if (device().architecture()==Pic::Architecture::P18J
	  || device().architecture()==Pic::Architecture::P24F) return "43";
      else                                                     return "45";
    case Pic::MemoryRangeType::UserId:
      if (device().architecture() == Pic::Architecture::P33F)  return "45";
      else                                                     return "46";
    case Pic::MemoryRangeType::DeviceId:                       break;
    case Pic::MemoryRangeType::Cal:
      if (device().isCalibrationAtCodeEnd())                   return "43";
      else                                                     return "45";
    case Pic::MemoryRangeType::DebugVector:                    return "41";
    case Pic::MemoryRangeType::ProgramExecutive:
    case Pic::MemoryRangeType::HardwareStack:
    case Pic::MemoryRangeType::Nb_Types:                       break;
  }
  ASSERT(false);
  return NULL;
}

bool Icd2::Hardware::uploadFirmware(const Pic::Memory &memory)
{
  if ( !internalConnect("V") ) return false;
  log(Log::LineType::Information, "  Uploading firmware to ICD2...");
  uint start = 0x0004, size = 0x1BFB;
  QString cmd = "10" + toHex(start, 4) + toHex(size, 4);
  if ( !command(cmd, 0) ) return false;
  uint nbBytesWord = memory.device().nbBytesWord(Pic::MemoryRangeType::Code); // should be 2 for 16F876
  Device::Array data = memory.arrayForWriting(Pic::MemoryRangeType::Code);
  if ( !writeBlock(nbBytesWord, data, start, size) ) return false;
  if ( !receiveResponse(cmd, 0, false) ) return false;
  if ( !internalConnect("U") ) return false;
  return true;
}

bool Icd2::Hardware::setTarget()
{
  log(Log::DebugLevel::Normal, "  Set target");
  // set target family
  const Icd2::Data &d = data(device().name());
  if ( !command(QString("2A") + toHex(d.famid, 2), 0) ) return false;
  // set code range end
  Address end = device().range(Pic::MemoryRangeType::Code).end;
  if (device().isCalibrationAtCodeEnd()) end += device().addressIncrement(Pic::MemoryRangeType::Code);
  if (!command("06" + toHex(end, 6), 0)) return false;
  return true;
}

bool Icd2::Hardware::setup()
{
  // ??
  if ( device().architecture()==Pic::Architecture::P30F
      || device().architecture()==Pic::Architecture::P24F)
    if ( !command("0900", 0) ) return false;

  // ??
  _port->send("$7F00\x0D", 6);
  QString s;
  if ( !_port->receive(2, s) ) return false;
  if ( s!="02" ) {
    log(Log::LineType::Error, i18n("Unexpected answer ($7F00) from ICD2 (%1).").arg(s));
    return false;
  }

  // ??
  if ( !command("08", 2) ) return false;
  if ( _rx.mid(5, 2)!="00" ) {
    log(Log::LineType::Error, i18n("Unexpected answer (08) from ICD2 (%1).").arg(_rx));
    return false;
  }

  return !hasError();
}

bool Icd2::Hardware::sendCommand(const QString &s)
{
  //format: <LLXX....CC>
  QString cs = s.upper();
  QString tx = "<";
  tx += toHex(cs.length() + 6, 2);
  tx += cs;
  uchar chk = tx[1].latin1() + tx[2].latin1();
  for (uint i=0; i<uint(s.length()); i++) chk += cs[i].latin1();
  tx += toHex(chk, 2);
  tx += '>';
  log(Log::DebugLevel::Extra, QString("send command: '%1'").arg(tx));
  QByteArray a = toAscii(tx);
  return _port->send(a.data(), a.count());
}

bool Icd2::Hardware::receiveResponse(const QString &command, uint responseSize, bool poll)
{
  // format: [LLXX...CC]
  uint size = responseSize + 8;
  if ( poll && _port->type()==PortType::USB ) {
    if ( !static_cast<USBPort *>(_port)->poll(size, _rx) ) return false;
  } else if ( !_port->receive(size, _rx, 180000) ) return false; // is 3 minutes enough ?? (we should really have an abort button here...)
  log(Log::DebugLevel::Extra, QString("received answer: '%1'").arg(_rx));
  const uint receivedSize = fromHex(_rx.mid(1, 2), 0);
  if ( size!=receivedSize) {
    log(Log::LineType::Error, i18n("Received length mismatch: received %1, expected: %2.").arg(receivedSize).arg(size));
    return false;
  }
  if ( uint(_rx.length())!=size ) {
    log(Log::LineType::Error, i18n("Received string too short."));
    return false;
  }
  if ( _rx[0]!='[' || _rx[size-1]!=']' ) {
    log(Log::LineType::Error, i18n("Malformed string received \"%1\"").arg(_rx));
    return false;
  }
  if ( command.mid(0, 2)!=_rx.mid(3, 2) ) {
    log(Log::LineType::Error, i18n("Wrong return value (\"%1\"; was expecting \"%2\")")
             .arg(_rx.mid(3, 2)).arg(command.mid(0, 2)));
    return false;
  }
  // verify the checksum
  uchar chk = 0;
  for (uint i=1; i<size-3; i++) chk += _rx[i].latin1();
  if ( chk!=fromHex(_rx.mid(size-3, 2), 0) ) {
    log(Log::LineType::Error, i18n("Bad checksum for received string"));
    return false;
  }
  return true;
}

bool Icd2::Hardware::command(const QString &command, uint responseSize)
{
  if ( hasError() ) return false;
  if ( !sendCommand(command) ) return false;
  if ( !receiveResponse(command, responseSize, false) ) return false;
  return true;
}

bool Icd2::Hardware::getFirmwareVersion(VersionData &version)
{
  if ( !command("01", 6) ) return false;
  version = VersionData::fromHexString(_rx.mid(5, 6));
  return true;
}

uint Icd2::Hardware::getFirmwareId()
{
  if ( !command("07", 2) ) return 0;
  return fromHex(_rx.mid(5, 2), 0);
}

bool Icd2::Hardware::getDebugExecVersion(VersionData &version)
{
  if ( !command("04", 6) ) return false;
  version = VersionData::fromHexString(_rx.mid(5, 6));
  return true;
}

bool Icd2::Hardware::setTargetPowerOn(bool on)
{
  if (on) {
    const double vdd = device().architecture().data().nominalVdd;
    if (vdd != 5.0) {
      log(Log::LineType::Warning, i18n("Set target power disabled because device does not support 5V supply (nomial vdd is %1V).").arg(vdd));
      return true;
    }
  }
  return command(QString("05") + (on ? "FF" : "00"), 0);
}

bool Icd2::Hardware::readVoltage(Device::VoltageType type, double &value)
{
  if ( !command(VOLTAGE_TYPE_DATA::data(type).command, 2) ) return false;
  value = VOLTAGE_TYPE_DATA::data(type).factor * fromHex(_rx.mid(5, 2), 0);
  return true;
}

bool Icd2::Hardware::readVoltages(Device::VoltageValues &voltages)
{
  FOR_EACH(Device::VoltageType, type) {
    double value;
    if ( !readVoltage(type, value) ) return false;
    voltages[type.type()] = Device::VoltageValue(Device::VoltageValue::Normal, value);
  }
  return true;
}

bool Icd2::Hardware::getTargetMode(Device::TargetMode &tmode)
{
  if ( !command("2C", 2) ) return false;
  uchar v = fromHex(_rx.mid(5, 2), 0);
  FOR_EACH(Device::TargetMode, mode) {
    if ( v!=TARGET_MODE_VALUES::data(mode) ) continue;
    tmode = mode;
    return true;
  }
  ASSERT(false);
  return false;
}

bool Icd2::Hardware::setTargetReset(Device::ResetMode mode)
{
  return command(QString("33") + RESET_MODE_VALUES::data(mode), 0);
}

bool Icd2::Hardware::selfTest(TestData &test)
{
  if ( !command("02", 10) ) return false;
  test = TestData(_rx);
  return true;
}

bool Icd2::Hardware::readBlock(uint nbBytesWord, uint nbWords, Device::Array &data)
{
  //qDebug("readBlock %i %s", nbBytesWord, toHex(nbWords, 8).data());
  // receive data
  uint length = 2*nbBytesWord*nbWords+4;
  QString s;
  uint i = 0;
  while ( i<length ) {
    uint maxSize = (_port->type()==PortType::Serial ? 2*nbBytesWord : 0x100);
    uint size = QMIN(maxSize, length-i);
    QString tmp;
    if ( _port->type()==PortType::USB ) {
      if ( !static_cast<USBPort *>(_port)->dataReceive(size, tmp) ) return false;
    } else if ( !_port->receive(size, tmp) ) return false;
    s += tmp;
    i += size;
  }

  // treat data
  log(Log::DebugLevel::Max, QString("received %1 * %2: %3").arg(nbBytesWord).arg(nbWords).arg(s));
  if ( s[0]!='{' || s[s.length()-1]!='}' ) {
    log(Log::LineType::Error, i18n("Invalid begin or end character for read block."));
    return false;
  }
  data.resize(nbWords);
  Q_UINT8 chk = 0;
  for (uint i=0; i<nbWords; i++) {
    QString ts = s.mid(1+2*nbBytesWord*i, 2*nbBytesWord);
    //if ( i<10 ) qDebug("%i: %s", i, ts.data());
    data[i] = 0;
    for (int k=nbBytesWord-1; k>=0; k--) {
      data[i] = data[i] << 8;
      data[i] |= fromHex(ts.mid(2*k, 2), 0);
      chk += ts[2*k].latin1() + ts[2*k+1].latin1();
    }
  }

  QString cs = s.mid(s.length()-3, 2);
  if ( chk!=fromHex(cs, 0) ) {
    log(Log::LineType::Error, i18n("Bad checksum for read block: %1 (%2 expected).").arg(cs).arg(toHex(chk, 2)));
    return false;
  }
  return true;
}

bool Icd2::Hardware::readMemory(Pic::MemoryRangeType type, uint wordOffset,
                                Device::Array &data, const ::Programmer::VerifyData *vdata)
{
  const char *r = readCommand(type);
  if ( r==0 ) return false;
  uint nbBytesWord = device().nbBytesWord(type);
  uint div = 2;
  if ( type==Pic::MemoryRangeType::Eeprom || nbBytesWord>=2 ) div = 1;
  uint inc = device().addressIncrement(type);
  Address start = device().range(type).start; // address
  uint todo = inc * data.count(); // address
  uint offset = inc * wordOffset; // address
  //qDebug("read size=%s div=%i nbBytes=%i", toHex(size, 8).data(), div, nbBytesWord);
  data.resize(0);
  do {
    uint size = QMIN(todo, uint(0x1000)); // addresses
    uint nb = size / inc; // word
    //qDebug("read %s start=%s size=%s", Pic::MEMORY_RANGE_TYPE_DATA[type].label, toHex(start+offset, 8).data(), toHex(nb, 8).data());
    QString cmd = r + toHex(start+offset, 8) + toHex(nb/div, 8);
    if ( !command(cmd, 0) ) return false;
    Device::Array pdata;
    if ( !readBlock(nbBytesWord, nb, pdata) ) return false;
    if ( !receiveResponse(cmd, 0, false) ) return false;
    if (vdata) {
      for (uint i=0; i<pdata.count(); i++)
        if ( !verifyWord(wordOffset+data.count()+i, pdata[i], type, *vdata) ) return false;
    }
    data += pdata;
    offset += size;
    todo -= size;
    if ( type==Pic::MemoryRangeType::Code || type==Pic::MemoryRangeType::Eeprom )
      _base.progressMonitor().addTaskProgress(nb);
  } while ( todo!=0 );
  return true;
}

bool Icd2::Hardware::writeBlock(uint nbBytesWord, const Device::Array &data, uint wordIndex, uint nbWords)
{
  log(Log::DebugLevel::Extra, QString("writeBlock offset:%1 nbWords:%2 (size: %3)").arg(toHex(wordIndex, 8)).arg(toHex(nbWords, 8)).arg(toHex(data.size(), 8)));
  ASSERT( wordIndex+nbWords<=data.size() );
  // prepare data
  QString s = "{";
  uchar chk = 0;
  for (uint i=0; i<nbWords; i++) {
    QString ts = toHex(data[wordIndex+i], 2*nbBytesWord);
    for (int k=nbBytesWord-1; k>=0; k--) {
      //if ( i<10 || i>=nbWords-10 ) qDebug("send: %i-%i %s", i, k, ts.mid(2*k, 2).data());
      s += ts.mid(2*k, 2);
      chk += ts[2*k].latin1() + ts[2*k+1].latin1();
    }
  }
  s += toHex(chk, 2);
  s += "}";
  log(Log::DebugLevel::Max, "send: " + s);

  // send data
  uint i = 0;
  while ( i<uint(s.length()) ) {
    uint maxSize = (_port->description().type==PortType::Serial ? 2*nbBytesWord : 0x100);
    if ( _port->description().type==PortType::Serial && i==0 ) maxSize = 1;
    uint size = QMIN(maxSize, s.length()-i);
    QByteArray a = toAscii(s);
    if ( _port->type()==PortType::USB ) {
      if ( !static_cast<USBPort *>(_port)->dataSend(a.data()+i, size) ) return false;
    } else if ( !_port->send(a.data()+i, size) ) return false;
    i += size;
  }

  //qDebug("done sending %i words (chk=%s)", nbWords, toHex(chk, 2).data());
  return true;
}

bool Icd2::Hardware::writeMemory(Pic::MemoryRangeType type, uint wordOffset, const Device::Array &data)
{
  //qDebug("write memory: offset:%s nbWords:%s (size: %s)", toHex(wordOffset, 4).data(), toHex(nbWords, 4).data(), toHex(data.size(), 4).data());
  const char *w = writeCommand(type);
  if ( w==0 ) return true;
  uint nbBytesWord = device().nbBytesWord(type);
  uint div = 2;
  if ( type==Pic::MemoryRangeType::Eeprom || nbBytesWord>=2 ) div = 1;
  uint inc = device().addressIncrement(type);
  Address start = device().range(type).start; // address
  uint todo = inc * data.count(); // address
  uint offset = inc * wordOffset; // address
  uint index = 0;
  //qDebug("write todo=%s div=%i nbBytes=%i dataSize=%i", toHex(todo, 8).data(), div, nbBytesWord, data.size());
  do {
    uint size = QMIN(todo, uint(0x1000)); // address
    uint nb = size / inc; // word
    //qDebug("write %s start=%s nbWords=%s", Pic::MEMORY_RANGE_TYPE_DATA[type].label, toHex(start+offset, 8).data(), toHex(nb, 8).data());
    QString cmd = w + toHex(start+offset+index, 8) + toHex(nb/div, 8);
    if ( !command(cmd, 0) ) return false;
    if ( !writeBlock(nbBytesWord, data, index/inc, nb) ) return false;
    if ( !receiveResponse(cmd, 0, false) ) return false;
    index += size;
    todo -= size;
    if ( type==Pic::MemoryRangeType::Code || type==Pic::MemoryRangeType::Eeprom )
      _base.progressMonitor().addTaskProgress(nb);
  } while ( todo!=0 );
  return true;
}

bool Icd2::Hardware::eraseAll()
{
  setTargetReset(Device::ResetMode::Held);
  if ( hasError() ) return false;
  if ( !sendCommand("29") ) return false;
  if ( !receiveResponse("29", 0, true) ) return false; // poll
  if (device().architecture() == Pic::Architecture::P33F
      || device().architecture() == Pic::Architecture::P24F) {
    if (!command("25", 0)) return false;
  }
  return true;
}

bool Icd2::Hardware::haltRun()
{
  return command("2E", 0);
}

bool Icd2::Hardware::step()
{
  return command("2F", 0);
}

bool Icd2::Hardware::resumeRun()
{
  return command("30", 0);
}

bool Icd2::Hardware::setWriteMode(Device::WriteMode mode)
{
  return command(QString("4B") + WRITE_MODE_VALUES::data(mode), 0);
}

bool Icd2::Hardware::writeRegister(Address address, BitValue value, uint nbBytes)
{
  QString cmd = "1B" + toHex(address, 8) + toHex(nbBytes, 8);
  if ( !command(cmd, 0) ) return false;
  Device::Array data(nbBytes);
  for (uint i=0; i<nbBytes; i++) data[nbBytes-i-1] = value.byte(i);
  if ( !writeBlock(1, data, 0, nbBytes) ) return false;
  return receiveResponse(cmd, 0, false);
}

bool Icd2::Hardware::readRegister(Address address, BitValue &value, uint nbBytes)
{
  QString cmd = "1E" + toHex(address, 8) + toHex(nbBytes, 8);
  if ( !command(cmd, 0) ) return false;
  Device::Array data;
  if ( !readBlock(1, nbBytes, data) ) return false;
  if ( !receiveResponse(cmd, 0, false) ) return false;
  value = 0;
  for (uint i=0; i<nbBytes; i++) {
    value <<= 8;
    value += data[i];
  }
  return true;
}

BitValue Icd2::Hardware::getProgramCounter()
{
  if ( !command("3D", 8) ) return 0;
  return fromHex(_rx.mid(5, 8), 0);
}
