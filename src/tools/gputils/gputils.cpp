/***************************************************************************
 *   Copyright (C) 2005-2007 Nicolas Hadacek <hadacek@kde.org>             *
 *   Copyright (C) 2003-2004 Alain Gibaud <alain.gibaud@free.fr>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "gputils.h"

#include <qregexp.h>

#include "gputils_compile.h"
#include "gputils_config.h"
#include "devices/pic/pic/pic_memory.h"
#include "common/global/process.h"
#include "gputils_generator.h"

//----------------------------------------------------------------------------
QString GPUtils::Base::baseExecutable(Tool::ExecutableType, Tool::OutputExecutableType) const
{
  switch (_category.type()) {
    case Tool::Category::Assembler: return "gpasm";
    case Tool::Category::Linker:    return "gplink";
    case Tool::Category::Librarian: return "gplib";
    default: break;
  }
  return QString::null;
}

QStringList GPUtils::Base::checkExecutableOptions(Tool::ExecutableType) const
{
  QStringList options;
  options += "-v";
  return options;
}

bool GPUtils::Base::checkExecutableResult(Tool::ExecutableType execType, QStringList &lines) const
{
  return ( lines.count()>0 && lines[0].startsWith(baseExecutable(execType, Tool::OutputExecutableType::Coff)) );
}

//----------------------------------------------------------------------------
QString GPUtils::Group::informationText() const
{
  return i18n("<a href=\"%1\">GPUtils</a> is an open-source assembler and linker suite.<br>").arg("http://gputils.sourceforge.net");
}

QStringList GPUtils::Group::checkDevicesOptions(uint) const
{
  QStringList options;
  options += "-l";
  return options;
}

Tool::Base* GPUtils::Group::baseFactory(Tool::Category category) const
{
  if ( category==Tool::Category::Assembler ) return new ::GPUtils::Base;
  if ( category==Tool::Category::Linker || category==Tool::Category::Librarian ) return new ::GPUtils::Base;
  return NULL;
}

PURL::Directory GPUtils::Group::internalAutodetectDirectory(Compile::DirectoryType type, const PURL::Directory &execDir, Tool::ExecutableType execType) const
{
  switch (type.type()) {
    case Compile::DirectoryType::LinkerScript: {
      QString exec = execDir.path() + base(Tool::Category::Linker)->baseExecutable(execType, Tool::OutputExecutableType::Coff);
      ::Process::StringOutput process;
      QStringList options;
      options += "-h";
      process.setup(exec, options, execType==Tool::ExecutableType::Windows);
      process.execute(1000);
      if ( process.state()!=::Process::Exited ) return PURL::Directory();
      QString s = process.allOutputs();
      QRegExp re(".*Default linker script path ([^\\n]*)\\n.*");
      if ( !re.exactMatch(s) ) return PURL::Directory();
      return PURL::Directory(re.cap(1));
    }
    case Compile::DirectoryType::Header: {
      QString exec = execDir.path() + base(Tool::Category::Assembler)->baseExecutable(execType, Tool::OutputExecutableType::Coff);
      ::Process::StringOutput process;
      QStringList options;
      options += "-h";
      process.setup(exec, options, execType==Tool::ExecutableType::Windows);
      process.execute(1000);
      if ( process.state()!=::Process::Exited ) return PURL::Directory();
      QString s = process.allOutputs();
      QRegExp re(".*Default header file path ([^\\n]*)\\n.*");
      if ( !re.exactMatch(s) ) return PURL::Directory();
      return PURL::Directory(re.cap(1));
    }
    case Compile::DirectoryType::Executable:
    case Compile::DirectoryType::Library:
    case Compile::DirectoryType::Source:
    case Compile::DirectoryType::Nb_Types: break;
  }
  return PURL::Directory();
}

Compile::Process *GPUtils::Group::processFactory(const Compile::Data &data) const
{
  switch (data.category.type()) {
    case Tool::Category::Assembler:
      return new GPUtils::AssembleProjectFile;
    case Tool::Category::Linker:
      return new GPUtils::LinkProject;
    case Tool::Category::Librarian:
      return new GPUtils::LibraryProject;
    default: break;
  }
  ASSERT(false);
  return 0;
}

Compile::Config *GPUtils::Group::configFactory(::Project& project) const
{
  return new Config(project);
}

Tool::SourceGenerator *GPUtils::Group::sourceGeneratorFactory() const
{
  return new SourceGenerator;
}
