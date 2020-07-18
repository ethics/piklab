/***************************************************************************
 *   Copyright (C) 2005-2007 Nicolas Hadacek <hadacek@kde.org>             *
 *   Copyright (C) 2004 Alain Gibaud <alain.gibaud@free.fr>                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "config_center.h"
#include "config_center.moc"

#include "global_config.h"
#include "device_gui.h"
#include "tools/list/tools_config_widget.h"
#include "progs/gui/prog_config_center.h"
#include "progs/gui/debug_config_center.h"
#include "tools/list/compile_config.h"

//----------------------------------------------------------------------------
GlobalConfigWidget::GlobalConfigWidget()
{
  uint row = numRows();
  _showDebug.reset(new KeyComboBox<Log::DebugLevel>(this));
  FOR_EACH(Log::DebugLevel, level) _showDebug->appendItem(level, level.label());
  addWidget(_showDebug->widget(), row,row, 0,0);
  row++;
}

void GlobalConfigWidget::loadConfig()
{
  BaseGlobalConfigWidget::loadConfig();
  _showDebug->setCurrentItem(GlobalConfig::debugLevel());
}

void GlobalConfigWidget::saveConfig()
{
  BaseGlobalConfigWidget::saveConfig();
  GlobalConfig::writeDebugLevel(_showDebug->currentItem());
}

QPixmap GlobalConfigWidget::pixmap() const
{
  return IconLoader::loadIcon("configure", IconLoader::Toolbar);
}

//----------------------------------------------------------------------------
ConfigWidget *ConfigCenter::factory(Type type)
{
  switch (type) {
    case General:      return new GlobalConfigWidget;
    case ProgOptions:  return new Programmer::OptionsConfigWidget;
    case DebugOptions: return new Debugger::OptionsConfigWidget;
    case Nb_Types:     break;
  }
  ASSERT(false);
  return 0;
}

ConfigCenter::ConfigCenter(Type showType, QWidget *parent)
  : PageDialog(parent, "configure_piklab_dialog", true, i18n("Configure Piklab"), Ok|Cancel, Cancel, false)
{
  setFaceType(List);
  for (uint i=0; i<Nb_Types; i++) {
    _configWidgets[i] = factory(Type(i));
    _configWidgets[i]->loadConfig();
    _pages[i] = addPage(_configWidgets[i], _configWidgets[i]->title());
    _pages[i]->setHeader(_configWidgets[i]->header());
    _pages[i]->setIcon(KIcon(_configWidgets[i]->pixmap()));
  }
  setCurrentPage(_pages[showType]);

  connect(this, SIGNAL(okClicked()), SLOT(slotOk()));
  connect(this, SIGNAL(applyClicked()), SLOT(slotApply()));
}

void ConfigCenter::slotApply()
{
  for (uint i=0; i<Nb_Types; i++) _configWidgets[i]->saveConfig();
}

void ConfigCenter::slotOk()
{
  slotApply();
  accept();
}
