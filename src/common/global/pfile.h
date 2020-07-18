/***************************************************************************
 *   Copyright (C) 2005-2006 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef PFILE_H
#define PFILE_H

#include <qtextstream.h>
#include <qfile.h>

#include "common/common/global.h"
#include "purl.h"

namespace PURL
{
//-----------------------------------------------------------------------------
class FileBase : public NoCopyClass
{
public:
  FileBase(Log::Generic &log, const QString &extension);
  QFile *qfile();
  const QFile *qfile() const;
  bool isOpen() const;
  QTextStream &stream();
  QString readText() { return stream().read(); }
  QString readLine() { return stream().readLine(); }
  QStringList readLines();
  QByteArray readAll();
  void appendText(const QString &text) { stream() << text; }
  void flush();
  bool hasError() const;
  QString errorString() const;

protected:
  std::auto_ptr<KTemporaryFile> _tmp;
  std::auto_ptr<QFile>       _file;
  std::auto_ptr<QTextStream> _stream;
  QString                    _error;
  QString                    _extension;
  Log::Generic&              _log;
};

//-----------------------------------------------------------------------------
class File : public FileBase
{
public:
  File(Log::Generic &log);
  File(const Url &url, Log::Generic &log);
  ~File() { close(); }
  void setUrl(const Url &url); // close file too
  Url url() const { return _url; }
  bool openForWrite();
  bool openForRead();
  bool close();
  bool remove();

private:
  Url _url;
};

class TempFile : public FileBase
{
public:
  TempFile(Log::Generic &log, const QString &extension = QString::null);
  ~TempFile() { close(); }
  Url url() const;
  bool close();
  bool openForWrite();
};

} // namespace

#endif
