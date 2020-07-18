/***************************************************************************
 *   Copyright (C) 2005 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef PROG_CONFIG_WIDGET
#define PROG_CONFIG_WIDGET

#include "common/gui/config_widget.h"
#include "common/port/port.h"
#include "common/gui/dialog.h"

namespace Programmer
{
class Group;
class HardwareDescription;

class ConfigWidget: public ::ConfigWidget
{
Q_OBJECT
public:
  ConfigWidget(const Group &group, QWidget *parent);
  virtual void loadConfig();
  const Group &group() const { return _group; }
  virtual void saveConfig();
  virtual bool setPort(const HardwareDescription &hd);
  virtual void updateStatus() {}

signals:
  void updatePortStatus(bool ok);

protected:
  const Group &_group;

private:
  KUrlRequester *_firmwareDir;
};

} // namespace

#endif
