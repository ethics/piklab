/***************************************************************************
 *   Copyright (C) 2005-2006 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "about.h"

#ifdef NO_KDE
#  include <QApplication>
#endif

#if defined(Q_WS_WIN)
#  define SVN_REVISION "windows"
#else
#  include "svn_revision.h"
#endif

#include "process.h"

//---------------------------------------------------------------------------
const Piklab::UrlType::Data Piklab::UrlType::DATA[Nb_Types] = {
  { "http://piklab.sourceforge.net"                                        },
  { "http://piklab.sourceforge.net/wiki/index.php/FAQ"                     },
  { "http://sourceforge.net/tracker/?func=add&group_id=138852&atid=743140" }
};

//-----------------------------------------------------------------------------
Piklab::OptionList::OptionList(const CmdLineOption* options)
{
  if (options != NULL) {
    for (uint i=0; options[i].name; i++) {
      append(options[i]);
    }
  }
}

void Piklab::OptionList::append(const CmdLineOption& option)
{
  add(option.name, ki18n(option.description), option.def);
}

//-----------------------------------------------------------------------------
void Piklab::initArgs(const KAboutData& about, int argc, char **argv, const KCmdLineOptions& options)
{
  KCmdLineArgs::init(argc, argv, &about);
  KCmdLineArgs::addCmdLineOptions(options);
}

void Piklab::createApplication(const QString& executable, int argc, char **argv,
                               bool gui, Log::Generic* log)
{
#if defined(NO_KDE)
  if (gui) (void)new QApplication(argc, argv);
  else (void)new QCoreApplication(argc, argv);
#else
  //  if (!gui) KApplication::disableAutoDcopRegistration();
  (void)new KApplication(gui);
  Q_UNUSED(argc);
  Q_UNUSED(argv);
#endif

  QString version = executable;
#if defined(NO_KDE)
#  if defined(Q_OS_WIN)
  version += " \"win32\"";
#  else
  version += " \"qt-only\"";
#  endif
#endif
  version += QString(": version ") + VERSION + " (rev. " + SVN_REVISION + ")";
  if (log != NULL) {
    log->log(Log::LineType::Comment, version);
    Process::LineOutput process;
    QStringList args;
    args += "-srmpo";
    process.setup("uname", args, false);
    process.execute(100);
    if (process.state() == Process::Exited) {
      QStringList lines = process.lines(Process::OutputType::Stdout);
      if (!lines.isEmpty() && !lines[0].isEmpty())
        log->log(Log::LineType::Comment, lines[0], Log::LogFileOnly);
    }
    process.setup("date", QStringList(), false);
    process.execute(100);
    if (process.state() == Process::Exited) {
      QStringList lines = process.lines(Process::OutputType::Stdout);
      if (!lines.isEmpty() && !lines[0].isEmpty())
        log->log(Log::LineType::Comment, lines[0], Log::LogFileOnly);
    }
  } else {
    version += "\n";
    fprintf(stdout, "%s", stringToChar(version));
  }
}

//---------------------------------------------------------------------------
Piklab::AboutData::AboutData(const char *executable,
			     const char *name,
                             const char *description)
  : KAboutData(executable, executable, ki18n(name), VERSION,
	       ki18n(description), KAboutData::License_GPL,
	       ki18n("(c) 2005-2012 Nicolas Hadacek\n(c) 2002-2005 Alain Gibaud\n(c) 2003-2004 Stephen Landamore\n(c) 2005 Lorenz Möenlechner and Matthias Kranz\n(c) 2001-2005 Craig Franklin"),
	       KLocalizedString(),
	       UrlType(UrlType::Homepage).data().url,
	       UrlType(UrlType::BugReport).data().url)
{
  addAuthor(ki18n("Nicolas Hadacek"),
	    ki18n("Author and maintainer."),
	    "hadacek@kde.org");
  addAuthor(ki18n("Alain Gibaud"),
	    ki18n("Author of PiKdev."),
	    "alain.gibaud@free.fr");
  addAuthor(ki18n("Stephen Landamore"),
	    ki18n("Author of LPLAB (original Microchip programmer support)."),
	    "webmaster@landamore.com");
  addAuthor(ki18n("Craig Franklin"),
	    ki18n("Author of gputils"),
	    "craigfranklin@users.sourceforge.net");
  addAuthor(ki18n("Sébastien Laoût"),
	    ki18n("Author of likeback"),
	    "slaout@linux62.org");

  addCredit(ki18n("Brian C. Lane"),
	    ki18n("Original code for direct programming."));
  addCredit(ki18n("Manwlis \"Manos\" Giannos"),
	    ki18n("Direct programming for PIC18F devices."),
	    "mgiannos2000@yahoo.gr");
  addCredit(ki18n("Sean A. Walberg"),
	    ki18n("Direct programming for 16F676/630."),
	    "sean@ertw.com");
  addCredit(ki18n("Mirko Panciri"),
	    ki18n("Support for direct programmers with bidirectionnal buffers."),
	    "mirko.panciri@adept.it");
  addCredit(ki18n("Keith Baker"),
	    ki18n("Direct programming for 16F73/74/76/77."),
	    "susyzygy@pubnix.org" );
  addCredit(ki18n("Lorenz Möenlechner and Matthias Kranz"),
	    ki18n("USB support for ICD2 programmer."),
	    "icd2linux@hcilab.org");
  addCredit(ki18n("Xiaofan Chen"),
	    ki18n("Test of PICkit2 and ICD2 programmer."),
	    "xiaofanc@gmail.com");
  addCredit(ki18n("Homer Reid"),
	    ki18n("Direct programming for dsPICs is inspired from his program \"dspicprg\"."),
	    "homer@homerreid.com");
  addCredit(ki18n("Frank Damgaard"),
            ki18n("Direct programming for 24C EEPROM is inspired from his program \"prog84\"."),
	    "frda@post3.tele.dk");
  addCredit(ki18n("Alberto Maccioni"),
	    ki18n("Designer of Open Programmer."),
	    "albertom78@gmail.com");
}
