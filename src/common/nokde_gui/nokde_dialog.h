/***************************************************************************
 *   Copyright (C) 2010 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef NOKDE_DIALOG_H
#define NOKDE_DIALOG_H

#include <QDialog>

#include "nokde_misc_ui.h"

namespace Piklab
{

//-----------------------------------------------------------------------------
class KDialogBase : public QDialog
{
Q_OBJECT
public:
  enum ButtonCode { Ok, Cancel, Close, Apply, User1, User2 };
  enum DialogType { Plain, IconList };

  KDialogBase(QWidget *parent, const char *name, bool modal,
              const QString &caption, int buttonMask, ButtonCode defaultButton, bool separator,
              const QSize &defaultSize = QSize());
  KDialogBase(DialogType type, const QString &caption,
              int buttonMask, ButtonCode defaultButton, QWidget *parent, const char *name,
              bool modal, bool separator, const QSize &defaultSize = QSize());

  void setMainWidget(QWidget* widget);
  QWidget* mainWidget() const;
  void setButtonOK(const KGuiItem& item);
  void setButtonCancel(const KGuiItem& item);
  void setButtonGuiItem(ButtonCode button, const KGuiItem& guiItem);
  void enableButtonOK(bool enabled);
  Q3Frame* addPage(const QStringList& items, const QString& hader, const QPixmap& pixmap);
  void showPage(int page);
  QWidget* plainPage() const;
};

//-----------------------------------------------------------------------------
namespace KFile
{
  enum Mode { Directory, ExistingOnly };
}

class KURLRequester : public KDialogBase
{
Q_OBJECT
public:
  KURLRequester(QWidget* parent);
  void setMode(int mode);
  void setURL(const KURL& url);
  const KURL& url() const;
};

//-----------------------------------------------------------------------------
class KURLLabel : public KDialogBase
{
Q_OBJECT
public:
  KURLLabel(QWidget* parent);
};

} // namespace Piklab

#endif
