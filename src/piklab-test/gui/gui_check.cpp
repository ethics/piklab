/***************************************************************************
 *   Copyright (C) 2012 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "gui_check.h"
#include "gui_check.moc"
#include "libgui/main_global.h"
#include "libgui/toplevel.h"

//----------------------------------------------------------------------------
GuiCheck::GuiCheck()
: _index(0),
  _subIndex(1)
{
}

void GuiCheck::invokeTest()
{
  if (_subIndex == 1) {
    if (!init(_index)) {
      MainWindow::self().close();
      return;
    }
  }
  if (QTest::currentTestFailed()) {
    QTimer::singleShot(300, &MainWindow::self(), SLOT(close()));
    return;
  }

  QString s1 = QString("test%1").arg(_subIndex);
  QString s2 = s1 + "()";
  if (metaObject()->indexOfMethod(s2.latin1()) == -1) {
    ++_index;
    _subIndex = 1;
  } else {
    QMetaObject::invokeMethod(this, s1.latin1());
    ++_subIndex;
  }
  QTimer::singleShot(400, this, SLOT(invokeTest()));
}

//----------------------------------------------------------------------------
void GuiCheckProxy::run(GuiCheck& check)
{
  int argc = 1;
  const char* argv[] = { "gui_check", NULL };
  Main::setupApplication(argc, (char**)argv);
  QTimer::singleShot(200, &check, SLOT(invokeTest()));
  kapp->exec();
}
