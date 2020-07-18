/***************************************************************************
 *   Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "boost_ui.h"
#include "boost_ui.moc"

//----------------------------------------------------------------------------
Boost::ConfigWidget::ConfigWidget(Project& project)
  : ToolConfigWidget(project)
{}

void Boost::ConfigWidget::initEntries()
{
  createIncludeDirectoriesEntry();
}
