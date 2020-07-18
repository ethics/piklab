/***************************************************************************
 *   Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "pic30.h"

#include "devices/list/device_list.h"
#include "devices/base/device_group.h"
#include "pic30_compile.h"
#include "pic30_config.h"
#include "devices/pic/pic/pic_memory.h"
#include "common/global/process.h"
#include "pic30_generator.h"

//----------------------------------------------------------------------------
QString PIC30::Base::baseExecutable(Tool::ExecutableType, Tool::OutputExecutableType type) const
{
  QString s = "pic30-";
  if ( type==Tool::OutputExecutableType::Coff ) s += "coff";
  else s += "elf";
  s += "-";
  switch (_category.type()) {
    case Tool::Category::Compiler:  return s + "gcc";
    case Tool::Category::Assembler: return s + "as";
    case Tool::Category::Linker:    return s + "ld";
    case Tool::Category::Librarian: return s + "ar";
    case Tool::Category::BinToHex:  return s + "bin2hex";
    case Tool::Category::Nb_Types:  break;
  }
  ASSERT(false);
  return QString::null;
}

QStringList PIC30::Base::checkExecutableOptions(Tool::ExecutableType) const
{
  QStringList options;
  if ( _category!=Tool::Category::BinToHex ) options += "--version";
  return options;
}

bool PIC30::Base::checkExecutableResult(Tool::ExecutableType, QStringList &lines) const
{
  if ( lines.count()==0 ) return false;
  lines[0] = lines[0].stripWhiteSpace();
  switch (_category.type()) {
    case Tool::Category::Compiler:  return lines[0].startsWith("pic30");
    case Tool::Category::Assembler: return lines[0].startsWith("GNU assembler");
    case Tool::Category::Linker:    return lines[0].startsWith("GNU ld");
    case Tool::Category::BinToHex:  return lines.join(" ").contains("Microchip ");
    case Tool::Category::Librarian: return lines[0].startsWith("GNU ar");
    case Tool::Category::Nb_Types:  break;
  }
  ASSERT(false);
  return false;
}

//----------------------------------------------------------------------------
QString PIC30::Group::informationText() const
{
  return i18n("The <a href=\"%1\">PIC30 ToolChain</a> is a toolsuite for 16-bit PICs available from Microchip. Most of it is available under the GNU Public License.").arg("http://microchip.com/stellent/idcplg?IdcService=SS_GET_PAGE&nodeId=1406&dDocName=en010065&part=SW006012");
}

Tool::Base* PIC30::Group::baseFactory(Tool::Category category) const
{
  if ( category==Tool::Category::Compiler || category==Tool::Category::Assembler
       || category==Tool::Category::Linker || category==Tool::Category::BinToHex ) return new ::PIC30::Base;
  if ( category==Tool::Category::Librarian ) return new ::PIC30::Base;
  return NULL;
}

QValueList<const Device::Data *> PIC30::Group::getSupportedDevices(const QString &) const
{
  QValueList<const Device::Data*> list;
  QStringList devices = Device::Lister::instance().group("pic")->supportedDevices();
  for (QStringList::const_iterator it = devices.begin(); it != devices.end(); ++it) {
    const Device::Data& data = *Device::Lister::instance().data(*it);
    if (static_cast<const Pic::Data&>(data).is16bitFamily()) list.append(&data);
  }
  return list;
}

Compile::Process *PIC30::Group::processFactory(const Compile::Data &data) const
{
  switch (data.category.type()) {
    case Tool::Category::Compiler:  return new PIC30::CompileProjectFile;
    case Tool::Category::Assembler: return new PIC30::AssembleProjectFile;
    case Tool::Category::Linker:    return new PIC30::LinkProject;
    case Tool::Category::Librarian: return new PIC30::LibraryProject;
    case Tool::Category::BinToHex:  return new PIC30::BinToHex;
    case Tool::Category::Nb_Types:  break;
  }
  ASSERT(false);
  return NULL;
}

Compile::Config *PIC30::Group::configFactory(::Project& project) const
{
  return new Config(project);
}

PURL::FileType PIC30::Group::implementationType(PURL::ToolType type) const
{
  if ( type==PURL::ToolType::Assembler ) return PURL::AsmPIC30;
  if ( type==PURL::ToolType::Compiler ) return PURL::CSource;
  return PURL::Nb_FileTypes;
}

Tool::SourceGenerator *PIC30::Group::sourceGeneratorFactory() const
{
  return new SourceGenerator;
}

PURL::Directory PIC30::Group::internalAutodetectDirectory(Compile::DirectoryType type, const PURL::Directory &execDir, Tool::ExecutableType execType) const
{
  if ( execType==Tool::ExecutableType::Unix ) return PURL::Directory();
  PURL::Directory baseDir = execDir.up();
  switch (type.type()) {
    case Compile::DirectoryType::LinkerScript: return baseDir.down("support/gld");
    case Compile::DirectoryType::Header: return baseDir.down("support/h");
    case Compile::DirectoryType::Library: return baseDir.down("lib");
    case Compile::DirectoryType::Executable:
    case Compile::DirectoryType::Source:
    case Compile::DirectoryType::Nb_Types: ASSERT(false); break;
  }
  return PURL::Directory();
}
