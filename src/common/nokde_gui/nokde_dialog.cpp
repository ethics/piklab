/***************************************************************************
 *   Copyright (C) 2010 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "nokde_dialog.h"
#include "nokde_dialog.moc"

using namespace Piklab;

//-----------------------------------------------------------------------------
KDialogBase::KDialogBase(QWidget *parent, const char *name, bool modal,
			 const QString &caption, int buttonMask, ButtonCode defaultButton, bool separator,
			 const QSize &defaultSize)
  : QDialog(parent, name)
{
}

KDialogBase::KDialogBase(DialogType type, const QString &caption,
			 int buttonMask, ButtonCode defaultButton, QWidget *parent, const char *name,
			 bool modal, bool separator, const QSize &defaultSize)
  : QDialog(parent, name)
{
}

void KDialogBase::setMainWidget(QWidget* widget)
{
}

QWidget* KDialogBase::mainWidget() const
{
  return NULL;
}

void KDialogBase::setButtonOK(const KGuiItem& item)
{
}

void KDialogBase::setButtonCancel(const KGuiItem& item)
{
}

void KDialogBase::setButtonGuiItem(ButtonCode button, const KGuiItem& guiItem)
{
}

void KDialogBase::enableButtonOK(bool enabled)
{
}

Q3Frame* KDialogBase::addPage(const QStringList& items, const QString& hader, const QPixmap& pixmap)
{
  return NULL;
}

void KDialogBase::showPage(int page)
{
}

QWidget* KDialogBase::plainPage() const
{
  return NULL;
}

//-----------------------------------------------------------------------------
KURLRequester::KURLRequester(QWidget* parent)
  : KDialogBase(parent, "kurl_requester", true, i18n("Enter url"), Ok|Cancel, Cancel, false)
{
}

void KURLRequester::setMode(int mode)
{
}

void KURLRequester::setURL(const KURL& url)
{
}

const KURL& KURLRequester::url() const
{
  static KURL url;
  return url;
}

//-----------------------------------------------------------------------------
KURLLabel::KURLLabel(QWidget* parent)
  : KDialogBase(parent, "kurl_label", true, "", Close, Close, false)
{
}
