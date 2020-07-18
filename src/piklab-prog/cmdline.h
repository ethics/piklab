/***************************************************************************
 *   Copyright (C) 2005-2009 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef CMDLINE_H
#define CMDLINE_H

#include "common/cli/cli_main.h"
#include "common/global/purl.h"
#include "devices/base/hex_buffer.h"
#include "devices/base/generic_memory.h"
#include "devices/base/device_group.h"

namespace Programmer { class Base; class Group; }

namespace CLI
{
//----------------------------------------------------------------------------
class Main : public MainBase
{
Q_OBJECT
public:
  Main();

private:
  std::auto_ptr<Device::MemoryRange> _range;

  ExitCode formatList();
  ExitCode programmerList();
  ExitCode hardwareList();
  ExitCode deviceList();
  ExitCode portList();
  ExitCode rangeList();
  ExitCode checkProgrammer();
  ExitCode registerList();
  ExitCode variableList();

  virtual const CommandData& commandData(uint i) const;
  virtual const CommandData& interactiveCommandData(uint i) const;
  virtual const PropertyData& propertyData(uint i) const;

  virtual ExitCode prepareCommand(const CommandProperties& properties);
  virtual ExitCode executeCommand(const QString &command);
  virtual ExitCode doExecuteSetCommand(const QString &property, const QString &value);
  virtual ExitCode doExecuteGetCommand(const QString &property, QString& value);
  virtual ExitCode list(const QString &command);
  ExitCode extractRange(const QString &range);

  virtual ExitCode processLine(const QStringList& words);
  ExitCode executeRegister(const QString &name, const QString &value);
  ExitCode executeRawCommands(const QString &filename);
  Address findRegisterAddress(const QString &name);
  virtual void processInterrupt();
};

extern const Programmer::Group*      _progGroup;
extern QString                       _hardware;
extern const Device::Data*           _device;
extern HexBuffer::Format             _format;
extern QString                       _port;
extern QString                       _targetSelfPowered;
extern PURL::Directory               _firmwareDir;
extern PURL::Url                     _coffUrl;
extern std::auto_ptr<Device::Memory> _memory;

} // namespace

#endif
