/***************************************************************************
 *   Copyright (C) 2012 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef GUI_CHECK_H
#define GUI_CHECK_H

#include <QtTest/QtTest>

//----------------------------------------------------------------------------
class GuiCheck : public QObject
{
  Q_OBJECT

  public:
    GuiCheck();

  private:
    uint _index;
    uint _subIndex;

    virtual bool init(uint index) = 0;

  private slots:
    void invokeTest();
};

//----------------------------------------------------------------------------
class GuiCheckProxy : public QObject
{
  Q_OBJECT
  public:
    void run(GuiCheck& check);
};

#endif
