/***************************************************************************
 *   Copyright (C) 2010 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "nokde_text_editor.h"
#include "nokde_text_editor.moc"

//-----------------------------------------------------------------------------
QPixmap SmallIcon(const char* pixmap)
{
  return QPixmap();
}

//-----------------------------------------------------------------------------
KParts::Part::Part(QObject* parent, const char* name)
  : QObject(parent, name)
{
}

void KParts::Part::setXMLFile(const QString& filename)
{
}

//-----------------------------------------------------------------------------
KParts::ReadWritePart::ReadWritePart(QObject* parent, const char* name)
  : Part(parent, name)
{
}

bool KParts::ReadWritePart::isReadWrite() const
{
  return false;
}

void KParts::ReadWritePart::setReadWrite(bool readWrite)
{
}

bool KParts::ReadWritePart::isModified() const
{
  return false;
}

void KParts::ReadWritePart::setModified(bool modified)
{
}

//-----------------------------------------------------------------------------
void Kate::View::setFocus()
{
}

void Kate::View::setCursorPosition(uint line, uint col)
{
}

void Kate::View::copy()
{
}

void Kate::View::setDynWordWrap(bool on)
{
}

void Kate::View::setEol(int type)
{
}

void Kate::View::show()
{
}

void Kate::View::installPopup(Q3PopupMenu* popup)
{
}

void Kate::View::cursorPosition(uint* line, uint* col)
{
}

void Kate::View::setIconBorder(bool on)
{
}

//-----------------------------------------------------------------------------
void Kate::Document::selectAll()
{
}

void Kate::Document::clearSelection()
{
}

bool Kate::Document::openURL(const KURL& url)
{
  return false;
}

const KURL& Kate::Document::url() const
{
  static KURL url;
  return url;
}

bool Kate::Document::saveAs(const KURL&)
{
  return false;
}

void Kate::Document::setPixmap(int type, const QPixmap& pixmap)
{
}

void Kate::Document::setMark(int line, int type)
{
}

void Kate::Document::removeMark(int line, int type)
{
}

const QPtrList<KTextEditor::Mark>& Kate::Document::marks() const
{
  static QPtrList<KTextEditor::Mark> list;
  return list;
}

const QValueList<Kate::View*>& Kate::Document::views() const
{
  static QValueList<View*> list;
  return list;
}

uint Kate::Document::hlModeCount() const
{
  return 0;
}

QString Kate::Document::hlModeName(uint i) const
{
  return QString();
}

uint Kate::Document::hlMode() const
{
  return 0;
}

void Kate::Document::setHlMode(uint mode)
{
}

Kate::View* Kate::Document::createView(QWidget* parent)
{
  return NULL;
}
