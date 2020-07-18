/***************************************************************************
 *   Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef C18_UI_H
#define C18_UI_H

#include "tools/gui/tool_config_widget.h"
#include "tools/gui/tool_group_ui.h"

namespace C18
{
//----------------------------------------------------------------------------
class ConfigWidget : public ToolConfigWidget
{
Q_OBJECT
public:
  ConfigWidget(Project& project);

private:
  QComboBox *_warningLevel;
  virtual void initEntries();
  virtual void internalLoadConfig(const Compile::Config &config);
  virtual void internalSaveConfig(Compile::Config &config) const;
};

//----------------------------------------------------------------------------
class GroupUI : public Tool::GroupUI
{
public:
  virtual ToolConfigWidget *configWidgetFactory(Tool::Category, ::Project& project) const { return new ConfigWidget(project); }
};

} // namespace

#endif
