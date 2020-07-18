/***************************************************************************
 *   Copyright (C) 2005-2009 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "purl.h"
#include "purl.moc"

#include <qfileinfo.h>
#include <qdatetime.h>
#include <qdir.h>
#include <qregexp.h>
#include <qmap.h>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include "common/common/synchronous.h"
#include "process.h"
#include "pfile.h"

//-----------------------------------------------------------------------------
PURL::NetAccess::NetAccess(QObject* parent)
: QObject(parent)
{
  _manager = new QNetworkAccessManager(this);
  connect(_manager, SIGNAL(finished(QNetworkReply*)), SLOT(finished(QNetworkReply*)));
}

bool PURL::NetAccess::exists(const Base& purl)
{
  QEventLoop eventLoop;
  connect(_manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
  QNetworkRequest req(purl.kurl());
  _manager->get(req);
  eventLoop.exec();
  return _reply->error() != QNetworkReply::NoError;
}

bool PURL::NetAccess::download(const Base& purl, QByteArray& data, Log::Generic& log)
{
  QEventLoop eventLoop;
  connect(_manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
  QNetworkRequest req(purl.kurl());
  _manager->get(req);
  eventLoop.exec();
  if (_reply->error() != QNetworkReply::NoError) {
    log.sorry(i18n("Download error %1").arg(_reply->error()), purl.pretty());
    return false;
  }
  data = _reply->readAll();
  return true;
}

bool PURL::NetAccess::upload(const Base& purl, const QByteArray& data, Log::Generic& log)
{
  QEventLoop eventLoop;
  connect(_manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
  QNetworkRequest req(purl.kurl());
  _manager->put(req, data);
  eventLoop.exec();
  if (_reply->error() != QNetworkReply::NoError) {
    log.sorry(i18n("Upload error %1").arg(_reply->error()), purl.pretty());
    return false;
  }
  return true;
}

bool PURL::NetAccess::del(const Base& purl, Log::Generic& log)
{
  QEventLoop eventLoop;
  connect(_manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
  QNetworkRequest req(purl.kurl());
  _manager->deleteResource(req);
  eventLoop.exec();
  if (_reply->error() != QNetworkReply::NoError) {
    log.sorry(i18n("Delete error %1").arg(_reply->error()), purl.pretty());
    return false;
  }
  return true;
}

void PURL::NetAccess::finished(QNetworkReply* reply)
{
  _reply.reset(reply);
}

//-----------------------------------------------------------------------------
#if defined(Q_OS_UNIX)
namespace PURL
{
class Private
{
public:
  static QString convertWindowsFilepath(const QString &filepath);

private:
  Private();
  static Private& self();

  static Private* _self;
  QString _winDrivesPath;           // root Windows drives
  QMap<QString, QString> _winPaths; // Windows path -> Unix path

  bool checkCachedPath(QString &filepath) const;
  QString cachePath(const QString &origin, const QString &filepath);
  QString convertWindowsShortFilepath(const QString &filepath);
  QString findName(const QString &path, const QString &name);
  static QString findName(const QString &filepath);
};

} // namespace

PURL::Private::Private()
{
  QStringList args;
  args += "winepath";
  args += "-u";
  args += "c:\\";
  ::Process::StringOutput process;
  process.setup("wine", args, false);
  process.execute(3000);
  if ( process.state()==::Process::Exited ) {
    QDir dir(process.string(Process::OutputType::Stdout).stripWhiteSpace());
    dir.cdUp();
    _winDrivesPath = dir.absPath();
  } else {
    //qWarning("Error running \"wine\" with \"%s\" (%i)", args.join(" ").latin1(), state);
  }
}

PURL::Private* PURL::Private::_self = NULL;

PURL::Private& PURL::Private::self()
{
  if (_self == NULL) _self = new Private;
  return *_self;
}

bool PURL::Private::checkCachedPath(QString &filepath) const
{
  if ( !_winPaths.contains(filepath) ) return false;
  filepath = _winPaths[filepath];
  return true;
}

QString PURL::Private::cachePath(const QString &origin, const QString &filepath)
{
  _winPaths[origin] = filepath;
  return filepath;
}

QString PURL::Private::convertWindowsFilepath(const QString &filepath)
{
  QString tmp = filepath;
  tmp.replace('\\', "/");

  // cached
  if (_self != NULL && _self->checkCachedPath(tmp)) return tmp;

  // appears to be an absolute Windows path
  if ( filepath[0]=='\\' ) return self().cachePath(filepath, self().convertWindowsShortFilepath(tmp));

  // appears to be a windows path with a drive
  if ( (tmp.length()>=2 && tmp[0].isLetter() && tmp[1]==':') ) {
    tmp = self()._winDrivesPath + "/" + tmp[0].lower() + ":" + tmp.mid(2);
    return self().cachePath(filepath, self().convertWindowsShortFilepath(tmp));
  }

  return filepath;
}

QString PURL::Private::findName(const QString &path, const QString &name)
{
  QString filepath = path + '/' + name;
  if ( checkCachedPath(filepath) ) return filepath;
  return cachePath(filepath, findName(filepath));
}

QString PURL::Private::findName(const QString &filepath)
{
  QFileInfo finfo(filepath);
  if ( finfo.exists() || !finfo.dir().exists() ) return finfo.filePath();
  QStringList list = finfo.dir().entryList(QDir::All, QDir::Name);
  // find if name is just in a different case
  for (uint j=0; j<uint(list.count()); j++) {
    if ( list[j].lower()!=finfo.fileName().lower() ) continue;
    return finfo.dirPath() + '/' + list[j];
  }
  // find if name is a shorted filename
  QRegExp rexp("([^~]+)~(\\d+).*");
  if ( !rexp.exactMatch(finfo.fileName()) ) return finfo.filePath();
  QString start = rexp.cap(1).lower();
  uint index = rexp.cap(2).toUInt();
  uint k = 0;
  for (uint j = 0; j<uint(list.count()); j++) {
    if ( !list[j].lower().startsWith(start) ) continue;
    k++;
    if ( k==index ) return finfo.dirPath() + '/' + list[j];
  }
  return finfo.filePath();
}

QString PURL::Private::convertWindowsShortFilepath(const QString &filepath)
{
  // apparently "winepath" cannot do that for us and it is a real pain too...
  // we assume filepath is an absolute Unix path
  // first see if we know the dirpath
  QFileInfo finfo(filepath);
  QString path = finfo.dirPath();
  if ( checkCachedPath(path) ) return findName(path, finfo.fileName());

  // otherwise go down the path
  QStringList names = QStringList::split('/', filepath);
  QString tmp;
  for (uint i=0; i<uint(names.count()); i++)
    tmp = findName(tmp, names[i]);
  if ( filepath.endsWith("/") ) tmp += "/";
  return tmp;
}
#endif

//-----------------------------------------------------------------------------
PURL::Base::Base(const QString &filepath)
  : _relative(true)
{
  if ( !filepath.isEmpty() ) {
#if defined(Q_OS_UNIX)
    QString tmp = Private::convertWindowsFilepath(filepath);
#else
    QString tmp = filepath;
#endif
    if ( tmp.startsWith("~") ) tmp = QDir::homeDirPath() + tmp.mid(1);
    _relative = Q3Url::isRelativeUrl(tmp);
#if defined(Q_OS_UNIX)
    if (tmp.startsWith("file://")) tmp = tmp.mid(7);
    if (!tmp.startsWith("/")) tmp = '/' + tmp;
#endif
#if defined(NO_KDE)
    _url.setPath(tmp);
#else
    _url = KUrl(tmp);
#endif
    _url.cleanPath();
  }
}

PURL::Base::Base(const KUrl &url)
  : _relative(false), _url(url)
{
  _url.cleanPath();
}

bool PURL::Base::isLocal() const
{
  return ( _url.protocol().isEmpty() || _url.protocol()=="file" );
}

bool PURL::Base::operator ==(const Base &url) const
{
  if ( _url.isEmpty() ) return url._url.isEmpty();
  return _url==url._url;
}

QString PURL::Base::path(TerminationType ttype, SeparatorType stype) const
{
  QString s;
  if (!_url.isEmpty()) s =_url.dirPath();
  switch (ttype) {
    case Terminated:
      if ( !s.isEmpty() && !s.endsWith("/") ) s += '/';
      break;
    case Unterminated:
      if ( s.endsWith("/") && s!="/" ) s = s.mid(0, s.length()-1);
      break;
  }

  switch (stype) {
    case UnixSeparator: break;
    case WindowsSeparator:
      for (uint i=0; i<uint(s.length()); i++)
        if ( s[i]=='/' ) s[i] = '\\';
      break;
  }
  return s.stripWhiteSpace();
}

QString PURL::Base::pretty() const
{
  return _url.pathOrUrl();
}

PURL::Directory PURL::Base::directory() const
{
  return Directory(path());
}

bool PURL::Base::isInto(const Directory &dir) const
{
  return path().startsWith(dir.path());
}

bool PURL::Base::exists(QDateTime *lastModified) const
{
  if (isEmpty()) return false;
  if (isLocal()) {
    QFileInfo fi(_url.path());
    if (lastModified) *lastModified = fi.lastModified();
    return fi.exists();
  }

  // ### TODO: lastModified
  std::auto_ptr<NetAccess> access(new NetAccess);
  return access->exists(_url);
}

//----------------------------------------------------------------------------
PURL::Url::Url(const Directory &dir, const QString &filename, FileType type)
  : Base(dir.path() + '/' + addExtension(filename, type))
{}

PURL::Url::Url(const Directory &dir, const QString &filepath)
  : Base(dir.path() + '/' + filepath)
{}

PURL::FileType PURL::Url::fileType() const
{
  QFileInfo info(filename());
  FOR_EACH(FileType, type)
    for (uint i=0; type.data().extensions[i]; i++)
      if ( info.extension(false).lower()==type.data().extensions[i] ) return type;
  return Unknown;
}

QString PURL::Url::basename() const
{
  QFileInfo info(_url.fileName(KUrl::ObeyTrailingSlash));
  return info.baseName(true);
}

QString PURL::Url::filename() const
{
  QFileInfo info(_url.fileName(KUrl::ObeyTrailingSlash));
  return info.fileName();
}

QString PURL::Url::filepath(SeparatorType type) const
{
  return path(Terminated, type) + filename();
}

PURL::Url PURL::Url::toExtension(const QString &extension) const
{
  QFileInfo info(filename());
  return Url(directory().path() + info.baseName(true) + '.' + extension);
}

PURL::Url PURL::Url::appendExtension(const QString &extension) const
{
  QFileInfo info(filename());
  return Url(directory().path() + info.fileName() + '.' + extension);
}

QString PURL::Url::relativeTo(const Directory &dir, SeparatorType type) const
{
  QString s = filepath(type);
  if ( !isInto(dir) ) return s;
  return s.right(s.length() - dir.path(Terminated, type).length());
}

PURL::Url PURL::Url::toAbsolute(const Directory &dir) const
{
  if ( isRelative() ) return Url(dir, filepath());
  return *this;
}

bool PURL::Url::isDosFile() const
{
  Log::Base log;
  File file(*this, log);
  if( !file.openForRead() ) return false;
  int oldc = 0;
  for (;;) {
    int c = file.qfile()->getch();
    if ( c==-1 )  break;
    if( c=='\n' && oldc=='\r' ) return true;
    oldc = c;
  }
  return false;
}

bool PURL::Url::write(const QString &text, Log::Generic &log) const
{
  File file(*this, log);
  if ( !file.openForWrite() ) return false;
  file.stream() << text;
  return file.close();
}

bool PURL::findExistingUrl(Url &url)
{
  if ( url.exists() ) return true;
  QFileInfo info(url.filename());
  Url tmp = url.toExtension(info.extension(false).upper());
  if ( !tmp.exists() ) {
    tmp = url.toExtension(info.extension(false).lower());
    if ( !tmp.exists() ) return false;
  }
  url = tmp;
  return true;
}

bool PURL::Url::copyTo(const Url &destination, Log::Generic &log) const
{
  if (isLocal() && destination.isLocal()) {
    bool ok = QFile::copy(filepath(), destination.filepath());
    if (!ok) log.sorry(i18n("Could not copy file"), destination.pretty());
    return ok;
  }

  std::auto_ptr<NetAccess> access(new NetAccess);
  QByteArray data;
  return access->download(*this, data, log) && access->upload(destination, data, log);
}

bool PURL::Url::create(Log::Generic &log) const
{
  if (isLocal()) {
    // do not overwrite
    QFile file(filepath());
    if (file.exists()) {
      return false;
    }
    bool ok = file.open(QIODevice::WriteOnly);
    if (!ok) log.sorry(i18n("Could not create file"), pretty());
    return ok;
  }

  std::auto_ptr<NetAccess> access(new NetAccess);
  if (access->exists(*this)) return false;
  QByteArray data;
  return access->upload(*this, data, log);
}

bool PURL::Url::del(Log::Generic &log) const
{
  if (isLocal()) {
    bool ok = QFile::remove(filepath());
    if (!ok) log.sorry(i18n("Could not delete file"), pretty());
    return ok;
  }

  std::auto_ptr<NetAccess> access(new NetAccess);
  return access->del(*this, log);
}

//-----------------------------------------------------------------------------
PURL::UrlList::UrlList(const KUrl::List &list)
{
  for (KUrl::List::const_iterator it=list.begin(); it!=list.end(); ++it) {
    Url url(*it);
    append(url);
  }
}

//-----------------------------------------------------------------------------
PURL::Directory::Directory(const QString &path)
  : Base(path.isEmpty() ? QString::null : path + '/')
{}

PURL::Directory PURL::Directory::up() const
{
  QDir dir(path());
  dir.cdUp();
  return PURL::Directory(dir.path());
}

PURL::Directory PURL::Directory::down(const QString &subPath) const
{
  ASSERT( QDir::isRelativePath(subPath) );
  QDir dir(path());
  dir.cd(subPath);
  return PURL::Directory(dir.path());
}

QStringList PURL::Directory::files(const QString &filter) const
{
  QDir dir(path());
  return dir.entryList(filter, QDir::Files);
}

PURL::Url PURL::Directory::findMatchingFilename(const QString &filename) const
{
  QDir dir(path());
  QStringList files = dir.entryList(QDir::Files);
  for (uint i=0; i<uint(files.count()); i++)
    if ( files[i].lower()==filename.lower() ) return Url(*this, files[i]);
  return Url(*this, filename);
}

bool PURL::Directory::create(Log::Generic &log) const
{
  if (isLocal()) {
    QDir root = QDir::root();
    bool ok = root.mkpath(path());
    if (!ok) log.sorry(i18n("Could not create directory"), pretty());
    return ok;
  }

  log.sorry(i18n("Could not create non-local directory"), pretty());
  return false;
}

//-----------------------------------------------------------------------------
bool PURL::isRelative(const QString &filepath)
{
  Base base(filepath);
  return base.isRelative();
}

PURL::Directory PURL::currentDirectory()
{
  return QDir::currentPath();
}

PURL::Url PURL::fromPathOrUrl(const QString &s, bool keepRelative)
{
  if (s.isEmpty()) return PURL::Url();
  KUrl kurl(s);
  if (!kurl.protocol().isEmpty()
       && kurl.protocol() != "file"
       && kurl.protocol().length() != 1) return Url(kurl);
  QString filepath = (s.startsWith("file://") ? s.mid(7) : s);
  if (isRelative(filepath) && !keepRelative) return Url(currentDirectory(), filepath);
  return Url(filepath);
}
