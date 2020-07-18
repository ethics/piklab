/***************************************************************************
 *   Copyright (C) 2006-2010 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "sdcc_compile.h"
#include "sdcc_compile.moc"

#include "sdcc.h"
#include "sdcc_config.h"
#include "tools/list/tool_list.h"
#include "sdcc_generator.h"
#include "devices/list/device_list.h"
#include "devices/pic/base/pic.h"

//-----------------------------------------------------------------------------
QString SDCC::Process::familyName() const
{
  return FAMILY_DATA[family(_data.device)].name;
}

QString SDCC::Process::deviceName() const
{
  return toDeviceName(_data.device);
}

QStringList SDCC::Process::genericArguments(const Compile::Config &) const
{
  QStringList args;
  args += "--use-non-free";
  args += "-m%FAMILY";
  args += "-%DEVICE";
  args += "-V"; // verbose
  args += "--debug";
  return args;
}

QString SDCC::Process::outputFiles() const
{
  return "PURL::Object PURL::AsmGPAsm adb p d PURL::Lst";
}

void SDCC::Process::logLine(::Process::OutputType, const QString &line)
{
  if ( parseErrorLine(line, Compile::ParseErrorData("([^:]*):([0-9]+):( syntax error):(.+)", 1, 2, 4, -1, Log::LineType::Error)) ) return;
  if ( parseErrorLine(line, Compile::ParseErrorData("([^:]*):([0-9]+):( error)(.+)", 1, 2, 4, -1, Log::LineType::Error)) ) return;
  if ( parseErrorLine(line, Compile::ParseErrorData("([^:]*):([0-9]+):(error|warning|message):(.+)", 1, 2, 4, 3, Log::LineType::Error)) ) return;
  if ( parseErrorLine(line, Compile::ParseErrorData("([^:]*):([0-9]+):(\\w+)\\s*\\[[0-9]+\\](.+)", 1, 2, 4, 3, Log::LineType::Error)) ) return;
  if ( parseErrorLine(line, Compile::ParseErrorData("([^:]*)\\s*[0-9]+:(.+)", -1, -1, 2, 1, Log::LineType::Warning)) ) return;
  if ( parseErrorLine(line, Compile::ParseErrorData("([^:]*):([0-9]+):(.+)", 1, 2, 3, -1, Log::LineType::Warning)) ) return;
  if ( parseErrorLine(line, Compile::ParseErrorData("([^:]*):([^:]+):([0-9]+):(.+)", 2, 3, 4, -1, Log::LineType::Warning)) ) return;
  if ( parseErrorLine(line, Compile::ParseErrorData("([^:]+):(.+)", -1, -1, 2, 1, Log::LineType::Warning)) ) return;
  doLog(filterType(line), line, QString::null, 0);
}

//-----------------------------------------------------------------------------
QStringList SDCC::CompileProjectFile::genericArguments(const Compile::Config &config) const
{
  QStringList args = Process::genericArguments(config);
  args += config.includeDirs(Tool::Category::Compiler, "-I");
  args += config.customOptions(Tool::Category::Compiler);
  args += "-c"; // compile only
  args += "%I";
  return args;
}

//-----------------------------------------------------------------------------
QStringList SDCC::LinkProjectFile::genericArguments(const Compile::Config &config) const
{
  QStringList args = Process::genericArguments(config);
  args += "-Wl-c"; // create coff file
  args += "-Wl-m"; // output map file
  args += "$LKR(-Wl-s%LKR)";
  if (family(_data.device) == P14) {
    const Device::Data* data = Device::Lister::instance().data(_data.device);
    if (static_cast<const Pic::Data&>(*data).architecture().type() == Pic::Architecture::EMidRange) {
      args += "libme.lib";
    } else {
      args += "libm.lib";
    }
  } else {
    args += "libio" + _data.device.toLower() + ".lib";
    args += "libm18f.lib";
    args += "libc18f.lib";
  }
  args += config.includeDirs(Tool::Category::Linker, "-I");
  args += config.customOptions(Tool::Category::Linker);
  args += "-o%O";
  args += "%OBJS";
  args += "%LIBS";
  args += config.customLibraries(Tool::Category::Linker);
  return args;
}

QString SDCC::LinkProjectFile::outputFiles() const
{
  return Process::outputFiles() + " PURL::Lkr PURL::Hex PURL::Cod PURL::Coff PURL::Map";
}
