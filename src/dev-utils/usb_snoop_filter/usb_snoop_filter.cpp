/***************************************************************************
 *   Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <qfile.h>
#include <qfileinfo.h>
#include <qtextstream.h>
#include <qregexp.h>

#include <kcmdlineargs.h>
#include <klocale.h>

#include "common/common/misc.h"
#include "common/common/number.h"
#include "common/global/about.h"

bool withTime, shortOutput;
QRegExp tbre("TransferBuffer: .+ \\(([0-9]+)\\) length");
QRegExp dre("\\d+.*down\\s+[\\w/]+\\s([\\d\\.]+).*");
QRegExp ure("\\d+.*up\\s+[\\w/]+\\s([\\d\\.]+).*");

QString extractBuffer(uint nb, QTextStream &sin)
{
  uint nbLines = nb/16 + (nb%16 ? 1 : 0);
  QString s;
  for (uint i=0; i<nbLines; i++) {
    QStringList list = QStringList::split(' ', sin.readLine());
    int k = 1;
    for (; k<list.count(); k++) {
      char c = char(fromHex(list[k], 0));
      if ( !isalnum(c) && c!=']' && c!='[' && c!='}' && c!='{' && c!='<' && c!='>' ) {
        if ( !shortOutput ) s += "\\" + list[k];
      } else {
        s += "  ";
        s += c;
      }
    }
  }
  return s;
}

QString parseTransfer(uint nb, QTextStream &sin, bool noTransfer)
{
  QString b = (noTransfer ? "no_transfer" : extractBuffer(nb, sin));
  QString down, up;
  for (;;) {
    QString line = sin.readLine();
    if ( line.isEmpty() ) break;
    if ( dre.exactMatch(line) ) down = dre.cap(1);
    else if ( ure.exactMatch(line) ) up = ure.cap(1);
  }
  QString s;
  if (withTime) {
    if ( !down.isEmpty() ) s += "<-(" + down + ") ";
    if ( !up.isEmpty() ) s += "->(" + up + ") ";
  }
  s += b;
  return s;
}

const Piklab::CmdLineOption OPTIONS[] = {
  { "short", I18N_NOOP("Short output"), 0 },
  { "with-time", I18N_NOOP("Add timestamps"), 0 },
  { "+file", I18N_NOOP("Text file copied and pasted from SnoopyPro"), 0 },
  { 0, 0, 0}
};

int main(int argc, char **argv)
{
  const Piklab::AboutData about("usb_snoop_filter", "usb_snoop_filter", "");
  static Piklab::OptionList list;
  for (uint i = 0; OPTIONS[i].name != NULL; ++i)
    list.append(OPTIONS[i]);
  Piklab::initArgs(about, argc, argv, list);

  KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
  if ( args->count()==0 ) args->usage();
  QFile in(args->url(0).fileName());
  if ( !in.open(IO_ReadOnly) ) qFatal("Could not open file \"%s\".\n", args->url(0).prettyUrl().latin1());
  QTextStream sin(&in);

  QFileInfo finfo(in);
  QFile out(finfo.baseName() + "_clean.txt");
  if ( !out.open(IO_WriteOnly) ) qFatal("Could not open output file.");
  QTextStream sout(&out);

  withTime = args->isSet("with-time");
  shortOutput = args->isSet("short");

  for (;;) {
    QString line = sin.readLine();
    if ( line.isNull() ) break;
    if ( tbre.exactMatch(line) ) {
      QString s = parseTransfer(tbre.cap(1).toUInt(), sin, false);
      if ( !s.isEmpty() ) sout << s << endl;
    }
    if ( line.startsWith("No TransferBuffer") ) {
      line = sin.readLine(); // skip empty line
      if ( !shortOutput) sout << parseTransfer(0, sin, true) << endl;
    } else if ( dre.exactMatch(line) ) {
      if (withTime) sout << "<-(" + dre.cap(1) + ")" << endl;
      for (;;) {
        line = sin.readLine();
        if ( line.isEmpty() ) break;
        if ( ure.exactMatch(line) ) {
          if (withTime) sout << "->(" + ure.cap(1) + ")" << endl;
        }
      }
    }
  }

  return 0;
}
