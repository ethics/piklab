/***************************************************************************
 *   Copyright (C) 2005-2006 Nicolas Hadacek <hadacek@kde.org>             *
 *   Copyright (C) 2004 Alain Gibaud <alain.gibaud@free.fr>                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef TOOL_CONFIG_WIDGET_H
#define TOOL_CONFIG_WIDGET_H

#include "common/gui/container.h"
#include "tools/base/generic_tool.h"
#include "tools/base/tool_group.h"
#include "common/gui/config_widget.h"
#include "common/gui/editlistbox.h"
namespace PURL { class DirectoriesWidget; }

class ToolConfigWidget : public ::ConfigWidget
{
Q_OBJECT
public:
  ToolConfigWidget(Project& project);
  void init(Tool::Category category, const Tool::Group &group);
  virtual ~ToolConfigWidget();

signals:
  void changed();
  void displayHelp(Tool::Category category);

public slots:
  virtual void loadConfig();
  virtual void saveConfig() { internalSaveConfig(*_config); }

private slots:
  void updateArguments();
  void resetCustomArguments();
  void displayHelpSlot();

protected:
  const Tool::Group *_group;
  Tool::Category _category;
  Project&       _project;
  QLineEdit     *_customOptions, *_customLibraries;
  PURL::DirectoriesWidget *_includeDirs;
  QComboBox     *_hexFormat;

  Container::Grid *container() { return _container; }
  PURL::DirectoriesWidget * createDirectoriesEntry(const QString &label);
  void createIncludeDirectoriesEntry() { _includeDirs = createDirectoriesEntry(i18n("Include directories:")); }
  void createCustomOptionsEntry();
  void createCustomLibrariesEntry();
  void createHexFormatEntry();
  virtual void initEntries() = 0;
  virtual void internalLoadConfig(const Compile::Config &config);
  virtual void internalSaveConfig(Compile::Config &config) const;

private:
  enum ArgumentsType { AutomaticArguments = 0, CustomArguments, Nb_ArgumentsTypes };
  static const char * const ARGUMENTS_TYPE_LABELS[Nb_ArgumentsTypes];
  Compile::Config  *_config, *_tmpConfig;
  Compile::Process *_process;
  Project          *_tmpProject;
  KComboBox        *_argumentsType;
  KLineEdit        *_arguments;
  EditListBox      *_argumentsEditor;
  Container::Grid  *_container;

  QStringList arguments(ArgumentsType type) const;
};

#endif
