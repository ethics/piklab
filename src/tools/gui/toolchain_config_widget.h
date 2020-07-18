/***************************************************************************
 *   Copyright (C) 2005-2006 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef TOOLCHAIN_CONFIG_WIDGET_H
#define TOOLCHAIN_CONFIG_WIDGET_H

#include <memory>

#include "common/gui/container.h"
#include "common/gui/key_gui.h"
#include "common/global/process.h"
#include "common/gui/config_widget.h"
#include "tools/list/compile_process.h"
namespace PURL { class DirectoryWidget; }
namespace Tool { class Group; }

//----------------------------------------------------------------------------
class ToolchainConfigWidget : public ::ConfigWidget
{
  Q_OBJECT
public:
  ToolchainConfigWidget(Tool::Group &group, QWidget *parent);
  Tool::Group &group() const { return _group; }
  void init();

public slots:
  void detect();
  void forceDetect();
  virtual void loadConfig();
  virtual void saveConfig();

protected:
  Tool::ExecutableType executableType() const;
  Tool::OutputExecutableType outputType() const;
  virtual void addCustomExecutableOptions(Container::Grid *) {}
  virtual QString baseExecutable(Tool::Category category) const;
  virtual ::Process::LineOutput *checkExecutableProcess(Tool::Category category) const;
  virtual ::Process::LineOutput *checkDevicesProcess(uint i) const;

protected slots:
  void showDetails();
  void showDeviceDetails();
  void checkExecutableDone();
  void checkDevicesDone();

protected:
  Tool::Group&     _group;
  std::auto_ptr<Compile::Config> _config;
  bool             _dirty;
  std::auto_ptr<KeyComboBox<Tool::ExecutableType> >       _execType;
  std::auto_ptr<KeyComboBox<Tool::OutputExecutableType> > _outputType;
  QLabel          *_devicesLabel;
  QMap<Compile::DirectoryType, PURL::DirectoryWidget *> _dirs;
  class ExecData {
  public:
    ExecData() : label(NULL), button(NULL), process(NULL) {}
    ~ExecData() { delete process; process = NULL; }
    QLabel                *label;
    QString                command;
    QStringList            checkLines;
    KPushButton           *button;
    ::Process::LineOutput *process;
  };
  QMap<Tool::Category, ExecData> _data;
  class DevicesData {
  public:
    DevicesData() : process(NULL) {}
    bool        done;
    QString     command;
    QStringList checkLines;
    ::Process::LineOutput *process;
  };
  QValueVector<DevicesData> _devicesData;
};

#endif
