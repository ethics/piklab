/***************************************************************************
 *   Copyright (C) 2010 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "nokde_misc_ui.h"
#include "nokde_misc_ui.moc"

#include "common/gui/misc_gui.h"
#include "common/gui/purl_gui.h"

//-----------------------------------------------------------------------------
KGuiItem KStdGuiItem::cont()
{
  return KGuiItem(i18n("Continue"));
}

KGuiItem KStdGuiItem::add()
{
  return KGuiItem(i18n("Add"));
}

KGuiItem KStdGuiItem::reset()
{
  return KGuiItem(i18n("Reset"));
}

KGuiItem KStdGuiItem::save()
{
  return KGuiItem(i18n("Save"));
}

KGuiItem KStdGuiItem::close()
{
  return KGuiItem(i18n("Close"));
}

KGuiItem KStdGuiItem::del()
{
  return KGuiItem(i18n("Delete"));
}

KGuiItem KStdGuiItem::discard()
{
  return KGuiItem(i18n("Discard"));
}

//-----------------------------------------------------------------------------
void KActionCollection::insert(KAction* action)
{
  if (action != NULL) {
    const char* name = action->objectName();
    if (name != NULL) _map[name] = action;
  }
}

KAction* KActionCollection::action(const QString& name) const
{
  qDebug("action %s\n", (const char*)name.toLatin1());
  Map::const_iterator it = _map.find(name);
  return it != _map.end() ? it->second : NULL;
}

//-----------------------------------------------------------------------------
KAction::KAction(const QString& label, const QString& icon, int key,
          QObject* receiver, const char* slot,
          KActionCollection* collection, const char* name)
: QAction(label, collection)
{
  if (name != NULL) {
    setObjectName(name);
    if (collection != NULL) collection->insert(this);
  }
  if (receiver != NULL
      && slot != NULL) {
    connect(this, SIGNAL(triggered()), receiver, slot);
  }
}

void KAction::plug(QWidget* widget)
{
  widget->addAction(this);
}

void KAction::activate()
{
  trigger();
}

void KAction::slotActivated()
{
}

//-----------------------------------------------------------------------------
KToggleAction::KToggleAction(const QString& label, const QString& icon, int key,
			     QObject* receiver, const char* slot,
			     KActionCollection* collection, const char* name)
: KAction(label, icon, key, receiver, slot, collection, name)
{
  setCheckable(true);
}

//-----------------------------------------------------------------------------
KRecentFilesAction::KRecentFilesAction(const QString& label, const QString& icon,
				       QObject* receiver, const char* slot,
				       KActionCollection* collection, const char* name)
: KAction(label, icon, 0, NULL, NULL, collection, name)
{
}

void KRecentFilesAction::addURL(const KURL& url)
{
}

void KRecentFilesAction::removeURL(const KURL& url)
{
}

void KRecentFilesAction::setMaxItems(uint max)
{
}

//-----------------------------------------------------------------------------
KActionMenu::KActionMenu(const QString& label, const QString& icon, const char* name)
: KAction(label, icon, 0, NULL, NULL, NULL, name)
{
  _popupMenu = new Q3PopupMenu;
}

void KActionMenu::insert(KAction* action)
{
  _popupMenu->addAction(action);
}

//-----------------------------------------------------------------------------
const char* KStdAction::name(StdAction action)
{
  return NULL;
}

KAction* KStdAction::openNew(QObject* receiver, const char* slot, KActionCollection* collection)
{
  return new KAction(i18n("New File..."), "file_new", 0, receiver, slot, collection, "file_new");
}

KAction* KStdAction::open(QObject* receiver, const char* slot, KActionCollection* collection)
{
  return new KAction(i18n("Open..."), "file_open", 0, receiver, slot, collection, "file_open");
}

KRecentFilesAction* KStdAction::openRecent(QObject* receiver, const char* slot, KActionCollection* collection)
{
  return NULL;
}

KAction* KStdAction::save(QObject* receiver, const char* slot, KActionCollection* collection)
{
  return new KAction(i18n("Save"), "file_save", 0, receiver, slot, collection, "file_save");
}

KAction* KStdAction::saveAs(QObject* receiver, const char* slot, KActionCollection* collection)
{
  return new KAction(i18n("Save As..."), "file_save_as", 0, receiver, slot, collection, "file_save_as");
}

KAction* KStdAction::close(QObject* receiver, const char* slot, KActionCollection* collection)
{
  return new KAction(i18n("Close"), "file_close", 0, receiver, slot, collection, "file_close");
}

KAction* KStdAction::quit(QObject* receiver, const char* slot, KActionCollection* collection)
{
  return new KAction(i18n("Quit"), "file_quit", 0, receiver, slot, collection, "file_quit");
}

KAction* KStdAction::preferences(QObject* receiver, const char* slot, KActionCollection* collection)
{
  return new KAction(i18n("Settings..."), "options_configure", 0, receiver, slot, collection, "options_configure");
}

//-----------------------------------------------------------------------------
KLineEdit::KLineEdit(QWidget* parent, const char* name)
: QLineEdit(parent, name)
{
}

KLineEdit::KLineEdit(const QString& text, QWidget* parent, const char* name)
: QLineEdit(text, parent, name)
{
}

void KLineEdit::setTrapReturnKey(bool trap)
{
}

void KLineEdit::setCompletedItems(const QStringList& items)
{
}

//-----------------------------------------------------------------------------
KPushButton::KPushButton(QWidget* parent, const char* name)
: QPushButton(parent, name)
{
}

KPushButton::KPushButton(const KGuiItem& item, QWidget* parent, const char* name)
: QPushButton(IconLoader::loadIcon(item._icon, IconLoader::Small), item._label, parent, name)
{
}

KPushButton::KPushButton(const QString& text, QWidget* parent, const char* name)
: QPushButton(text, parent, name)
{
}

KPushButton::KPushButton(const QPixmap& icon, const QString& text, QWidget* parent, const char* name)
: QPushButton(icon, text, parent, name)
{
}

//-----------------------------------------------------------------------------
KPopupMenu::KPopupMenu(QWidget* parent, const char* name)
: Q3PopupMenu(parent, name)
{
}

void KPopupMenu::insertTitle(const QString& title)
{
  setTitle(title);
}

int KPopupMenu::insertItem(const QPixmap& icon, const QString& text,
			   QObject* receiver, const char* slot,
			   const QKeySequence& accel, int id)
{
  addAction(icon, text, receiver, slot, accel);
  return 0;
}

int KPopupMenu::insertItem(const QPixmap& icon, const QString& text, int id)
{
  addAction(icon, text);
  return 0;
}

int KPopupMenu::insertItem(const QPixmap& icon, int id)
{
  addAction(icon, "");
  return 0;
}

int KPopupMenu::insertItem(const QPixmap& icon, const QString& text,
			   Q3PopupMenu* menu, int id)
{
  QAction* action = addMenu(menu);
  action->setIcon(icon);
  action->setText(text);
  return 0;
}

int KPopupMenu::insertItem(const QString& text, Q3PopupMenu* menu, int id)
{
  QAction* action = addMenu(menu);
  action->setText(text);
  return 0;
}

//-----------------------------------------------------------------------------
KIntNumInput::KIntNumInput(QWidget* parent)
  : QSpinBox(parent)
{
}

KIntNumInput::KIntNumInput(int def, Qt::Orientation orientation, QWidget* parent)
:QSpinBox(parent)
{
}

void KIntNumInput::setRange(int min, int max, int step)
{
  QSpinBox::setRange(min, max);
  setSingleStep(step);
}

//-----------------------------------------------------------------------------
void QToolTip::add(QWidget* widget, const QString& text)
{
}

//-----------------------------------------------------------------------------
QPixmap PURL::icon(FileType type)
{
  if (type.data().xpm_icon) return QPixmap(type.data().xpm_icon);
  if ( hasMimetype(type) ) return QPixmap();
  return QPixmap();
}

bool PURL::hasMimetype(FileType type)
{
  if ( type.data().mimetype==0 ) return false;
  return false;
}

//-----------------------------------------------------------------------------
PURL::Label::Label(const QString &url, const QString &text,
                   QWidget *parent, const char *name)
: KURLLabel(parent)
{
}

void PURL::Label::urlClickedSlot()
{
}

//-----------------------------------------------------------------------------
KURL KFileDialog::getOpenURL(const QString& startDir, const QString& filter, QWidget* widget, const QString& caption)
{
  static KURL url;
  return url;
}

KURL::List KFileDialog::getOpenURLs(const QString& startDir, const QString& filter, QWidget* widget, const QString& caption)
{
  static KURL::List urls;
  return urls;
}

KURL KFileDialog::getSaveURL(const QString& startDir, const QString& filter, QWidget* widget, const QString& caption)
{
  static KURL url;
  return url;
}

//-----------------------------------------------------------------------------
KURL KDirSelectDialog::selectDirectory(const QString& startDir, bool, QWidget* widget, const QString& caption)
{
  static KURL url;
  return url;
}
