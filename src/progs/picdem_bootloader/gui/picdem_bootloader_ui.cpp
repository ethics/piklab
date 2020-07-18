/***************************************************************************
 *   Copyright (C) 2007 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "picdem_bootloader_ui.h"
#include "picdem_bootloader_ui.moc"

#include "progs/picdem_bootloader/base/picdem_bootloader.h"

//-----------------------------------------------------------------------------
PicdemBootloader::ConfigWidget::ConfigWidget(const ::Programmer::Group &group, QWidget *parent)
  : ::Programmer::ConfigWidget(group, parent)
{
  uint row = numRows();

  QLabel *label = new QLabel(i18n("USB Vendor Id:"), this);
  addWidget(label, row,row, 0,0);
  _vendorId = new HexWordEditor(4, this);
  addWidget(_vendorId, row,row, 1,1);
  row++;

  label = new QLabel(i18n("USB Product Id:"), this);
  addWidget(label, row,row, 0,0);
  _productId = new HexWordEditor(4, this);
  addWidget(_productId, row,row, 1,1);
  row++;
}

void PicdemBootloader::ConfigWidget::saveConfig()
{
  Config::writeVendorId(_vendorId->value().toUInt());
  Config::writeProductId(_productId->value().toUInt());
}

void PicdemBootloader::ConfigWidget::loadConfig()
{
  _vendorId->setValue(Config::readVendorId());
  _productId->setValue(Config::readProductId());
}

//-----------------------------------------------------------------------------
::Programmer::ConfigWidget *PicdemBootloader::GroupUI::createConfigWidget(QWidget *parent) const
{
  return new ConfigWidget(static_cast<const ::Programmer::Group &>(group()), parent);
}
