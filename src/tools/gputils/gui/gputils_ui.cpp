/***************************************************************************
 *   Copyright (C) 2005-2006 Nicolas Hadacek <hadacek@kde.org>             *
 *   Copyright (C) 2003-2004 Alain Gibaud <alain.gibaud@free.fr>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "gputils_ui.h"
#include "gputils_ui.moc"

#include <qlabel.h>
#include "tools/gputils/gputils_config.h"

//----------------------------------------------------------------------------
GPUtils::ConfigWidget::ConfigWidget(Project& project)
  : ToolConfigWidget(project), _gpasmWarning(0)
{}

void GPUtils::ConfigWidget::initEntries()
{
  if ( _category==Tool::Category::Assembler ) {
    uint row = container()->numRows();
    QLabel *label = new QLabel(i18n("Warning level:"), container());
    container()->addWidget(label, row,row, 0,0);
    _gpasmWarning = new QComboBox(container());
    connect(_gpasmWarning, SIGNAL(activated(int)), SIGNAL(changed()));
    for (uint i=0; i<GPUtils::Config::Nb_WarningLevels; i++)
      _gpasmWarning->insertItem(i18n(GPUtils::Config::WARNING_LEVEL_LABELS[i]));
    _gpasmWarning->insertItem(i18n("as in LIST directive"));
    container()->addWidget(_gpasmWarning, row,row, 1,1);
    createIncludeDirectoriesEntry();
  }
  if ( _category==Tool::Category::Linker ) {
    createHexFormatEntry();
    createIncludeDirectoriesEntry();
  }
}

void GPUtils::ConfigWidget::internalLoadConfig(const Compile::Config &config)
{
  ToolConfigWidget::internalLoadConfig(config);
  if ( _category==Tool::Category::Assembler )
    _gpasmWarning->setCurrentItem(static_cast<const Config &>(config).gpasmWarningLevel());
}

void GPUtils::ConfigWidget::internalSaveConfig(Compile::Config &config) const
{
  ToolConfigWidget::internalSaveConfig(config);
  if ( _category==Tool::Category::Assembler )
    static_cast<Config &>(config).setGPAsmWarningLevel(_gpasmWarning->currentItem());
}
