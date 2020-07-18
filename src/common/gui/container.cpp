/***************************************************************************
 *   Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "container.h"
#include "container.moc"

#include "misc_gui.h"

//----------------------------------------------------------------------------
const uint Container::SPACING = 10;

//----------------------------------------------------------------------------
void Container::Base::setFrame(Type type)
{
  _type = type;
  switch (type) {
  case Container::Flat:
    setMargin(parent() && parent()->inherits("QTabWidget") ? 10 : 0);
    setFrameStyle(QFrame::NoFrame);
    break;
  case Container::Sunken:
    setMargin(10);
    setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    break;
  case Container::Raised:
    setMargin(5);
    setFrameStyle(QFrame::Panel | QFrame::Raised);
    break;
  }
}

//----------------------------------------------------------------------------
Container::Grid::Grid(QWidget *parent, Type type)
  : Base(parent, type)
{
  initLayout();
}

Container::Grid::Grid(Q3WidgetStack *stack, uint index, Type type)
  : Base(stack, type)
{
  initLayout();
  stack->addWidget(this, index);
}

Container::Grid::Grid(KTabWidget *tabw, const QString &title, Type type)
  : Base(tabw, type)
{
  initLayout();
  tabw->addTab(this, title);
}

void Container::Grid::initLayout()
{
  _topLayout = new QGridLayout(this, 1, 1, 0, SPACING);
  _gridLayout = new QGridLayout(0, 0, SPACING);
  _topLayout->addLayout(_gridLayout, 0, 0);
  _topLayout->setRowStretch(1, 1);
  setFrame(_type);
}

void Container::Grid::addWidget(QWidget *w, uint startRow, uint endRow, uint startCol, uint endCol, int alignment)
{
  ASSERT( startRow<=endRow );
  ASSERT( startCol<=endCol );
  w->show();
  _gridLayout->addMultiCellWidget(w, startRow, endRow, startCol, endCol, Qt::Alignment(alignment));
}

void Container::Grid::addLayout(QLayout *l, uint startRow, uint endRow, uint startCol, uint endCol, int alignment)
{
  ASSERT( startRow<=endRow );
  ASSERT( startCol<=endCol );
  _gridLayout->addMultiCellLayout(l, startRow, endRow, startCol, endCol, Qt::Alignment(alignment));
}

//----------------------------------------------------------------------------
Container::Button::Button(const QString &title, QWidget *parent)
  : Grid(parent, Container::Sunken)
{
  _button = new PopupButton(title, this);
  addWidget(static_cast<QWidget*>(_button), 0,0, 0,1);
  setColStretch(2, 1);
}

//----------------------------------------------------------------------------
Container::Shrinkable::Shrinkable(QWidget *parent, Type type)
  : Base(parent, type), _margin(0)
{
  setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
}

void Container::Shrinkable::computeMinimumSize()
{
  int minWidth = 0, rowHeight = 0;
  QValueList<QWidget *>::const_iterator it;
  for (it=_widgets.begin(); it!=_widgets.end(); ++it) {
    minWidth = QMAX(minWidth, (*it)->width());
    rowHeight = QMAX(rowHeight, (*it)->sizeHint().height());
  }
  setMinimumWidth(_margin + minWidth + _margin);
  for (it=_widgets.begin(); it!=_widgets.end(); ++it) (*it)->setFixedHeight(rowHeight);
}

void Container::Shrinkable::appendWidget(QWidget *widget)
{
  connect(widget, SIGNAL(destroyed(QObject *)), SLOT(childDestroyed(QObject *)));
  widget->setFixedWidth(widget->sizeHint().width());
  _widgets += widget;
  computeMinimumSize();
  doLayout();
}

void Container::Shrinkable::doLayout()
{
  int rw = width() - 2*_margin;
  int usedWidth = 0, y = _margin, rowHeight = 0;
  bool first = true;
  QValueList<QWidget *>::const_iterator it;
  for (it=_widgets.begin(); it!=_widgets.end(); ++it) {
    rowHeight = (*it)->height();
    if (!first) usedWidth += SPACING;
    int x = _margin + usedWidth;
    usedWidth += (*it)->width();
    if ( usedWidth>rw ) {
      ASSERT( !first ) ;
      first = true;
      usedWidth = (*it)->width();
      x = _margin;
      y += SPACING + rowHeight;
    }
    (*it)->move(x, y);
    first = false;
  }
  setMinimumHeight(y + rowHeight + _margin + 1);
}

void Container::Shrinkable::childDestroyed(QObject *o)
{
  _widgets.remove(static_cast<QWidget *>(o));
  computeMinimumSize();
  doLayout();
}
