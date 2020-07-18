/***************************************************************************
 *   Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "boost.h"

#include <qregexp.h>

#include "boostc_compile.h"
#include "boost_config.h"
#include "devices/pic/pic/pic_memory.h"
#include "devices/list/device_list.h"
#include "devices/base/device_group.h"

//----------------------------------------------------------------------------
QString Boost::Base::baseExecutable(Tool::ExecutableType, Tool::OutputExecutableType) const
{
  QString exec = execName();
  if (static_cast<const Group*>(_group)->newExecutableNames()) {
      int i = exec.find('.');
      if (i !=-1) {
          exec.replace(i, 1, "_");
      }
   }
   return exec;
}

//----------------------------------------------------------------------------
bool Boost::Linker::checkExecutableResult(Tool::ExecutableType, QStringList &lines) const
{
  return ( lines.count()>0 && lines[0].startsWith("BoostLink ") );
}

PURL::Directory Boost::Linker::checkExecutableWorkingDirectory() const
{
  return static_cast<const Group *>(_group)->checkExecutableUrl().directory();
}

QStringList Boost::Linker16::checkExecutableOptions(Tool::ExecutableType) const
{
  // #### otherwise may stall...
  QStringList args;
  args += "-t PIC16F873";
  args += static_cast<const Group *>(_group)->checkExecutableUrl().toExtension("obj").filename();
  return args;
}

QStringList Boost::Linker18::checkExecutableOptions(Tool::ExecutableType) const
{
  // #### otherwise may stall...
  QStringList args;
  args += "-t PIC18F452";
  args += static_cast<const Group *>(_group)->checkExecutableUrl().toExtension("obj").filename();
  return args;
}

//----------------------------------------------------------------------------
QStringList Boost::Compiler::checkExecutableOptions(Tool::ExecutableType) const
{
  // #### otherwise may stall...
  QStringList options;
  options += static_cast<const Group *>(_group)->checkExecutableUrl().filename();
  return options;
}

PURL::Directory Boost::Compiler::checkExecutableWorkingDirectory() const
{
  return static_cast<const Group *>(_group)->checkExecutableUrl().directory();
}

//----------------------------------------------------------------------------
Boost::Group::Group(const QString &extension, const QString &text)
: _checkExecTmp(_sview, extension),
  _newExecutableNames(false)
{
  if ( _checkExecTmp.openForWrite() ) _checkExecTmp.appendText(text);
  _checkExecTmp.close();
}

QString Boost::Group::comment() const
{
  return i18n("The Boost toolchain needs to be run by Wine with \"Windows NT 4.0\" compatibility. This can be configured with the Wine configuration utility.");
}

PURL::Directory Boost::Group::internalAutodetectDirectory(Compile::DirectoryType type, const PURL::Directory &execDir, Tool::ExecutableType) const
{
  if ( type==Compile::DirectoryType::Library ) return execDir.path() + "Lib";
  return PURL::Directory();
}

Compile::Process *Boost::Group::processFactory(const Compile::Data &data) const
{
  switch (data.category.type()) {
    case Tool::Category::Compiler: return new Boost::CompileFile;
    case Tool::Category::Linker: return new Boost::Link;
    default: break;
  }
  ASSERT(false);
  return 0;
}

Compile::Config *Boost::Group::configFactory(::Project& project) const
{
  return new Boost::Config(project);
}

QValueList<const Device::Data *> Boost::Group::getSupportedDevices(const QString &) const
{
  QValueList<const Device::Data*> list;
  QStringList devices = Device::Lister::instance().group("pic")->supportedDevices();
  for (QStringList::const_iterator it = devices.begin(); it != devices.end(); ++it) {
    const Device::Data& data = *Device::Lister::instance().data(*it);
    if (supportedArchitecture(static_cast<const Pic::Data&>(data).architecture())) list.append(&data);
  }
  return list;
}

VersionData Boost::Group::getToolchainVersion()
{
    QStringList lines;
    for (uint i=0; i<2; ++i) {
        _newExecutableNames = (i == 0);
        if (checkExecutable(Tool::Category::Linker, lines)) {
            QRegExp regexp("Boost.*Version\\s+([0-9]+)\\.([0-9]+).*");
            for (int i=0; i<lines.count(); ++i) {
                if (regexp.exactMatch(lines[i])) {
                    return VersionData(regexp.cap(1).toUInt(), regexp.cap(2).toUInt(), 0);
                }
            }
        }
    }
    return VersionData(0, 0, 0);
}
