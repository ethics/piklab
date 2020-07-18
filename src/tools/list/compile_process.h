/***************************************************************************
 *   Copyright (C) 2005-2006 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef COMPILE_PROCESS_H
#define COMPILE_PROCESS_H

#include <memory>

#include "common/gui/global_ui.h"
#include "common/global/purl.h"
#include "common/global/process.h"
#include "common/common/args.h"
#include "libgui/log_view.h"
#include "devices/base/hex_buffer.h"
#include "tools/base/tool_group.h"
#include "tools/base/generic_tool.h"
#include "libgui/main_global.h"
#include "compile_config.h"
namespace Process { class LineSignal; }

namespace Tool
{
BEGIN_DECLARE_ENUM(OutputType)
  Executable = 0, Library
END_DECLARE_ENUM_KEY_LABEL(OutputType)

} // namespace

namespace Compile
{
class Manager;

enum FileAction { NoAction = 0, Show = 1, InProject = 2, Generated = 8, Included = 16 };
Q_DECLARE_FLAGS(FileActions, FileAction)
Q_DECLARE_OPERATORS_FOR_FLAGS(FileActions)

class FileData {
public:
  FileData() {}
  FileData(const PURL::Url &u, FileActions a) : url(u), actions(a) {}
  PURL::Url url;
  FileActions actions;
  class List : public QValueList<FileData> {
  public:
    List() {}
    List(const FileData &data) { append(data); }
    List onlyExistingFiles() const;
    void cleanGenerated() const;
  };
};

BEGIN_DECLARE_ENUM(ArgType)
  Common = 0, Normal
END_DECLARE_ENUM_NO_DATA(ArgType)

BEGIN_DECLARE_ENUM(CommonArgType)
  SourcePath = 0, OutputFilepath, ProjectName, DeviceName, RelativeInputFilepath
END_DECLARE_ENUM(CommonArgType, CustomArgument::Data)

BEGIN_DECLARE_ENUM(NormalArgType)
  ReplaceIfCustomLinkerScript = 0, ReplaceIfWine,
  AdditionnalObjects, AdditionnalLibraries, DeviceFamilyName,
  LinkerScriptPath, LinkerScriptBasename, LinkerScriptFilename,
  CoffFilename, MapFilename, SymFilename, ListFilename, ObjectFilename,
  Separator
END_DECLARE_ENUM(NormalArgType, CustomArgument::Data)

class ParseErrorData {
public:
  ParseErrorData(const QString &p, int iFile, int iLine, int iMessage, uint iLogType, Log::LineType dLineType)
    : pattern(p), indexFile(iFile), indexLine(iLine), indexMessage(iMessage), indexLogType(iLogType),
      defaultLineType(dLineType) {}
  QString pattern;
  int indexFile, indexLine, indexMessage, indexLogType;
  Log::LineType defaultLineType;
};

class Item {
public:
  enum Type { NotGenerated, Generated };
  Item() {}
  Item(const PURL::Url &u, Type t) : url(u), type(t) {}
  PURL::Url url;
  Type type;
};

enum Operation { Build, Clean, CreateDebugInfo };

class Data {
public:
  Data()
    : category(Tool::Category::Nb_Types), project(NULL) {}
  Data(Tool::Category c, const QValueList<Item> &i, const QString &d, const Project& p, LinkingType lt, Operation o)
    : category(c), items(i), device(d), project(&p), linkingType(lt), operation(o) {}
  Tool::Category category;
  QValueList<Item> items;
  QString device;
  const Project *project;
  LinkingType linkingType;
  Operation operation;
};

//-----------------------------------------------------------------------------
class LogData
{
public:
  LogData() {}
  LogData(const PURL::Url &u, uint l) : url(u), line(l) {}
  PURL::Url url;
  uint line;
};

class LogWidget : public Log::Widget
{
Q_OBJECT
public:
public:
  LogWidget(QWidget *parent);
  void appendLine(Log::LineType type, const QString &message, const LogData &data);
  virtual void clear();

protected:
  virtual void lineClicked(int line);

private:
  QMap<int, LogData> _map;
};

//-----------------------------------------------------------------------------
class GenericProcess : public QObject, public CustomArgument::Processor
{
Q_OBJECT
public:
  GenericProcess();

protected:
  ::Process::LineSignal* _process;

  const Tool::Group& group() const { return Main::toolGroup(); }
  virtual uint nbFiles() const = 0;
  virtual PURL::Directory directory(uint i = 0) const;
  virtual PURL::Url url(PURL::FileType type = PURL::Nb_FileTypes, uint i = 0) const;
  QString filepath(PURL::FileType type, uint i=0) const;
  virtual QString outputFilepath() const;
  virtual QString inputFilepath(uint i) const { return filepath(PURL::Nb_FileTypes, i); }
  virtual QString deviceName() const = 0;
  virtual QString libraryExtension() const { return "lib"; }
  virtual QStringList replaceAllValue(const IdData &idata) const;
  virtual bool replaceIfCondition(const IdData &idata) const;
  virtual QString replaceValue(const IdData &idata) const;
  virtual QString postReplaceValue(const IdData &idata, const QString &arg) const;
};

//-----------------------------------------------------------------------------
class BaseProcess : public GenericProcess
{
Q_OBJECT
public:
  BaseProcess();
  virtual void init(const Data &data, Manager *manager);
  virtual bool check() const = 0;
  virtual FileData::List files(bool *ok) const;
  void start();

signals:
  void success();
  void failure();

protected:
  Manager *_manager;
  Data    _data;
  QString _stdout, _stderr;

  virtual void setupProcess() = 0;
  virtual PURL::Url url(PURL::FileType type = PURL::Nb_FileTypes, uint i = 0) const;
  virtual QString outputFilepath() const;
  virtual QString outputFiles() const = 0;
  virtual uint nbFiles() const { return _data.items.count(); }
  FileData fileData(PURL::FileType type, FileActions actions) const;

protected slots:
  virtual void logLine(::Process::OutputType type, const QString &line) = 0;
  virtual void done();
  void error();
  void timeout();
};

//-----------------------------------------------------------------------------
class Process : public BaseProcess
{
Q_OBJECT
public:
  Process();
  virtual void init(const Data &data, Manager *manager);
  virtual bool check() const;
  virtual QStringList genericArguments(const Compile::Config &config) const = 0;
  void checkArguments() const;

protected:
  std::auto_ptr<Config> _config;

  virtual QString familyName() const { return QString::null; }
  virtual QString objectExtension() const { return QString::null; }
  virtual bool hasLinkerScript() const { return group().hasCustomLinkerScript(*_data.project); }
  bool parseErrorLine(const QString &s, const ParseErrorData &data);
  virtual Log::LineType filterType(const QString &type) const;
  void doLog(const QString &type, const QString &message, const QString &surl, uint line);
  void doLog(Log::LineType type, const QString &message, const QString &surl, uint line);
  virtual void setupProcess();
  QStringList arguments() const;
  const Tool::Base *tool() const { return group().base(_data.category); }

  virtual QStringList replaceAllValue(const IdData &idata) const;
  virtual bool replaceIfCondition(const IdData &idata) const;
  virtual QString replaceValue(const IdData &idata) const;
  virtual QString postReplaceValue(const IdData &idata, const QString &arg) const;
};

} // namespace

#endif
