/***************************************************************************
 *   Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef BOOST_H
#define BOOST_H

#include "common/global/pfile.h"
#include "tools/base/tool_group.h"
#include "devices/pic/base/pic.h"

namespace Boost
{
//----------------------------------------------------------------------------
class Base : public Tool::Base
{
private:
  virtual QString baseExecutable(Tool::ExecutableType, Tool::OutputExecutableType) const;
  virtual QString execName() const = 0;
};

class Linker : public Base
{
private:
  virtual QString execName() const { return "boostlink.pic.exe"; }
  virtual bool checkExecutableResult(Tool::ExecutableType execType, QStringList &lines) const;
  virtual PURL::Directory checkExecutableWorkingDirectory() const;
};

class Linker16 : public Linker
{
private:
  virtual QStringList checkExecutableOptions(Tool::ExecutableType execType) const;
};

class Linker18 : public Linker
{
private:
  virtual QStringList checkExecutableOptions(Tool::ExecutableType execType) const;
};

//----------------------------------------------------------------------------
class Compiler : public Base
{
private:
  virtual QStringList checkExecutableOptions(Tool::ExecutableType execType) const;
  virtual PURL::Directory checkExecutableWorkingDirectory() const;
};

//----------------------------------------------------------------------------
class Group : public Tool::Group
{
public:
  Group(const QString &extension, const QString &text);
  PURL::Url checkExecutableUrl() const { return _checkExecTmp.url(); }
  bool newExecutableNames() const { return _newExecutableNames; }
  virtual QString comment() const;
  virtual Tool::Category checkDevicesCategory() const { return Tool::Category::Nb_Types; }
  virtual Tool::ExecutableType preferedExecutableType() const { return Tool::ExecutableType::Windows; }
  virtual Tool::CompileFileType compileFileType() const { return Tool::CompileFileType::Separate; }
  virtual bool hasDirectory(Compile::DirectoryType type) const { return type==Compile::DirectoryType::Library; }

protected:
  virtual bool supportedArchitecture(Pic::Architecture architecture) const = 0;

private:
  Log::StringView _sview;
  PURL::TempFile  _checkExecTmp;
  bool            _newExecutableNames;

  virtual QValueList<const Device::Data *> getSupportedDevices(const QString &s) const;
  virtual Compile::Process *processFactory(const Compile::Data &data) const;
  virtual Compile::Config *configFactory(::Project& project) const;
  virtual PURL::Directory internalAutodetectDirectory(Compile::DirectoryType type, const PURL::Directory &execDir, Tool::ExecutableType execType) const;
  virtual VersionData getToolchainVersion();
};

} // namespace

#endif
