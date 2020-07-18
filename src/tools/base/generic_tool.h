/***************************************************************************
 *   Copyright (C) 2005-2007 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef GENERIC_TOOL_H
#define GENERIC_TOOL_H

#include "common/global/purl.h"
#include "common/common/key_enum.h"
namespace Device { class Data; class Memory; }
namespace Process { class LineOutput; }
class Project;

namespace Tool
{
//----------------------------------------------------------------------------
class Group;

BEGIN_DECLARE_ENUM(Category)
  Compiler = 0, Assembler, Linker, BinToHex, Librarian
END_DECLARE_ENUM_KEY_LABEL(Category)

struct OutputExecutableTypeData {
  const char *label, *key;
  PURL::FileType type;
};
BEGIN_DECLARE_ENUM(OutputExecutableType)
  Coff = 0, Elf
END_DECLARE_ENUM(OutputExecutableType, OutputExecutableTypeData)

struct ExecutableTypeData {
  const char *key, *label;
  PURL::SeparatorType separator;
};
BEGIN_DECLARE_ENUM(ExecutableType)
  Unix = 0, Windows
END_DECLARE_ENUM(ExecutableType, ExecutableTypeData)

//-----------------------------------------------------------------------------
class Base
{
public:
  Base() : _group(NULL) {}
  virtual ~Base() {}
  const Group &group() const { return *_group; }
  virtual QString baseExecutable(ExecutableType execType, OutputExecutableType outType) const = 0;
  PURL::Directory executableDirectory() const;
  virtual bool checkExecutable() const { return true; }
  virtual QStringList checkExecutableOptions(ExecutableType execType) const = 0;
  virtual PURL::Directory checkExecutableWorkingDirectory() const { return PURL::Directory(); }
  virtual ::Process::LineOutput *checkExecutableProcess(const PURL::Directory &dir, ExecutableType execType, OutputExecutableType outType) const;
  virtual bool checkExecutableResult(ExecutableType execType, QStringList &lines) const = 0;

protected:
  Category _category;
  const Group *_group;

  friend class Group;
};

} // namespace

#endif
