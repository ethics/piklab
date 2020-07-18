/***************************************************************************
 *   Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef GLOBAL_H
#define GLOBAL_H

#include <memory>
#include <map>
#include <vector>
#include <list>

#include <qglobal.h>
#include <qdatastream.h>
#include <qstringlist.h>
#include <qobject.h>
#include <qdir.h>
#include <qurl.h>
#include <qtemporaryfile.h>
#include <qmap.h>

#include <QCoreApplication>
#include <QPointer>

#include <Q3ValueVector>
#include <Q3ValueList>
#include <Q3MemArray>
#include <Q3Url>
#include <Q3CString>

#define QValueVector Q3ValueVector
#define QValueList Q3ValueList
#define QMemArray Q3MemArray
#define QMimeSourceFactory Q3MimeSourceFactory
#define QCString Q3CString

#if defined(NO_KDE)

typedef QTemporaryFile KTemporaryFile;

#  include "common/nokde/nokde_klocale.h"
#  include "common/nokde/nokde_kurl.h"
#  include "common/nokde/nokde_kcmdlineargs.h"
#  include "common/nokde/nokde_kaboutdata.h"
#  include "common/nokde/nokde_kprocess.h"

#else

#  include <kapplication.h>
#  include <kglobal.h>
#  include <klocale.h>
#  include <kcmdlineargs.h>
#  include <ktemporaryfile.h>
#  include <kconfig.h>
#  include <kaboutdata.h>
#  include <kurl.h>
#  include <kprocess.h>

#endif

#if defined(Q_OS_WIN)
#  include <windows.h>
#endif

#include "config.h"

#endif
