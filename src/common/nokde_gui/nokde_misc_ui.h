/***************************************************************************
 *   Copyright (C) 2010 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef NOKDE_MISC_UI_H
#define NOKDE_MISC_UI_H

#include <QString>
#include <QObject>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>

#include "common/gui/global_ui.h"

namespace Piklab
{

class KAction;

//-----------------------------------------------------------------------------
struct KGuiItem
{
  KGuiItem(const QString& label = QString::null, const QString& icon = QString::null)
    : _label(label), _icon(icon) {}
  QString _label;
  QString _icon;
};

//-----------------------------------------------------------------------------
namespace KStdGuiItem
{
  KGuiItem cont();
  KGuiItem add();
  KGuiItem reset();
  KGuiItem save();
  KGuiItem close();
  KGuiItem del();
  KGuiItem discard();
} // namespace

//-----------------------------------------------------------------------------
class KActionCollection : public QObject
{
Q_OBJECT
public:
  void insert(KAction* action);
  KAction* action(const QString& name) const;

private:
  typedef std::map<QString, KAction*> Map;
  Map _map;
};

//-----------------------------------------------------------------------------
class KAction : public QAction
{
Q_OBJECT
public:
  KAction(const QString& label, const QString& icon, int key,
          QObject* receiver, const char* slot,
          KActionCollection* collection, const char* name = NULL);
  void plug(QWidget*);
  void activate();

public slots:
  void slotActivated();
};

//-----------------------------------------------------------------------------
class KToggleAction : public KAction
{
Q_OBJECT
public:
  KToggleAction(const QString& label, const QString& icon = QString::null, int key = 0,
                QObject* receiver = NULL, const char* slot = NULL,
                KActionCollection* collection = NULL, const char* name = NULL);
};

//-----------------------------------------------------------------------------
class KRecentFilesAction : public KAction
{
Q_OBJECT
public:
  KRecentFilesAction(const QString& label, const QString& icon = QString::null,
                     QObject* receiver = NULL, const char* slot = NULL,
                     KActionCollection* collection = NULL, const char* name = NULL);
  void addURL(const KUrl& url);
  void removeURL(const KUrl& url);
  void setMaxItems(uint max);
};

//-----------------------------------------------------------------------------
class KActionMenu : public KAction
{
Q_OBJECT
public:
  KActionMenu(const QString& label, const QString& icon, const char* name);
  void insert(KAction* action);
  Q3PopupMenu* popupMenu() { return _popupMenu; }

private:
  Q3PopupMenu* _popupMenu;
};

//-----------------------------------------------------------------------------
namespace KStdAction
{
  enum StdAction { };
  const char* name(StdAction action);
  KAction* openNew(QObject* receiver, const char* slot, KActionCollection*);
  KAction* open(QObject* receiver, const char* slot, KActionCollection*);
  KRecentFilesAction* openRecent(QObject* receiver, const char* slot, KActionCollection*);
  KAction* save(QObject* receiver, const char* slot, KActionCollection*);
  KAction* saveAs(QObject* receiver, const char* slot, KActionCollection*);
  KAction* close(QObject* receiver, const char* slot, KActionCollection*);
  KAction* quit(QObject* receiver, const char* slot, KActionCollection*);
  KAction* preferences(QObject* receiver, const char* slot, KActionCollection*);
}

//-----------------------------------------------------------------------------
class KLineEdit : public QLineEdit
{
Q_OBJECT
public:
  KLineEdit(QWidget* parent = NULL, const char* name = NULL);
  KLineEdit(const QString& text, QWidget* parent = NULL, const char* name = NULL);
  void setTrapReturnKey(bool trap);
  void setCompletedItems(const QStringList& items);
};

//-----------------------------------------------------------------------------
class KPushButton : public QPushButton
{
Q_OBJECT
public:
  KPushButton(QWidget* parent = NULL, const char* name = NULL);
  KPushButton(const KGuiItem& item, QWidget* parent = NULL, const char* name = NULL);
  KPushButton(const QString& text, QWidget* parent = NULL, const char* name = NULL);
  KPushButton(const QPixmap& icon, const QString& text,
              QWidget* parent = NULL, const char* name = NULL);
};

//-----------------------------------------------------------------------------
class KPopupMenu : public Q3PopupMenu
{
Q_OBJECT
public:
  KPopupMenu(QWidget* parent = NULL, const char* name = NULL);
  void insertTitle(const QString& title);
  int insertItem(const QPixmap&, const QString&, QObject* receiver, const char* slot,
                 const QKeySequence & accel = 0, int id = -1);
  int insertItem(const QPixmap&, const QString&, int id = -1);
  int insertItem(const QPixmap&, int id = -1);
  int insertItem(const QPixmap&, const QString&, Q3PopupMenu*, int id = -1);
  int insertItem(const QString&, Q3PopupMenu*, int id = -1);
};

//-----------------------------------------------------------------------------
class KIntNumInput : public QSpinBox
{
Q_OBJECT
public:
  KIntNumInput(QWidget* parent);
  KIntNumInput(int def, Qt::Orientation orientation, QWidget* parent);
  void setRange(int min, int max, int step);
  int getValue() const { return value(); }
};

//-----------------------------------------------------------------------------
namespace QToolTip
{

  void add(QWidget* widget, const QString& text);

} // namespace

//-----------------------------------------------------------------------------
namespace KFileDialog
{

  KUrl getOpenURL(const QString& startDir, const QString& filter, QWidget* widget, const QString& caption);
  KUrl::List getOpenURLs(const QString& startDir, const QString& filter, QWidget* widget, const QString& caption);
  KUrl getSaveURL(const QString& startDir, const QString& filter, QWidget* widget, const QString& caption);

} // namespace

//-----------------------------------------------------------------------------
namespace KDirSelectDialog
{

  KUrl selectDirectory(const QString& startDir, bool, QWidget* widget, const QString& caption);

} // namespace

} // namespace Piklab

#endif
