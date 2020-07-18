/***************************************************************************
 *   Copyright (C) 2005-2006 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "pfile.h"

#include <qfile.h>

//-----------------------------------------------------------------------------
PURL::FileBase::FileBase(Log::Generic &log, const QString &extension)
: _extension(extension),
  _log(log)
{}

const QFile *PURL::FileBase::qfile() const
{
  return (_tmp.get() != NULL ? _tmp.get() : _file.get());
}

QFile *PURL::FileBase::qfile()
{
  return (_tmp.get() != NULL ? _tmp.get() : _file.get());
}

bool PURL::FileBase::isOpen() const
{
  if (qfile() == NULL) return false;
  return qfile()->isOpen();
}

void PURL::FileBase::flush()
{
  if (qfile() != NULL) qfile()->flush();
}

QTextStream &PURL::FileBase::stream()
{
  if (_stream.get() == NULL) _stream.reset(new QTextStream(qfile()));
  return *_stream;
}

bool PURL::FileBase::hasError() const
{
  if (qfile() == NULL || !_error.isEmpty()) return true;
  return (uint(qfile()->status()) != IO_Ok);
}

QString PURL::FileBase::errorString() const
{
  if (_error.isEmpty()) {
    if (qfile() == NULL) return i18n("File not opened.");
    else return qfile()->errorString();
  }
  return _error;
}

QStringList PURL::FileBase::readLines()
{
  QStringList lines;
  while (true) {
    QString line = stream().readLine();
    if (line.isNull()) break;
    lines.append(line);
  }
  return lines;
}

QByteArray PURL::FileBase::readAll()
{
  if (qfile() != NULL) return qfile()->readAll();
  return QByteArray();
}

//-----------------------------------------------------------------------------
PURL::File::File(Log::Generic &log)
: FileBase(log, QString::null)
{
  _file.reset(new QFile);
}

PURL::File::File(const Url &url, Log::Generic &log)
: FileBase(log, QString::null),
  _url(url)
{
  _file.reset(new QFile);
}

void PURL::File::setUrl(const Url &url)
{
  close();
  _url = url;
}

bool PURL::File::openForWrite()
{
  close();
  _tmp.reset(new KTemporaryFile);
  if (!_extension.isEmpty()) {
    _tmp->setSuffix("." + _extension);
  }
  if (!_tmp->open()) {
    _error = i18n("Could not create temporary file.");
    _log.sorry(_error, i18n("File: %1").arg(_tmp->name()));
    return false;
  }
  return true;
}

bool PURL::File::close()
{
  if (_tmp.get() != NULL) _tmp->close();
  else _file->close();
  _stream.reset(NULL);

  bool ok = true;
  if (_tmp.get() != NULL
    && !_url.isEmpty()) {
    _file->setName(_tmp->fileName());
    if ( !_file->open(IO_ReadOnly) ) {
      _error = i18n("Could not open temporary file.");
      _log.sorry(_error, i18n("File: %1").arg(_file->name()));
      return false;
    }
    QByteArray data = _file->readAll();
    std::auto_ptr<NetAccess> access(new NetAccess);
    ok = access->upload(_url, data, _log);
    _file->close();
    _tmp.reset(NULL);
  }
  return ok;
}

bool PURL::File::openForRead()
{
  close();

  if (_url.isLocal()) {
    _file->setName(_url.filepath());
  } else {
    std::auto_ptr<NetAccess> access(new NetAccess);
    QByteArray data;
    if (!access->download(_url, data, _log)) {
      _error = i18n("Could not download file.");
      return false;
    }

    _tmp.reset(new KTemporaryFile);
    if (!_extension.isEmpty()) {
      _tmp->setSuffix("." + _extension);
    }
    if (!_tmp->open()) {
      _error = i18n("Could not create temporary file.");
      _log.sorry(_error, i18n("File: %1").arg(_tmp->name()));
      return false;
    }
    _tmp->write(data);
    _tmp->close();
    _file->setName(_tmp->fileName());
  }

  if ( !_file->open(IO_ReadOnly) ) {
    _error = i18n("Could not open temporary file.");
    _log.sorry(_error, i18n("File: %1").arg(_file->name()));
    return false;
  }
  return true;
}

bool PURL::File::remove()
{
  close();
  if ( !_url.isEmpty() ) return _url.del(_log);
  return false;
}

//-----------------------------------------------------------------------------
PURL::TempFile::TempFile(Log::Generic &log, const QString &extension)
: FileBase(log, extension)
{}

PURL::Url PURL::TempFile::url() const
{
  return (_tmp.get() != NULL ? fromPathOrUrl(_tmp->name(), false) : Url());
}

bool PURL::TempFile::close()
{
  _stream.reset(NULL);
  if (_tmp.get() != NULL) {
    _tmp->close();
  }
  return true;
}

bool PURL::TempFile::openForWrite()
{
  close();
  _tmp.reset(new KTemporaryFile);
#if defined(NO_KDE)
  _tmp->setFileTemplate(QLatin1String("XXXXXX") + _extension);
  _tmp->setAutoRemove(true);
#else
  if (!_extension.isEmpty()) {
    _tmp->setSuffix("." + _extension);
  }
#endif
  if (!_tmp->open()) {
    _error = i18n("Could not create temporary file.");
    _log.sorry(_error, i18n("File: %1").arg(_tmp->name()));
    return false;
  }
  return true;
}
