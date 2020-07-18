/***************************************************************************
 *   Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                  *
 *   Copyright (C) 1992-2003 Trolltech AS.                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "list_view.h"
#include "list_view.moc"

//----------------------------------------------------------------------------
ListView::ListView(QWidget *parent, const char *name)
  : ListViewBase(parent)
{
  setObjectName(name);
}

ListView::~ListView()
{
}

QString ListView::tooltip(Q3ListViewItem*, int) const
{
  return QString::null;
}

void ListView::clear()
{
  _editItems.clear();
  ListViewBase::clear();
}

bool ListView::eventFilter(QObject *o, QEvent *e)
{
  if (o == viewport() && e->type() == QEvent::ToolTip) {
    QHelpEvent *helpEvent = static_cast<QHelpEvent*>(e);
    Q3ListViewItem* item = itemAt(helpEvent->pos());
    QString s;
    if (item != NULL) s = tooltip(item, 0);
    if (!s.isEmpty()) {
      QToolTip::showText(helpEvent->globalPos(), s);
    } else {
      QToolTip::hideText();
      e->ignore();
    }
    return true;
  }

  for (QValueList<EditListViewItem *>::const_iterator it = _editItems.begin();
       it != _editItems.end();
       ++it) {
    for (uint i=0; i<(*it)->_editWidgets.size(); i++) {
      if ( (*it)->_editWidgets[i]==o ) {
        //qDebug("event %i", e->type());
        if (e->type() == 9999) {
          (*it)->renameDone(false);
          return true;
        }
        switch (e->type()) {
          case QEvent::KeyPress: {
            QKeyEvent *ke = static_cast<QKeyEvent *>(e);
            switch (ke->key()) {
            case Qt::Key_Enter:
            case Qt::Key_Return:
                (*it)->renameDone(true);
                return true;
            case Qt::Key_Escape:
                (*it)->removeEditBox();
                return true;
            }
            break;
          }
          case QEvent::FocusOut: {
            //qDebug("focus out %i %i=%i", qApp->focusWidget(), focusWidget(), (*it)->_editWidgets[i]);
#ifdef NO_KDE
            QWidget* fw = QApplication::focusWidget();
#else
            QWidget* fw = qApp->focusWidget();
#endif
            if ( fw != NULL && focusWidget()==(*it)->_editWidgets[i] ) break;
            //qDebug("ext");
            QCustomEvent *e = new QCustomEvent(9999);
#ifdef NO_KDE
            QCoreApplication::postEvent(o, e);
#else
            QApplication::postEvent(o, e);
#endif
            return true;
          }
          default:
            break;
        }
      }
    }
  }
  return ListViewBase::eventFilter(o, e);
}

void ListView::stopRenaming(bool force)
{
  QValueList<EditListViewItem *>::const_iterator it;
  for (it=_editItems.begin(); it!=_editItems.end(); ++it)
    if ( (*it)->isRenaming() ) (*it)->renameDone(force);
}

//----------------------------------------------------------------------------
EditListViewItem::EditListViewItem(ListView *list)
  : KListViewItem(list), _renaming(false)
{
  setRenameEnabled(0, true);
  list->_editItems.append(this);
}

EditListViewItem::EditListViewItem(KListViewItem *item)
  : KListViewItem(item), _renaming(false)
{
  setRenameEnabled(0, true);
  static_cast<ListView *>(listView())->_editItems.append(this);
}

EditListViewItem::~EditListViewItem()
{
  if ( listView() ) static_cast<ListView *>(listView())->_editItems.remove(this);
}

void EditListViewItem::paintCell(QPainter *p, const QColorGroup &cg, int column, int width, int align)
{
  if ( column<int(_editWidgets.size()) && _editWidgets[column] )
    p->fillRect(0, 0, width, height(), cg.color(QColorGroup::Background));
  else KListViewItem::paintCell(p, cg, column, width, align);
}

void EditListViewItem::startRename()
{
  if ( !renameEnabled(0) ) return;
  Q3ListView *lv = listView();
  if ( !lv ) return;
  KListViewItem::startRename(0);
  if (renameBox != NULL) {
    renameBox->removeEventFilter(lv);
    renameBox->hide();
    lv->removeChild(renameBox);
  }
  _renaming = true;
  _editWidgets.resize(lv->columns());
  for (uint i=0; i<_editWidgets.size(); i++) {
    QRect r = lv->itemRect(this);
    r = QRect(lv->viewportToContents(r.topLeft()), r.size());
    r.setLeft(lv->header()->sectionPos(i));
    r.setWidth(lv->header()->sectionSize(i) - 1);
    if ( i==0 ) r.setLeft(r.left() + lv->itemMargin() + (depth() + (lv->rootIsDecorated() ? 1 : 0)) * lv->treeStepSize() - 1);
    if ( (lv->contentsX() + lv->visibleWidth())<(r.x() + r.width()) )
      lv->scrollBy((r.x() + r.width() ) - ( lv->contentsX() + lv->visibleWidth() ), 0);
    if ( r.width()>lv->visibleWidth() ) r.setWidth(lv->visibleWidth());

    _editWidgets[i] = editWidgetFactory(i);
    if ( _editWidgets[i]==NULL ) continue;
    _editWidgets[i]->installEventFilter(lv);
    lv->addChild(_editWidgets[i], r.x(), r.y());
    uint w = QMIN(r.width(), _editWidgets[i]->sizeHint().width());
    _editWidgets[i]->resize(w, r.height());
    lv->viewport()->setFocusProxy(_editWidgets[i]);
    _editWidgets[i]->setFocus();
    _editWidgets[i]->show();
  }
}

void EditListViewItem::removeEditBox()
{
  Q3ListView *lv = listView();
  if (lv == NULL) return;
  _renaming = false;
  bool resetFocus = false;
  for (uint i=0; i<_editWidgets.size(); i++) {
    if ( _editWidgets[i]==NULL ) continue;
    if ( lv->viewport()->focusProxy()==_editWidgets[i] ) resetFocus = true;
    lv->viewport()->setFocusProxy(NULL);
    _editWidgets[i]->hide();
    _editWidgets[i] = NULL;
  }
  _editWidgets.clear();
  delete renameBox;
  renameBox = NULL;
  if (resetFocus) {
    lv->viewport()->setFocusProxy(lv);
    lv->setFocus();
  }
}

void EditListViewItem::editDone(int col, const QWidget *edit)
{
  QString s = edit->property("currentText").toString();
  emit listView()->itemRenamed(this, col, s);
}

void EditListViewItem::renameDone(bool force)
{
  Q3ListView *lv = listView();
  if (lv == NULL || !_renaming) return;
  _renaming = false;
  for (uint i=0; i<_editWidgets.size(); i++) {
    if ( !force && !alwaysAcceptEdit(i) ) continue;
    emit lv->itemRenamed(this, i);
    if (_editWidgets[i] != NULL) editDone(i, _editWidgets[i]);
  }
  removeEditBox();
}

int EditListViewItem::width(const QFontMetrics &fm, const Q3ListView *lv, int col) const
{
  int w = KListViewItem::width(fm, lv, col);
  QWidget* widget = _renaming ? _editWidgets[col] : editWidgetFactory(col);
  if (widget == NULL) return w;
  w = QMAX(w, widget->sizeHint().width());
  return w;
}
