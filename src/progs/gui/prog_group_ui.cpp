/***************************************************************************
 *   Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "prog_group_ui.h"
#include "prog_group_ui.moc"

#include "common/common/misc.h"
#include "common/global/purl.h"
#include "common/gui/purl_gui.h"
#include "progs/base/generic_prog.h"
#include "progs/base/prog_group.h"
#include "progs/base/prog_config.h"
#include "libgui/main_global.h"
#include "libgui/toplevel.h"
#include "devices/base/device_group.h"
#include "devices/pic/prog/pic_prog.h"

//----------------------------------------------------------------------------
Programmer::StandaloneMemoryCalibrationEditor::StandaloneMemoryCalibrationEditor(const Pic::Memory &memory, QWidget *parent)
  : Pic::MemoryCalibrationEditor(0, const_cast<Pic::Memory &>(memory), parent)
{}

void Programmer::StandaloneMemoryCalibrationEditor::init(bool first)
{
  Pic::MemoryCalibrationEditor::init(first);
  KAction *action = new KAction(KIcon("view-refresh"), i18n("Read"), this);
  connect(action, SIGNAL(triggered()), this, SIGNAL(updateCalibration()));
  addAction(action);
  action = new KAction(i18n("Regenerating..."), this);
  connect(action, SIGNAL(triggered()), this, SIGNAL(regenerate()));
  addAction(action);
}

//----------------------------------------------------------------------------
Programmer::ButtonContainer::ButtonContainer(const QString &title,
                                    QObject *receiver, const char *updateSlot, QWidget *parent)
  : Container::Button(title, parent)
{
  if (receiver) button().appendAction(i18n("Read"), "view-refresh", receiver, updateSlot);
}

//----------------------------------------------------------------------------
Programmer::AdvancedDialog::AdvancedDialog(Base &base, QWidget *parent, const char *name)
: PageDialog(parent, name, true, i18n("Advanced Dialog"), Close, Close, false),
  _base(base), _calEditor(0)
{
  setFaceType(List);

  // programmer
  _programmerContainer = new Container::Grid(this);
  KPageWidgetItem* item = addPage(_programmerContainer, _base.group().label());
  item->setHeader(i18n("Programmer"));
  item->setIcon(KIcon("arrow-down"));

  Properties properties = _base.group().properties();
  uint row = _programmerContainer->numRows();
  if ( properties & HasFirmware ) {
    _firmwareContainer = new ButtonContainer(i18n("Firmware"), this, SLOT(updateFirmware()), _programmerContainer);
    _programmerContainer->addWidget(_firmwareContainer, row,row, 0,1);
    if ( _base.group().properties() & CanUploadFirmware )
      _firmwareContainer->button().appendAction(i18n("Uploading..."), "arrow-down", this, SLOT(uploadFirmware()));
    QLabel *label = new QLabel(i18n("Version:"), _firmwareContainer);
    _firmwareContainer->addWidget(label, 1,1, 0,0);
    _firmwareLabel = new QLabel(_firmwareContainer);
    _firmwareContainer->addWidget(_firmwareLabel, 1,1, 1,1);
    row++;
  } else {
    _firmwareContainer = 0;
    _firmwareLabel = 0;
  }

  if ( _base.group().canReadVoltages() ) {
    _voltagesContainer = new ButtonContainer(i18n("Voltages"), this, SLOT(updateVoltages()), _programmerContainer);
    _programmerContainer->addWidget(_voltagesContainer, row,row, 0,1);
    row++;

    uint k = 1;
    FOR_EACH(Device::VoltageType, type) {
      if ( !base.group().canReadVoltage(type) ) _voltages[type.type()] = 0;
      else {
        QLabel *label = new QLabel(type.semicolonLabel(), _voltagesContainer);
        _voltagesContainer->addWidget(label, k,k, 0,0);
        _voltages[type.type()] = new QLabel(_voltagesContainer);
        _voltagesContainer->addWidget(_voltages[type.type()], k,k, 1,1);
        k++;
      }
    }
  } else _voltagesContainer = 0;

  if ( properties & HasSelfTest ) {
    _selfTestContainer = new ButtonContainer(i18n("Self-test"), this, SLOT(updateSelfTest()), _programmerContainer);
    _programmerContainer->addWidget(_selfTestContainer, row,row, 0,1);
    row++;
  } else _selfTestContainer = 0;

  // calibration
  _calibrationContainer = new Container::Grid(this);
  item = addPage(_calibrationContainer, i18n("Calibration"));
  item->setHeader(i18n("Calibration"));
  item->setIcon(KIcon("configure"));
  const Device::Data& data = Main::deviceData();
  QString s;
  if (data.group().name() != "pic" || !static_cast<const Pic::Data&>(data).isReadable(Pic::MemoryRangeType::Cal))
    s = i18n("This device has no calibration information.");
  else if (!_base.group().isSupported(data.name())) s = i18n("The selected device is not supported by this programmer.");
  else {
    const ::Programmer::PicBase& pbase = static_cast<const ::Programmer::PicBase&>(_base);
    _calEditor = new StandaloneMemoryCalibrationEditor(pbase.deviceMemory(), _calibrationContainer);
    connect(_calEditor, SIGNAL(updateCalibration()), SLOT(updateCalibration()));
    connect(_calEditor, SIGNAL(regenerate()), SLOT(regenerateCalibration()));
    _calEditor->init(true);
    _calEditor->setReadOnly(true);
    _calibrationContainer->addWidget(_calEditor, 0,0, 0,0);
  }
  if ( !s.isEmpty() ) {
    QLabel *label = new QLabel(s, _calibrationContainer);
    _calibrationContainer->addWidget(label, 0,0, 0,0);
  }
}

bool Programmer::AdvancedDialog::connectProgrammer()
{
  _base.disconnectHardware();
  if ( !_base.connectHardware() ) {
    MessageBox::sorry(i18n("Could not connect programmer."), Log::Show);
    return false;
  }
  return true;
}

bool Programmer::AdvancedDialog::ensureConnected()
{
  if ( (_base.state()==NotConnected || _base.state()==Running) && !connectProgrammer() ) return false;
  return _base.setTargetPowerOn(!readConfigEntry(Config::TargetSelfPowered).toBool());
}

void Programmer::AdvancedDialog::updateFirmware()
{
  BusyCursorStarter bc;
  if ( ensureConnected() ) _base.readFirmwareVersion();
  updateDisplay();
}

void Programmer::AdvancedDialog::uploadFirmware()
{
  PURL::Url url = PURL::getOpenUrl("kfiledialog:///open_firmware", PURL::filter(PURL::Hex), this ,i18n("Open Firmware"));
  if ( url.isEmpty() ) return;
  BusyCursorStarter bc;
  if ( !connectProgrammer() ) return;
  if ( _base.uploadFirmware(url) )
    MessageBox::information(i18n("Firmware uploaded successfully."), Log::Show);
  else MessageBox::sorry(i18n("Error uploading firmware."), Log::Show);
  updateFirmware();
}

void Programmer::AdvancedDialog::updateVoltages()
{
  BusyCursorStarter bc;
  if ( ensureConnected() ) _base.readVoltages();
  updateDisplay();
}

void Programmer::AdvancedDialog::updateSelfTest()
{
  BusyCursorStarter bc;
  if ( ensureConnected() ) _base.selfTest(false);
  updateDisplay();
}

void Programmer::AdvancedDialog::updateCalibration()
{
  BusyCursorStarter bc;
  if ( ensureConnected() ) static_cast< ::Programmer::PicBase &>(_base).readCalibration();
  updateDisplay();
}

void Programmer::AdvancedDialog::regenerateCalibration()
{
  MessageBox::sorry(i18n("Oscillator calibration regeneration is not available with the selected programmer."), Log::Show);
}

void Programmer::AdvancedDialog::updateDisplay()
{
  if (_firmwareLabel) _firmwareLabel->setText(_base.firmwareVersion().pretty());
  if (_calEditor) _calEditor->updateDisplay();
  FOR_EACH(Device::VoltageType, type)
    if (_voltages[type.type()] != NULL) _voltages[type.type()]->setText(_base.voltage(type).label());
}
