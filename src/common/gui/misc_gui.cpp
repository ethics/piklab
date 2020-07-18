/***************************************************************************
 *   Copyright (C) 2005-2007 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "misc_gui.h"
#include "misc_gui.moc"

#include <qapplication.h>
#include <qpushbutton.h>
#include <qtimer.h>
#include <qpainter.h>
#include <qmetaobject.h>
#include <qvariant.h>

#include "dialog.h"
#include "common/common/misc.h"
#include "common/gui/number_gui.h"

//-----------------------------------------------------------------------------
QPixmap IconLoader::loadIcon(const QString &name, IconSize size)
{
#ifdef NO_KDE
  QIcon icon(name);
  return icon.pixmap(size);
#else
  KIconLoader loader;
  return loader.loadIcon(name, (KIconLoader::Group)size);
#endif
}

//-----------------------------------------------------------------------------
int BusyCursorStarter::_count = 0;

BusyCursorStarter::BusyCursorStarter()
{
  if (_count == 0) start();
  ++_count;
}

void BusyCursorStarter::start()
{
  QApplication::setOverrideCursor(Qt::WaitCursor, true);
}

BusyCursorStarter::~BusyCursorStarter()
{
  --_count;
  if (_count == 0) QApplication::restoreOverrideCursor();
}

//-----------------------------------------------------------------------------
int BusyCursorPauser::_count = 0;
bool BusyCursorPauser::_overridePaused = false;

BusyCursorPauser::BusyCursorPauser()
{
  if (_count == 0) {
    _overridePaused = QApplication::overrideCursor();
  }
  ++_count;
}

BusyCursorPauser::~BusyCursorPauser()
{
  --_count;
  if (_count==0 && _overridePaused) BusyCursorStarter::start();
}

//-----------------------------------------------------------------------------
void MessageBox::information(const QString &text, Log::ShowMode show, const QString &dontShowAgainName)
{
  if ( show==Log::DontShow ) return;
  BusyCursorPauser pauser;
#ifdef NO_KDE
  QMessageBox::information(qApp->mainWidget(), "Information", text, QMessageBox::Close);
#else
  KMessageBox::information(qApp->mainWidget(), text, QString::null, dontShowAgainName, KMessageBox::Notify | KMessageBox::AllowLink);
#endif
}

void MessageBox::detailedSorry(const QString &text, const QString &details, Log::ShowMode show)
{
  if ( show==Log::DontShow ) return;
  BusyCursorPauser pauser;
#ifdef NO_KDE
  QString msg = text;
  if (!details.isEmpty()) msg += "\n" + details;
  QMessageBox::warning(qApp->mainWidget(), "Sorry", msg, QMessageBox::Close);
#else
  if ( details.isEmpty() ) KMessageBox::sorry(qApp->mainWidget(), text, QString::null, KMessageBox::Notify | KMessageBox::AllowLink);
  else KMessageBox::detailedSorry(qApp->mainWidget(), text, details, QString::null, KMessageBox::Notify | KMessageBox::AllowLink);
#endif
}

bool MessageBox::askContinue(const QString &text, const KGuiItem &buttonContinue, const QString &caption)
{
  BusyCursorPauser pauser;
#ifdef NO_KDE
  // ### TODO: take KGuiItem in consideration
  QMessageBox::StandardButton res = QMessageBox::question(qApp->mainWidget(), caption, text, QMessageBox::Cancel | QMessageBox::Ok, QMessageBox::Ok);
  return (res == QMessageBox::Ok);
#else
  int res = KMessageBox::warningContinueCancel(qApp->mainWidget(), text, caption, buttonContinue);
  return (res == KMessageBox::Continue);
#endif
}

bool MessageBox::questionYesNo(const QString &text, const KGuiItem &yesButton, const KGuiItem &noButton, const QString &caption)
{
  BusyCursorPauser pauser;
#ifdef NO_KDE
  // ### TODO: take KGuiItem in consideration
  QMessageBox::StandardButton res = QMessageBox::question(qApp->mainWidget(), caption, text, QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);
  return (res == QMessageBox::Yes);
#else
  int res = KMessageBox::questionYesNo(qApp->mainWidget(), text, caption, yesButton, noButton);
  return ( res==KMessageBox::Yes );
#endif
}

MessageBox::Result MessageBox::questionYesNoCancel(const QString &text, const KGuiItem &yesButton, const KGuiItem &noButton,
                                                   const QString &caption)
{
  BusyCursorPauser pauser;
#ifdef NO_KDE
  // ### TODO: take KGuiItem in consideration
  QMessageBox::StandardButton res = QMessageBox::question(qApp->mainWidget(), caption, text, QMessageBox::No | QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Yes);
  if (res == QMessageBox::Yes) return Yes;
  if (res == QMessageBox::No) return No;
  return Cancel;
#else
  int res = KMessageBox::questionYesNoCancel(qApp->mainWidget(), text, caption, yesButton, noButton);
  if ( res==KMessageBox::Yes ) return Yes;
  if ( res==KMessageBox::No ) return No;
  return Cancel;
#endif
}

void MessageBox::text(const QString &text, Log::ShowMode show)
{
  if ( show==Log::DontShow ) return;
  BusyCursorPauser pauser;
  TextEditorDialog dialog(text, QString::null, false, qApp->mainWidget());
  dialog.exec();
}

//----------------------------------------------------------------------------
PopupButton::PopupButton(QWidget *parent, const char *name)
: KPushButton(parent)
{
  setObjectName(name);
  init();
}

PopupButton::PopupButton(const QString &text, QWidget *parent, const char *name)
: KPushButton(text, parent)
{
  setObjectName(name);
  init();
}

void PopupButton::init()
{
  _separator = false;
  setFlat(true);
  KMenu *popup = new KMenu(this);
  connect(popup, SIGNAL(activated(int)), SIGNAL(activated(int)));
  setPopup(popup);
}

void PopupButton::appendAction(KAction *action)
{
  if ( _separator && popup()->count()!=0 ) popup()->insertSeparator();
  _separator = false;
  popup()->addAction(action);
}

void PopupButton::appendAction(const QString &label, const QString &icon,
                               QObject *receiver, const char *slot)
{
  KAction* action = new KAction(this);
  action->setText(label);
  action->setIcon(KIcon(icon));
  connect(action, SIGNAL(triggered()), receiver, slot);
  appendAction(action);
}

int PopupButton::appendItem(const QString &label, const QString &icon, int id)
{
  QPixmap pixmap = IconLoader::loadIcon(icon, IconLoader::Small);
  return appendItem(label, pixmap, id);
}

int PopupButton::appendItem(const QString &label, const QPixmap &icon, int id)
{
  if ( _separator && popup()->count()!=0 ) popup()->insertSeparator();
  _separator = false;
#ifdef NO_KDE
  QIcon qicon(icon);
  return popup()->insertItem(qicon, label, id);
#else
  return popup()->insertItem(icon, label, id);
#endif
}

//-----------------------------------------------------------------------------
Splitter::Splitter(const QValueList<int> &defaultSizes, Qt::Orientation o, QWidget *parent, const char *name)
  : QSplitter(o, parent, name), _defaultSizes(defaultSizes)
{
  ASSERT(name);
  setOpaqueResize(true);
  QTimer::singleShot(0, this, SLOT(updateSizes()));
}

Splitter::~Splitter()
{
  GuiConfig gc;
  gc.writeEntry(QString(name()) + "_sizes", sizes());
}

void Splitter::updateSizes()
{
  GuiConfig gc;
  QValueList<int> sizes = gc.readIntListEntry(QString(name()) + "_sizes");
  for (int i=sizes.count(); i<_defaultSizes.count(); i++) sizes.append(_defaultSizes[i]);
  setSizes(sizes);
}

//-----------------------------------------------------------------------------
TabBar::TabBar(QWidget *parent, const char *)
  : KTabBar(parent), _ignoreWheelEvent(false)
{}

void TabBar::wheelEvent(QWheelEvent *e)
{
  if (_ignoreWheelEvent) QApplication::sendEvent(parent(), e); // #### not sure why ignoring is not enough...
  else KTabBar::wheelEvent(e);
}

TabWidget::TabWidget(QWidget *parent, const char *name)
  : KTabWidget(parent)
{
  setObjectName(name);
  setTabBar(new TabBar(this));
}

void TabWidget::setIgnoreWheelEvent(bool ignore)
{
  static_cast<TabBar *>(tabBar())->_ignoreWheelEvent = ignore;
}

void TabWidget::wheelEvent(QWheelEvent *e)
{
  if (static_cast<TabBar *>(tabBar())->_ignoreWheelEvent) e->ignore();
  else KTabWidget::wheelEvent(e);
}

void TabWidget::setTabBar(TabBar *tabbar)
{
  KTabWidget::setTabBar(tabbar);
  connect(tabBar(), SIGNAL(contextMenu( int, const QPoint & )), SLOT(contextMenu( int, const QPoint & )));
  connect(tabBar(), SIGNAL(mouseDoubleClick( int )), SLOT(mouseDoubleClick( int )));
  connect(tabBar(), SIGNAL(mouseMiddleClick( int )), SLOT(mouseMiddleClick( int )));
  connect(tabBar(), SIGNAL(initiateDrag( int )), SLOT(initiateDrag( int )));
  connect(tabBar(), SIGNAL(testCanDecode(const QDragMoveEvent *, bool & )), SIGNAL(testCanDecode(const QDragMoveEvent *, bool & )));
  connect(tabBar(), SIGNAL(receivedDropEvent( int, QDropEvent * )), SLOT(receivedDropEvent( int, QDropEvent * )));
  connect(tabBar(), SIGNAL(moveTab( int, int )), SLOT(moveTab( int, int )));
  connect(tabBar(), SIGNAL(closeRequest( int )), SLOT(closeRequest( int )));
  connect(tabBar(), SIGNAL(wheelDelta( int )), SLOT(wheelDelta( int )));
}

//-----------------------------------------------------------------------------
ComboBox::ComboBox(QWidget *parent, const char *name)
  : KComboBox(parent), _ignoreWheelEvent(false)
{
  setObjectName(name);
}

void ComboBox::wheelEvent(QWheelEvent *e)
{
  if (_ignoreWheelEvent) QApplication::sendEvent(parent(), e); // #### not sure why ignoring is not enough...
  else KComboBox::wheelEvent(e);
}
