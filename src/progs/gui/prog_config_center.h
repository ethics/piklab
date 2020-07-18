/***************************************************************************
 *   Copyright (C) 2005 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef PROG_CONFIG_CENTER_H
#define PROG_CONFIG_CENTER_H

#include <map>
#include <memory>

#include "common/gui/dialog.h"
#include "common/gui/config_widget.h"
#include "common/gui/key_gui.h"
#include "common/gui/container.h"
#include "progs/base/prog_config.h"
#include "progs/list/prog_list.h"
class PortSelector;

namespace Programmer
{
class ProgConfigWidget;

BEGIN_DECLARE_CONFIG_WIDGET(Config, OptionsConfigWidget)
  virtual QString header() const { return i18n("Programmer Options"); }
  virtual QString title() const { return i18n("Programmer Options"); }
  virtual QPixmap pixmap() const;
END_DECLARE_CONFIG_WIDGET

//-----------------------------------------------------------------------------
class ConfigCenter : public TreeListDialog
{
Q_OBJECT
public:
  ConfigCenter(const Programmer::Group &group, QWidget *parent);

private slots:
  virtual void slotOk();
  virtual void slotApply();
  virtual void slotUser1();
  void currentPageChangedSlot(KPageWidgetItem*);

private:
  typedef std::vector<ProgConfigWidget*> ConfigWidgetVector;
  ConfigWidgetVector _configWidgets;
};

//----------------------------------------------------------------------------
class ProgConfigWidget : public ::ConfigWidget
{
Q_OBJECT
public:
  ProgConfigWidget(const Programmer::Group& group, QWidget* parent);
  virtual void loadConfig();

public slots:
  virtual void saveConfig();
  void detect();

private:
  const Programmer::Group&    _group;
  PortSelector*               _portSelector;
  ::Programmer::ConfigWidget* _specific;
};

} // namespace

#endif
