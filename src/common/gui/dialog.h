/***************************************************************************
 *   Copyright (C) 2006-2010 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef DIALOG_H
#define DIALOG_H

#include "global_ui.h"
#include "misc_gui.h"
#include "list_view.h"
#include "common/global/process.h"

#ifdef NO_KDE
#  include "common/nokde_gui/nokde_dialog.h"
#else
#  include <kdialog.h>
#  include <kpagedialog.h>
#endif

//-----------------------------------------------------------------------------
class Dialog : public KDialog
{
Q_OBJECT
public:
  Dialog(QWidget *parent, const char *name,
	 bool modal, const QString &caption,
	 ButtonCodes buttons, ButtonCode defaultButton,
	 bool separator, const QSize &defaultSize = QSize());
  virtual ~Dialog();

  static int spacingHint();

private slots:
  void updateSize();

private:
  QSize                       _defaultSize;
  std::auto_ptr<BusyCursorStarter> _busyCursor;
};
//-----------------------------------------------------------------------------
class PageDialog : public KPageDialog
{
Q_OBJECT
public:
  PageDialog(QWidget *parent, const char *name,
	     bool modal, const QString &caption,
	     ButtonCodes buttons, ButtonCode defaultButton,
	     bool separator, const QSize &defaultSize = QSize());
  virtual ~PageDialog();

private slots:
  void updateSize();

private:
  QSize                       _defaultSize;
  std::auto_ptr<BusyCursorStarter> _busyCursor;
};

//-----------------------------------------------------------------------------
class TreeListDialog : public PageDialog
{
Q_OBJECT
public:
  TreeListDialog(QWidget *parent, const char *name,
		 bool modal, const QString &caption,
		 ButtonCodes buttons, ButtonCode defaultButton,
		 bool separator);
};

//-----------------------------------------------------------------------------
class TextEditorDialog : public Dialog
{
Q_OBJECT
public:
  TextEditorDialog(const QString &text, const QString &caption,
                   bool wrapAtWidgetWidth, QWidget *parent);
};

//-----------------------------------------------------------------------------
namespace Process
{

class SynchronousDialog : public Dialog
{
Q_OBJECT
public:
    SynchronousDialog(const QString& msg);

public slots:
  void finished();
};

extern State runSynchronouslyDialog(Base &process, const QString& msg);

} // namespace

#endif
