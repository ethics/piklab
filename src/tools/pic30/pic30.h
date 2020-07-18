/***************************************************************************
 *   Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef PIC30_H
#define PIC30_H

#include "tools/base/tool_group.h"

namespace PIC30
{

//----------------------------------------------------------------------------
class Base : public Tool::Base
{
public:
  virtual QString baseExecutable(Tool::ExecutableType execType, Tool::OutputExecutableType type) const;

private:
  virtual QStringList checkExecutableOptions(Tool::ExecutableType execType) const;
  virtual bool checkExecutableResult(Tool::ExecutableType execType, QStringList &lines) const;
};

//----------------------------------------------------------------------------
class Group : public Tool::Group
{
public:
  virtual QString name() const { return "pic30"; }
  virtual QString label() const { return i18n("PIC30 Toolchain"); }
  virtual QString informationText() const;
  virtual bool hasDirectory(Compile::DirectoryType type) const { return type==Compile::DirectoryType::Header || type==Compile::DirectoryType::LinkerScript || type==Compile::DirectoryType::Library; }
  virtual bool hasOutputExecutableType(Tool::OutputExecutableType type) const { return ( type==Tool::OutputExecutableType::Coff || type==Tool::OutputExecutableType::Elf ); }
  virtual Tool::Category checkDevicesCategory() const { return Tool::Category::Nb_Types; }
  virtual Tool::ExecutableType preferedExecutableType() const { return Tool::ExecutableType::Unix; }
  virtual Tool::CompileFileType compileFileType() const { return Tool::CompileFileType::Separate; }
  virtual PURL::FileType linkerScriptType() const { return PURL::Gld; }
  virtual PURL::FileType implementationType(PURL::ToolType type) const;

private:
  virtual ::Tool::Base* baseFactory(Tool::Category category) const;
  virtual QValueList<const Device::Data*> getSupportedDevices(const QString &s) const;
  virtual Compile::Process* processFactory(const Compile::Data &data) const;
  virtual Compile::Config* configFactory(::Project& project) const;
  virtual Tool::SourceGenerator *sourceGeneratorFactory() const;
  virtual PURL::Directory internalAutodetectDirectory(Compile::DirectoryType type, const PURL::Directory &execDir, Tool::ExecutableType execType) const;
};

} // namespace

#endif
