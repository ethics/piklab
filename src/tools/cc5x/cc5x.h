/***************************************************************************
 *   Copyright (C) 2007 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef CC5X_H
#define CC5X_H

#include "tools/base/tool_group.h"
#include "common/global/pfile.h"

namespace CC5X
{
//----------------------------------------------------------------------------
class Base : public Tool::Base
{
public:
  virtual QString baseExecutable(Tool::ExecutableType, Tool::OutputExecutableType) const { return "cc5x"; }

private:
  virtual QStringList checkExecutableOptions(Tool::ExecutableType) const { return QStringList(); }
  virtual bool checkExecutableResult(Tool::ExecutableType execType, QStringList &lines) const;
};

//----------------------------------------------------------------------------
class Group : public Tool::Group
{
public:
  virtual QString name() const { return "cc5x"; }
  virtual QString label() const { return i18n("CC5X Compiler"); }
  virtual QString informationText() const;
  virtual Tool::Category checkDevicesCategory() const { return Tool::Category::Nb_Types; }
  virtual Tool::ExecutableType preferedExecutableType() const { return Tool::ExecutableType::Windows; }
  virtual Tool::CompileFileType compileFileType() const { return Tool::CompileFileType::Single; }
  virtual PURL::FileType implementationType(PURL::ToolType type) const { return (type==PURL::ToolType::Compiler ? PURL::CSource : PURL::Nb_FileTypes); }
  virtual QString auxiliaryDebugInfoGenerator() const { return "gputils"; }

private:
  virtual QValueList<const Device::Data *> getSupportedDevices(const QString &s) const;
  virtual Compile::Process *processFactory(const Compile::Data &data) const;
  virtual Compile::Config *configFactory(::Project& project) const;
  virtual ::Tool::Base* baseFactory(Tool::Category) const;
  virtual Tool::SourceGenerator *sourceGeneratorFactory() const { /*TODO*/ return NULL; }
  virtual PURL::Directory internalAutodetectDirectory(Compile::DirectoryType, const PURL::Directory &, Tool::ExecutableType) const { return PURL::Directory(); }
};

} // namespace

#endif
