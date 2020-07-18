/***************************************************************************
 *   Copyright (C) 2005-2007 Nicolas Hadacek <hadacek@kde.org>             *
 *   Copyright (C) 2003-2004 Alain Gibaud <alain.gibaud@free.fr>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "hardware_config_widget.h"
#include "hardware_config_widget.moc"

#include <qlabel.h>

#include "progs/base/prog_config.h"
#include "devices/base/device_group.h"
#include "common/gui/misc_gui.h"

//-----------------------------------------------------------------------------
Hardware::HConfigWidget::HConfigWidget(::Programmer::Base &base, QWidget *parent, bool edit)
  : QFrame(parent, "hardware_config_widget"), _edit(edit), _connected(false), _base(base)
{
  _hardware = 0;

  QHBoxLayout *top = new QHBoxLayout(this, 0, 10);
  _mainVBox = new QVBoxLayout(top);

  if (edit) {
    _editVBox = new QVBoxLayout(top);
    top->setStretchFactor(_editVBox, 1);
  } else _editVBox = 0;
}

//-----------------------------------------------------------------------------
Hardware::EditDialog::EditDialog(ConfigWidget *cwidget, const QString &name, const Port::Description &pd, Data *data)
  : Dialog(cwidget, "hardware_edit_dialog", true, i18n("Edit and test hardware"), Ok|Cancel, Cancel, true),
    _cwidget(cwidget), _savedName(name), _oldData(data)
{
  setButtonText(Ok, i18n("Save"));
  setButtonText(Cancel, i18n("Close"));

  QGridLayout *grid = new QGridLayout(mainWidget(), 1, 1, 0, 10);
  grid->setColStretch(2, 1);

  QLabel *label = new QLabel(i18n("Hardware name:"), mainWidget());
  grid->addWidget(label, 0, 0);
  _name = new QLineEdit(name, mainWidget());
  grid->addWidget(_name, 0, 1);

  label = new QLabel(i18n("%1 at %2:").arg(pd.type.label()).arg(pd.device), mainWidget());
  grid->addWidget(label, 1, 0);
  label = new QLabel(mainWidget());
  grid->addWidget(label, 1, 1);

  _hc = cwidget->createHardwareConfigWidget(mainWidget(), true);
  grid->addMultiCellWidget(_hc, 2,2, 0,2);

  grid->setRowStretch(3, 1);

  bool ok = _hc->set(pd, *data);
  label->setText(ok ? i18n("Connected") : i18n("Not Connected"));

  connect(this, SIGNAL(okClicked()), SLOT(slotOk()));
  connect(this, SIGNAL(cancelClicked()), SLOT(slotCancel()));
}

void Hardware::EditDialog::slotOk()
{
  if ( _name->text().isEmpty() ) {
    MessageBox::sorry(i18n("Could not save configuration: hardware name is empty."), Log::Show);
    return;
  }
  if ( _cwidget->_config->isStandardHardware(_name->text()) ) {
    MessageBox::sorry(i18n("The hardware name is already used for a standard hardware."), Log::Show);
    return;
  }
  QStringList names = _cwidget->_config->hardwareNames(PortType::Nb_Types); // all hardwares
  if ( names.contains(_name->text()) ) {
    if ( !MessageBox::askContinue(i18n("Do you want to overwrite this custom hardware \"%1\"?").arg(_name->text()),
                                      KStandardGuiItem::save()) ) return;
  }
  delete _oldData;
  _oldData = _hc->data();
  _cwidget->_config->writeCustomHardware(_name->text(), *_oldData);
  _savedName = _name->text();
  Dialog::accept();
}

void Hardware::EditDialog::slotCancel()
{
  std::auto_ptr<Data> data(_hc->data());
  bool equal = _oldData->isEqual(*data);
  if ( !equal && !MessageBox::askContinue(i18n("Closing will discard changes you have made. Close anyway?"), KStandardGuiItem::close()) )
    return;
  Dialog::reject();
}

//-----------------------------------------------------------------------------
Hardware::ConfigWidget::ConfigWidget(::Programmer::Base *base, Config *config, QWidget *parent)
: ::Programmer::ConfigWidget(base->group(), parent),
  _base(base),
  _config(config)
{
// programmer combo
  uint row = numRows();
  _configCombo = new KComboBox(this);
  connect(_configCombo, SIGNAL(activated(int)), SLOT(configChanged(int)));
  addWidget(_configCombo, row,row, 0,0);
  row++;

// hardware config
  QHBoxLayout *hbox = new QHBoxLayout(10);
  _hbox = new QHBoxLayout(10);
  hbox->addLayout(_hbox);
  addLayout(hbox, row,row, 0,1);
  row++;

// comment
  _comment = new KTextBrowser(this);
  addWidget(_comment, row,row, 0,1);
  row++;

// buttons
  QVBoxLayout *vbox  = new QVBoxLayout(hbox);
  _editButton = new KPushButton(this);
  connect(_editButton, SIGNAL(clicked()), SLOT(editClicked()));
  vbox->addWidget(_editButton);
  _deleteButton = new KPushButton(i18n("Delete"), this);
  connect(_deleteButton, SIGNAL(clicked()), SLOT(deleteClicked()));
  vbox->addWidget(_deleteButton);
  vbox->addStretch(1);
}

void Hardware::ConfigWidget::saveConfig()
{
  ::Programmer::ConfigWidget::saveConfig();
  if (_hc.get() != NULL) _config->writeCurrentHardware(_hc->portDescription().type, _names[_configCombo->currentItem()]);
}

void Hardware::ConfigWidget::configChanged(int i)
{
  if (_hc.get() != NULL)  set(_hc->portDescription(), i);
}

bool Hardware::ConfigWidget::set(const Port::Description &pd, uint i)
{
  std::auto_ptr<Data> hd(_config->hardwareData(_names[i]));
  if (_hc.get() == NULL) {
    _hc.reset(createHardwareConfigWidget(this, false));
    _hc->show();
    _hbox->addWidget(_hc.get());
  }
  bool ok = _hc->set(pd, *hd);
  QString s = _config->comment(_names[i]);
  if ( s.isEmpty() ) _comment->hide();
  else {
    _comment->setText(s);
    _comment->show();
  }
  bool custom = !_config->isStandardHardware(_names[i]);
  _editButton->setText(custom ? i18n("Edit/Test...") : i18n("New/Test..."));
  _deleteButton->setEnabled(custom);
  return ok;
}

bool Hardware::ConfigWidget::setPort(const ::Programmer::HardwareDescription &hd)
{
  updateList(hd.port.type);
  int i = _names.findIndex(_config->currentHardware(hd.port.type));
  if ( i!=-1 ) _configCombo->setCurrentIndex(i);
  return set(hd.port, _configCombo->currentIndex());
}

void Hardware::ConfigWidget::updateList(PortType type)
{
  _configCombo->clear();
  _names = _config->hardwareNames(type);
  for (int i=0; i<_names.count(); i++) {
    bool standard = _config->isStandardHardware(_names[i]);
    QString s = (standard ? _config->label(_names[i]) : i18n("%1 <custom>").arg(_names[i]));
    _configCombo->insertItem(s);
  }
}

void Hardware::ConfigWidget::editClicked()
{
  QString name = _names[_configCombo->currentItem()];
  QString cname = (_config->isStandardHardware(name) ? QString::null : name);
  Port::Description pd = _hc->portDescription();
  EditDialog *hcd = new EditDialog(this, cname, pd, _hc->data());
  int res = hcd->exec();
  if ( res==QDialog::Rejected ) return;
  updateList(pd.type);
  int index = _names.findIndex(hcd->savedName());
  _configCombo->setCurrentIndex(index);
  configChanged(_configCombo->currentIndex());
}

void Hardware::ConfigWidget::deleteClicked()
{
  QString name = _names[_configCombo->currentItem()];
  if ( !MessageBox::askContinue(i18n("Do you want to delete custom hardware \"%1\"?").arg(name),
                                KStandardGuiItem::del()) ) return;
  _config->deleteCustomHardware(name);
  updateList(_hc->portDescription().type);
  configChanged(_configCombo->currentItem());
}
