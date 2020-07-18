/***************************************************************************
 *   Copyright (C) 2005 Nicolas Hadacek <hadacek@kde.org>                  *
 *   Copyright (C) 2003-2004 Alain Gibaud <alain.gibaud@free.fr>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "global_config.h"

#include "progs/list/prog_list.h"
#include "tools/list/tool_list.h"

const BaseGlobalConfig::Data BaseGlobalConfig::DATA[Nb_Types] = {
  { "auto_rebuild_modified",                 I18N_NOOP("Automatically rebuild project before programming if it is modified."), QVariant(true, 0) },
  { "program_after_build",                   I18N_NOOP("Program device after successful build."),                              QVariant(false, 0) },
  { "user_id_set_to_checksum",               I18N_NOOP("Set User Ids to unprotected checksum (if User Ids are empty)."),       QVariant(false, 0) },
  { "show_tab_close_buttons",                I18N_NOOP("Show close buttons on tabs (need restart to take effect)."),           QVariant(true, 0) },
  { "parse_disassembly_listing_after_build", I18N_NOOP("Parse disassembly listing after build."),                              QVariant(true, 0) }
};

PURL::Url GlobalConfig::openedProject()
{
  GenericConfig config(QString::null);
  return PURL::fromPathOrUrl(config.readEntry("project", QString::null), true);
}
void GlobalConfig::writeOpenedProject(const PURL::Url &p)
{
  GenericConfig config(QString::null);
  config.writeEntry("project", p.filepath());
}

PURL::UrlList GlobalConfig::openedFiles()
{
  GenericConfig config(QString::null);
  PURL::UrlList files;
  for (uint i = 0; ; ++i) {
    QString file = config.readEntry(QString("file%1").arg(i), QString::null);
    if ( file.isEmpty() ) break;
    files += PURL::fromPathOrUrl(file, true);
  }
  return files;
}
void GlobalConfig::writeOpenedFiles(const PURL::UrlList &files)
{
  GenericConfig config(QString::null);
  for (int i=0; i<=files.count(); i++) {
    QString s = (i==files.count() ? QString::null : files[i].filepath());
    config.writeEntry(QString("file%1").arg(i), s);
  }
}

void GlobalConfig::writeDebugLevel(Log::DebugLevel level)
{
  GenericConfig config(QString::null);
  config.writeEnumEntry<Log::DebugLevel>("log_debug_level", level);
}
Log::DebugLevel GlobalConfig::debugLevel()
{
  GenericConfig config(QString::null);
  return config.readEnumEntry<Log::DebugLevel>("log_debug_level", Log::DebugLevel::Quiet);
}

void GlobalConfig::writeLogOutputType(Log::OutputType type)
{
  GenericConfig config(QString::null);
  config.writeEntry("log_output_type", type);
}
Log::OutputType GlobalConfig::logOutputType()
{
  GenericConfig config(QString::null);
  uint output = config.readUIntEntry("log_output_type", Log::GuiOnly);
  if ( output>=Log::Nb_OutputTypes ) return Log::GuiOnly;
  return Log::OutputType(output);
}

void GlobalConfig::writeShowLineNumbers(bool show)
{
  GenericConfig config(QString::null);
  config.writeEntry("show_line_numbers", show);
}
bool GlobalConfig::showLineNumbers()
{
  GenericConfig config(QString::null);
  return config.readBoolEntry("show_line_numbers", false);
}
