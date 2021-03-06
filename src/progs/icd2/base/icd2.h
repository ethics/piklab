/***************************************************************************
 *   Copyright (C) 2005 Nicolas Hadacek <hadacek@kde.org>                  *
 *   Copyright (C) 2002-2003 Stephen Landamore <stephen@landamore.com>     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef ICD2_H
#define ICD2_H

#include "icd.h"

namespace Icd2
{
//-----------------------------------------------------------------------------
DECLARE_ENUM_DATA(Device::TargetMode, uchar, TARGET_MODE_VALUES)
DECLARE_ENUM_DATA(Device::ResetMode, const char *, RESET_MODE_VALUES)

class TestData {
public:
  BEGIN_DECLARE_ENUM(VoltageType)
    TargetVdd = 0, ModuleVpp, MclrGround, MclrVdd, MclrVpp
  END_DECLARE_ENUM_LABEL_ONLY(VoltageType)

public:
  TestData();
  TestData(const QString &rx);
  bool pass() const;
  QString result(VoltageType type) const;
  QString pretty(VoltageType type) const;

private:
  EnumVector<VoltageType, int> _voltages;
};

DECLARE_ENUM_DATA(::Programmer::ResultType, int, RESULT_TYPE_VALUES)

//-----------------------------------------------------------------------------
class Hardware : public Icd::Hardware
{
public:
  Hardware(::Programmer::Base &base, Port::Base *port) : Icd::Hardware(base, port) {}
  bool command(const QString &command, uint responseSize);

// initialization
  virtual bool uploadFirmware(const Pic::Memory &memory);
  virtual bool setTarget();
  bool setup();

// status
  virtual bool getFirmwareVersion(VersionData &version);
  uint getFirmwareId();
  bool getDebugExecVersion(VersionData &version);
  virtual bool setTargetPowerOn(bool on);
  virtual bool readVoltage(Device::VoltageType type, double &value);
  virtual bool readVoltages(Device::VoltageValues &voltages);
  virtual bool getTargetMode(Device::TargetMode &mode);
  virtual bool setTargetReset(Device::ResetMode mode);
  bool selfTest(TestData &test);

// programming
  virtual bool readMemory(Pic::MemoryRangeType type, uint wordOffset, Device::Array &data, const ::Programmer::VerifyData *vdata);
  virtual bool writeMemory(Pic::MemoryRangeType type, uint wordOffset, const Device::Array &data);
  virtual bool eraseAll();
  bool setWriteMode(Device::WriteMode mode);

// debugging
  virtual bool readRegister(Address address, BitValue &value, uint nbBytes);
  virtual bool writeRegister(Address address, BitValue value, uint nbBytes);
  virtual bool resumeRun();
  virtual bool step();
  virtual bool haltRun();
  virtual BitValue getProgramCounter();

protected:
  virtual QString receivedData() const { return _rx.mid(5, _rx.length()-8); }

private:
  struct VoltageTypeData {
    const char *command;
    double factor;
  };
  DECLARE_ENUM_DATA(Device::VoltageType, VoltageTypeData, VOLTAGE_TYPE_DATA)
  DECLARE_ENUM_DATA(Device::WriteMode, const char *, WRITE_MODE_VALUES)

  bool sendCommand(const QString &command);
  bool receiveResponse(const QString &command, uint responseSize, bool poll);
  bool readBlock(uint nbBytesWord, uint nbWords, Device::Array &data);
  bool writeBlock(uint nbBytesWord, const Device::Array &data, uint wordOffset, uint nbWords);
  const char *readCommand(Pic::MemoryRangeType type) const;
  const char *writeCommand(Pic::MemoryRangeType type) const;
};

} // namespace

#endif
