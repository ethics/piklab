/***************************************************************************
 *   Copyright (C) 2005-2006 Nicolas Hadacek <hadacek@kde.org>             *
 *   Copyright (C) 2003-2004 Alain Gibaud <alain.gibaud@free.fr>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "gputils_compile.h"
#include "gputils_compile.moc"

#include "gputils.h"
#include "gputils_config.h"
#include "devices/list/device_list.h"
#include "coff/base/disassembler.h"

//-----------------------------------------------------------------------------
QString GPUtils::Process::deviceName() const
{
  return toDeviceName(_data.device);
}

//-----------------------------------------------------------------------------
void GPUtils::AssembleFile::logLine(::Process::OutputType, const QString &line)
{
  if ( parseErrorLine(line, Compile::ParseErrorData("([^:]*):([0-9]+):(.+)\\[[0-9]+\\](.+)", 1, 2, 4, 3, Log::LineType::Error)) ) return;
  if ( parseErrorLine(line, Compile::ParseErrorData("([^:]*):([^:]+):([0-9]+):(.+)", 2, 3, 4, -1, Log::LineType::Warning)) ) return;
  doLog(Log::LineType::Normal, line, QString::null, 0); // unrecognized
}

//-----------------------------------------------------------------------------
QStringList GPUtils::AssembleProjectFile::genericArguments(const Compile::Config &config) const
{
  QStringList args;
  args += "-c"; // relocatable code
  args += config.includeDirs(Tool::Category::Assembler, "-I");
  if ( !_data.items.empty() && _data.items[0].type==Compile::Item::NotGenerated ) args += "-p%DEVICE";
  uint wl = static_cast<const Config &>(config).gpasmWarningLevel() ;
  if( wl!=Config::Nb_WarningLevels ) args += "-w" + QString::number(wl);
  args += config.customOptions(Tool::Category::Assembler);
  args += "%I";
  return args;
}

QString GPUtils::AssembleProjectFile::outputFiles() const
{
  return "PURL::Object PURL::Lst";
}

//-----------------------------------------------------------------------------
QStringList GPUtils::LinkProject::genericArguments(const Compile::Config &config) const
{
  QStringList args;
  args += "-o%O";
  args += "-c"; // create coff file
  HexBuffer::Format f = config.hexFormat();
  if ( f!=HexBuffer::Nb_Formats ) args += QString("-a") + HexBuffer::FORMATS[f];
  args += "-m";  // with map
  args += config.includeDirs(Tool::Category::Linker, "-I");
  args += "$LKR(-s%LKR)";
  args += config.customOptions(Tool::Category::Linker);
  args += "%OBJS";
  args += "%LIBS";
  return args;
}

QString GPUtils::LinkProject::outputFiles() const
{
  return "PURL::Lkr PURL::Map PURL::Lst PURL::Cod PURL::Coff PURL::Hex";
}

//-----------------------------------------------------------------------------
QStringList GPUtils::LibraryProject::genericArguments(const Compile::Config &config) const
{
  QStringList args;
  args += "-c"; // create archive
  args += "%O";
  args += config.customOptions(Tool::Category::Librarian);
  args += "%OBJS";
  args += "%LIBS";
  return args;
}

QString GPUtils::LibraryProject::outputFiles() const
{
  return "PURL::Library";
}
