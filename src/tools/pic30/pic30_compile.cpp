/***************************************************************************
 *   Copyright (C) 2006-2007 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "pic30_compile.h"
#include "pic30_compile.moc"

#include "config.h"
#include "pic30_config.h"
#include "devices/list/device_list.h"

//-----------------------------------------------------------------------------
QStringList PIC30::CompileFile::genericArguments(const Compile::Config &config) const
{
  QStringList args;
  args += "-S"; // compile only
  args += "-mcpu=%DEVICE";
  args += config.includeDirs(Tool::Category::Compiler, "-I");
  PURL::Directory purl = Compile::Config::directory(group(), Compile::DirectoryType::Header);
  if ( !purl.isEmpty() ) args += "-I" + purl.path();
  args += config.customOptions(Tool::Category::Compiler);
  return args;
}

QString PIC30::CompileFile::outputFiles() const
{
  return "PURL::AsmPIC30";
}

void PIC30::CompileFile::logLine(::Process::OutputType, const QString &line)
{
  // #### TODO
  doLog(Log::LineType::Normal, line, QString::null, 0); // unrecognized
}

//-----------------------------------------------------------------------------
QStringList PIC30::CompileProjectFile::genericArguments(const Compile::Config &config) const
{
  QStringList args = CompileFile::genericArguments(config);
  args += "-g";
  args += "%I";
  return args;
}

//-----------------------------------------------------------------------------
QStringList PIC30::AssembleFile::genericArguments(const Compile::Config &config) const
{
  QStringList args;
  args += "-g";
  args += "-a=%LIST"; // listing
  args += "-o%O";
  args += config.includeDirs(Tool::Category::Assembler, "-I");
  args += config.customOptions(Tool::Category::Assembler);
  return args;
}

QString PIC30::AssembleFile::outputFiles() const
{
  return "PURL::Object PURL::Lst";
}

void PIC30::AssembleFile::logLine(::Process::OutputType, const QString &line)
{
  // #### TODO
  doLog(Log::LineType::Normal, line, QString::null, 0); // unrecognized
}

//-----------------------------------------------------------------------------
QStringList PIC30::AssembleProjectFile::genericArguments(const Compile::Config &config) const
{
  QStringList args = AssembleFile::genericArguments(config);
  if ( !_data.items.empty() && _data.items[0].type==Compile::Item::NotGenerated ) args += "-p%DEVICE";
  args += "%I";
  return args;
}

//-----------------------------------------------------------------------------
QString PIC30::Link::outputFilepath() const
{
  PURL::FileType type = Compile::Config::outputExecutableType(group()).data().type;
  return filepath(type);
}

QStringList PIC30::Link::genericArguments(const Compile::Config &config) const
{
  QStringList args;
  args += "-Map";
  args += "%MAP";
  args += "-o%O";
  args += config.includeDirs(Tool::Category::Linker, "-L");
  PURL::Directory purl = Compile::Config::directory(group(), Compile::DirectoryType::Library);
  if ( !purl.isEmpty() ) args += "-L" + purl.path();
  args += "$LKR(-T%LKR)";
  args += config.customOptions(Tool::Category::Linker);
  args += "%OBJS";
  return args;
}

QString PIC30::Link::outputFiles() const
{
  PURL::FileType type = Compile::Config::outputExecutableType(group()).data().type;
  return QString("PURL::Map PURL::") + type.key();
}

void PIC30::Link::logLine(::Process::OutputType, const QString &line)
{
  // #### TODO
  doLog(Log::LineType::Normal, line, QString::null, 0); // unrecognized
}

//-----------------------------------------------------------------------------
QStringList PIC30::LinkProject::genericArguments(const Compile::Config &config) const
{
  QStringList args = Link::genericArguments(config);
  args += "%LIBS";
  args += config.customLibraries(Tool::Category::Linker);
  return args;
}

//-----------------------------------------------------------------------------
QStringList PIC30::LibraryProject::genericArguments(const Compile::Config &) const
{
  QStringList args;
  args += "-rc"; // insert new + do not warn if creating library
  args += "%O";
  args += "%LIBS";
  args += "%OBJS";
  return args;
}

void PIC30::LibraryProject::logLine(::Process::OutputType, const QString &line)
{
  // #### TODO
  doLog(Log::LineType::Normal, line, QString::null, 0); // unrecognized
}

QString PIC30::LibraryProject::outputFiles() const
{
  return "PURL::Library";
}

//-----------------------------------------------------------------------------
QString PIC30::BinToHex::inputFilepath(uint) const
{
  PURL::FileType type = Compile::Config::outputExecutableType(group()).data().type;
  return filepath(type);
}

QStringList PIC30::BinToHex::genericArguments(const Compile::Config &) const
{
  QStringList args;
  args += "%I";
  return args;
}

QString PIC30::BinToHex::outputFiles() const
{
  return "PURL::Hex";
}

void PIC30::BinToHex::logLine(::Process::OutputType, const QString &line)
{
  // #### TODO
  doLog(Log::LineType::Normal, line, QString::null, 0); // unrecognized
}
