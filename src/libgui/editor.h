/***************************************************************************
 *   Copyright (C) 2005 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef EDITOR_H
#define EDITOR_H

#include "common/gui/global_ui.h"
#include "common/global/purl.h"

class Editor : public QWidget
{
Q_OBJECT
public:
  Editor(const QString &title, const QString &tag, QWidget *parent, const char *name);
  Editor(QWidget *parent, const char *name);
  virtual QSizePolicy sizePolicy() const;
  virtual PURL::FileType fileType() const = 0;
  virtual bool isModified() const = 0;
  void setModified(bool modified);
  virtual PURL::Url url() const = 0;
  QString name() const { return _title; }
  QString tag() const { return _tag; }
  void setReadOnly(bool readOnly);
  virtual bool isReadOnly() const = 0;
  bool checkSaved();
  bool reload();
  virtual void setFocus() = 0;
  virtual bool open(const PURL::Url &url) = 0;
  virtual bool save(const PURL::Url &url) = 0;
  virtual void addGui() = 0;
  virtual void removeGui() = 0;
  virtual QValueList<uint> bookmarkLines() const = 0;
  virtual void setBookmarkLines(const QValueList<uint> &lines) = 0;

public slots:
  bool slotLoad();
  virtual bool save();
  virtual bool saveAs();
  void toggleReadOnly() { setReadOnly(!isReadOnly()); }
  virtual void statusChanged() = 0;

signals:
  void modified();
  void guiChanged();
  void statusTextChanged(const QString &text);
  void dropEventPass(QDropEvent *e);

protected:
  QString filename() const;
  virtual void setModifiedInternal(bool modified) = 0;
  virtual void setReadOnlyInternal(bool readOnly) = 0;

private:
  QString _title, _tag;
};

#endif
