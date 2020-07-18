/***************************************************************************
 *   Copyright (C) 2005-2006 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "misc.h"

#include <unistd.h>

#include <qregexp.h>
#include <qtimer.h>

#include "global.h"
#include "number.h"

//-----------------------------------------------------------------------------
uchar bin2bcd(uchar bin)
{
  char h = bin / 10;
  char l = bin % 10;
  return (h*16) + l;
}

uchar bcd2bin(uchar bcd)
{
  char h = bcd / 16;
  char l = bcd % 16;
  return (h*10) + l;
}

QString escapeXml(const QString &cs)
{
  QString s;
  for (uint i=0; i<uint(cs.length()); i++) {
    if ( cs[i]=='<' ) s += "&lt;";
    else if ( cs[i]=='>' ) s += "&gt;";
    else s += cs[i];
  }
  return s;
}

QString htmlTableRow(const QString &title, const QString &value)
{
  return "<tr><td>" + title + ":</td><td>" + value + "</td></tr>";
}

void piklabAssert(const char *s, const char *file, int line)
{
  qDebug("ASSERT: \"%s\" in %s (%d)", s, file, line);
}

bool checkAvailable(const QByteArray &data, uint offset, uint nbBytes)
{
  return ( offset+nbBytes<=uint(data.size()) );
}

Q_UINT32 getULong(const QByteArray &data, uint offset, uint nbBytes, bool *ok)
{
  ASSERT( nbBytes<=8 );
  if ( !checkAvailable(data, offset, nbBytes) ) {
    if (ok) *ok = false;
    return 0;
  }
  if (ok) *ok = true;
  Q_UINT32 r = 0;
  for (uint i=0; i<nbBytes; i++) r += Q_UINT8(data[offset+i]) << (8*i);
  return r;
}

QString formatDouble(double v, uint precision)
{
#if defined(NO_KDE)
  return QString::number(v, 'g', precision);
#else
  return KGlobal::locale()->formatNumber(v, precision);
#endif
}

//-----------------------------------------------------------------------------
const char* stringToChar(const QString &s)
{
  static QByteArray ba;
  ba = s.toLatin1();
  return ba.constData();
}
