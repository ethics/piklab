/***************************************************************************
 *   Copyright (C) 2006-2007 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef CCSC_H
#define CCSC_H

#include "common/global/pfile.h"
#include "tools/base/tool_group.h"

namespace CCSC
{
//----------------------------------------------------------------------------
class Base : public Tool::Base
{
public:
  virtual QString baseExecutable(Tool::ExecutableType, Tool::OutputExecutableType) const { return "ccsc"; }

private:
  virtual QStringList checkExecutableOptions(Tool::ExecutableType execType) const;
  virtual bool checkExecutableResult(Tool::ExecutableType execType, QStringList &lines) const;
  virtual PURL::Directory checkExecutableWorkingDirectory() const;
};

//----------------------------------------------------------------------------
class Group : public Tool::Group
{
public:
  Group();
  PURL::Url checkExecutableUrl() const { return _checkExecTmp.url(); }
  virtual QString name() const { return "ccsc"; }
  virtual QString label() const { return i18n("CCS Compiler"); }
  virtual QString informationText() const;
  virtual Tool::Category checkDevicesCategory() const { return Tool::Category::Nb_Types; }
  virtual Tool::ExecutableType preferedExecutableType() const { return Tool::ExecutableType::Unix; }
  virtual Tool::CompileFileType compileFileType() const { return Tool::CompileFileType::Single; }
  virtual PURL::FileType implementationType(PURL::ToolType type) const { return (type==PURL::ToolType::Compiler ? PURL::CSource : PURL::Nb_FileTypes); }

private:
  Log::StringView _sview;
  PURL::TempFile  _checkExecTmp;

  virtual QValueList<const Device::Data *> getSupportedDevices(const QString &s) const;
  virtual Compile::Process *processFactory(const Compile::Data &data) const;
  virtual Compile::Config *configFactory(::Project& project) const;
  virtual ::Tool::Base* baseFactory(Tool::Category) const;
  virtual Tool::SourceGenerator *sourceGeneratorFactory() const { /*TODO*/ return NULL; }
  virtual VersionData getToolchainVersion();
  virtual PURL::Directory internalAutodetectDirectory(Compile::DirectoryType, const PURL::Directory &, Tool::ExecutableType) const { return PURL::Directory(); }
};

} // namespace

#endif
