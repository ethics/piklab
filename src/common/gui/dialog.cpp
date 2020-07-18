/***************************************************************************
 *   Copyright (C) 2006-2010 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "dialog.h"
#include "dialog.moc"

#include <qtimer.h>
#include <qlabel.h>

#include "misc_gui.h"
#include "common/common/synchronous.h"

//-----------------------------------------------------------------------------
int Dialog::spacingHint()
{
#ifdef NO_KDE
  return 10;
#else
  return KDialog::spacingHint();
#endif
}

Dialog::Dialog(QWidget *parent, const char *name, bool modal,
               const QString &caption,
	       ButtonCodes buttons, ButtonCode defaultButton,
               bool, const QSize &defaultSize)
  : KDialog(parent),
    _defaultSize(defaultSize)
{
  _busyCursor.reset(new BusyCursorStarter);
  ASSERT(name != NULL);
  setObjectName(name);
  setModal(modal);
  setCaption(caption);
  setButtons(buttons);
  setDefaultButton(defaultButton);
  QWidget *main = new QWidget(this);
  setMainWidget(main);

  QTimer::singleShot(0, this, SLOT(updateSize()));
}

Dialog::~Dialog()
{
  GuiConfig gc;
  gc.writeEntry(QString(name()) + "_size", size());
}

void Dialog::updateSize()
{
  GuiConfig gc;
  resize(gc.readSizeEntry(QString(name()) + "_size", _defaultSize));
  _busyCursor.reset(NULL);
}

//-----------------------------------------------------------------------------
PageDialog::PageDialog(QWidget *parent, const char *name, bool modal,
		       const QString &caption,
		       ButtonCodes buttons, ButtonCode defaultButton,
		       bool, const QSize &defaultSize)
  : KPageDialog(parent),
    _defaultSize(defaultSize)
{
  _busyCursor.reset(new BusyCursorStarter);
  ASSERT(name != NULL);
  setObjectName(name);
  setModal(modal);
  setCaption(caption);
  setButtons(buttons);
  setDefaultButton(defaultButton);

  QTimer::singleShot(0, this, SLOT(updateSize()));
}

PageDialog::~PageDialog()
{
  GuiConfig gc;
  gc.writeEntry(QString(name()) + "_size", size());
}

void PageDialog::updateSize()
{
  GuiConfig gc;
  resize(gc.readSizeEntry(QString(name()) + "_size", _defaultSize));
  _busyCursor.reset(NULL);
}

//-----------------------------------------------------------------------------
TreeListDialog::TreeListDialog(QWidget *parent, const char *name, bool modal,
                               const QString &caption,
			       ButtonCodes buttons, ButtonCode defaultButton,
                               bool separator)
  : PageDialog(parent, name, modal, caption, buttons, defaultButton, separator)
{
  setFaceType(Tree);
}

//-----------------------------------------------------------------------------
TextEditorDialog::TextEditorDialog(const QString &text, const QString &caption,
                                   bool wrapAtWidgetWidth, QWidget *parent)
  : Dialog(parent, "text_editor_dialog", true, caption, Close, Close, false, QSize(200, 100))
{
  KTextEdit *w = new KTextEdit(text, this);
  w->setReadOnly(true);
  w->setLineWrapMode(wrapAtWidgetWidth ? KTextEdit::WidgetWidth : KTextEdit::NoWrap);
  setMainWidget(w);
}

//-----------------------------------------------------------------------------
Process::SynchronousDialog::SynchronousDialog(const QString &msg)
: Dialog(qApp->mainWidget(), "sync_process_dialog", true, i18n("Executing..."),
         Cancel, Cancel, false, QSize(200, 100))
{
  QLabel* label = new QLabel(msg, this);
  setMainWidget(label);
}

void Process::SynchronousDialog::finished()
{
  emit accept();
}

Process::State Process::runSynchronouslyDialog(Base &process, const QString& msg)
{
  Synchronous sync(1000);
  QObject::connect(&process, SIGNAL(done(int)), &sync, SLOT(done()));
  if (process.state()!=Running) process.start();
  if (sync.enterLoop()) return process.state();

  // continue running the process with a dialog
  SynchronousDialog dialog(msg);
  QObject::connect(&process, SIGNAL(done(int)), &dialog, SLOT(finished()));
  int res = dialog.exec();
  if (res == QDialog::Rejected) {
    process.kill();
  }
  return process.state();
}
