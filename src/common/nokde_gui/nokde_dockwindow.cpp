/***************************************************************************
 *   Copyright (C) 2010 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "nokde_dockwindow.h"
#include "nokde_dockwindow.moc"

//-----------------------------------------------------------------------------
KDockWidget::KDockWidget(const QString &title, QWidget* parent)
: QDockWidget(title, parent),
  _pos(DockNone)
{
}

QString KDockWidget::tabPageLabel() const
{
  return QString::null;
}

void KDockWidget::setEnableDocking(int pos)
{
}

void KDockWidget::changeHideShowState()
{
}

bool KDockWidget::mayBeHide()
{
  return false;
}

void KDockWidget::manualDock(KDockWidget* target, int pos, int spliPos)
{
  if (target != NULL
      && target != KDockMainWindow::self()->centralWidget()) {
    setDockSite(target->dockSite());
    KDockMainWindow::self()->tabifyDockWidget(target, this);
  } else {
    setDockSite(pos);
    Qt::DockWidgetArea area = Qt::LeftDockWidgetArea;
    switch (DockPosition(pos)) {
      case DockLeft:   area = Qt::LeftDockWidgetArea;   break;
      case DockRight:  area = Qt::RightDockWidgetArea;  break;
      case DockTop:    area = Qt::TopDockWidgetArea;    break;
      case DockBottom: area = Qt::BottomDockWidgetArea; break;
      case DockCorner: break;
      case DockNone:   break;
    }
    KDockMainWindow::self()->addDockWidget(area, this);
  }
}

//-----------------------------------------------------------------------------
KAction* KXMLGUIClient::action(const char* name) const
{
  return _actionCollection.action(name);
}

//-----------------------------------------------------------------------------
QWidget* KXMLGUIFactory::container(const QString& name, KXMLGUIClient* client)
{
  return NULL;
}

void KXMLGUIFactory::addClient(KXMLGUIClient* client)
{
}

void KXMLGUIFactory::removeClient(KXMLGUIClient* client)
{
}

//-----------------------------------------------------------------------------
KDockWidget* KDockManager::getDockWidgetFromName(const QString& name) const
{
  NameMap::const_iterator it = _nameMap.find(name);
  return it != _nameMap.end() ? it->second : NULL;
}

//-----------------------------------------------------------------------------
KDockMainWindow* KDockMainWindow::_self = NULL;

KDockMainWindow::KDockMainWindow()
{
  _self = this;
}

KDockMainWindow::~KDockMainWindow()
{
  _self = NULL;
}

KXMLGUIFactory* KDockMainWindow::guiFactory() const
{
  return NULL;
}

KDockWidget* KDockMainWindow::createDockWidget(const QString& name, const QPixmap& pixmap,
                                               QWidget* parent, const QString& caption)
{
  KDockWidget* w = new KDockWidget(caption, parent);
  w->setName(name);
  _manager._nameMap[name] = w;
  return w;
}

void KDockMainWindow::setView(QWidget* widget)
{
}

void KDockMainWindow::setMainDockWidget(KDockWidget* widget)
{
  widget->setFeatures(QDockWidget::NoDockWidgetFeatures);
  setCentralWidget(widget);
}

void KDockMainWindow::setupGUI()
{
}

void KDockMainWindow::makeWidgetDockVisible(QWidget* widget)
{
  KDockManager::WidgetMap::const_iterator it = _manager._widgetMap.find(widget);
  if (it != _manager._widgetMap.end()) {
    it->second->raise();
  }
}
