/***************************************************************************
 *   Copyright (C) 2010 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef NOKDE_DOCKWINDOW_H
#define NOKDE_DOCKWINDOW_H

#include <QMainWindow>
#include <QDockWidget>

#include "nokde_misc_ui.h"

namespace Piklab
{

class KDockMainWindow;

//-----------------------------------------------------------------------------
class KDockWidget : public QDockWidget
{
Q_OBJECT
public:
  enum DockPosition { DockCenter, DockCorner, DockNone,
                      DockLeft, DockRight, DockBottom, DockTop };

  KDockWidget(const QString &title, QWidget* parent);
  QString tabPageLabel() const;
  void setDockSite(int pos) { _pos = pos; }
  int dockSite() const { return _pos; }
  void setEnableDocking(int pos);
  void changeHideShowState();
  bool mayBeHide();
  void manualDock(KDockWidget* target, int pos, int spliPos);

private:
  int _pos;
};

//-----------------------------------------------------------------------------
class KXMLGUIClient
{
public:
  KAction* action(const char* name) const;
  KActionCollection* actionCollection() { return &_actionCollection; }

private:
  KActionCollection _actionCollection;
};

//-----------------------------------------------------------------------------
class KXMLGUIFactory
{
public:
  QWidget* container(const QString& name, KXMLGUIClient* client);
  void addClient(KXMLGUIClient* client);
  void removeClient(KXMLGUIClient* client);
};

//-----------------------------------------------------------------------------
class KDockManager : public QObject
{
Q_OBJECT
public:
  KDockWidget* getDockWidgetFromName(const QString& name) const;

private:
  typedef std::map<QString, KDockWidget*>  NameMap;
  typedef std::map<QWidget*, KDockWidget*> WidgetMap;

  NameMap   _nameMap;
  WidgetMap _widgetMap;

friend class KDockMainWindow;
};

//-----------------------------------------------------------------------------
class KDockMainWindow : public QMainWindow, public KXMLGUIClient
{
Q_OBJECT
public:
  KDockMainWindow();
  virtual ~KDockMainWindow();
  static KDockMainWindow* self() { return _self; }

  KXMLGUIFactory* guiFactory() const;
  KDockWidget* createDockWidget(const QString& name, const QPixmap& pixmap,
                                QWidget* parent = NULL, const QString& caption = QString::null);
  void setView(QWidget* widget);
  void setMainDockWidget(KDockWidget* widget);
  void setupGUI();
  void makeWidgetDockVisible(QWidget* widget);
  KDockManager* manager() { return &_manager; }

private:
  static KDockMainWindow* _self;
  KDockManager _manager;
};

} // namespace Piklab

#endif
