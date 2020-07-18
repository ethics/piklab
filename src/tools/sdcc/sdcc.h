/***************************************************************************
 *   Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef SDCC_H
#define SDCC_H

#include "common/global/pfile.h"
#include "tools/list/tool_list.h"
#include "sdcc_generator.h"

namespace SDCC
{
//----------------------------------------------------------------------------
class Base : public Tool::Base
{
public:
  virtual QString baseExecutable(Tool::ExecutableType, Tool::OutputExecutableType) const { return "sdcc"; }

private:
  virtual QStringList checkExecutableOptions(Tool::ExecutableType) const;
  virtual bool checkExecutableResult(Tool::ExecutableType execType, QStringList &lines) const;
};

//----------------------------------------------------------------------------
class Group : public Tool::Group
{
public:
  Group();
  virtual QString name() const { return "sdcc"; }
  virtual QString label() const { return i18n("Small Device C Compiler"); }
  virtual QString informationText() const;
  virtual const ::Tool::Base *base(Tool::Category category) const;
  virtual uint nbCheckDevices() const { return SDCC::Nb_Families; }
  virtual bool hasDirectory(Compile::DirectoryType type) const { return type==Compile::DirectoryType::Header || type==Compile::DirectoryType::LinkerScript; }
  virtual PURL::FileType linkerScriptType() const { return PURL::Lkr; }
  virtual PURL::Url linkerScript(const ::Project& project, Compile::LinkingType type) const { return Tool::Lister::instance().group("gputils")->linkerScript(project, type); }
  virtual Tool::ExecutableType preferedExecutableType() const { return Tool::ExecutableType::Unix; }
  virtual Tool::CompileFileType compileFileType() const { return Tool::CompileFileType::Separate; }
  virtual Tool::Category checkDevicesCategory() const { return Tool::Category::Compiler; }
  virtual QStringList checkDevicesOptions(uint i) const;
  virtual bool needs(Tool::Category category) const;
  virtual PURL::FileType implementationType(PURL::ToolType type) const;
  virtual bool generateDebugInformation(const QString &device) const;

private:
  Log::StringView _sview;
  PURL::TempFile  _checkDevicesTmp;
  virtual ::Tool::Base* baseFactory(Tool::Category category) const;
  virtual QValueList<const Device::Data *> getSupportedDevices(const QString &s) const { return SDCC::getSupportedDevices(s); }
  virtual Compile::Process *processFactory(const Compile::Data &data) const;
  virtual Compile::Config *configFactory(::Project& project) const;
  virtual Tool::SourceGenerator *sourceGeneratorFactory() const;
  virtual PURL::Directory internalAutodetectDirectory(Compile::DirectoryType type, const PURL::Directory &execDir, Tool::ExecutableType execType) const;
};

} // namespace

#endif
