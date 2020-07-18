/***************************************************************************
 *   Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "picc_compile.h"
#include "picc_compile.moc"

#include "picc_config.h"

//-----------------------------------------------------------------------------
QStringList PICC::Process::genericArguments(const Compile::Config &) const
{
  QStringList args;
  args += "--ERRFORMAT";
  args += "--WARNFORMAT";
  args += "--MSGFORMAT";
  args += "--CHIP=%DEVICE";
  args += "--IDE=mplab";
  args += "-Q"; // suppress copyright message
  return args;
}

void PICC::Process::logLine(::Process::OutputType, const QString &line)
{
  // #### TODO
  doLog(Log::LineType::Normal, line, QString::null, 0);
}

//-----------------------------------------------------------------------------
QStringList PICC::CompileProjectFile::genericArguments(const Compile::Config &config) const
{
  QStringList args = Process::genericArguments(config);
  args += "-C";
  args += config.includeDirs(Tool::Category::Compiler, "-I");
  args += config.customOptions(Tool::Category::Compiler);
  args += "%I";
  return args;
}

QString PICC::CompileProjectFile::outputFiles() const
{
  return "obj PURL::Lst rlf";
}

//-----------------------------------------------------------------------------
QStringList PICC::AssembleProjectFile::genericArguments(const Compile::Config &config) const
{
  QStringList args = Process::genericArguments(config);
  args += "-C";
  args += "--ASMLIST";
  args += config.includeDirs(Tool::Category::Assembler, "-I");
  args += config.customOptions(Tool::Category::Assembler);
  args += "%I";
  return args;
}

QString PICC::AssembleProjectFile::outputFiles() const
{
  return "obj PURL::Lst rlf sdb";
}

//-----------------------------------------------------------------------------
QStringList PICC::LinkProject::genericArguments(const Compile::Config &config) const
{
  QStringList args = Process::genericArguments(config);
  args += "-O%O";
  args += "-M%MAP";
  args += "-G%SYM";
  if ( _data.linkingType==Compile::Icd2DebugLinking ) args += "--DEBUGGER=icd2";
  args += config.customOptions(Tool::Category::Linker);
  args += "%OBJS";
  args += "%LIBS";
  args += config.customLibraries(Tool::Category::Linker);
  return args;
}

QString PICC::LinkProject::outputFiles() const
{
  return "PURL::Map PURL::Hex PURL::Coff sym hxl";
}

//-----------------------------------------------------------------------------
QStringList PICC::LibraryProject::genericArguments(const Compile::Config &config) const
{
  QStringList args;
  args += config.customOptions(Tool::Category::Librarian);
  args += "r";
  args += "%O";
  args += "%OBJS";
  args += "%LIBS";
  return args;
}

QString PICC::LibraryProject::outputFiles() const
{
  return "PURL::Library";
}
