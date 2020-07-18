/***************************************************************************
 *   Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "sdcc_ui.h"
#include "sdcc_ui.moc"

#include "tools/gputils/gui/gputils_ui.h"

//----------------------------------------------------------------------------
SDCC::ConfigWidget::ConfigWidget(Project& project)
  : ToolConfigWidget(project)
{}

void SDCC::ConfigWidget::initEntries()
{
  if ( _category==Tool::Category::Linker ) createHexFormatEntry();
  createIncludeDirectoriesEntry();
}

//----------------------------------------------------------------------------
ToolConfigWidget *SDCC::GroupUI::configWidgetFactory(Tool::Category category, Project& project) const
{
  if ( category==Tool::Category::Librarian ) return new GPUtils::ConfigWidget(project);
  return new ConfigWidget(project);
}
