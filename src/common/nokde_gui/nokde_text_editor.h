/***************************************************************************
 *   Copyright (C) 2010 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef NOKDE_TEXT_EDITOR_H
#define NOKDE_TEXT_EDITOR_H

#include "common/gui/global_ui.h"
#include "common/global/purl.h"
#include "nokde_dockwindow.h"

//-----------------------------------------------------------------------------
QPixmap SmallIcon(const char* pixmap);

namespace Piklab
{

//-----------------------------------------------------------------------------
namespace KParts
{

class Part : public QObject, public KXMLGUIClient
{
Q_OBJECT
public:
  Part(QObject* parent = NULL, const char* name = NULL);
  void setXMLFile(const QString& filename);
};

class ReadWritePart : public Part
{
Q_OBJECT
public:
  ReadWritePart(QObject* parent = NULL, const char* name = NULL);
  bool isReadWrite() const;
  void setReadWrite(bool readWrite);
  bool isModified() const;
  void setModified(bool modified);
};

} // namespace

//-----------------------------------------------------------------------------
namespace KTextEditor
{

class MarkInterface
{
public:
  enum MarkTypes {
    Execution, markType08, BreakpointActive,
    BreakpointDisabled, BreakpointReached, Error, Bookmark
  };
};

struct Mark
{
  int type;
  int line;
};

} //namespace

//-----------------------------------------------------------------------------
namespace Kate
{

class View : public QWidget, public KXMLGUIClient
{
Q_OBJECT
public:
  void setFocus();
  void setCursorPosition(uint line, uint col);
  void copy();
  void setDynWordWrap(bool on);
  void setEol(int type);
  void show();
  void installPopup(Q3PopupMenu* popup);
  void cursorPosition(uint* line, uint* col);
  void setIconBorder(bool on);

signals:
  void gotFocus(Kate::View* view);
  void cursorPositionChanged();
  void dropEventPass(QDropEvent* event);
  void newStatus();
};

class Document : public KParts::ReadWritePart
{
Q_OBJECT
public:
  void selectAll();
  void clearSelection();
  bool openURL(const KURL& url);
  const KURL& url() const;
  bool saveAs(const KURL&);
  void setPixmap(int type, const QPixmap& pixmap);
  void setMark(int line, int type);
  void removeMark(int line, int type);
  const QPtrList<KTextEditor::Mark>& marks() const;
  const QValueList<View*>& views() const;
  uint hlModeCount() const;
  QString hlModeName(uint i) const;
  uint hlMode() const;
  void setHlMode(uint mode);
  View* createView(QWidget* parent);

signals:
  void hlChanged();
};

} // namespace KPart

} // namespace Piklab

#endif
