/***************************************************************************
 *   Copyright (C) 2005-2006 Nicolas Hadacek <hadacek@kde.org>             *
 *   Copyright (C) 2003 Alain Gibaud <alain.gibaud@free.fr>                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "project.h"

#include <memory>

#ifndef NO_KDE
#  include <ksimpleconfig.h>
#endif

#include "devices/list/device_list.h"
#include "progs/base/prog_group.h"
#include "global_config.h"
#include "tools/gputils/gputils_config.h"


Project::Project(const PURL::Url& url)
: XmlDataFile(url, "piklab")
{
}

bool Project::load(QString &error)
{
  if ( _url.fileType()==PURL::Project ) return XmlDataFile::load(error);

#ifdef NO_KDE
  error = i18n("Format for project file %1 not supported.").arg(_url.pretty());
  return false;
#else
  if ( !_url.exists() ) {
    error = i18n("Project file %1 does not exist.").arg(_url.pretty());
    return false;
  }
  PURL::Url tmp = _url;
  _url = _url.toFileType(PURL::Project);
  if ( _url.exists() && XmlDataFile::load(error) ) return true;
  std::auto_ptr<KConfig> config(new KConfig(tmp.filepath(), KConfig::SimpleConfig));

  KConfigGroup group = config->group("Files");
  QStringList list = group.readEntry("inputFiles", QStringList());
  QStringList::const_iterator it = list.begin();
  for (; it!=list.end(); ++it) addFile(PURL::Url(directory(), *it));

  group = config->group("General");
  setVersion(group.readEntry("version", "0.1"));
  setDescription(group.readEntry("description", QString()));

  group = config->group("Assembler");
  QString device = group.readEntry("target-device");
  Compile::Config::setDevice(this, Device::Lister::instance().checkedData(device));
  std::auto_ptr<GPUtils::Config> gconfig(new GPUtils::Config(*this));
  gconfig->setGPAsmWarningLevel(QMIN(group.readEntry("warn-level", (uint)0), uint(GPUtils::Config::Nb_WarningLevels)));
  QStringList includeDirs;
  includeDirs.append(group.readEntry("include-dir", QString()));
  gconfig->setRawIncludeDirs(Tool::Category::Assembler, includeDirs);
  gconfig->setRawCustomOptions(Tool::Category::Assembler, group.readEntry("other-options", QString()));

  group = config->group("Linker") ;
  QString hexFormatString = group.readEntry("hex-format", HexBuffer::FORMATS[HexBuffer::IHX32]);
  for (uint i=0; i<HexBuffer::Nb_Formats; i++)
    if ( hexFormatString==HexBuffer::FORMATS[i] ) gconfig->setHexFormat(HexBuffer::Format(i));
  QStringList libDirs;
  libDirs.append(group.readEntry("objs-libs-dir", QString()));
  gconfig->setRawIncludeDirs(Tool::Category::Linker, libDirs);
  gconfig->setRawCustomOptions(Tool::Category::Linker, group.readEntry("other-options", QString()));
  return true;
#endif
}

PURL::UrlList Project::openedFiles() const
{
  PURL::UrlList files;
  QStringList list = listValues("general", "opened_files", QStringList());
  for (QStringList::const_iterator it = list.begin(); it != list.end(); ++it) {
    PURL::Url url = PURL::fromPathOrUrl(*it, true);
    if (url.isRelative()) files += PURL::Url(directory(), *it);
    else files += url;
  }
  return files;
}
void Project::setOpenedFiles(const PURL::UrlList &list)
{
  clearList("general", "opened_files");
  PURL::UrlList::const_iterator it = list.begin();
  for (; it!=list.end(); ++it)
    appendListValue("general", "opened_files", (*it).relativeTo(directory()));
}

PURL::UrlList Project::absoluteFiles() const
{
  PURL::UrlList abs;
  QStringList files = listValues("general", "files", QStringList());
  for (QStringList::const_iterator it = files.begin(); it != files.end(); ++it)
    abs += PURL::fromPathOrUrl(*it, true).toAbsolute(directory());
  return abs;
}
void Project::addFile(const PURL::Url &url)
{
  appendListValue("general", "files", url.relativeTo(directory()));
}
void Project::removeFile(const PURL::Url &url)
{
  removeListValue("general", "files", url.relativeTo(directory()));
}
void Project::clearFiles()
{
  clearList("general", "files");
}

QStringList Project::objectsForLinker(const QString &extension, Tool::ExecutableType execType) const
{
  QStringList objs;
  PURL::UrlList files = absoluteFiles();
  // objects files corresponding to src files
  for (PURL::UrlList::const_iterator it=files.begin(); it!=files.end(); ++it)
    if ( (*it).data().group==PURL::Source ) {
      if (extension.isEmpty()) objs += (*it).toFileType(PURL::Object).filename();
      else objs += (*it).toExtension(extension).filename();
    }
  // objects
  for (PURL::UrlList::const_iterator it=files.begin(); it!=files.end(); ++it)
    if ( (*it).fileType()==PURL::Object ) objs += (*it).relativeTo(directory(), execType.data().separator);
  return objs;
}

QStringList Project::librariesForLinker(const QString &prefix, Tool::ExecutableType execType) const
{
  QStringList libs;
  PURL::UrlList files = absoluteFiles();
  for (PURL::UrlList::const_iterator it=files.begin(); it!=files.end(); ++it)
    if ( (*it).fileType()==PURL::Library ) libs += prefix + (*it).relativeTo(directory(), execType.data().separator);
  return libs;
}

QString Project::version() const
{
  return Compile::Config::globalValue(this, "version", "0.1");
}
void Project::setVersion(const QString &version)
{
  Compile::Config::setGlobalValue(this, "version", version);
}

Tool::OutputType Project::outputType() const
{
  return Tool::OutputType::fromKey(Compile::Config::globalValue(this, "output_type", Tool::OutputType(Tool::OutputType::Executable).key()));
}
void Project::setOutputType(Tool::OutputType type)
{
  Compile::Config::setGlobalValue(this, "output_type", type.key());
}

QString Project::description() const
{
  return Compile::Config::globalValue(this, "description", QString::null);
}
void Project::setDescription(const QString &description)
{
  Compile::Config::setGlobalValue(this, "description", description);
}

PURL::Url Project::customLinkerScript() const
{
  QString s = Compile::Config::globalValue(this, "custom_linker_script", QString::null);
  return PURL::fromPathOrUrl(s, true);
}
void Project::setCustomLinkerScript(const PURL::Url &url)
{
  Compile::Config::setGlobalValue(this, "custom_linker_script", url.filepath());
}

QValueList<Register::TypeData> Project::watchedRegisters() const
{
  QValueList<Register::TypeData> watched;
  QStringList list = listValues("general", "watched_registers", QStringList());
  for (QStringList::const_iterator it = list.begin(); it != list.end(); ++it) {
    Register::TypeData rtd = Register::TypeData::fromString(*it);
    if ( rtd.type()!=Register::Invalid ) watched.append(rtd);
  }
  return watched;
}
void Project::setWatchedRegisters(const QValueList<Register::TypeData> &watched)
{
  clearList("general", "watched_registers");
  QValueList<Register::TypeData>::const_iterator it;
  for (it=watched.begin(); it!=watched.end(); ++it)
    appendListValue("general", "watched_registers", (*it).toString());
}

QValueList<uint> Project::bookmarkLines(const PURL::Url &url) const
{
  QValueList<uint> lines;
  QStringList list = listValues("editors", "bookmarks", QStringList());
  for (QStringList::const_iterator it = list.begin(); it != list.end(); ++it) {
    QStringList slist = QStringList::split(",", *it);
    QStringList::const_iterator sit = slist.begin();
    KUrl& kurl = const_cast<KUrl&>(url.kurl());
    if ( sit==slist.end() || (*sit)!=kurl.url() ) continue;
    for (; sit!=slist.end(); ++sit) {
      bool ok;
      uint line = (*sit).toUInt(&ok);
      if (!ok) continue;
      lines.append(line);
    }
    break;
  }
  return lines;
}
void Project::setBookmarkLines(const PURL::Url &url, const QValueList<uint> &lines)
{
  KUrl& kurl = const_cast<KUrl&>(url.kurl());
  QStringList list = listValues("editors", "bookmarks", QStringList());
  QStringList nlist;
  for (QStringList::const_iterator it = list.begin(); it != list.end(); ++it) {
    QStringList slist = QStringList::split(",", *it);
    QStringList::const_iterator sit = slist.begin();
    if ( sit!=slist.end() && slist.count()>1 && (*sit)!=kurl.url() ) nlist += *it;
  }
  if ( lines.count()!=0 ) {
    QStringList slist;
    slist += kurl.url();
    for (QValueList<uint>::const_iterator lit = lines.begin(); lit!=lines.end(); ++lit) {
      slist += QString::number(*lit);
    }
    nlist += slist.join(",");
  }
  setListValues("editors", "bookmarks", nlist);
}
