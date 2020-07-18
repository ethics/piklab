/***************************************************************************
 *   Copyright (C) 2005-2009 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "prog_config_center.h"
#include "prog_config_center.moc"

#include <memory>

#include "progs/list/prog_list.h"
#include "prog_config_widget.h"
#include "prog_group_ui.h"
#include "libgui/global_config.h"
#include "port_selector.h"
#include "libgui/main_global.h"

//----------------------------------------------------------------------------
QPixmap Programmer::OptionsConfigWidget::pixmap() const
{
  return IconLoader::loadIcon("audio-card", IconLoader::Toolbar);
}

//----------------------------------------------------------------------------
Programmer::ConfigCenter::ConfigCenter(const Programmer::Group &sgroup, QWidget *parent)
: TreeListDialog(parent, "configure_programmers_dialog", true,
                 i18n("Configure Programmers"), Ok|User1|Apply|Cancel, Cancel, false)
{
  setButtonGuiItem(User1, KGuiItem(i18n("Update"), "view-refresh"));

  KPageWidgetItem* current = NULL;
  for (Lister::ConstIterator it = Lister::instance().begin();
       it != Lister::instance().end();
       ++it) {
     const Programmer::Group& group = *it.data();
     QStringList names;
     names += group.label();
     ProgConfigWidget *cw = new ProgConfigWidget(group, NULL);
     _configWidgets.push_back(cw);
     cw->loadConfig();
     KPageWidgetItem* page = addPage(cw, group.label());
     if ( it.key()==sgroup.name() ) current = page;
  }
  setCurrentPage(current);
  currentPageChangedSlot(current);

  connect(this, SIGNAL(currentPageChanged(KPageWidgetItem*, KPageWidgetItem*)), SLOT(currentPageChangedSlot(KPageWidgetItem*)));
  connect(this, SIGNAL(okClicked()), SLOT(slotOk()));
  connect(this, SIGNAL(applyClicked()), SLOT(slotApply()));
  connect(this, SIGNAL(user1Clicked()), SLOT(slotUser1()));
}

void Programmer::ConfigCenter::currentPageChangedSlot(KPageWidgetItem* page)
{
  ProgConfigWidget* cw = static_cast<ProgConfigWidget*>(page->widget());
  if (cw != NULL) {
    QTimer::singleShot(0, cw, SLOT(detect()));
  }
}

void Programmer::ConfigCenter::slotApply()
{
  for (ConfigWidgetVector::const_iterator it = _configWidgets.begin(); it != _configWidgets.end(); ++it) (*it)->saveConfig();
}

void Programmer::ConfigCenter::slotOk()
{
  slotApply();
  accept();
}

void Programmer::ConfigCenter::slotUser1()
{
  ProgConfigWidget* cw = static_cast<ProgConfigWidget*>(currentPage()->widget());
  if (cw != NULL) {
    cw->detect();
  }
}

//----------------------------------------------------------------------------
Programmer::ProgConfigWidget::ProgConfigWidget(const Programmer::Group& group,
                                               QWidget* parent)
: ::ConfigWidget(parent),
  _group(group)
{
  uint row = 0;

// tab widget
  QTabWidget* tabWidget = new QTabWidget(this);
  addWidget(tabWidget, row,row, 0,2);
  row++;

  // port selector
  if (!group.isSoftware()) {
    Container::Grid* container = new Container::Grid;
    container->setMargin(10);
    _portSelector = new PortSelector(container);
    connect(_portSelector, SIGNAL(changed()), SLOT(detect()));
    container->addWidget(_portSelector, 0,0, 0,0);
    QPixmap icon = IconLoader::loadIcon("network-wired-activated", IconLoader::Small);
    tabWidget->insertTab(container, icon, i18n("Port Selection"), 0);
  } else {
    _portSelector = NULL;
  }

  // specific programmer config
  _specific = static_cast<const ::Programmer::GroupUI *>(group.gui())->createConfigWidget(tabWidget);
  QPixmap icon = IconLoader::loadIcon("configure", IconLoader::Small);
  tabWidget->addTab(_specific, icon, i18n("Specific"));
}

void Programmer::ProgConfigWidget::detect()
{
  BusyCursorStarter bc; // can take a few seconds to connect programmer...
  if (_portSelector == NULL) {
    _specific->updateStatus();
    return;
  }
  HardwareDescription hd;
  hd.port = _portSelector->portDescription();
  std::auto_ptr< ::Hardware::Config> config(Main::programmerGroup().hardwareConfig());
  if (config.get() != NULL) hd.name = config->currentHardware(hd.port.type);
  _specific->setPort(hd);
  //_portSelector->setStatus(hd.port.type, ok ? i18n("Connection: Ok") : i18n("Connection: Error"));
}

void Programmer::ProgConfigWidget::saveConfig()
{
  if (_portSelector != NULL) _portSelector->saveConfig();
  _specific->saveConfig();
}

void Programmer::ProgConfigWidget::loadConfig()
{
  if (_portSelector != NULL) _portSelector->setGroup(_group);
  _specific->loadConfig();
}
