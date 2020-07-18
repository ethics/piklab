/***************************************************************************
 *   Copyright (C) 2005-2006 Nicolas Hadacek <hadacek@kde.org>             *
 *   Copyright (C) 2003-2004 Alain Gibaud <alain.gibaud@free.fr>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "compile_manager.h"
#include "compile_manager.moc"

#include <qtimer.h>

#include "libgui/project.h"
#include "common/gui/misc_gui.h"
#include "compile_config.h"
#include "libgui/main_global.h"
#include "progs/base/prog_group.h"
#include "tools/custom/custom_process.h"
#include "tool_list.h"

Compile::Manager::Manager(QObject *parent)
: QObject(parent, "compile_manager")
{}

Compile::LinkingType Compile::Manager::linkingType() const
{
  if ( !Main::programmerGroup().isDebugger() ) return NormalLinking;
  return (Main::programmerGroup().name()=="icd2_debugger" ? Icd2DebugLinking : DebugLinking);
}

bool Compile::Manager::addCompileItem(const QValueList<Item> &items, Operation operation)
{
  const Tool::Group& group = (operation == CreateDebugInfo
                              ? *Tool::Lister::instance().group(Main::toolGroup().auxiliaryDebugInfoGenerator())
                              : Main::toolGroup());
  for (QValueList<Item>::const_iterator it = items.begin(); it != items.end(); ++it) {
    PURL::SourceFamily family = (*it).url.data().sourceFamily;
    if ( family.data().toolType!=PURL::ToolType::Compiler ) return true;
    if ( !group.needs(Tool::Category::Compiler) ) return true;
    PURL::FileType type = group.implementationType(PURL::ToolType::Compiler);
    if ( operation!=Clean && type != PURL::FileType::Nb_Types && (*it).url.fileType()!=type ) {
      QString e = PURL::extensions(type);
      MessageBox::detailedSorry(i18n("The selected toolchain (%1) cannot compile file. It only supports files with extensions: %2")
                               .arg(group.label()).arg(e), i18n("File: %1").arg((*it).url.pretty()), Log::Show);
      log(Log::LineType::Error, i18n("*** Aborted ***"), Log::Delayed);
      return false;
    }
  }
  _todo += Data(Tool::Category::Compiler, items, Main::deviceData().name(),
                *Main::project(), linkingType(), operation);
  return true;
}

bool Compile::Manager::addAssembleItem(const QValueList<Item> &items, Operation operation)
{
  const Tool::Group& group = (operation == CreateDebugInfo
                              ? *Tool::Lister::instance().group(Main::toolGroup().auxiliaryDebugInfoGenerator())
                              : Main::toolGroup());
  QValueList<Item> nitems = items;
  QValueList<Item>::iterator it;
  for (it=nitems.begin(); it!=nitems.end(); ++it) {
    PURL::SourceFamily family = (*it).url.data().sourceFamily;
    if ( family.data().toolType!=PURL::ToolType::Assembler ) {
      if ( !group.needs(Tool::Category::Assembler) ) return true;
      PURL::FileType type = group.implementationType(PURL::ToolType::Assembler);
      (*it).url = (*it).url.toFileType(type);
      (*it).type = Item::Generated;
    }
    PURL::FileType type = group.implementationType(PURL::ToolType::Assembler);
    if ( operation!=Clean && type != PURL::FileType::Nb_Types && (*it).url.fileType()!=type ) {
      QString e = PURL::extensions(type);
      MessageBox::detailedSorry(i18n("The selected toolchain (%1) cannot assemble file. It only supports files with extensions: %2")
                                .arg(group.label()).arg(e), i18n("File: %1").arg((*it).url.pretty()), Log::Show);
      log(Log::LineType::Error, i18n("*** Aborted ***"), Log::Delayed);
      return false;
    }
  }
  _todo += Data(Tool::Category::Assembler, nitems, Main::deviceData().name(),
                *Main::project(), linkingType(), operation);
  return true;
}

void Compile::Manager::addLinkItem(const QValueList<Item> &items, Operation operation)
{
  const Tool::Group& group = (operation == CreateDebugInfo
                              ? *Tool::Lister::instance().group(Main::toolGroup().auxiliaryDebugInfoGenerator())
                              : Main::toolGroup());
  Tool::Category category = (Main::project()->outputType()==Tool::OutputType::Library ? Tool::Category::Librarian : Tool::Category::Linker);
  if ( group.needs(category) ) {
    _todo += Data(category, items, Main::deviceData().name(),
                  *Main::project(), linkingType(), operation);
  }
  if ( category==Tool::Category::Linker && group.needs(Tool::Category::BinToHex) ) {
    _todo += Data(Tool::Category::BinToHex, items, Main::deviceData().name(),
                  *Main::project(), linkingType(), operation);
  }
}

void Compile::Manager::cleanFile(const PURL::Url &url)
{
  clearAll();
  QValueList<Item> items;
  items += Item(url, Item::NotGenerated);
  addCompileItem(items, Clean);
  addAssembleItem(items, Clean);
  addLinkItem(items, Clean);
  execute();
}

bool Compile::Manager::compileFile(const Item &item)
{
  Log::Base::clear();
  clearAll();
  _compileOnly = true;
  _label = i18n("Compiling file...");
  QValueList<Item> items;
  items += item;
  if ( !addCompileItem(items, Build) ) return false;
  if ( !addAssembleItem(items, Build) ) return false;
  execute();
  return true;
}

bool Compile::Manager::addProjectSources(Operation operation, uint &nb)
{
  nb = 0;
  PURL::UrlList files = Main::project()->absoluteFiles();
  if (Main::toolGroup().compileFileType() == Tool::CompileFileType::All) {
    QValueList<Item> items;
    for (PURL::UrlList::const_iterator it = files.begin(); it != files.end(); ++it) {
      if ((*it).data().group != PURL::Source) continue;
      items += Item(*it, Item::NotGenerated);
      nb++;
    }
    if (!addCompileItem(items, operation)) return false;
    if (!addAssembleItem(items, operation)) return false;
  } else {
    for (PURL::UrlList::const_iterator it = files.begin(); it != files.end(); ++it) {
      if ((*it).data().group != PURL::Source) continue;
      QValueList<Item> items;
      items += Item(*it, Item::NotGenerated);
      if (!addCompileItem(items, operation)) return false;
      if (!addAssembleItem(items, operation)) return false;
      nb++;
    }
  }
  return true;
}

void Compile::Manager::cleanProject()
{
  clearAll();
  uint nb = 0;
  addProjectSources(Clean, nb);
  QValueList<Item> items;
  items += Item(Main::project()->url(), Item::NotGenerated);
  addLinkItem(items, Clean);
  execute();
}

bool Compile::Manager::buildProject()
{
  Log::Base::clear();
  clearAll();
  _compileOnly = false;
  _label = i18n("Building project...");
  PURL::UrlList files = Main::project()->absoluteFiles();
  if ( files.count()==0 ) {
    MessageBox::sorry(i18n("Cannot build empty project."), Log::Show);
    return false;
  }
  uint nb = 0;
  if ( !addProjectSources(Build, nb) ) return false;
  if ( nb>1 && Main::toolGroup().compileFileType()==Tool::CompileFileType::Single ) {
    MessageBox::sorry(i18n("The selected toolchain only supports single-file project."), Log::Show);
    return false;
  }
  QValueList<Item> items;
  items += Item(Main::project()->url(), Item::NotGenerated);
  addLinkItem(items, Build);
  const Tool::Group* group = Tool::Lister::instance().group(Main::toolGroup().auxiliaryDebugInfoGenerator());
  if (group != NULL
      && linkingType()!=NormalLinking) {
    log(Log::LineType::Warning, i18n("The selected toolchain does not generate debug information. You can manually generate it with the \"%1\" toolchain.").arg(group->label()));
  }
  return execute();
}

void Compile::Manager::kill()
{
  if ( clearAll() ) log(Log::LineType::Error, i18n("*** Aborted ***"), Log::Delayed);
  emit failure();
}

bool Compile::Manager::clearAll()
{
  bool running = (_base.get() != NULL);
  _base.reset(NULL);
  _todo.clear();
  _customCommandIndex = 0;
  return running;
}

bool Compile::Manager::execute()
{
  log(Log::DebugLevel::Extra, QString("Todo: %1").arg(_todo.count()));
  for (int i=0; i<_todo.count(); i++) {
    QString s;
    for (int k=0; k<_todo[i].items.count(); k++) {
      if ( k!=0 ) s += ",";
      s += _todo[i].items[k].url.pretty();
    }
    log(Log::DebugLevel::Extra, QString("  %1: %2 operation=%3").arg(_todo[i].category.label()).arg(s).arg(_todo[i].operation));
  }
  _base.reset(NULL);
  if ( Main::toolGroup().isCustom() ) return executeCustom();
  return executeNext();
}

bool Compile::Manager::executeNext()
{
  if ( _todo.isEmpty() ) {
    if ( _next.operation!=Clean ) {
      log(Log::LineType::Information, i18n("*** Success ***"), Log::Delayed);
      emit success();
    }
    return true;
  }
  _next = _todo[0];
  _todo.pop_front();
  _base.reset(Main::toolGroup().createCompileProcess(_next, this));
  ASSERT(_base.get() != NULL);
  if ( _next.operation==Clean) {
    FileData::List files = _base->files(0).onlyExistingFiles();
    for (FileData::List::const_iterator it = files.begin(); it != files.end(); ++it) {
      if ( !((*it).actions & Generated) ) continue;
      log(Log::DebugLevel::Extra, QString("File to clean: %1").arg((*it).url.pretty()));
    }
    files.cleanGenerated();
    QTimer::singleShot(0, this, SLOT(execute()));
  } else {
    if ( !_base->check() ) {
      processFailed();
      return false;
    }
    _base->start();
  }
  return true;
}

void Compile::Manager::processDone()
{
  if ( hasError() ) {
    processFailed();
    return;
  }
  if ( Main::toolGroup().isCustom() ) _customCommandIndex++;
  else {
    FileData::List list = _base->files(NULL).onlyExistingFiles();
    for (FileData::List::const_iterator it = list.begin(); it != list.end(); ++it) emit updateFile(*it);
  }
  QTimer::singleShot(0, this, SLOT(execute()));
}

void Compile::Manager::processFailed()
{
  clearAll();
  emit failure();
}

bool Compile::Manager::executeCustom()
{
  if ( _customCommandIndex==0 ) {
    if ( Compile::Config::customCommands(Main::project()).isEmpty() ) {
      log(Log::LineType::Error, i18n("No custom commands specified."), Log::Delayed);
      emit failure();
      return false;
    }
    log(Log::LineType::Information, i18n("Executing custom commands..."));
  }
  QStringList commands = Compile::Config::customCommands(Main::project());
  if ( _customCommandIndex==(uint)commands.count() ) {
    log(Log::LineType::Information, i18n("*** Success ***"), Log::Delayed);
    emit success();
    return true;
  }
  QString command = commands[_customCommandIndex];
  _base.reset(new CustomProcess(command));
  QValueList<Item> items;
  const PURL::UrlList files = Main::project()->absoluteFiles();
  for (PURL::UrlList::const_iterator it = files.begin(); it != files.end(); ++it) {
    items += Item(*it, Item::NotGenerated);
  }
  Compile::Data data(Tool::Category::Nb_Types, items, Main::deviceData().name(),
                     *Main::project(), NormalLinking, Build);
  _base->init(data, this);
  _base->start();
  return true;
}
