/***************************************************************************
 *   Copyright (C) 2005-2007 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "generic_prog.h"

#include <qdir.h>

#include "prog_group.h"
#include "prog_config.h"
#include "devices/base/device_group.h"
#include "generic_debug.h"
#include "hardware_config.h"

//-----------------------------------------------------------------------------
const Programmer::ResultType::Data Programmer::ResultType::DATA[Nb_Types] = {
  { I18N_NOOP("Pass") },
  { I18N_NOOP("Low")  },
  { I18N_NOOP("High") },
  { I18N_NOOP("Fail") }
};

const Programmer::Task::Data Programmer::Task::DATA[Nb_Types] = {
  { I18N_NOOP("Reading...")        },
  { I18N_NOOP("Programming...")    },
  { I18N_NOOP("Verifying...")      },
  { I18N_NOOP("Erasing...")        },
  { I18N_NOOP("Blank Checking...") }
};

//-----------------------------------------------------------------------------
Programmer::VerifyData::VerifyData(VerifyActions pactions, const Device::Memory &memory)
: actions(pactions)
{
  _memory = memory.clone();
}

Programmer::VerifyData::VerifyData(const VerifyData& vdata)
{
  *this = vdata;
}

Programmer::VerifyData& Programmer::VerifyData::operator=(const VerifyData& vdata)
{
  if (this != &vdata) {
    actions = vdata.actions;
    protectedRanges = vdata.protectedRanges;
    _memory = vdata.memory().clone();
  }
  return *this;
}

//-----------------------------------------------------------------------------
Programmer::Generic::Generic(const Group &group, const Device::Data *device)
  : _device(device), _state(NotConnected), _group(group)
{}

//-----------------------------------------------------------------------------
Programmer::Base::Base(const Group &group, const Device::Data *device, const char *)
: Generic(group, device),
  _debugger(group.createDebugger(*this)),
  _targetPowerOn(false)
{
}

void Programmer::Base::init(bool targetSelfPowered, Hardware *hardware, DeviceSpecific *deviceSpecific)
{
  clear();
  _targetSelfPowered = targetSelfPowered;
  _hardware.reset(hardware);
  _deviceSpecific.reset(deviceSpecific);
}

Programmer::Base::~Base()
{
}

void Programmer::Base::clear()
{
  _firmwareVersion.clear();
  _voltages = Device::VoltageValues();
  _mode = NormalMode;
  resetError();
}

bool Programmer::Base::simpleConnectHardware()
{
  log(Log::DebugLevel::Extra, "Programmer::Base::simpleConnectHardware");
  ASSERT(hardware() != NULL);
  disconnectHardware();
  clear();
  if (_device != NULL) {
    QString label = _group.label();
    if ( group().isSoftware() )
      log(Log::LineType::Information, i18n("Connecting %1 with device %2...").arg(label).arg(_device->name()));
    else {
      if ( !_hardware->name().isEmpty() ) label += "[" + _hardware->name() + "]";
      Port::Description pd = _hardware->portDescription();
      QString s = pd.type.label();
      if (pd.type.data().withDevice) s += " (" + pd.device + ")";
      log(Log::LineType::Information, i18n("Connecting %1 on %2 with device %3...").arg(label).arg(s).arg(_device->name()));
    }
  }
  return _hardware->connectHardware();
}

bool Programmer::Base::connectHardware()
{
  _progressMonitor.insertTask(i18n("Connecting..."), 2);
  if ( !simpleConnectHardware() ) return false;
  _progressMonitor.addTaskProgress(1);
  if ( !group().isSoftware() ) {
    if ( !readFirmwareVersion() ) return false;
    if (_deviceSpecific.get() == NULL) return true;
    if ( _mode==BootloadMode ) return true;
    if ( !setupFirmware() ) return false;
    if ( !checkFirmwareVersion() ) return false;
    if ( !setTargetPowerOn(false) ) return false;
    if ( !setTarget() ) return false;
    log(Log::LineType::Information, i18n("  Set target power: %1").arg(!_targetSelfPowered ? "true" : "false"));
    if ( !setTargetPowerOn(!_targetSelfPowered) ) return false;
    if ( !internalSetupHardware() ) return false;
    if ( !readVoltages() ) return false;
    if ( !selfTest(true) ) return false;
  }
  if ( hasError() ) return false;
  log(Log::LineType::Information, i18n("Connected."));
  _state = Stopped;
  return true;
}

bool Programmer::Base::initProgramming()
{
  _state = ::Programmer::Stopped;
  if ( group().properties() & CanReleaseReset ) {
    log(Log::DebugLevel::Normal, "  Hold reset");
    if ( !hardware()->setTargetReset(Device::ResetMode::Held) ) return false;
  }
  Device::TargetMode mode;
  if ( !getTargetMode(mode) ) return false;
  if ( mode!=Device::TargetMode::InProgramming ) {
    log(Log::LineType::Error, i18n("Device not in programming"));
    return false;
  }
  return true;
}

void Programmer::Base::disconnectHardware()
{
  _state = NotConnected;
  clear();
  _hardware->disconnectHardware();
}

PURL::Directory Programmer::Base::firmwareDirectory()
{
  if (_firmwareDirectory.isEmpty()) _firmwareDirectory = GroupConfig::firmwareDirectory(group());
  if (_firmwareDirectory.isEmpty()) {
    log(Log::LineType::Error, i18n("Firmware directory is not configured."));
    return PURL::Directory();
  }
  if (!_firmwareDirectory.exists()) {
    log(Log::LineType::Error, i18n("Firmware directory '%1' does not exist.").arg(_firmwareDirectory.pretty()));
    return PURL::Directory();
  }
  return _firmwareDirectory;
}

bool Programmer::Base::checkFirmwareVersion()
{
  if ( _mode==BootloadMode ) log(Log::LineType::Information, i18n("Programmer is in bootload mode."));
  if ( !_firmwareVersion.isValid() ) return true;
  log(Log::LineType::Information, i18n("Firmware version is %1").arg(_firmwareVersion.pretty()));
  VersionData vd = _firmwareVersion.toWithoutDot();
  VersionData tmp = firmwareVersion(FirmwareVersionType::Max);
  if ( tmp.isValid() && tmp.toWithoutDot()<vd ) {
    log(Log::LineType::Warning, i18n("The firmware version (%1) is higher than the version tested with piklab (%2).\n"
                                     "You may experience problems.").arg(_firmwareVersion.pretty()).arg(tmp.pretty()));
    return true;
  }
  tmp = firmwareVersion(FirmwareVersionType::Min);
  if ( tmp.isValid() && vd<tmp.toWithoutDot() ) {
    log(Log::LineType::Warning, i18n("The firmware version (%1) is lower than the version tested with piklab (%2).\n"
                                     "You may experience problems.").arg(_firmwareVersion.pretty()).arg(tmp.pretty()));
    return true;
  }
  tmp = firmwareVersion(FirmwareVersionType::Recommended);
  if ( tmp.isValid() && vd<tmp.toWithoutDot() ) {
    log(Log::LineType::Warning, i18n("The firmware version (%1) is lower than the recommended version (%2).\n"
                                     "It is recommended to upgrade the firmware.").arg(_firmwareVersion.pretty()).arg(tmp.pretty()));
    return true;
  }
  return true;
}

bool Programmer::Base::readVoltages()
{
  if ( !hardware()->readVoltages(_voltages) ) return false;
  bool ok = true;
  FOR_EACH(Device::VoltageType, type) {
    if ( !group().canReadVoltage(type) ) continue;
    switch (voltage(type).type()) {
      case Device::VoltageValue::Error:
        ok = false;
        log(Log::LineType::Error, i18n("  %1 = %2 V: error in voltage level.").arg(type.label()).arg(voltage(type).value()));
        break;
      case Device::VoltageValue::Normal:
        log(Log::DebugLevel::Normal, QString("  %1 = %2 V").arg(type.label()).arg(voltage(type).value()));
        break;
      case Device::VoltageValue::Unknown: break;
    }
  }
  return ok;
}

bool Programmer::Base::enterMode(Mode mode)
{
  log(Log::DebugLevel::Normal, mode==BootloadMode ? "  Enter bootload mode" : "  Enter normal mode");
  if ( _mode==mode ) {
    log(Log::DebugLevel::Normal, "  Already in requested mode.");
    return true;
  }
  if ( !internalEnterMode(mode) ) return false;
  return ( _mode==mode );
}

void Programmer::Base::log(Log::LineType type, const QString &message, Log::Action action)
{
  if ( type==Log::LineType::Error ) _state = NotConnected;
  Log::Base::log(type, message, action);
}

void Programmer::Base::log(Log::DebugLevel level, const QString &message, Log::Action action)
{
  Log::Base::log(level, message, action);
}

bool Programmer::Base::setTargetPowerOn(bool on)
{
  _targetPowerOn = on;
  return _deviceSpecific.get() != NULL ? _deviceSpecific->setTargetPowerOn(on) : true;
}

void Programmer::Base::appendTask(Task task, const Device::MemoryRange& range)
{
  _progressMonitor.appendTask(task.label(), nbSteps(task, range));
}

bool Programmer::Base::connectDevice()
{
  _progressMonitor.clear();
  bool ok = doConnectDevice();
  endProgramming();
  return ok;
}

bool Programmer::Base::doConnectDevice()
{
  if ( _state==NotConnected ) {
    log(Log::DebugLevel::Max, QString("Programmer::Base::doConnectDevice"));
    if ( !connectHardware() ) return false;
    if ( !enterMode(NormalMode) ) return false;
    if ( !verifyDeviceId() ) return false;
  } else {
    setTargetPowerOn(!_targetSelfPowered);
  }
  _state = Stopped;
  return true;
}

bool Programmer::Base::run()
{
  _progressMonitor.setActionMessage(i18n("Running..."));
  _progressMonitor.clear();
  if ( !doConnectDevice() ) return false;
  log(Log::LineType::Information, i18n("Run..."));
  _state = ::Programmer::Running;
  return hardware()->setTargetReset(Device::ResetMode::Released);
}

bool Programmer::Base::stop()
{
  if ( !doConnectDevice() ) return false;
  _progressMonitor.setActionMessage(i18n("Breaking..."));
  _state = ::Programmer::Stopped;
  return hardware()->setTargetReset(Device::ResetMode::Held);
}

bool Programmer::Base::getTargetMode(Device::TargetMode &mode)
{
  return hardware()->getTargetMode(mode);
}

void Programmer::Base::endProgramming()
{
  if ( _state==Stopped && readConfigEntry(Config::PowerDownAfterProgramming).toBool() )
    setTargetPowerOn(false);
  if ( !(group().properties() & HasConnectedState) ) disconnectHardware();
  _progressMonitor.clear();
}

bool Programmer::Base::uploadFirmware(const PURL::Url &url)
{
  _progressMonitor.clear();
  log(Log::DebugLevel::Normal, QString("  Firmware file: %1").arg(url.pretty()));
  Log::StringView sview;
  PURL::File file(url, sview);
  if ( !file.openForRead() ) {
    log(Log::LineType::Error, i18n("Could not open firmware file \"%1\".").arg(url.pretty()));
    return false;
  }
  bool ok  = doUploadFirmware(file);
  _firmwareVersion.clear();
  if (ok) ok = readFirmwareVersion();
  endProgramming();
  return ok;
}

bool Programmer::Base::doUploadFirmware(PURL::File &file)
{
  _progressMonitor.setActionMessage(i18n("Uploading firmware..."));
  return internalUploadFirmware(file);
}

//-----------------------------------------------------------------------------
bool Programmer::Base::erase(const Device::MemoryRange &range)
{
  _progressMonitor.clear();
  appendTask(Task::Erase, range);
  if ( readConfigEntry(Config::BlankCheckAfterErase).toBool() ) appendTask(Task::BlankCheck, range);
  bool ok = doErase(range);
  endProgramming();
  return ok;
}

bool Programmer::Base::doErase(const Device::MemoryRange &range)
{
  if ( !checkErase() ) return false;
  if ( !doConnectDevice() ) return false;
  _progressMonitor.startNextTask();
  log(Log::LineType::Information, i18n("Erasing..."));
  if ( !internalErase(range) ) return false;
  log(Log::LineType::Information, i18n("Erasing done"));
  if ( readConfigEntry(Config::BlankCheckAfterErase).toBool() ) {
    _progressMonitor.startNextTask();
    log(Log::LineType::Information, i18n("Blank checking..."));
    if ( !doVerify(BlankCheckVerify, range, 0) ) return false;
    log(Log::LineType::Information, i18n("Blank checking done."));
  }
  return true;
}

//-----------------------------------------------------------------------------
bool Programmer::Base::checkCanRead()
{
  if ( !(group().properties() & CanReadMemory) ) {
    log(Log::LineType::Error, i18n("The selected programmer cannot read device memory."));
    return false;
  }
  return true;
}

bool Programmer::Base::read(Device::Memory &memory, const Device::MemoryRange &range)
{
  if ( !checkCanRead() ) return false;
  _progressMonitor.clear();
  appendTask(Task::Read, range);
  bool ok = doRead(memory, range);
  endProgramming();
  return ok;
}

bool Programmer::Base::doRead(Device::Memory &memory, const Device::MemoryRange &range)
{
  if ( !checkRead() ) return false;
  if ( !doConnectDevice() ) return false;
  _progressMonitor.startNextTask();
  log(Log::LineType::Information, i18n("Reading device memory..."));
  memory.clear();
  if ( !internalRead(&memory, range, 0) ) return false;
  log(Log::LineType::Information, i18n("Reading done."));
  return true;
}

//-----------------------------------------------------------------------------
bool Programmer::Base::program(const Device::Memory &memory, const Device::MemoryRange &range)
{
  _progressMonitor.clear();
  appendTask(Task::Write, range);
  bool ok = doProgram(memory, range);
  endProgramming();
  return ok;
}

bool Programmer::Base::doProgram(const Device::Memory &memory, const Device::MemoryRange &range)
{
  if ( !checkProgram(memory) ) return false;
  if ( !doConnectDevice() ) return false;
  _progressMonitor.startNextTask();
  log(Log::LineType::Information, i18n("Programming device memory..."));
  if ( !internalProgram(memory, range) ) return false;
  log(Log::LineType::Information, i18n("Programming successful."));
  if ( group().isDebugger() && !_debugger->init() ) return false;
  return true;
}

//-----------------------------------------------------------------------------
bool Programmer::Base::verify(const Device::Memory &memory, const Device::MemoryRange &range)
{
  if ( !checkCanRead() ) return false;
  _progressMonitor.clear();
  appendTask(Task::Verify, range);
  bool ok = doVerify(memory, range);
  endProgramming();
  return ok;
}

bool Programmer::Base::doVerify(VerifyAction action, const Device::MemoryRange &range, const Device::Memory *memory)
{
  std::auto_ptr<Device::Memory> vmemory;
  if (memory == NULL) {
    ASSERT(action & BlankCheckVerify);
    vmemory = _device->group().createMemory(*_device);
  }
  VerifyData vdata(action, memory != NULL ? *memory : *vmemory);
  return  internalRead(0, range, &vdata);
}

bool Programmer::Base::doVerify(const Device::Memory &memory, const Device::MemoryRange &range)
{
  if ( !checkRead() ) return false;
  if ( !doConnectDevice() ) return false;
  _progressMonitor.startNextTask();
  log(Log::LineType::Information, i18n("Verifying..."));
  if ( !doVerify(NormalVerify, range, &memory) ) return false;
  log(Log::LineType::Information, i18n("Verifying successful."));
  return true;
}

bool Programmer::Base::blankCheck(const Device::MemoryRange &range)
{
  if ( !checkCanRead() ) return false;
  _progressMonitor.clear();
  appendTask(Task::BlankCheck, range);
  bool ok = doBlankCheck(range);
  endProgramming();
  return ok;
}

bool Programmer::Base::doBlankCheck(const Device::MemoryRange &range)
{
  if ( !checkRead() ) return false;
  if ( !doConnectDevice() ) return false;
  _progressMonitor.startNextTask();
  log(Log::LineType::Information, i18n("Blank checking..."));
  if ( !doVerify(BlankCheckVerify, range, 0) ) return false;
  log(Log::LineType::Information, i18n("Blank checking successful."));
  return true;
}
