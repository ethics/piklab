/***************************************************************************
 *   Copyright (C) 2012 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef PICKIT3_H
#define PICKIT3_H

#include "common/port/usb_port.h"
#include "devices/pic/prog/pic_prog.h"

namespace Pickit3
{

enum { BUF_SIZE = 64 };

struct SystemStatus {

  enum MajorValue {
    MajInvalid       = 0x0,
    MajInitPass      = 0x1,
    MajTgtProgReady  = 0x2,
    MajTgtDebugReady = 0x3,
    MajExtIRQ        = 0x4,
    MajRun           = 0x5,
    MajDevOp         = 0x6,
    MajHWError       = 0x7,
    MajFWError       = 0x8
  };
  enum Init0Value {
    Init              = 0x0,
    //                  0x1
    Fail175           = 0x2,
    MemFail           = 0x3,
    FPGAFail          = 0x4,
    //                  0x5
    APNotFound        = 0x7,
    APOld             = 0x8,
    RSOld             = 0x9,
    FPGAOld           = 0xA,
    MainSerialFail    = 0xB,
    DriverSerFail     = 0xC,
    DACFail           = 0xD,
    FailTrigger       = 0xE,
    ClockSwitch       = 0xF,
    DriverBoardFail   = 0x10,
    FailHWRev         = 0x11,
    DriverBoardHWFail = 0x12,
    IOTestFailed      = 0x13,
    DrvIOExpander     = 0x14,
    NoDebugComm       = 0x15,
    SelfTestBoard     = 0x16,
    DPRAMTestFail     = 0x17,
    AdditionTest      = 0x18,
    AlgBootLoader     = 0x19
  };
  enum Warn0value {
    BadCmd       = 0x0,
    TgtVdd       = 0x1,
    TgtConnected = 0x2,
    TgtWrong     = 0x3,
    TgtResync    = 0x4,
    TgtHeldReset = 0x5,
    PowerChanged = 0x6
  };
  enum Warn1Value {
    TriggerAssert       = 0x0,
    PushButton0         = 0x1,
    PushButton1         = 0x2,
    VddOvercurrent      = 0x3,
    NoMemObject         = 0x4,
    MemObjectCorrupt    = 0x5,
    NoProgExec          = 0x6,
    NoDebugExec         = 0x7,
    VppNotSettable      = 0x9,
    VddNotSettable      = 0xA,
    ClckDataOverCurrent = 0xB,
    VppOverCurrent      = 0xC,
    NoDSS               = 0xD,
    ResetReceived       = 0xE,
    DMATest             = 0xF,
    SelfTestPassed      = 0x10,
    UnknownException    = 0x1F
  };
  enum Mode0Value {
    ProcessCmd    = 0x0,
    EnteringDebug = 0x1,
    DebugMode     = 0x2,
    Halted        = 0x3,
    Run           = 0x4,
    UsingProgExec = 0x9,
    AppInFull     = 0xA,
    ProgramDebug  = 0x1E,
    ProgramGoto   = 0x1F

  };
  enum Mode1Value {
    StartDevOp      = 0x0,
    ErasingProg     = 0x1,
    ErasingDataEE   = 0x2,
    ErasingConfigs  = 0x3,
    ErasingIds      = 0x4,
    ErasingTest     = 0x5,
    ErasingProgExec = 0x6,
    BlankProg       = 0x7,
    BlankDataEE     = 0x8,
    BlankConfigs    = 0x9,
    BlankIds        = 0xA,
    BlankTest       = 0xB,
    BlankProgExec   = 0xC,
    ProgramProg     = 0xD,
    ProgramDataEE   = 0xE,
    ProgramConfigs  = 0xF,
    ProgramIds      = 0x10,
    ProgramTest     = 0x11,
    ProgramProgExec = 0x12,
    VerifyProg      = 0x13,
    VerifyDataEE    = 0x14,
    VerifyConfigs   = 0x15,
    VerifyIds       = 0x16,
    VerifyTest      = 0x17,
    VerifyProgExec  = 0x18,
    ReadProg        = 0x19,
    ReadDataEE      = 0x1A,
    ReadConfigs     = 0x1B,
    ReadIds         = 0x1C,
    ReadTest        = 0x1D,
    ReadProgExec    = 0x1E,
    CompDevOp       = 0x1F
  };

  SystemStatus();
  QString pretty() const;

  uint _major;
  uint _init0;
  uint _init1;
  uint _warn0;
  uint _warn1;
  uint _mode0;
  uint _mode1;
};

enum PowerOption {
  NoPowerOption            = 0,
  UseProgrammerPower       = 1,
  PowerOn                  = 2,
  HoldReset                = 4,
  UsePic24HighVoltageEntry = 8
};
Q_DECLARE_FLAGS(PowerOptions, PowerOption)
Q_DECLARE_OPERATORS_FOR_FLAGS(PowerOptions)

struct ProgRegionData {
  Pic::MemoryRangeType type;
  uint                 opType;
  uint                 memType;
};
BEGIN_DECLARE_ENUM(ProgRegion)
Code = 0, Eeprom, UserIds, Config, Test, OscCal, ProgExec, DebugExec, BootConfig
END_DECLARE_ENUM_DATA_ONLY(ProgRegion, ProgRegionData)

ProgRegion getProgRegion(Pic::MemoryRangeType);

enum ProgOption {
  NoProgOption                = 0x000,
  EraseBeforeProgram          = 0x001,
  PreserveEeprom              = 0x002,
  BlankCheckBeforeProgramming = 0x004,
  OverwriteOscCalValue        = 0x008,
  EraseProgramBootSegment     = 0x010,
  EraseProgramSecureSegment   = 0x020,
  EraseProgramGeneralSegment  = 0x040,
  EraseEedataBootSegment      = 0x080,
  EraseEedataSecureSegment    = 0x100,
  EraseEedataGeneralSegment   = 0x200,
  DoNotVerify                 = 0x400
};
Q_DECLARE_FLAGS(ProgOptions, ProgOption)
Q_DECLARE_OPERATORS_FOR_FLAGS(ProgOptions)

inline ByteArray createArray() { return ByteArray(BUF_SIZE, 'Z', PrintEscapeAll); }

//-----------------------------------------------------------------------------
class Port : public ::Port::USB//HID
{
public:
  Port(Log::Base& log);

  bool send(const ByteArray& cmd);
  bool receive(ByteArray& array);
};

//-----------------------------------------------------------------------------
class Hardware : public ::Programmer::PicHardware
{
public:
  Hardware(::Programmer::Base &base);

  bool connectToDevice(BitValue& deviceId);
  bool eraseRange(Pic::MemoryRangeType type);
  bool eraseAll();
  bool read(Pic::MemoryRangeType type, uint wordOffset, Device::Array& in, const ::Programmer::VerifyData *vdata);
  bool write(Pic::MemoryRangeType type, uint wordOffset, const Device::Array& data);
  bool setPower(bool on, bool targetSelfPowered);
  virtual bool setTargetReset(Device::ResetMode mode);
  virtual bool readVoltages(Device::VoltageValues&);
  ByteArray createConfigPacket() const;
  uint computeChecksum(const ByteArray& data) const;
  bool sendConfigPacket();
  bool getVersions();
  const VersionData& getFirmwareVersion() const { return _firmwareVersion; }
  const VersionData& getOSVersion() const { return _osVersion; }
  const VersionData& getAPVersion() const { return _apVersion; }
  uint getAPId() const { return _apId; }
  bool uploadAP(const Pic::Memory&);

private:
  VersionData _osVersion, _apVersion, _firmwareVersion;
  uint _apId, _hardwareRevision, _firmwareId;
  PowerOptions _powerOptions;

  virtual bool internalConnectHardware();
  Port& port() { return static_cast<Port&>(*_port); }
  bool getStatus(SystemStatus& status);
  bool selfTest();
  bool readSerialNumber();
  bool queryProgToGoConfig();
  bool getProtocolVersion();
  bool beginBracket();
  bool endBracket();
  bool setDebugOption();
  ByteArray createOpDescriptor(ProgOptions options) const;
  void addMemRangeToOpDescriptor(Pic::MemoryRangeType type, uint offset, uint size, ByteArray& data);
  void addMemRangeToOpDescriptor(Pic::MemoryRangeType type, ByteArray& data);
  bool getMem(Pic::MemoryRangeType type, uint offset, uint size, ByteArray& in);
  bool setMem(Pic::MemoryRangeType type, uint offset, const ByteArray& out);
  bool setPower(PowerOptions options);

  bool command(uchar cmd, const ByteArray* out = NULL);
  bool commandWithSimpleResponse(uchar cmd, ByteArray& in, const ByteArray* out = NULL);
  bool commandWithComplexResponse(uchar cmd, const char* cmdName, ByteArray& in, const ByteArray* out = NULL);
  bool sendBulk(const ByteArray& out);
};

} // namespace

#endif
