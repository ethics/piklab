/***************************************************************************
 *   Copyright (C) 2005-2006 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "common/gui/purl_gui.h"

#include <krun.h>

//-----------------------------------------------------------------------------
QPixmap PURL::icon(FileType type)
{
  if (type.data().xpm_icon) return QPixmap(type.data().xpm_icon);
  if ( hasMimetype(type) ) {
    QString icon = KMimeType::mimeType(type.data().mimetype)->iconName();
    return KIconLoader::global()->loadMimeTypeIcon(icon, KIconLoader::Small);
  }
  return QPixmap();
}

bool PURL::hasMimetype(FileType type)
{
  if ( type.data().mimetype==0 ) return false;
  KMimeType::Ptr ptr = KMimeType::mimeType(type.data().mimetype);
  return ( ptr!=KMimeType::defaultMimeTypePtr() );
}

//-----------------------------------------------------------------------------
PURL::Label::Label(const QString &url, const QString &text,
                   QWidget *parent, const char *name)
  : KUrlLabel(url, text, parent)
{
  setObjectName(name);
  connect(this, SIGNAL(leftClickedUrl()), SLOT(urlClickedSlot()));
}

void PURL::Label::urlClickedSlot()
{
  (void)new KRun(url(), this);
}

