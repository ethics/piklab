/***************************************************************************
 *   Copyright (C) 2010 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <iostream>

#include <qfile.h>
#include <qtextstream.h>
#include <qregexp.h>

#include "common/common/number.h"

// read output of usbmon

int main(int argc, char **argv)
{
  if ( argc!=2 ) qFatal("Need file as argument.\n");

  QFile in(argv[1]);
  if ( !in.open(IO_ReadOnly) ) qFatal("Could not open file \"%s\".\n", argv[1]);
  QTextStream sin(&in);

  QFile out(QString(argv[1]) + "_clean.txt");
  if ( !out.open(IO_WriteOnly) ) qFatal("Could not open output file.");
  QTextStream sout(&out);

  QRegExp rexp1("([0-9a-f]{8})\\s(\\d+)\\s([SCE])\\s(\\w)(\\w):(\\d+):(\\d+):(\\d+)\\s(.+)");
  QRegExp rexp2("(-?\\d+)\\s(\\d+)\\s(.+)");
  for (;;) {
    QString line = sin.readLine();
    if ( line.isNull() ) break;
    if (!rexp1.exactMatch(line)) {
      qFatal("Unrecognized line: \"%s\".\n", line.latin1());
    }
    QString urbType = rexp1.cap(4);
    QString urbDir = rexp1.cap(5);
    uint endPoint = rexp1.cap(8).toUInt();
    QString s2 = rexp1.cap(9);
    if (s2.startsWith("s")) {
      sout << "setup packet: " << s2.mid(2) << "\n";
    } else {
      if (urbType != "B") continue;
      if (urbDir == "i" && endPoint != 2) continue;
      if (urbDir == "o" && endPoint != 1) continue;
      if (!rexp2.exactMatch(s2)) {
        qFatal("Unrecognized data: \"%s\".\n", s2.latin1());
      }
      s2 = rexp2.cap(3);
      std::cout << (const char*)s2.toAscii() << std::endl;
      if (s2.startsWith("=")) {
        s2 = s2.mid(2);
        QString s;
        for (int i = 0; i<s2.length(); ++i) {
          if (s2[i] == ' ') continue;
          bool ok = false;
          char c = fromString(NumberBase::Hex, s2.mid(i, 2).toAscii(), 2, &ok);
          if (!ok) {
            qFatal("Unrecognized hex: \"%s\".\n", s2.mid(i, 2).latin1());
          }
          if (!isgraph(c)) {
            if (i == 0) break;
          }
          s += c;
          ++i;
        }
        if (!s.isEmpty()) {
          sout << s << "\n";
          if (s.startsWith("[")) {
            sout << "\n";
          }
        }
      }
    }
  }
  return 0;
}
