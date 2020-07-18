/***************************************************************************
 *   Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "picc.h"

#include <qregexp.h>

#include "picc_compile.h"
#include "picc_config.h"
#include "devices/pic/pic/pic_memory.h"
#include "devices/list/device_list.h"
#include "common/global/process.h"

//----------------------------------------------------------------------------
QStringList PICC::Base::checkExecutableOptions(Tool::ExecutableType) const
{
  QStringList options;
  options += "--ver";
  return options;
}

bool PICC::Base::checkExecutableResult(Tool::ExecutableType, QStringList &lines) const
{
  return lines.join(" ").contains("HI-TECH");
}

QString PICC::BaseLite::baseExecutable(Tool::ExecutableType, Tool::OutputExecutableType) const
{
  if ( _category.type()==Tool::Category::Librarian ) return "libr";
  return "picl";
}

QString PICC::BaseNormal::baseExecutable(Tool::ExecutableType, Tool::OutputExecutableType) const
{
  if ( _category.type()==Tool::Category::Librarian ) return "libr";
  return "picc";
}

QString PICC::Base18::baseExecutable(Tool::ExecutableType, Tool::OutputExecutableType) const
{
  if ( _category.type()==Tool::Category::Librarian ) return "libr";
  return "picc18";
}

QString PICC::BaseDs::baseExecutable(Tool::ExecutableType, Tool::OutputExecutableType) const
{
  if ( _category.type()==Tool::Category::Librarian ) return "libr";
  return "dspicc";
}

//----------------------------------------------------------------------------
QStringList PICC::Group::checkDevicesOptions(uint) const
{
  QStringList options;
  options += "--CHIPINFO";
  return options;
}

QValueList<const Device::Data *> PICC::Group::getSupportedDevices(const QString &s) const
{
  QValueList<const Device::Data *> list;
  QStringList lines = QStringList::split('\n', s);
  for (int i=0; i<lines.count(); i++) {
    QRegExp re("([A-Za-z0-9]+):.*");
    if ( !re.exactMatch(lines[i]) ) continue;
    const Device::Data* data = Device::Lister::instance().data(re.cap(1));
    if (data != NULL) list.append(data);
  }
  return list;
}

Compile::Process *PICC::Group::processFactory(const Compile::Data &data) const
{
  switch (data.category.type()) {
    case Tool::Category::Compiler:
      return new PICC::CompileProjectFile;
    case Tool::Category::Assembler:
      return new PICC::AssembleProjectFile;
    case Tool::Category::Linker:
      ASSERT(data.project);
      return new PICC::LinkProject;
    case Tool::Category::Librarian:
      ASSERT(data.project);
      return new PICC::LibraryProject;
    default: break;
  }
  ASSERT(false);
  return 0;
}

Compile::Config *PICC::Group::configFactory(::Project& project) const
{
  return new Config(project);
}

PURL::FileType PICC::Group::implementationType(PURL::ToolType type) const
{
  if ( type==PURL::ToolType::Assembler ) return PURL::AsmPICC;
  if ( type==PURL::ToolType::Compiler ) return PURL::CSource;
  return PURL::Nb_FileTypes;
}

bool PICC::Group::needs(Tool::Category category) const
{
  if (category == Tool::Category::Assembler) return false;
  return Tool::Group::needs(category);
}

//----------------------------------------------------------------------------
QString PICC::PICCLiteGroup::informationText() const
{
  return i18n("<a href=\"%1\">PICC-Lite</a> is a freeware C compiler distributed by HTSoft.").arg("http://www.htsoft.com");
}

Tool::Base* PICC::PICCLiteGroup::baseFactory(Tool::Category category) const
{
  if ( category==Tool::Category::Compiler || category==Tool::Category::Assembler ) return new BaseLite;
  if ( category==Tool::Category::Linker || category==Tool::Category::Librarian )   return new BaseLite;
  return NULL;
}

//----------------------------------------------------------------------------
QString PICC::PICCGroup::informationText() const
{
  return i18n("<a href=\"%1\">PICC</a> is a C compiler distributed by HTSoft.").arg("http://www.htsoft.com");
}

Tool::Base* PICC::PICCGroup::baseFactory(Tool::Category category) const
{
  if ( category==Tool::Category::Compiler || category==Tool::Category::Assembler ) return new BaseNormal;
  if ( category==Tool::Category::Linker || category==Tool::Category::Librarian )   return new BaseNormal;
  return NULL;
}

//----------------------------------------------------------------------------
QString PICC::PICC18Group::informationText() const
{
  return i18n("<a href=\"%1\">PICC 18</a> is a C compiler distributed by HTSoft.").arg("http://www.htsoft.com");
}

Tool::Base* PICC::PICC18Group::baseFactory(Tool::Category category) const
{
  if ( category==Tool::Category::Compiler || category==Tool::Category::Assembler ) return new Base18;
  if ( category==Tool::Category::Linker || category==Tool::Category::Librarian )   return new Base18;
  return NULL;
}

//----------------------------------------------------------------------------
QString PICC::DsPICCGroup::informationText() const
{
  return i18n("<a href=\"%1\">dsPICC</a> is a C compiler distributed by HTSoft.").arg("http://www.htsoft.com");
}

Tool::Base* PICC::DsPICCGroup::baseFactory(Tool::Category category) const
{
  if ( category==Tool::Category::Compiler || category==Tool::Category::Assembler ) return new BaseDs;
  if ( category==Tool::Category::Linker || category==Tool::Category::Librarian )   return new BaseDs;
  return NULL;
}
