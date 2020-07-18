/***************************************************************************
 *   Copyright (C) 2007 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "console.h"
#include "console.moc"

#include <qlabel.h>
#include <qdir.h>
#include <klibloader.h>
#include <kparts/part.h>
#include <kservice.h>
#include <kde_terminal_interface.h>

#include "common/common/misc.h"
#include "common/gui/global_ui.h"

ConsoleView::ConsoleView(QWidget *parent)
: QWidget(parent, "console_view"),
  _initialized(false)
{}

void ConsoleView::showEvent(QShowEvent *e)
{
  if ( !_initialized ) {
    _initialized = true;

    QVBoxLayout *top = new QVBoxLayout(this, 0, 10);
    KService::Ptr service = KService::serviceByDesktopName("konsolepart");
    if (!service) {
      QLabel *label = new QLabel(i18n("Could not find \"konsolepart\"; please install konsole."), this);
      label->show();
      top->addWidget(label);
      return;
    }
    KParts::ReadOnlyPart* part = service->createInstance<KParts::ReadOnlyPart>(this, this, QVariantList());
    ASSERT(part != NULL);
    QWidget* widget = part->widget();
    ASSERT(widget != NULL);
    widget->show();
    top->addWidget(widget);
    setFocusProxy(widget);

    QWidget::showEvent(e);
  }
}
