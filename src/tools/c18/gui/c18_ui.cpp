/***************************************************************************
 *   Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "c18_ui.h"
#include "c18_ui.moc"

#include <qlabel.h>
#include "tools/c18/c18_config.h"

//----------------------------------------------------------------------------
C18::ConfigWidget::ConfigWidget(Project& project)
: ToolConfigWidget(project),
  _warningLevel(0)
{}

void C18::ConfigWidget::initEntries()
{
  if ( _category==Tool::Category::Compiler ) {
    uint row = container()->numRows();
    QLabel *label = new QLabel(i18n("Warning level:"), container());
    container()->addWidget(label, row,row, 0,0);
    _warningLevel = new QComboBox(container());
    connect(_warningLevel, SIGNAL(activated(int)), SIGNAL(changed()));
    for (uint i=0; i<Config::Nb_WarningLevels; i++)
      _warningLevel->insertItem(i18n(Config::WARNING_LEVEL_LABELS[i]));
    container()->addWidget(_warningLevel, row,row, 1,1);
    createIncludeDirectoriesEntry();
  }
  if ( _category==Tool::Category::Linker ) {
    createCustomLibrariesEntry();
    createHexFormatEntry();
  }
}

void C18::ConfigWidget::internalLoadConfig(const Compile::Config &config)
{
  ToolConfigWidget::internalLoadConfig(config);
  if ( _category==Tool::Category::Compiler )
    _warningLevel->setCurrentItem(static_cast<const Config &>(config).warningLevel());
}

void C18::ConfigWidget::internalSaveConfig(Compile::Config &config) const
{
  ToolConfigWidget::internalSaveConfig(config);
  if ( _category==Tool::Category::Compiler )
    static_cast<Config &>(config).setWarningLevel(_warningLevel->currentItem());
}
