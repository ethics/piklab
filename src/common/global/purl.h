/***************************************************************************
 *   Copyright (C) 2005-2006 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef PURL_H
#define PURL_H

#include "common/common/global.h"

#include <QDateTime>
#include <QtNetwork/QNetworkAccessManager>

#include "log.h"
#include "common/common/purl_base.h"

namespace PURL
{
//----------------------------------------------------------------------------
class Directory;
class Base;

enum TerminationType {
  Terminated,   // terminate with '/' unless empty
  Unterminated
};

enum SeparatorType {
  UnixSeparator,
  WindowsSeparator
};

//----------------------------------------------------------------------------
class NetAccess : public QObject
{
Q_OBJECT
public:
  NetAccess(QObject* parent = NULL);

  bool exists(const Base& url);
  bool download(const Base& url, QByteArray& data, Log::Generic& log);
  bool upload(const Base& url, const QByteArray& data, Log::Generic& log);
  bool del(const Base& url, Log::Generic& log);

private slots:
  void finished(QNetworkReply* reply);

private:
  QNetworkAccessManager* _manager;
  std::auto_ptr<QNetworkReply> _reply;
};

//----------------------------------------------------------------------------
class Base
{
public:
  Base(const QString &filepath = QString::null);
  Base(const KUrl &url);
  bool operator <(const Base &url) const { return _url<url._url; }
  bool operator ==(const Base &url) const;
  bool operator !=(const Base &url) const { return !(_url==url._url); }
  const KUrl &kurl() const { return _url; }
  QString pretty() const;
  bool isEmpty() const { return _url.isEmpty(); }
  bool isLocal() const;
  QString path(TerminationType ttype = Terminated, SeparatorType stype = UnixSeparator) const;
  Directory directory() const;
  bool isInto(const Directory &dir) const;
  bool isRelative() const { return _relative; }
  bool exists(QDateTime *lastModified = 0) const;

protected:
  bool _relative;
  KUrl _url;
};

//----------------------------------------------------------------------------
class Url : public Base
{
public:
  Url() {}
  Url(const KUrl &url) : Base(url) {}
  // add correct extension if filename has no extension
  Url(const Directory &path, const QString &filename, FileType type);
  Url(const Directory &path, const QString &filepath);

  Url toFileType(FileType type) const { return toExtension(type.data().extensions[0]); }
  Url toExtension(const QString &extension) const;
  Url appendExtension(const QString &extension) const;

  const FileType::Data &data() const { return fileType().data(); }
  FileType fileType() const;
  QString basename() const; // filename without extension
  QString filename() const; // filename without path
  QString filepath(SeparatorType type = UnixSeparator) const; // filename with path
  QString relativeTo(const Directory &dir, SeparatorType type = UnixSeparator) const;
  Url toAbsolute(const Directory &dir) const;
  bool isDosFile() const;
  bool create(Log::Generic &log) const; // do not overwrite
  bool write(const QString &text, Log::Generic &log) const;
  bool copyTo(const Url &destination, Log::Generic &log) const; // do not overwrite
  bool del(Log::Generic &log) const;

private:
  Url(const QString &filepath) : Base(filepath) {}

friend Url fromPathOrUrl(const QString &, bool);
};

//----------------------------------------------------------------------------
class UrlList : public QValueList<Url>
{
public:
  UrlList() {}
  UrlList(const Url &url) { append(url); }
  UrlList(const QValueList<Url> &list) : QValueList<Url>(list) {}
  UrlList(const KUrl::List &list);
};

//----------------------------------------------------------------------------
class Directory : public Base
{
public:
  Directory(const QString &path = QString::null);
  QStringList files(const QString &filter) const;
  Url findMatchingFilename(const QString &filename) const;
  Directory up() const;
  Directory down(const QString &path) const;
  bool create(Log::Generic &log) const;
};

//----------------------------------------------------------------------------
bool isRelative(const QString &filepath);
Url fromPathOrUrl(const QString &s, bool keepRelative);
Directory currentDirectory();
bool findExistingUrl(Url &url); // may transform extension's case if needed

} // namespace

#endif
