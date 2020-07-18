/***************************************************************************
 *   Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef PIC30_COMPILE_H
#define PIC30_COMPILE_H

#include "tools/list/compile_process.h"

namespace PIC30
{
//-----------------------------------------------------------------------------
class Process : public Compile::Process
{
Q_OBJECT
protected:
  virtual QString deviceName() const { return _data.device; }
};

//-----------------------------------------------------------------------------
class CompileFile : public Process
{
Q_OBJECT
protected:
  virtual QStringList genericArguments(const Compile::Config &config) const;
  virtual QString outputFiles() const;
  virtual void logLine(::Process::OutputType type, const QString &line);
};

//-----------------------------------------------------------------------------
class CompileProjectFile : public CompileFile
{
Q_OBJECT
protected:
  virtual QStringList genericArguments(const Compile::Config &config) const;
};

//-----------------------------------------------------------------------------
class AssembleFile : public Process
{
Q_OBJECT
protected:
  virtual QString outputFilepath() const { return filepath(PURL::Object); }
  virtual QStringList genericArguments(const Compile::Config &config) const;
  virtual QString outputFiles() const;
  virtual void logLine(::Process::OutputType type, const QString &line);
};

//-----------------------------------------------------------------------------
class AssembleProjectFile : public AssembleFile
{
Q_OBJECT
protected:
  virtual QStringList genericArguments(const Compile::Config &config) const;
};

//-----------------------------------------------------------------------------
class Link : public Process
{
Q_OBJECT
protected:
  virtual QString outputFilepath() const;
  virtual QStringList genericArguments(const Compile::Config &config) const;
  virtual QString outputFiles() const;
  virtual void logLine(::Process::OutputType type, const QString &line);
};

//-----------------------------------------------------------------------------
class LinkProject : public Link
{
Q_OBJECT
protected:
  virtual QStringList genericArguments(const Compile::Config &config) const;
};

//-----------------------------------------------------------------------------
class LibraryProject : public Process
{
Q_OBJECT
protected:
  virtual QStringList genericArguments(const Compile::Config &config) const;
  virtual QString outputFiles() const;
  virtual void logLine(::Process::OutputType type, const QString &line);
};

//-----------------------------------------------------------------------------
class BinToHex : public Process
{
Q_OBJECT
protected:
  virtual QString inputFilepath(uint i) const;
  virtual QStringList genericArguments(const Compile::Config &config) const;
  virtual QString outputFiles() const;
  virtual void logLine(::Process::OutputType type, const QString &line);
};

} // namespace

#endif
