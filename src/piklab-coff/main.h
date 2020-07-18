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
namespace Device { class Data; class Memory; }
namespace Coff { class Symbol; class AuxSymbol; }

namespace CLI
{
//----------------------------------------------------------------------------
class Main : public MainBase
{
Q_OBJECT
public:
  Main();

private:
  const Device::Data *_device;

  virtual const CommandData& commandData(uint i) const;
  virtual const PropertyData& propertyData(uint i) const;

  virtual ExitCode prepareCommand(const CommandProperties& properties);
  virtual ExitCode executeCommand(const QString &command);
  virtual ExitCode doExecuteSetCommand(const QString &property, const QString &value);
  virtual ExitCode doExecuteGetCommand(const QString &property, QString& value);
  ExitCode executeCommandArchive(const QString &command, Log::KeyList &keys);
  ExitCode executeCommandObject(const QString &command, Log::KeyList &keys);

  virtual ExitCode list(const QString &listCommand);
  ExitCode deviceList();

  static QString prettyAuxSymbol(const Coff::AuxSymbol &aux);
  static QString prettySymbol(const Coff::Symbol &sym);
};

} // namespace

#endif
