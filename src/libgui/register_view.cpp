/***************************************************************************
 *   Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "register_view.h"
#include "register_view.moc"

#include "devices/base/device_group.h"
#include "devices/gui/device_group_ui.h"
#include "progs/base/prog_group.h"
#include "common/gui/misc_gui.h"
#include "main_global.h"
#include "editor_manager.h"
#include "gui_debug_manager.h"

//-----------------------------------------------------------------------------
Register::MainView::MainView(const QString &title, const QString &tag, QWidget *parent)
  : DeviceEditor(title, tag, parent, "register_view"), _debugging(false)
{}

void Register::MainView::setDevice(bool force)
{
  bool oldDebugging = _debugging;
  _debugging = Main::programmerGroup().isDebugger();
  DeviceEditor::setDevice(force || oldDebugging!=_debugging);
}

QWidget *Register::MainView::createView(const Device::Data& data, QWidget *parent)
{
  Register::View *view = Device::groupui(data).createRegisterView(parent);
  if (view != NULL) {
    view->updateView();
    return view;
  }

  QWidget *w = new QWidget(parent);
  QVBoxLayout *vbox = new QVBoxLayout(w, 10, 10);
  QLabel *label = new QLabel(i18n("The selected device has no register."), w);
  vbox->addWidget(label);
  vbox->addStretch(1);
  return w;
}
