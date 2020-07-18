/***************************************************************************
 *   Copyright (C) 2005-2010 Nicolas Hadacek <hadacek@kde.org>             *
 *   Copyright (C) 2003-2004 Alain Gibaud <alain.gibaud@free.fr>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "compile_process.h"
#include "compile_process.moc"

#include <qtimer.h>
#include <qregexp.h>

#include "devices/list/device_list.h"
#include "common/global/process.h"
#include "common/gui/misc_gui.h"
#include "libgui/text_editor.h"
#include "libgui/editor_manager.h"
#include "libgui/project.h"
#include "compile_config.h"
#include "compile_manager.h"

//-----------------------------------------------------------------------------
const Tool::OutputType::Data Tool::OutputType::DATA[Nb_Types] = {
  { "executable", I18N_NOOP("Executable") },
  { "library",    I18N_NOOP("Library")    }
};

//-----------------------------------------------------------------------------
const CustomArgument::Data Compile::CommonArgType::DATA[Nb_Types] = {
  { CustomArgument::ArgType::Replace,    "$(SRCPATH)",  I18N_NOOP("Replaced by the source directory.") },
  { CustomArgument::ArgType::Replace,    "%O",          I18N_NOOP("Replaced by the output filename.") },
  { CustomArgument::ArgType::Replace,    "%PROJECT",    I18N_NOOP("Replaced by the project name.") },
  { CustomArgument::ArgType::Replace,    "%DEVICE",     I18N_NOOP("Replaced by the device name.") },
  { CustomArgument::ArgType::ReplaceAll, "%I",          I18N_NOOP("Replaced by the relative input filepath(s).") },
};

const CustomArgument::Data Compile::NormalArgType::DATA[Nb_Types] = {
  { CustomArgument::ArgType::ReplaceIf,  "$LKR(xxx)",  I18N_NOOP("Replaced by \"xxx\" when there is a custom linker script.") },
  { CustomArgument::ArgType::ReplaceIf,  "$WINE(xxx)", I18N_NOOP("Replaced by \"xxx\" when \"wine\" is used.") },
  { CustomArgument::ArgType::ReplaceAll, "%OBJS",      I18N_NOOP("Replaced by the list of additionnal objects.") },
  { CustomArgument::ArgType::ReplaceAll, "%LIBS",      I18N_NOOP("Replaced by the list of additionnal libraries.") },
  { CustomArgument::ArgType::Replace,    "%FAMILY",    I18N_NOOP("Replaced by the device family name (when needed).") },
  { CustomArgument::ArgType::Replace,    "%LKR_PATH",  I18N_NOOP("Replaced by the linker script path.") },
  { CustomArgument::ArgType::Replace,    "%LKR_NAME",  I18N_NOOP("Replaced by the linker script basename.") },
  { CustomArgument::ArgType::Replace,    "%LKR",       I18N_NOOP("Replaced by the linker script filename.") },
  { CustomArgument::ArgType::Replace,    "%COFF",      I18N_NOOP("Replaced by the COFF filename.") },
  { CustomArgument::ArgType::Replace,    "%MAP",       I18N_NOOP("Replaced by the map filename.") },
  { CustomArgument::ArgType::Replace,    "%SYM",       I18N_NOOP("Replaced by the symbol filename.") },
  { CustomArgument::ArgType::Replace,    "%LIST",      I18N_NOOP("Replaced by the list filename.") },
  { CustomArgument::ArgType::Replace,    "%OBJECT",    I18N_NOOP("Replaced by the object file name.") },
  { CustomArgument::ArgType::Separator,  "%SEP",       I18N_NOOP("Replaced by a separation into two arguments.") }
};


//-----------------------------------------------------------------------------
Compile::FileData::List Compile::FileData::List::onlyExistingFiles() const
{
  List list;
  List::const_iterator it;
  for (it=begin(); it!=end(); ++it) {
    FileData data = *it;
    if ( PURL::findExistingUrl(data.url) ) list.append(data);
  }
  return list;
}

void Compile::FileData::List::cleanGenerated() const
{
  Log::StringView sview;
  List::const_iterator it;
  for (it=begin(); it!=end(); ++it)
    if ( (*it).actions & Generated ) (*it).url.del(sview);
}

//-----------------------------------------------------------------------------
Compile::LogWidget::LogWidget(QWidget *parent)
  : Log::Widget(parent, "compile_log")
{}

void Compile::LogWidget::clear()
{
  Log::Widget::clear();
  _map.clear();
}

void Compile::LogWidget::appendLine(Log::LineType type, const QString &message, const LogData &data)
{
  log(type, message, Log::Delayed);
  _map[document()->blockCount()-1] = data;
}

void Compile::LogWidget::lineClicked(int line)
{
  if ( !_map.contains(line) ) return;
  TextEditor *e = qobject_cast<TextEditor *>(Main::editorManager().openEditor(_map[line].url));
  if (e == NULL) return;
  e->setCursorPosition(_map[line].line, 0);
}

//-----------------------------------------------------------------------------
Compile::GenericProcess::GenericProcess()
: _process(NULL)
{
  FOR_EACH(CommonArgType, type) {
    addArg(ArgType::Common, type.type(), type.data());
  }
}

PURL::Directory Compile::GenericProcess::directory(uint) const
{
  return Main::project()->directory();
}

QStringList Compile::GenericProcess::replaceAllValue(const IdData &idata) const
{
  ASSERT(idata.id == ArgType::Common);
  CommonArgType::Type type =  CommonArgType::Type(idata.index);
  QStringList args;
  switch (type) {
    case CommonArgType::RelativeInputFilepath:
      for (uint k=0; k<nbFiles(); k++) args += inputFilepath(k);
      break;
    default: ASSERT(false); break;
  }
  return args;
}

QString Compile::GenericProcess::replaceValue(const IdData &idata) const
{
  ASSERT(idata.id == ArgType::Common);
  CommonArgType::Type type = CommonArgType::Type(idata.index);
  switch (type) {
    case CommonArgType::SourcePath:
      return directory().path();
    case CommonArgType::OutputFilepath:
      return outputFilepath();
    case CommonArgType::ProjectName:
      return Main::project()->name();
    case CommonArgType::DeviceName:
      return deviceName();
    default: break;
  }
  ASSERT(false);
  return QString::null;
}

QString Compile::GenericProcess::postReplaceValue(const IdData &idata,
                                                  const QString &arg) const
{
  ASSERT(idata.id == ArgType::Common);
  CommonArgType::Type type = CommonArgType::Type(idata.index);
  switch (type) {
    case CommonArgType::SourcePath: {
      QString tmp = arg;
      tmp.replace("//", "/");
      return tmp;
    }
    default: break;
  }
  return arg;
}

bool Compile::GenericProcess::replaceIfCondition(const IdData &idata) const
{
  ASSERT(idata.id == ArgType::Common);
  ASSERT(false);
  return false;
}

PURL::Url Compile::GenericProcess::url(PURL::FileType type, uint) const
{
  if (type==PURL::Hex)
    return Main::project()->url().toFileType(type);
  else if (type==PURL::Library)
    return Main::project()->url().toExtension(libraryExtension());
  else {
    return PURL::Url();
  }
}

QString Compile::GenericProcess::filepath(PURL::FileType type, uint i) const
{
  return url(type, i).relativeTo(directory(), Compile::Config::executableType(group()).data().separator);
}

QString Compile::GenericProcess::outputFilepath() const
{
  return filepath(PURL::Hex);
}

//-----------------------------------------------------------------------------
Compile::BaseProcess::BaseProcess()
: _manager(NULL)
{
}

void Compile::BaseProcess::init(const Data &data, Manager *manager)
{
  _data = data;
  _manager = manager;
}

void Compile::BaseProcess::start()
{
  _stdout = QString::null;
  _stderr = QString::null;
  delete _process;
  _process = new ::Process::LineSignal;
  connect(_process, SIGNAL(done()), SLOT(done()));
  connect(_process, SIGNAL(error()), SLOT(error()));
  connect(_process, SIGNAL(timeout()), SLOT(timeout()));
  connect(_process, SIGNAL(logLine(::Process::OutputType, const QString &)),
          SLOT(logLine(::Process::OutputType, const QString &)));
  _process->setWorkingDirectory(directory().path());
  setupProcess();
  _manager->log(Log::LineType::Command, _process->arguments().join(" "));
  _process->start();
}

void Compile::BaseProcess::error()
{
  _manager->log(Log::LineType::Error, i18n("*** Error executing command ***"));
  _manager->processFailed();
}

void Compile::BaseProcess::done()
{
  int code = _process->exitCode();
  if ( code!=0 ) {
    _manager->log(Log::LineType::Error, i18n("*** Exited with status: %1 ***").arg(code));
    _manager->processFailed();
    return;
  }
  if ( _manager->hasError() ) {
    _manager->log(Log::LineType::Error, i18n("*** Error ***"));
    _manager->processFailed();
    return;
  }
  _manager->processDone();
}

void Compile::BaseProcess::timeout()
{
  _manager->log(Log::LineType::Error, i18n("*** Timeout ***"));
  _manager->processFailed();
}

PURL::Url Compile::BaseProcess::url(PURL::FileType type, uint i) const
{
  PURL::Url url;
  if (type==PURL::Hex || _data.category==Tool::Category::Linker || _data.category==Tool::Category::BinToHex)
    url = Main::project()->url();
  else if (type==PURL::Library || _data.category==Tool::Category::Librarian)
    return Main::project()->url().toExtension(libraryExtension());
  else {
    ASSERT( i<(uint)_data.items.count() );
    url = _data.items[i].url;
  }
  if (type!=PURL::Nb_FileTypes) url = url.toFileType(type);
  return url;
}

Compile::FileData Compile::BaseProcess::fileData(PURL::FileType type, FileActions actions) const
{
  PURL::Url tmp = url(type, nbFiles()-1);
  if (actions & Compile::Generated) tmp = PURL::Url(Main::project()->directory(), tmp.filename());
  return FileData(tmp, actions);
}

QString Compile::BaseProcess::outputFilepath() const
{
  if ( _data.category==Tool::Category::Librarian ) return filepath(PURL::Library);
  return filepath(PURL::Hex);
}

Compile::FileData::List Compile::BaseProcess::files(bool *ok) const
{
  if (ok) *ok = true;
  FileData::List list;
  QRegExp rexp("PURL::(.*)");
  QStringList files = QStringList::split(" ", outputFiles());
  for (int i=0; i<files.count(); i++) {
    if ( rexp.exactMatch(files[i]) ) {
      PURL::FileType type = PURL::FileType::fromKey(rexp.cap(1));
      if ( type==PURL::Nb_FileTypes ) {
        if (ok) *ok = false;
        qWarning("Unknown PURL::FileType in file list for %s", _manager->label().latin1());
        continue;
      }
      if ( type.data().group==PURL::LinkerScript ) {
        PURL::Url lkr = Main::toolGroup().linkerScript(*_data.project, _data.linkingType);
        list += FileData(lkr, Included | InProject);
      } else {
        FileActions actions = Generated;
        if ( type.data().group==PURL::Source || type==PURL::Hex
             || type==PURL::Map || type==PURL::Coff || type==PURL::Library ) actions |= InProject;
        if ( type==PURL::Hex && _data.project==0 ) actions |= Show;
        list += fileData(type, actions);
      }
    } else {
      PURL::Url url = _data.items[nbFiles()-1].url;
      url = PURL::Url(Main::project()->directory(), url.filename()).toExtension(files[i]);
      list += FileData(url, Compile::Generated);
    }
  }
  return list;
}

//-----------------------------------------------------------------------------
Compile::Process::Process()
{
  FOR_EACH(NormalArgType, type) {
    addArg(ArgType::Normal, type.type(), type.data());
  }
}

void Compile::Process::init(const Data &data, Manager *manager)
{
  BaseProcess::init(data, manager);
  _config.reset(group().createConfig(const_cast<Project&>(*data.project)));
}

bool Compile::Process::check() const
{
  return group().check(_data.device, _manager);
}

QStringList Compile::Process::replaceAllValue(const IdData &idata) const
{
  if (idata.id == ArgType::Common) return BaseProcess::replaceAllValue(idata);
  ASSERT(idata.id == ArgType::Normal);
  NormalArgType::Type type = NormalArgType::Type(idata.index);
  Tool::ExecutableType execType = Compile::Config::executableType(group());
  QStringList args;
  switch (type) {
    case NormalArgType::AdditionnalLibraries:
      if (_data.project) args += _data.project->librariesForLinker(QString::null, execType);
      break;
    case NormalArgType::AdditionnalObjects:
      if (_data.project) args += _data.project->objectsForLinker(objectExtension(), execType);
      else {
        PURL::Url tmp = url(PURL::Object);
        if ( !objectExtension().isEmpty() ) tmp = tmp.toExtension(objectExtension());
        args += tmp.relativeTo(directory(), execType.data().separator);
      }
      break;
    default: ASSERT(false); break;
  }
  return args;
}

QString Compile::Process::replaceValue(const IdData &idata) const
{
  if (idata.id == ArgType::Common) return BaseProcess::replaceValue(idata);
  ASSERT(idata.id == ArgType::Normal);
  NormalArgType::Type type = NormalArgType::Type(idata.index);
  Tool::ExecutableType execType = Compile::Config::executableType(group());
  switch(type) {
    case NormalArgType::DeviceFamilyName:
      return familyName();
    case NormalArgType::LinkerScriptPath: {
      PURL::Url lkr = Main::toolGroup().linkerScript(*_data.project, _data.linkingType);
      return lkr.path();
    }
    case NormalArgType::LinkerScriptBasename: {
      PURL::Url lkr = Main::toolGroup().linkerScript(*_data.project, _data.linkingType);
      return lkr.filename();
    }
    case NormalArgType::LinkerScriptFilename: {
      PURL::Url lkr = Main::toolGroup().linkerScript(*_data.project, _data.linkingType);
      return lkr.filepath();
    }
    case NormalArgType::CoffFilename:
      return filepath(PURL::Coff);
    case NormalArgType::MapFilename:
      return filepath(PURL::Map);
    case NormalArgType::SymFilename:
      return url().toExtension("sym").relativeTo(directory(), execType.data().separator);
    case NormalArgType::ListFilename:
      return filepath(PURL::Lst);
    case NormalArgType::ObjectFilename:
      return filepath(PURL::Object);
    default: break;
  }
  ASSERT(false);
  return QString::null;
}

QString Compile::Process::postReplaceValue(const IdData &idata,
                                           const QString &arg) const
{
  if (idata.id == ArgType::Common) return BaseProcess::postReplaceValue(idata, arg);
  ASSERT(idata.id == ArgType::Normal);
  return arg;
}

bool Compile::Process::replaceIfCondition(const IdData &idata) const
{
  if (idata.id == ArgType::Common) return BaseProcess::replaceIfCondition(idata);
  ASSERT(idata.id == ArgType::Normal);
  NormalArgType::Type type = NormalArgType::Type(idata.index);
  switch(type) {
    case NormalArgType::ReplaceIfCustomLinkerScript:
      return hasLinkerScript();
    case NormalArgType::ReplaceIfWine: {
      Tool::ExecutableType execType = Compile::Config::executableType(group());
      return (execType == Tool::ExecutableType::Windows);
    }
    default: break;
  }
  ASSERT(false);
  return false;
}

QStringList Compile::Process::arguments() const
{
  bool custom = _config->hasCustomArguments(_data.category);
  QStringList args = (custom ? _config->customArguments(_data.category) : genericArguments(*_config));
  return process(args);
}

void Compile::Process::setupProcess()
{
  Tool::ExecutableType execType = Compile::Config::executableType(group());
  QString exec = tool()->baseExecutable(execType, Compile::Config::outputExecutableType(group()));
  QString path = tool()->executableDirectory().path();
  _process->setup(path + exec, arguments(), execType==Tool::ExecutableType::Windows);
}

Log::LineType Compile::Process::filterType(const QString &type) const
{
  QString s = type.lower();
  if ( s.startsWith("warning") ) return Log::LineType::Warning;
  if ( s.startsWith("error") )   return Log::LineType::Error;
  if ( s.startsWith("message") ) return Log::LineType::Information;
  return Log::LineType::Normal;
}

bool Compile::Process::parseErrorLine(const QString &s, const ParseErrorData &data)
{
  QRegExp re(data.pattern);
  if ( !re.exactMatch(s) ) return false;
  QString file;
  if ( data.indexFile>=0 ) {
    file = re.cap(data.indexFile).stripWhiteSpace();
    if ( file.endsWith(".") ) file = file.mid(0, file.length()-1);
    if ( file=="-" ) file = QString::null;
  }
  bool ok;
  int line = -1;
  if ( data.indexLine>=0 ) line = re.cap(data.indexLine).stripWhiteSpace().toUInt(&ok) - 1;
  if ( !ok ) line = -1;
  QString message;
  if ( data.indexMessage>=0 ) message= re.cap(data.indexMessage).stripWhiteSpace();
  Log::LineType type = data.defaultLineType;
  if ( data.indexLogType>=0 ) {
    QString s = re.cap(data.indexLogType).stripWhiteSpace();
    if ( s.isEmpty() ) type = data.defaultLineType;
    else type = filterType(s);
  }
  doLog(type, message, file, line);
  return true;
}

void Compile::Process::doLog(const QString &type, const QString &message, const QString &surl, uint line)
{
  doLog(filterType(type), message, surl, line);
}

void Compile::Process::doLog(Log::LineType type, const QString &message, const QString &surl, uint line)
{
  if ( surl.isEmpty() ) {
    // GUI update is delayed so as not to miss some output lines
    _manager->log(type, message, Log::Delayed);
    return;
  }
  PURL::Url url = PURL::fromPathOrUrl(surl, true);
  QString s;
  if ( !url.isEmpty() ) {
    if ( !url.exists() && !url.isInto(directory()) ) url = PURL::Url(directory(), surl);
    s += url.filename() + ":" + QString::number(line+1) + ": ";
  }
  switch (type.type()) {
    case Log::LineType::Warning:     s += i18n("warning: "); break;
    case Log::LineType::Error:       s += i18n("error: ");   break;
    case Log::LineType::Information: s += i18n("message: "); break;
    default: break;
  }
  static_cast<LogWidget *>(_manager->view())->appendLine(type, s + message.stripWhiteSpace(), LogData(url, line));
}
