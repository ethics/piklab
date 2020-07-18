/***************************************************************************
 *   Copyright (C) 2005-2007 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef TOPLEVEL_UI_H
#define TOPLEVEL_UI_H

#include <ktoggleaction.h>

#include "devices/base/generic_device.h"
#include "common/gui/dockwindow.h"

//----------------------------------------------------------------------------
class VoltageStatusWidget : public QLabel
{
Q_OBJECT
public:
  VoltageStatusWidget(const QString &label, QWidget *parent);
  void setVoltage(const Device::VoltageValue &v);

private:
  QString _label;
};

//----------------------------------------------------------------------------
class ViewMenuAction : public KToggleAction
{
Q_OBJECT
public:
  ViewMenuAction(K3DockWidget *widget);

signals:
  void activated(QWidget *);

private slots:
  void slotActivated();

private:
  K3DockWidget *_widget;
};

//----------------------------------------------------------------------------
class MenuBarButton : public QToolButton
{
Q_OBJECT
public:
  MenuBarButton(const QString &icon, QWidget *parent);
  virtual QSize sizeHint() const;
};

#endif
