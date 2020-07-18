/***************************************************************************
 *   Copyright (C) 2006-2007 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "watch_view.h"
#include "watch_view.moc"

#include "main_global.h"
#include "register_view.h"
#include "devices/base/device_group.h"
#include "devices/gui/device_group_ui.h"
#include "gui_debug_manager.h"
#include "editor_manager.h"
#include "common/gui/list_container.h"

//-----------------------------------------------------------------------------
Register::BaseListView::BaseListView(QWidget *parent)
  : ListView(parent), _root(0)
{
  header()->hide();
  setSorting(-1);
  setFullWidth(true);
  setRootIsDecorated(false);
  setAllColumnsShowFocus(true);
  connect(this, SIGNAL(mouseButtonClicked(int, Q3ListViewItem *, const QPoint &, int)),
          SLOT(itemClicked(int, Q3ListViewItem *, const QPoint &, int)));
  connect(this, SIGNAL(contextMenuRequested(Q3ListViewItem *, const QPoint &, int)),
          SLOT(contextMenu(Q3ListViewItem *, const QPoint &, int)));
}

//-----------------------------------------------------------------------------
Register::RegisterListView::RegisterListView(QWidget *parent)
  : BaseListView(parent)
{
  addColumn(QString::null);
}

void Register::RegisterListView::init(const Device::Data *data)
{
  delete _root;
  _root = new ListViewItemContainer(i18n("Registers"), this);
  _root->setPixmap(0, IconLoader::loadIcon("media-flash", IconLoader::Small));
  _root->setSelectable(false);
  _root->setOpen(true);
  if (data) Device::groupui(*data).fillWatchListContainer(_root, _ids);
}

void Register::RegisterListView::updateView()
{
  Q3ListViewItemIterator it(_root);
  for (; it.current(); ++it) {
    int id = _root->id(it.current());
    if ( id==-1 || _ids[id].type()==Invalid ) continue;
    bool watched = Register::list().isWatched(_ids[id]);
    static_cast<Q3CheckListItem *>(it.current())->setOn(watched);
  }
}

void Register::RegisterListView::itemClicked(int button, Q3ListViewItem *item, const QPoint &, int)
{
  if ( item==0 || button!=Qt::LeftButton ) return;
  if ( item==firstChild() ) Main::editorManager().openEditor(ProjectManager::SpecialEditorType::Registers);
  int id = _root->id(item);
  if ( id==-1 || _ids[id].type()==Invalid ) return;
  bool watched = Register::list().isWatched(_ids[id]);
  static_cast<Q3CheckListItem *>(item)->setOn(!watched);
  Debugger::manager->setRegisterWatched(_ids[id], !watched);
}

//-----------------------------------------------------------------------------
Register::WatchedListView::WatchedListView(QWidget *parent)
  : BaseListView(parent), _popup(0), _base(NumberBase::Hex)
{
  setSorting(0);
  addColumn(QString::null);
  addColumn(QString::null);
  addColumn(QString::null);

  _root = new ListViewItemContainer(i18n("Watched"), this);
  _root->setPixmap(0, IconLoader::loadIcon("zoom-select", IconLoader::Small));
  _root->setSelectable(false);
  _root->setOpen(true);
}

KMenu* Register::WatchedListView::appendFormatMenu(KMenu* parent, uint offset)
{
  QPixmap icon = IconLoader::loadIcon("fonts", IconLoader::Small);
  KMenu* popup = new KMenu;
  popup->addTitle(i18n("Format"));
  FOR_EACH(NumberBase, base) popup->insertItem(base.label(), offset + base.type());
  parent->insertItem(icon, i18n("Format"), popup);
  return popup;
}

void Register::WatchedListView::init(const Device::Data *data)
{
  delete _popup;
  _popup = 0;
  if ( data==0 ) return;
  _popup = new PopupContainer(i18n("Watch Register"), this);
  Device::groupui(*data).fillWatchListContainer(_popup, _ids);
  _popup->insertSeparator();
  _formatPopup = appendFormatMenu(_popup, _ids.count());
  QPixmap icon = IconLoader::loadIcon("edit-clear", IconLoader::Small);
  _popup->insertItem(icon, i18n("Clear"), Debugger::manager, SLOT(stopWatchAll()));
}

void Register::WatchedListView::updateView()
{
  // delete items not watched anymore
  for (Q3ListViewItem *item=_root->firstChild(); item;) {
    ListViewItem *ritem = static_cast<ListViewItem *>(item);
    item = item->nextSibling();
    if ( !Register::list().isWatched(ritem->data()) ) delete ritem;
  }
  // add new items
  QValueList<Register::TypeData> watched = Register::list().watched();
  QValueVector<ListViewItem *> items(watched.count());
  for (int k=0; k<watched.count(); k++) {
    Q3ListViewItem *item = _root->firstChild();
    for (; item; item=item->nextSibling())
      if ( static_cast<ListViewItem *>(item)->data()==watched[k] ) break;
    if (item) {
      items[k] = static_cast<ListViewItem *>(item);
      items[k]->updateView();
    } else {
      items[k] = Device::groupui(Main::deviceData()).createWatchItem(watched[k], _root);
      items[k]->setBase(_base);
    }
  }
}

QString Register::WatchedListView::tooltip(Q3ListViewItem *item, int col) const
{
  if ( item==0 ) return QString::null;
  if ( item->rtti()==Register::PortBitRtti ) return static_cast<PortBitListViewItem *>(item)->tooltip(col);
  if ( item->rtti()==Register::RegisterRtti ) return static_cast<ListViewItem *>(item)->tooltip(col);
  return QString::null;
}

void Register::WatchedListView::itemClicked(int button, Q3ListViewItem *item, const QPoint &, int col)
{
  if ( item==0 || button!=Qt::LeftButton ) return;
  else if ( item->rtti()==RegisterRtti ) {
    if ( col==2 && Main::programmerState()==Programmer::Halted ) static_cast<ListViewItem *>(item)->startRename();
    else item->setOpen(!item->isOpen());
  }
}

void Register::WatchedListView::contextMenu(Q3ListViewItem *item, const QPoint &p, int)
{
  if ( item==0 ) return;
  if ( item==firstChild() ) {
    if ( _popup==0 ) return;
    FOR_EACH(NumberBase, base) _formatPopup->setItemChecked(_ids.count()+base.type(), _base==base);
    QAction* action = _popup->exec(p);
    if ( action==NULL ) return;
    // #### FIXME
    // if ( res<int(_ids.count()) ) Debugger::manager->setRegisterWatched(_ids[res], true);
    //else {
    //  _base = NumberBase::Type(res-_ids.count());
    //  for (Q3ListViewItem *item=_root->firstChild(); item; item=item->nextSibling())
    //    static_cast<ListViewItem *>(item)->setBase(_base);
    //}
  } else {
    if ( item->rtti()==Register::PortBitRtti ) return;
    Register::ListViewItem *ritem = static_cast<ListViewItem *>(item);
    std::auto_ptr<KMenu> pop(new KMenu);
    pop->addTitle(ritem->label());
    KMenu* fpop = appendFormatMenu(pop.get(), 0);
    FOR_EACH(NumberBase, base) fpop->setItemChecked(base.type(), ritem->base()==base);
    pop->insertSeparator();
    QPixmap icon = IconLoader::loadIcon("edit-rename", IconLoader::Small);
    int editId = pop->insertItem(icon, i18n("Edit"));
    pop->setItemEnabled(editId, Main::programmerState()==Programmer::Halted);
    icon = IconLoader::loadIcon("edit-delete", IconLoader::Small);
    int removeId = pop->insertItem(icon, i18n("Remove"));
    QAction* action = pop->exec(p);
    // ### FIXME
    //if ( res==editId ) ritem->startRename();
    //else if ( res==removeId ) Debugger::manager->setRegisterWatched(ritem->data(), false);
    //else if ( res>=0 ) ritem->setBase(NumberBase::Type(res));
  }
}

//-----------------------------------------------------------------------------
Register::WatchView::WatchView(QWidget *parent)
: QWidget(parent, "watch_view"),
  GenericView(Register::list()),
  _data(NULL)
{
  QVBoxLayout *top = new QVBoxLayout(this);
  QValueList<int> sizes;
  sizes.append(50);
  Splitter *splitter = new Splitter(sizes, Qt::Vertical, this, "watch_window_splitter");
  top->addWidget(splitter);

  _registerListView = new RegisterListView(splitter);
  _watchedListView = new WatchedListView(splitter);
}

void Register::WatchView::init(bool force)
{
  if ( !force && _data == &Main::deviceData() ) return;
  _data = &Main::deviceData();
  _registerListView->init(_data);
  _watchedListView->init(_data);
  updateView();
}

void Register::WatchView::updateView()
{
  _registerListView->updateView();
  _watchedListView->updateView();
}
