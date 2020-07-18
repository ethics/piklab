/***************************************************************************
 *   Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "pic30_ui.h"
#include "pic30_ui.moc"

#include <qlabel.h>

#include "common/gui/purl_gui.h"
#include "tools/pic30/pic30_config.h"

//----------------------------------------------------------------------------
PIC30::ConfigWidget::ConfigWidget(Project& project)
  : ToolConfigWidget(project)
{}

void PIC30::ConfigWidget::initEntries()
{
  if ( _category==Tool::Category::Compiler || _category==Tool::Category::Assembler || _category==Tool::Category::Linker )
    createIncludeDirectoriesEntry();
  if ( _category==Tool::Category::Linker ) createCustomLibrariesEntry();
}
