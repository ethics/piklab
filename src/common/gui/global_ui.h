/***************************************************************************
 *   Copyright (C) 2009 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef GLOBAL_UI_H
#define GLOBAL_UI_H

#include "common/common/global.h"

#include <qvalidator.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qtimer.h>
#include <qpainter.h>
#include <qradiobutton.h>

#include <QLineEdit>
#include <QTabBar>
#include <QTabWidget>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>
#include <QWheelEvent>
#include <QApplication>
#include <QPainter>
#include <QMetaObject>
#include <QMetaProperty>
#include <QToolButton>
#include <QStatusBar>
#include <QToolTip>

#include <Q3ListView>
#include <Q3WidgetStack>
#include <Q3Frame>
#include <Q3Header>
#include <Q3TextEdit>
#include <Q3PopupMenu>
#include <Q3VGroupBox>
#include <Q3Grid>
#include <Q3VButtonGroup>
#include <Q3PointArray>
#include <Q3TextBrowser>
#include <Q3Wizard>
#include <Q3StrList>
#include <Q3UriDrag>
#include <Q3ProgressBar>

typedef Q3WidgetStack QWidgetStack;

#ifdef NO_KDE

namespace Piklab
{
   typedef QTabBar KTabBar;
   typedef QTabWidget KTabWidget;
   typedef QComboBox KComboBox;
   typedef Q3CheckListItem KCheckListItem;
   typedef Q3TextEdit KTextEdit;
   typedef Qt::Alignment Q3Alignment;
   typedef Q3TextBrowser KTextBrowser;
   typedef Q3Wizard KWizard;
   class KAction;
   class KActionCollection;
}

#else // NO_KDE

#  include <klineedit.h>
#  include <ktabbar.h>
#  include <ktabwidget.h>
#  include <kcombobox.h>
#  include <kpushbutton.h>
#  include <kcursor.h>
#  include <ktextedit.h>
#  include <kmessagebox.h>
#  include <kmenu.h>
#  include <kiconloader.h>
#  include <ktextbrowser.h>
#  include <kurlrequester.h>
#  include <kurllabel.h>
#  include <knuminput.h>
#  include <kfiledialog.h>
#  include <ktextbrowser.h>
#  include <k3wizard.h>
#  include <kstatusbar.h>
#  include <kmenubar.h>
#  include <kedittoolbar.h>
#  include <kdirselectdialog.h>
#  include <kstandardguiitem.h>

typedef Q3ListViewItem KListViewItem;
typedef K3Wizard KWizard;

#endif // NO_KDE

#endif
