/***************************************************************************
 *   Copyright (C) 2006-2009 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef GPSIM_H
#define GPSIM_H

#include "common/common/version_data.h"
#include "devices/pic/prog/pic_prog_specific.h"
#include "common/global/process.h"
#include "common/global/purl.h"

namespace GPSim
{
//-----------------------------------------------------------------------------
class Process : public ::Process::LineOutput, public Log::Base
{
Q_OBJECT
public:
  Process(Log::Base *base);
  bool runSynchronously();

private slots:
  virtual void timeoutSlot();
  void dataReceivedSlot(::Process::OutputType type);

private:
  bool _ready;

  virtual void addLine(::Process::OutputType type, const QString &line);

  friend class ProcessManager;
};

//-----------------------------------------------------------------------------
class ProcessManager : public Log::Base
{
public:
  ProcessManager(Log::Base *base);
  Process &process() { return _process; }
  bool isReady() const { return _process._ready; }
  bool start();
  bool start(const PURL::Directory& execDir);
  bool runSynchronously();
  bool sendCommand(const QString &cmd, bool synchronous);
  bool sendSignal(uint n, bool synchronous);
  QStringList outputLines() const;
  bool getVersion(VersionData &version);
  bool getSupportedDevices(VersionData &version, QStringList &devices);

private:
  Process _process;
  uint    _retries;
};

//-----------------------------------------------------------------------------
class Hardware : public Programmer::Hardware
{
public:
  Hardware(::Programmer::Base &base) : Programmer::Hardware(base, 0, QString::null), _manager(0) {}
  bool isReady() const { return _manager->isReady(); }
  bool execute(const QString &command, bool synchronous, QStringList *output = 0);
  bool signal(uint n, bool synchronous, QStringList *output = 0);
  const VersionData &version() const { return _version; }

private:
  std::auto_ptr<ProcessManager> _manager;
  VersionData                   _version;

  virtual bool internalConnectHardware();
  virtual void internalDisconnectHardware();
};

//-----------------------------------------------------------------------------
class DeviceSpecific : public ::Programmer::PicSpecific
{
public:
  DeviceSpecific(::Programmer::Base &base) : ::Programmer::PicSpecific(base) {}
  virtual bool setPowerOff() { return false; }
  virtual bool setPowerOn() { return false; }
  virtual bool setTargetPowerOn(bool) { return true; }
  virtual ::Programmer::Capabilities capabilities() const { return ::Programmer::NoCapability; }
  virtual bool canEraseRange(Pic::MemoryRangeType) const { return false; }
  virtual bool canReadRange(Pic::MemoryRangeType) const { return false; }
  virtual bool canWriteRange(Pic::MemoryRangeType) const { return false; }

protected:
  virtual bool doErase(bool) { return false; }
  virtual bool doEraseRange(Pic::MemoryRangeType) { return false; }
  virtual bool doRead(Pic::MemoryRangeType, uint, Device::Array &, const ::Programmer::VerifyData*) { return false; }
  virtual bool doWrite(Pic::MemoryRangeType, uint, const Device::Array &, bool) { return false; }
};

} // namespace

#endif
