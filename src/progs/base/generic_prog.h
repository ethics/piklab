/***************************************************************************
 *   Copyright (C) 2005-2006 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef GENERIC_PROG_H
#define GENERIC_PROG_H

#include "common/global/log.h"
#include "common/global/pfile.h"
#include "common/common/version_data.h"
#include "common/port/port_base.h"
#include "common/global/progress_monitor.h"
#include "devices/base/generic_device.h"
#include "devices/base/generic_memory.h"
#include "prog_specific.h"
namespace Debugger { class Base; }
namespace Device { class MemoryRange; }

namespace Programmer
{
//-----------------------------------------------------------------------------
enum Mode { NormalMode, BootloadMode };
enum State { NotConnected = 0, Stopped, Running, Halted };
class Hardware;
class Group;
class DeviceSpecific;

enum VerifyAction { NormalVerify = 0, BlankCheckVerify = 1,
                    IgnoreProtectedVerify = 2, OnlyProgrammedVerify = 4 };
Q_DECLARE_FLAGS(VerifyActions, VerifyAction)
Q_DECLARE_OPERATORS_FOR_FLAGS(VerifyActions)
class VerifyData {
public:
  VerifyData(VerifyActions pactions, const Device::Memory &memory);
  VerifyData(const VerifyData& vdata);
  VerifyData& operator=(const VerifyData& vdata);

  VerifyActions actions;
  AddressRangeVector protectedRanges;
  const Device::Memory& memory() const { return *_memory; }
  Device::Memory& memory() { return *_memory; }

private:
  std::auto_ptr<Device::Memory> _memory;
};

BEGIN_DECLARE_ENUM(ResultType)
  Pass = 0, Low, High, Fail
END_DECLARE_ENUM_LABEL_ONLY(ResultType)

class HardwareDescription {
public:
  Port::Description port;
  QString name;
};

BEGIN_DECLARE_ENUM(Task)
  Read = 0, Write, Verify, Erase, BlankCheck
END_DECLARE_ENUM_LABEL_ONLY(Task)

BEGIN_DECLARE_ENUM(FirmwareVersionType)
  Min, Max, Recommended
END_DECLARE_ENUM_NO_DATA(FirmwareVersionType)

//-----------------------------------------------------------------------------
class Generic : public Log::Base
{
public:
  Generic(const Group &group, const Device::Data *data);
  virtual ~Generic() {}

  const Device::Data *device() const { return _device; }
  const Group &group() const { return _group; }
  State state() const { return _state; }
  void setState(State state) { _state = state; }
  bool isConnected() const { return ( _state!=NotConnected ); }
  bool isActive() const { return ( _state==Halted || _state==Running ); }
  ProgressMonitor &progressMonitor() { return _progressMonitor; }

  virtual uint runUpdateWait() const { return 300; }
  virtual ::Debugger::Base *debugger() const = 0;
  virtual bool connectHardware() = 0;
  virtual bool connectDevice() = 0;
  virtual void disconnectHardware() = 0;
  virtual bool isTargetSelfPowered() const = 0;
  virtual bool setTargetPowerOn(bool on) = 0;
  virtual bool isTargetPowerOn() const = 0;

  virtual bool erase(const Device::MemoryRange &range) = 0;
  virtual bool read(Device::Memory &memory, const Device::MemoryRange &range) = 0;
  virtual bool program(const Device::Memory &memory, const Device::MemoryRange &range) = 0;
  virtual bool verify(const Device::Memory &memory, const Device::MemoryRange &range) = 0;
  virtual bool blankCheck(const Device::MemoryRange &range) = 0;
  virtual bool run() = 0;
  virtual bool stop() = 0;

protected:
  const Device::Data *_device;
  State               _state;
  const Group        &_group;
  ProgressMonitor     _progressMonitor;
};

//-----------------------------------------------------------------------------
class Base : public Generic
{
public:
  Base(const Group &group, const Device::Data *data, const char *name);
  virtual ~Base();

  virtual void log(Log::LineType type, const QString &message, Log::Action action = Log::Immediate);
  virtual void log(Log::DebugLevel level, const QString &message, Log::Action action = Log::Immediate);
  void init(bool targetSelfPowered, Hardware *hardware, DeviceSpecific *deviceSpecific);
  Hardware *hardware() const { return _hardware.get(); }
  DeviceSpecific* deviceSpecific() const { return _deviceSpecific.get(); }
  virtual ::Debugger::Base *debugger() const { return _debugger.get(); }
  virtual uint maxNbBreakpoints() const { return 1; }

  bool simpleConnectHardware();
  virtual bool connectHardware();
  virtual void disconnectHardware();
  virtual bool connectDevice();
  virtual bool setTargetPowerOn(bool on);
  virtual bool isTargetPowerOn() const { return _targetPowerOn; }
  virtual bool isTargetSelfPowered() const { return _targetSelfPowered; }
  PURL::Directory firmwareDirectory();
  void setFirmwareDirectory(const PURL::Directory &dir) { _firmwareDirectory = dir; }
  const VersionData &firmwareVersion() const { return _firmwareVersion; }
  virtual bool readFirmwareVersion() { return true; }
  bool uploadFirmware(const PURL::Url &url);
  bool readVoltages();
  const Device::VoltageValue &voltage(Device::VoltageType type) const { return _voltages[type.type()]; }
  virtual bool selfTest(bool ask) { Q_UNUSED(ask); return true; }
  void appendTask(Task task, const Device::MemoryRange &range);

  virtual bool erase(const Device::MemoryRange &range);
  virtual bool read(Device::Memory &memory, const Device::MemoryRange &range);
  virtual bool program(const Device::Memory &memory, const Device::MemoryRange &range);
  virtual bool verify(const Device::Memory &memory, const Device::MemoryRange &range);
  virtual bool blankCheck(const Device::MemoryRange &range);
  virtual bool run();
  virtual bool stop();
  bool enterMode(Mode mode);
  bool doConnectDevice();
  virtual void clear();
  bool getTargetMode(Device::TargetMode &mode);

protected:
  std::auto_ptr<Hardware>          _hardware;
  std::auto_ptr<DeviceSpecific>    _deviceSpecific;
  std::auto_ptr< ::Debugger::Base> _debugger;
  bool                             _targetPowerOn;
  PURL::Directory                  _firmwareDirectory;
  VersionData                      _firmwareVersion;
  Mode                             _mode;
  bool                             _targetSelfPowered;
  Device::VoltageValues            _voltages;

  virtual bool setupFirmware() { return true; }
  virtual VersionData firmwareVersion(FirmwareVersionType type) const { Q_UNUSED(type); return VersionData(); }
  virtual bool checkFirmwareVersion();
  virtual bool setTarget() { return true; }
  virtual bool internalSetupHardware() { return initProgramming(); }
  virtual bool initProgramming();
  virtual bool verifyDeviceId() = 0;
  virtual uint nbSteps(Task task, const Device::MemoryRange &range) const = 0;
  virtual bool doUploadFirmware(PURL::File &);
  virtual bool internalUploadFirmware(PURL::File &) { return false; }
  virtual bool internalEnterMode(Mode) { return false; }

  virtual bool checkErase() = 0;
  virtual bool internalErase(const Device::MemoryRange &range) = 0;
  bool checkCanRead();
  virtual bool checkRead() = 0;
  virtual bool internalRead(Device::Memory *memory, const Device::MemoryRange &range, const VerifyData *vdata) = 0;
  virtual bool checkProgram(const Device::Memory &memory) = 0;
  virtual bool internalProgram(const Device::Memory &memory, const Device::MemoryRange &range) = 0;

  void endProgramming();
  bool doErase(const Device::MemoryRange &range);
  bool doRead(Device::Memory &memory, const Device::MemoryRange &range);
  bool doVerify(const Device::Memory &memory, const Device::MemoryRange &range);
  bool doVerify(VerifyAction action, const Device::MemoryRange &range, const Device::Memory *memory);
  bool doBlankCheck(const Device::MemoryRange &range);
  virtual bool doProgram(const Device::Memory &memory, const Device::MemoryRange &range);

  friend class DeviceSpecific;
};

} // namespace

#endif
