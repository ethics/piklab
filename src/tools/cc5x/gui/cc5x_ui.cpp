/***************************************************************************
 *   Copyright (C) 2007 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "cc5x_ui.h"
#include "cc5x_ui.moc"

//----------------------------------------------------------------------------
CC5X::ConfigWidget::ConfigWidget(Project& project)
  : ToolConfigWidget(project)
{}

void CC5X::ConfigWidget::initEntries()
{
  if ( _category!=Tool::Category::Linker ) createIncludeDirectoriesEntry();
}
