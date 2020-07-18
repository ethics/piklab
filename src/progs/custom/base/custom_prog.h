/***************************************************************************
 *   Copyright (C) 2007-2008 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef CUSTOM_PROG_H
#define CUSTOM_PROG_H

#include "progs/base/prog_group.h"
#include "tools/list/compile_process.h"

namespace CustomProgrammer
{
BEGIN_DECLARE_ENUM(Action)
  Read = 0, Erase, Program, Verify, BlankCheck, Run, Stop
END_DECLARE_ENUM_KEY_LABEL(Action)

//-----------------------------------------------------------------------------
class Config : public GenericConfig
{
public:
  Config() : GenericConfig("custom_programmer") {}
  static QString command(Action action);
  static void writeCommand(Action action, const QString &command);
};

//-----------------------------------------------------------------------------
class Base;

class ProcessManager : public Compile::GenericProcess, public Log::Base
{
Q_OBJECT
public:
  ProcessManager(::CustomProgrammer::Base &base);
  bool execute(const QString &command, const QString& actionLabel);
  virtual uint nbFiles() const { return 0; }
  virtual QString deviceName() const;

private slots:
  void logLine(::Process::OutputType type, const QString &s);

private:
  ::CustomProgrammer::Base& _base;
};

//-----------------------------------------------------------------------------
class Base : public ::Programmer::Generic
{
public:
  Base(const ::Programmer::Group &group, const Device::Data *data);

  virtual ::Debugger::Base *debugger() const { return NULL; }
  virtual bool connectHardware() { return false; }
  virtual bool connectDevice() { return false; }
  virtual void disconnectHardware() {}
  virtual bool isTargetSelfPowered() const { return false; }
  virtual bool setTargetPowerOn(bool) { return false; }
  virtual bool isTargetPowerOn() const { return false; }

  virtual bool erase(const Device::MemoryRange &range) { return execute(Action::Erase, 0, &range); }
  virtual bool read(Device::Memory &memory, const Device::MemoryRange &range) { return execute(Action::Read, &memory, &range); }
  virtual bool program(const Device::Memory &memory, const Device::MemoryRange &range) { return execute(Action::Program, const_cast<Device::Memory *>(&memory), &range); }
  virtual bool verify(const Device::Memory &memory, const Device::MemoryRange &range) { return execute(Action::Verify, const_cast<Device::Memory *>(&memory), &range); }
  virtual bool blankCheck(const Device::MemoryRange &range) { return execute(Action::BlankCheck, 0, &range); }
  virtual bool run() { return execute(Action::Run, 0, 0); }
  virtual bool stop() { return execute(Action::Stop, 0, 0); }

private:
  bool execute(Action action, Device::Memory *memory, const Device::MemoryRange *range);

private:
  ProcessManager _manager;
};

//-----------------------------------------------------------------------------
class Group : public ::Programmer::Group
{
public:
  virtual QString name() const { return "custom"; }
  virtual QString label() const { return i18n("Custom Programmer"); }
  virtual ::Programmer::Properties properties() const { return ::Programmer::Programmer | ::Programmer::Custom | Programmer::CanReleaseReset; }
  virtual ::Programmer::TargetPowerMode targetPowerMode() const { return ::Programmer::TargetSelfPowered; }
  virtual bool isPortSupported(PortType) const { return false; }
  virtual bool canReadVoltage(Device::VoltageType) const { return false; }

protected:
  virtual void initSupported();
  virtual ::Programmer::Generic *createBase(const Device::Data *data) const { return new CustomProgrammer::Base(*this, data); }
  virtual ::Programmer::Hardware *createHardware(::Programmer::Base &, const ::Programmer::HardwareDescription &) const { return 0; }
  virtual ::Programmer::DeviceSpecific *createDeviceSpecific(::Programmer::Base &) const { return 0; }
  virtual ::Debugger::DeviceSpecific *createDebuggerDeviceSpecific(::Debugger::Base &) const { return 0; }
};

} // namespace

#endif
