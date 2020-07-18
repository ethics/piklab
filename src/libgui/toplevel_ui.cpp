/***************************************************************************
 *   Copyright (C) 2005-2007 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "toplevel_ui.h"
#include "toplevel_ui.moc"

#include "common/gui/misc_gui.h"

//----------------------------------------------------------------------------
VoltageStatusWidget::VoltageStatusWidget(const QString &label, QWidget *parent)
  : QLabel(parent), _label(label)
{}

void VoltageStatusWidget::setVoltage(const Device::VoltageValue &v)
{
  setText(i18n("%1: %2").arg(_label).arg(v.label()));
}

//----------------------------------------------------------------------------
ViewMenuAction::ViewMenuAction(K3DockWidget *widget)
  : KToggleAction(widget->tabPageLabel(), widget), _widget(widget)
{
  connect(this, SIGNAL(triggered()), SLOT(slotActivated()));
}

void ViewMenuAction::slotActivated()
{
  emit activated(_widget);
}

//----------------------------------------------------------------------------
MenuBarButton::MenuBarButton(const QString &icon, QWidget *parent)
  : QToolButton(parent, "menu_bar_button")
{
  QFontMetrics fm(font());
  int h = fm.height();
  //h += 2*style().pixelMetric(QStyle::PM_DefaultFrameWidth, this);
  setIconSet(KIcon(icon));
  setFixedHeight(h);
  setUsesTextLabel(false);
  setAutoRaise(true);
}

QSize MenuBarButton::sizeHint() const
{
  return QSize(QToolButton::sizeHint().width(), height());
}
