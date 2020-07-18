/***************************************************************************
 *   Copyright (C) 2007 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef MAIN_H
#define MAIN_H

#include "common/global/pfile.h"
#include "devices/base/hex_buffer.h"
#include "common/cli/cli_main.h"
#include "devices/base/generic_memory.h"

namespace CLI
{
//----------------------------------------------------------------------------
struct FillOption {
  const char *name, *description;
};
extern const FillOption NullFillOption;

//----------------------------------------------------------------------------
class Main : public MainBase
{
Q_OBJECT
public:
  Main();

private:
  HexBuffer                        _source1;
  HexBuffer                        _source2;
  HexBuffer::Format                _format;
  QValueList<HexBuffer::ErrorData> _errors;
  const Device::Data*              _device;
  std::auto_ptr<Device::Memory>    _memory;
  QString                          _fill;

  virtual const CommandData& commandData(uint i) const;
  virtual const PropertyData& propertyData(uint i) const;

  ExitCode setInputHex(const CommandProperties& properties);
  ExitCode setSecondInputHex(const CommandProperties& properties);
  virtual ExitCode prepareCommand(const CommandProperties& properties);
  virtual ExitCode executeCommand(const QString &command);
  virtual ExitCode doExecuteSetCommand(const QString &property, const QString &value);
  virtual ExitCode doExecuteGetCommand(const QString &property, QString& value);

  virtual ExitCode list(const QString &listCommand);
  ExitCode deviceList();
  ExitCode fillOptionList();
};

} // namespace

#endif
