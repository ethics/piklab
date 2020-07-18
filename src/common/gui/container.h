/***************************************************************************
 *   Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef CONTAINER_H
#define CONTAINER_H

#include <qlayout.h>

#include "global_ui.h"

class PopupButton;

namespace Container
{
enum Type { Flat, Sunken, Raised };
extern const uint SPACING;

//----------------------------------------------------------------------------
class Base : public Q3Frame
{
Q_OBJECT
public:
  Base(QWidget *parent, Type type) : Q3Frame(parent), _type(type) {}
  void setFrame(Type type);

protected:
  Type _type;
};

//----------------------------------------------------------------------------
class Grid : public Base
{
Q_OBJECT
public:
  Grid(QWidget *parent = 0, Type type = Flat);
  Grid(Q3WidgetStack *stack, uint index, Type type = Flat);
  Grid(KTabWidget *tabw, const QString &title, Type type = Flat);
  void addWidget(QWidget *widget, uint startRow, uint endRow, uint startCol, uint endCol, int alignment = 0);
  void addLayout(QLayout *layout, uint startRow, uint endRow, uint startCol, uint endCol, int alignment = 0);
  uint numRows() const { return _gridLayout->numRows(); }
  uint numCols() const { return _gridLayout->numCols(); }
  virtual void setMargin(int margin) { _topLayout->setMargin(margin); }
  void setRowSpacing(uint row, uint spacing) { _gridLayout->setRowSpacing(row, spacing); }
  void setColSpacing(uint col, uint spacing) { _gridLayout->setColSpacing(col, spacing); }
  void setRowStretch(uint row, uint stretch) { _gridLayout->setRowStretch(row, stretch); }
  void setColStretch(uint col, uint stretch) { _gridLayout->setColStretch(col, stretch); }

private:
  QGridLayout *_topLayout, *_gridLayout;

  void initLayout();
};

//----------------------------------------------------------------------------
class Button : public Grid
{
Q_OBJECT
public:
  Button(const QString &title, QWidget *parent);
  PopupButton &button() { return *_button; }

private:
  PopupButton *_button;
};

//----------------------------------------------------------------------------
class Shrinkable : public Base
{
Q_OBJECT
public:
  Shrinkable(QWidget *parent = 0, Type type = Flat);
  virtual void setMargin(int margin) { _margin = margin; }
  void appendWidget(QWidget *widget);

protected:
  virtual void resizeEvent(QResizeEvent *) { doLayout(); }

protected slots:
  void childDestroyed(QObject *o);

private:
  int _margin;
  QValueList<QWidget *> _widgets;

  void computeMinimumSize();
  void doLayout();
};

} // namespace

#endif
