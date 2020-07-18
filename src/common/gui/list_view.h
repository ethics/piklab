/***************************************************************************
 *   Copyright (C) 2006-2009 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef KDE_LIST_VIEW_H
#define KDE_LIST_VIEW_H

#define private public
#define protected public
#ifdef NO_KDE
#  include "common/nokde_gui/nokde_klistview.h"
#else
#  include <k3listview.h>
  typedef K3ListView KListView;
#endif
typedef KListView ListViewBase;
#undef private
#undef protected

#include "global_ui.h"

//-----------------------------------------------------------------------------
class EditListViewItem;

class ListView : public ListViewBase
{
Q_OBJECT
public:
  ListView(QWidget *parent = 0, const char *name = 0);
  virtual ~ListView();
  virtual void clear();
  void stopRenaming(bool force);
  virtual QString tooltip(Q3ListViewItem *item, int col) const;

public slots:
  void cancelRenaming() { stopRenaming(false); }
  void finishRenaming() { stopRenaming(true); }

protected:
  virtual bool eventFilter(QObject *o, QEvent *e);

private:
  QValueList<EditListViewItem *> _editItems;

  friend class EditListViewItem;
};

//-----------------------------------------------------------------------------
class EditListViewItem : public KListViewItem
{
public:
  EditListViewItem(ListView *list);
  EditListViewItem(KListViewItem *item);
  virtual ~EditListViewItem();
  void startRename();
  bool isRenaming() const { return _renaming; }

protected:
  virtual int width(const QFontMetrics &fm, const Q3ListView *lv, int c) const;
  virtual void editDone(int col, const QWidget *editWidget);
  virtual void paintCell(QPainter *p, const QColorGroup &cg, int column, int width, int align);

private:
  bool _renaming;
  std::vector<QWidget *> _editWidgets;

  virtual void activate() { startRename(); }
  virtual void startRename(int) { startRename(); }
  virtual void okRename(int) { renameDone(true); }
  virtual void cancelRename(int) { renameDone(false); }
  void renameDone(bool force);
  virtual QWidget *editWidgetFactory(int col) const = 0;
  virtual bool alwaysAcceptEdit(int col) const = 0;
  void removeEditBox();

  friend class ListView;
};

#endif
