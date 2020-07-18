/***************************************************************************
 *   Copyright (C) 2006-2007 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "config_gen.h"
#include "config_gen.moc"

#include "device_gui.h"
#include "devices/base/generic_memory.h"
#include "devices/gui/hex_view.h"
#include "devices/gui/memory_editor.h"
#include "devices/base/device_group.h"
#include "devices/gui/device_group_ui.h"
#include "devices/list/device_list.h"
#include "text_editor.h"
#include "tools/list/tool_list.h"

//-----------------------------------------------------------------------------
GeneratorDialog::GeneratorDialog(const QString &title, QWidget *parent, const char *name)
  : Dialog(parent, name, true, title, Close|User1, Close, false, QSize(400, 300))
{
  QVBoxLayout *top = new QVBoxLayout(mainWidget(), 10, 10);

  QHBoxLayout *hbox = new QHBoxLayout(top);
  QLabel *label = new QLabel(i18n("Device:"), mainWidget());
  hbox->addWidget(label);
  _deviceChooser = new DeviceChooser::Button(mainWidget());
  connect(_deviceChooser, SIGNAL(changed()), SLOT(reset()));
  hbox->addWidget(_deviceChooser);
  hbox->addSpacing(20);

  label = new QLabel(i18n("Toolchain:"), mainWidget());
  hbox->addWidget(label);
  _configType.reset(new ListerComboBox<Tool::Lister>(mainWidget()));
  connect(_configType->widget(), SIGNAL(activated(int)), SLOT(typeChanged()));
  hbox->addWidget(_configType->widget());

  label = new QLabel(i18n("Tool Type:"), mainWidget());
  hbox->addWidget(label);
  _toolType.reset(new KeyComboBox<PURL::ToolType>(mainWidget()));
  FOR_EACH(PURL::ToolType, type) _toolType->appendItem(type, type.label());
  _toolType->fixMinimumWidth();
  connect(_toolType->widget(), SIGNAL(activated(int)), SLOT(compute()));
  hbox->addWidget(_toolType->widget());
  hbox->addStretch(1);

  _hbox = new QHBoxLayout(top);

  _text = new SimpleTextEditor(false, mainWidget());
  _text->setReadOnly(true);
  top->addWidget(_text);
  top->setStretchFactor(_text, 10);

  _warning = new QLabel(mainWidget());
  top->addWidget(_warning);

  setButtonText(User1, i18n("Copy to clipboard"));
}

void GeneratorDialog::set(const Device::Data *data, const Tool::Group &group, PURL::ToolType stype)
{
  QString device;
  if ( data==0 ) {
    QStringList devices = group.supportedDevices();
    if ( devices.isEmpty() ) return;
    device = devices.first();
  } else device = data->name();
  _deviceChooser->setDevice(device);
  _configType->setGroup(group);
  setToolType(stype);
  reset();
}

void GeneratorDialog::typeChanged()
{
  setToolType(PURL::ToolType::Nb_Types);
  compute();
}

void GeneratorDialog::setToolType(PURL::ToolType stype)
{
  const Tool::Group& group = _configType->group();
  _toolType->clear();
  FOR_EACH(PURL::ToolType, type)
    if ( group.implementationType(type)!=PURL::Nb_FileTypes ) _toolType->appendItem(type, type.label());
  _toolType->setCurrentItem(stype);
  _toolType->widget()->setEnabled( _toolType->size()>=2 );
}

PURL::ToolType GeneratorDialog::toolType() const
{
  return _toolType->currentItem();
}

void GeneratorDialog::compute()
{
  const Tool::Group& group = _configType->group();
  _warning->hide();
  if ( group.isSupported(_deviceChooser->device()) ) {
    const Tool::SourceGenerator *generator = group.sourceGenerator();
    if ( generator==0 ) {
      _text->setFileType(PURL::Nb_FileTypes);
      _text->setText(i18n("Generation is not supported yet for the selected toolchain or device."));
    } else {
      bool ok = true;
      PURL::FileType type = group.implementationType(toolType());
      SourceLine::List lines = generateLines(ok);
      if ( ok && lines.isEmpty() ) {
        _text->setFileType(PURL::Nb_FileTypes);
        _text->setText(i18n("This toolchain does not need explicit config bits."));
      } else {
        _text->setFileType(type);
        _text->setText(SourceLine::text(type.data().sourceFamily, lines, 4));
      }
      if ( !ok ) {
        _warning->show();
        _warning->setText(i18n("Generation is only partially supported for this device."));
      }
    }
  } else {
   _text->setFileType(PURL::Nb_FileTypes);
    if ( group.supportedDevices().isEmpty() ) _text->setText(i18n("Could not detect supported devices for selected toolchain. Please check installation."));
    else _text->setText(i18n("Device not supported by the selected toolchain."));
  }
}

void GeneratorDialog::slotUser1()
{
  _text->selectAll();
  _text->copy();
  _text->deselect();
}

//-----------------------------------------------------------------------------
ConfigGenerator::ConfigGenerator(QWidget *parent)
  : GeneratorDialog(i18n("Config Generator"), parent, "config_generator"), _memory(0), _configEditor(0)
{}

void ConfigGenerator::reset()
{
  const Device::Data& data = *Device::Lister::instance().data(_deviceChooser->device());
  _memory = data.group().createMemory(data);
  _configEditor.reset(Device::groupui(data).createConfigEditor(*_memory, mainWidget()));
  if (_configEditor.get() != NULL) {
    _configEditor->show();
    connect(_configEditor.get(), SIGNAL(modified()), SLOT(compute()));
    _configEditor->updateDisplay();
    _hbox->addWidget(_configEditor.get());
  }
  compute();
}

SourceLine::List ConfigGenerator::generateLines(bool &ok) const
{
  const Tool::Group& group = _configType->group();
  const Tool::SourceGenerator& generator = *group.sourceGenerator();
  return generator.configLines(toolType(), *_memory, ok);
}

//-----------------------------------------------------------------------------
TemplateGenerator::TemplateGenerator(QWidget *parent)
  : GeneratorDialog(i18n("Template Generator"), parent, "template_generator")
{}

SourceLine::List TemplateGenerator::generateLines(bool &ok) const
{
  const Tool::Group& group = _configType->group();
  const Tool::SourceGenerator& generator = *group.sourceGenerator();
  const Device::Data& data = *Device::Lister::instance().data(_deviceChooser->device());
  return generator.templateSourceFile(toolType(), data, ok);
}
