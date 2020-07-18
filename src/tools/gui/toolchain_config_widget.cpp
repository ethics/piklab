/***************************************************************************
 *   Copyright (C) 2005-2006 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "toolchain_config_widget.h"
#include "toolchain_config_widget.moc"

#include "tools/list/compile_config.h"
#include "common/gui/purl_gui.h"
#include "common/global/process.h"
#include "common/gui/container.h"

//----------------------------------------------------------------------------
ToolchainConfigWidget::ToolchainConfigWidget(Tool::Group &group, QWidget *parent)
  : ::ConfigWidget(parent),
    _group(group), _dirty(false), _outputType(0), _devicesData(group.nbCheckDevices())
{
  _config.reset(group.createConfig(*Main::project()));
}

void ToolchainConfigWidget::init()
{
  Container::Grid *container = new Container::Grid(this, Container::Sunken);
  addWidget(container, 0,0, 0,3);
  container->setColStretch(3, 1);

  uint row = 0;
  QLabel *label = new QLabel(Compile::DirectoryType(Compile::DirectoryType::Executable).label() + ":", container);
  container->addWidget(label, row,row, 0,0);
  _dirs[Compile::DirectoryType::Executable] = new PURL::DirectoryWidget(container);
  connect(_dirs[Compile::DirectoryType::Executable], SIGNAL(changed()), SLOT(forceDetect()));
  container->addWidget(_dirs[Compile::DirectoryType::Executable], row,row, 1,3);
  row++;

  label = new QLabel(i18n("Executable Type:"), container);
  container->addWidget(label, row,row, 0,0);
  _execType.reset(new KeyComboBox<Tool::ExecutableType>(container));
  FOR_EACH(Tool::ExecutableType, type) _execType->appendItem(type, type.label());
  connect(_execType->widget(), SIGNAL(activated(int)), SLOT(forceDetect()));
  container->addWidget(_execType->widget(), row,row, 1,2);
  row++;

  uint nbOutputTypes = 0;
  FOR_EACH(Tool::OutputExecutableType, type)
    if ( _group.hasOutputExecutableType(type) ) nbOutputTypes++;
  if ( nbOutputTypes>1 ) {
    label = new QLabel(i18n("Output Executable Type:"), container);
    container->addWidget(label, row,row, 0,0);
    _outputType.reset(new KeyComboBox<Tool::OutputExecutableType>(container));
    FOR_EACH(Tool::OutputExecutableType, type)
      if ( _group.hasOutputExecutableType(type) ) _outputType->appendItem(type, type.label());
    connect(_outputType->widget(), SIGNAL(activated(int)), SLOT(forceDetect()));
    container->addWidget(_outputType->widget(), row,row, 1,1);
    row++;
  }

  addCustomExecutableOptions(container);

  FOR_EACH(Tool::Category, k) {
    const Tool::Base *base = _group.base(k);
    if ( base==0 ) continue;
    label = new QLabel(QString("%1:").arg(k.label()), container);
    container->addWidget(label, row,row, 0,0);
    _data[k].label = new QLabel(container);
    container->addWidget(_data[k].label, row,row, 1,1);
    _data[k].button = new KPushButton(KGuiItem(QString::null, "zoom-in"), container);
    connect(_data[k].button, SIGNAL(clicked()), SLOT(showDetails()));
    container->addWidget(_data[k].button, row,row, 2,2);
    row++;
  }

  label = new QLabel(i18n("Device detection:"), container);
  container->addWidget(label, row,row, 0,0);
  _devicesLabel = new QLabel(container);
  container->addWidget(_devicesLabel, row,row, 1,1);
  KPushButton *button = new KPushButton(KGuiItem(QString::null, "zoom-in"), container);
  connect(button, SIGNAL(clicked()), SLOT(showDeviceDetails()));
  container->addWidget(button, row,row, 2,2);
  row++;

  row = numRows();
  FOR_EACH(Compile::DirectoryType, type) {
    if ( type==Compile::DirectoryType::Executable ) continue;
    if ( !_group.hasDirectory(type) ) _dirs[type] = 0;
    else {
      label = new QLabel(type.label() + ":", this);
      addWidget(label, row,row, 0,0);
      _dirs[type] = new PURL::DirectoryWidget(this);
      addWidget(_dirs[type], row,row, 1,3);
      row++;
    }
  }

  if ( !_group.comment().isEmpty() ) {
    KTextEdit *w = new KTextEdit(_group.comment(), this);
    w->setReadOnly(true);
    w->setLineWrapMode(KTextEdit::WidgetWidth);
    addWidget(w, row,row, 0,3);
    row++;
  }

  setColStretch(3, 1);
}

void ToolchainConfigWidget::loadConfig()
{
  _execType->widget()->blockSignals(true);
  _execType->setCurrentItem(Compile::Config::executableType(_group));
  _execType->widget()->blockSignals(false);
  if (_outputType.get() != NULL) {
    _outputType->widget()->blockSignals(true);
    _outputType->setCurrentItem(Compile::Config::outputExecutableType(_group));
    _outputType->widget()->blockSignals(false);
  }
  FOR_EACH(Compile::DirectoryType, type) {
    if ( _dirs[type]==0 ) continue;
    _dirs[type]->blockSignals(true);
    _dirs[type]->setDirectory(Compile::Config::directory(_group, type));
    _dirs[type]->blockSignals(false);
  }
  _dirty = true;
}

void ToolchainConfigWidget::saveConfig()
{
  Compile::Config::setExecutableType(_group, executableType());
  if (_outputType.get() != NULL) Compile::Config::setOutputExecutableType(_group, outputType());
  FOR_EACH(Compile::DirectoryType, type)
    if ( _dirs[type] ) Compile::Config::setDirectory(_group, type, _dirs[type]->directory());
}

void ToolchainConfigWidget::forceDetect()
{
  _dirty = true;
  detect();
}

void ToolchainConfigWidget::checkExecutableDone()
{
  FOR_EACH(Tool::Category, i) {
    if ( _data[i].process!=sender() ) continue;
    if ( _data[i].process->state()==::Process::Timedout ) {
      _data[i].label->setText(i18n("Timeout"));
      return;
    }
    _data[i].checkLines = _data[i].process->allOutputs();
    const Tool::Base *base = _group.base(i);
    QString exec = base->baseExecutable(executableType(), outputType());
    if ( base->checkExecutableResult(executableType(), _data[i].checkLines) ) _data[i].label->setText(i18n("\"%1\" found").arg(exec));
    else _data[i].label->setText(i18n("\"%1\" not recognized").arg(exec));
    break;
  }
}

void ToolchainConfigWidget::checkDevicesDone()
{
  for(int i=0; i<_devicesData.count(); i++) {
    if ( _devicesData[i].process!=sender() ) continue;
    if ( _devicesData[i].process->state()==::Process::Timedout ) {
      _devicesLabel->setText(i18n("Timeout"));
      return;
    }
    _devicesData[i].checkLines = _devicesData[i].process->allOutputs();
    _devicesData[i].done = true;
    break;
  }
  QValueList<const Device::Data *> list;
  for(int i=0; i<_devicesData.count(); i++) {
    if ( !_devicesData[i].done ) return;
    list += _group.getSupportedDevices(_devicesData[i].checkLines.join("\n"));
  }
  _devicesLabel->setText(i18n("Detected (%1)").arg(list.count()));
}

Tool::ExecutableType ToolchainConfigWidget::executableType() const
{
  return _execType->currentItem();
}

Tool::OutputExecutableType ToolchainConfigWidget::outputType() const
{
  return (_outputType.get() == NULL ? Compile::Config::outputExecutableType(_group) : _outputType->currentItem());
}

QString ToolchainConfigWidget::baseExecutable(Tool::Category category) const
{
  return _group.base(category)->baseExecutable(executableType(), outputType());
}

::Process::LineOutput *ToolchainConfigWidget::checkExecutableProcess(Tool::Category category) const
{
  PURL::Directory execDir = _dirs[Compile::DirectoryType::Executable]->directory();
  return _group.base(category)->checkExecutableProcess(execDir, executableType(), outputType());
}

::Process::LineOutput *ToolchainConfigWidget::checkDevicesProcess(uint i) const
{
  PURL::Directory execDir = _dirs[Compile::DirectoryType::Executable]->directory();
  return _group.checkDevicesProcess(i, execDir, executableType());
}

void ToolchainConfigWidget::detect()
{
  if ( !_dirty ) return;
  _dirty = false;

  _group.getToolchainVersion();

  FOR_EACH(Tool::Category, k) {
    if ( _data[k].label==0 ) continue;
    if ( !_group.base(k)->checkExecutable() ) _data[k].label->setText(i18n("Unknown"));
    else {
      _data[k].checkLines.clear();
      if (_data[k].process != NULL) {
        _data[k].process->blockSignals(true);
        delete _data[k].process;
      }
      _data[k].process = checkExecutableProcess(k);
      _data[k].command = _data[k].process->prettyCommand();
      connect(_data[k].process, SIGNAL(done()), SLOT(checkExecutableDone()));
      connect(_data[k].process, SIGNAL(error()), SLOT(checkExecutableDone()));
      connect(_data[k].process, SIGNAL(timeout()), SLOT(checkExecutableDone()));
      QString exec = baseExecutable(k);
      _data[k].process->start(5000);
      _data[k].label->setText(i18n("Detecting \"%1\"...").arg(exec));
    }
  }

  if ( _group.checkDevicesCategory()==Tool::Category::Nb_Types ) {
    QStringList supported = _group.supportedDevices();
    _devicesLabel->setText(i18n("Hardcoded (%1)").arg(supported.count()));
  } else {
    for (int i=0; i<_devicesData.count(); i++) {
      delete _devicesData[i].process;
      _devicesData[i].process = checkDevicesProcess(i);
      _devicesData[i].command = _devicesData[i].process->prettyCommand();
      connect(_devicesData[i].process, SIGNAL(done()), SLOT(checkDevicesDone()));
      connect(_devicesData[i].process, SIGNAL(error()), SLOT(checkDevicesDone()));
      connect(_devicesData[i].process, SIGNAL(timeout()), SLOT(checkDevicesDone()));
      _devicesData[i].done = false;
      _devicesData[i].checkLines.clear();
      _devicesData[i].process->start(5000);
      _devicesLabel->setText(i18n("Detecting ..."));
    }
  }

  FOR_EACH(Compile::DirectoryType, type) {
    if ( _dirs[type]==0 || type==Compile::DirectoryType::Executable ) continue;
    PURL::Directory execDir = _dirs[Compile::DirectoryType::Executable]->directory();
    PURL::Directory dir = _group.autodetectDirectory(type, execDir, executableType());
    if ( !dir.isEmpty() ) _dirs[type]->setDirectory(dir);
  }
}

void ToolchainConfigWidget::showDetails()
{
  FOR_EACH(Tool::Category, k) {
    if ( sender()!=_data[k].button ) continue;
    QString s;
    const Tool::Base *base = _group.base(k);
    if ( base->checkExecutable() ) {
      s += i18n("<qt><b>Command for executable detection:</b><br>%1<br>").arg(_data[k].command);
      s += i18n("<b>Version string:</b><br>%1<br></qt>").arg(_data[k].checkLines.join("<br>"));
    } else s += i18n("This tool cannot be automatically detected.");
    MessageBox::text(s, Log::Show);
    break;
  }
}

void ToolchainConfigWidget::showDeviceDetails()
{
  QString s;
  if ( _group.checkDevicesCategory()==Tool::Category::Nb_Types ) {
    //s += "<qt>";
    QStringList supported = _group.supportedDevices();
    uint i = 0;
    QStringList::const_iterator it;
    for (it=supported.begin(); it!=supported.end(); ++it) {
      if ( i!=0 ) {
        if ( (i%10)==0 ) s += "<br>";
        s += " ";
      }
      s += *it;
      i++;
    }
    //s += "</qt>";
  } else {
    uint nb = _group.nbCheckDevices();
    for (uint i=0; i<nb; i++) {
      if ( nb==1 ) s += i18n("<b>Command for devices detection:</b><br>%1<br>").arg(_devicesData[i].command);
      else s += i18n("<b>Command #%1 for devices detection:</b><br>%2<br>").arg(i+1).arg(_devicesData[i].command);
      QString ss = _devicesData[i].checkLines.join("<br>");
      if ( nb==1 ) s += i18n("<b>Device string:</b><br>%1<br>").arg(ss);
      else s += i18n("<b>Device string #%1:</b><br>%2<br>").arg(i+1).arg(ss);
    }
  }
  MessageBox::text(s, Log::Show);
}
