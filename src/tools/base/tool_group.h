/***************************************************************************
 *   Copyright (C) 2005-2007 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef TOOL_GROUP_H
#define TOOL_GROUP_H

#include "common/global/purl.h"
#include "common/common/group.h"
#include "common/common/version_data.h"
#include "generic_tool.h"
#include "coff/base/disassembler.h"

//-----------------------------------------------------------------------------
namespace Compile
{
class Config;
class Process;
class Manager;
class Data;
enum LinkingType { NormalLinking, DebugLinking, Icd2DebugLinking };

BEGIN_DECLARE_ENUM(DirectoryType)
  Executable = 0, Header, LinkerScript, Library, Source
END_DECLARE_ENUM_KEY_LABEL(DirectoryType)

} // namespace

//-----------------------------------------------------------------------------
namespace Tool
{
class SourceGenerator;

BEGIN_DECLARE_ENUM(CompileFileType)
 Separate = 0, All, Single
END_DECLARE_ENUM_NO_DATA(CompileFileType)

//-----------------------------------------------------------------------------
class Group : public ::Group::Base
{
public:
  static const char *CUSTOM_NAME;
  Group();
  bool isCustom() const { return ( name()==CUSTOM_NAME ); }
  virtual QString comment() const { return QString::null; }
  virtual void init();
  virtual const ::Tool::Base *base(Category category) const { return _bases[category]; }
  virtual QString informationText() const = 0;
  virtual ExecutableType preferedExecutableType() const = 0;
  virtual bool hasDirectory(Compile::DirectoryType) const { return false; }
  virtual PURL::FileType linkerScriptType() const { return PURL::Nb_FileTypes; }
  PURL::Directory autodetectDirectory(Compile::DirectoryType type, const PURL::Directory &execDir, Tool::ExecutableType execType) const;
  virtual bool hasOutputExecutableType(Tool::OutputExecutableType type) const { return ( type==Tool::OutputExecutableType::Coff ); }
  virtual uint nbCheckDevices() const { return 1; }
  bool hasCheckDevicesError() const { return _checkDevicesError; }
  virtual Tool::Category checkDevicesCategory() const = 0;
  virtual QStringList checkDevicesOptions(uint) const { return QStringList(); }
  ::Process::LineOutput *checkDevicesProcess(uint i, const PURL::Directory &execDir, Tool::ExecutableType execType) const;
  virtual QValueList<const Device::Data *> getSupportedDevices(const QString &s) const = 0;
  virtual CompileFileType compileFileType() const = 0;
  virtual bool needs(Category category) const;
  Compile::Process *createCompileProcess(const Compile::Data &data, Compile::Manager *manager) const;
  Compile::Config *createConfig(::Project& project) const;
  bool hasCustomLinkerScript(const ::Project& project) const;
  virtual PURL::Url linkerScript(const ::Project& project, Compile::LinkingType type) const;
  virtual PURL::FileType implementationType(PURL::ToolType type) const = 0;
  virtual Compile::Process *processFactory(const Compile::Data &data) const = 0;
  const SourceGenerator *sourceGenerator() const { return _sourceGenerator; }
  bool check(const QString &device, Log::Generic *log) const;
  const VersionData &version() const { return _version; }
  virtual bool generateCompleteDebugInfo(const QString &device) const { Q_UNUSED(device); return true; }
  virtual QString auxiliaryDebugInfoGenerator() const { return QString::null; }
  virtual VersionData getToolchainVersion() { return VersionData(); }

protected:
  virtual void initSupported();
  virtual ::Tool::Base* baseFactory(Category category) const = 0;
  virtual QString defaultLinkerScriptFilename(Compile::LinkingType type, const QString &device) const;
  virtual Compile::Config *configFactory(::Project& project) const = 0;
  virtual SourceGenerator *sourceGeneratorFactory() const = 0;
  bool checkExecutable(Tool::Category category, QStringList &lines);
  virtual PURL::Directory internalAutodetectDirectory(Compile::DirectoryType type, const PURL::Directory &execDir, Tool::ExecutableType execType) const = 0;

private:
  SourceGenerator *_sourceGenerator;
  QMap<Category, ::Tool::Base*> _bases;
  mutable QMap<Compile::DirectoryType, PURL::Directory> _dirs;
  bool _checkDevicesError;
  VersionData _version;
};

} // namespace

#endif
