/***************************************************************************
 *   Copyright (C) 2005-2012 Nicolas Hadacek <hadacek@kde.org>             *
 *   Copyright (C) 2003-2004 Alain Gibaud <alain.gibaud@free.fr>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include "common/global/pfile.h"
#include "common/gui/viewer.h"
#include "common/gui/misc_gui.h"
#include "editor.h"
#include "progs/manager/breakpoint.h"

//-----------------------------------------------------------------------------
class TextEditor : public Editor
{
Q_OBJECT
public:
  TextEditor(const QString& title, const QString& tag, QWidget* parent, const char* name = NULL);
  TextEditor(bool withDebugger, QWidget *parent, const char *name = NULL);
  virtual PURL::FileType fileType() const { return url().fileType(); }
  virtual PURL::Url url() const { return _document->url(); }
  virtual bool isModified() const;
  virtual bool isReadOnly() const;
  virtual void addGui();
  virtual void removeGui();
  virtual void setFocus() { _view->setFocus(); }
  static QPixmap pixmap(Breakpoint::MarkType type);
  void setMark(uint line, Breakpoint::MarkType type);
  void clearBreakpointMarks();
  void setCursorPosition(uint line, uint column) { _view->setCursorPosition(KTextEditor::Cursor(line, column)); }
  uint cursorLine() const;
  virtual bool open(const PURL::Url &url);
  virtual bool save() { return Editor::save(); }
  virtual bool save(const PURL::Url &url) { return _document->saveAs(url.kurl()); }
  virtual bool eventFilter(QObject *o, QEvent *e);
  virtual QValueList<uint> bookmarkLines() const;
  virtual void setBookmarkLines(const QValueList<uint> &lines);
  static void configure();

public slots:
  void addView();
  void removeCurrentView();
  void focusIn(KTextEditor::View *);
  void highlightModeChanged();
  void modeChanged();
  virtual void statusChanged();
  void selectAll() { _view->setSelection(KTextEditor::Range(KTextEditor::Cursor(), _document->documentEnd())); }
  void deselect() { _view->removeSelection(); }
  void copy() { /*_view->copy();*/ }

protected:
  enum EolType { Dos = 1, Unix = 0, Mac = 2 };
  KTextEditor::Document *_document;
  KTextEditor::View     *_view;

private slots:
  void addToDebugManager();

private:
  QSplitter *_split;
  bool       _oldModified, _oldReadOnly;
  struct MarkTypeData {
    uint        type;
    const char* pixmap;
  };
  static const MarkTypeData MARK_TYPE_DATA[Breakpoint::Nb_MarkTypes];

private:
  virtual void setModifiedInternal(bool modified);
  virtual void setReadOnlyInternal(bool readOnly);
  uint highlightMode(const QString &name) const;
  void clearMarks(uint type);
  void init(bool withDebugger);
  void updateBreakpointActions(const Breakpoint::Data*);
};

//-----------------------------------------------------------------------------
class SimpleTextEditor : public TextEditor
{
Q_OBJECT
public:
  SimpleTextEditor(const QString& title, const QString& tb, QWidget* parent, const char* name = NULL);
  SimpleTextEditor(bool withDebugger, QWidget *parent, const char *name = NULL);
  void setFileType(PURL::FileType type) { _type = type; }
  virtual PURL::FileType fileType() const { return _type; }
  bool setText(const QString &text);
  virtual bool open(const PURL::Url &url);

private:
  Log::StringView _sview;
  PURL::FileType  _type;
  PURL::TempFile  _file;
};

#endif
