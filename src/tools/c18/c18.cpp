/***************************************************************************
 *   Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "c18.h"

#include <qdir.h>

#include "c18_compile.h"
#include "c18_config.h"
#include "devices/pic/pic/pic_memory.h"
#include "devices/list/device_list.h"
#include "devices/pic/pic/pic_group.h"
#include "common/global/process.h"

//----------------------------------------------------------------------------
QStringList C18::Compiler::checkExecutableOptions(Tool::ExecutableType) const
{
  QStringList options;
  options += "-v";
  return options;
}

bool C18::Compiler::checkExecutableResult(Tool::ExecutableType, QStringList &lines) const
{
  return ( lines.count()>0 && lines[0].startsWith("MPLAB C18") );
}

QStringList C18::Linker::checkExecutableOptions(Tool::ExecutableType) const
{
  QStringList options;
  options += "/v";
  return options;
}

bool C18::Linker::checkExecutableResult(Tool::ExecutableType, QStringList &lines) const
{
  return ( lines.count()>0 && lines[0].startsWith("MPLINK") );
}

//----------------------------------------------------------------------------
QValueList<const Device::Data *> C18::Group::getSupportedDevices(const QString &) const
{
  QValueList<const Device::Data*> list;
  QStringList devices = Device::Lister::instance().group("pic")->supportedDevices();
  for (QStringList::const_iterator it = devices.begin(); it != devices.end(); ++it) {
    const Device::Data& data = *Device::Lister::instance().data(*it);
    if (static_cast<const Pic::Data&>(data).is18Family() ) list.append(&data);
  }
  return list;
}

Compile::Process *C18::Group::processFactory(const Compile::Data &data) const
{
  if ( data.category==Tool::Category::Compiler ) return new CompileFile;
  ASSERT( data.category==Tool::Category::Linker );
  return new Link;
}

PURL::Directory C18::Group::internalAutodetectDirectory(Compile::DirectoryType type, const PURL::Directory &execDir, Tool::ExecutableType) const
{
  QDir dir(execDir.path());
  if ( !dir.cdUp() ) return PURL::Directory();
  switch (type.type()) {
    case Compile::DirectoryType::LinkerScript:
      if ( dir.cd("lkr") ) return dir.path();
      break;
    case Compile::DirectoryType::Header:
      if ( dir.cd("h") ) return dir.path();
      break;
    case Compile::DirectoryType::Library:
      if ( dir.cd("lib") ) return dir.path();
      break;
    case Compile::DirectoryType::Executable:
    case Compile::DirectoryType::Source:
    case Compile::DirectoryType::Nb_Types: ASSERT(false); break;
  }
  return PURL::Directory();
}

Compile::Config *C18::Group::configFactory(::Project& project) const
{
  return new Config(project);
}

Tool::Base* C18::Group::baseFactory(Tool::Category category) const
{
  if ( category==Tool::Category::Compiler ) return new Compiler;
  if ( category==Tool::Category::Linker )   return new Linker;
  return NULL;
}

QString C18::Group::informationText() const
{
  return i18n("<qt><a href=\"%1\">C18</a> is a C compiler distributed by Microchip.</qt>").arg("http://www.microchip.com/stellent/idcplg?IdcService=SS_GET_PAGE&nodeId=1406&dDocName=en010014&part=SW006011");
}
