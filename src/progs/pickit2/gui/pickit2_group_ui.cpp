/***************************************************************************
 * Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "pickit2_group_ui.h"
#include "pickit2_group_ui.moc"

#include <qfiledialog.h>

#include "common/global/purl.h"
#include "common/gui/misc_gui.h"
#include "progs/gui/prog_config_widget.h"
#include "progs/base/prog_group.h"
#include "progs/pickit2/base/pickit2_prog.h"
#include "progs/pickit2/base/pickit2.h"

//----------------------------------------------------------------------------
Pickit::AdvancedDialog::AdvancedDialog(Base &base, QWidget *parent, const char *name)
  : ::Programmer::PicAdvancedDialog(base, parent, name)
{}

void Pickit::AdvancedDialog::regenerateCalibration()
{
  if ( !base().deviceHasOsccalRegeneration() ) {
    MessageBox::sorry(i18n("Osccal regeneration not available for the selected device."), Log::Show);
    return;
  }
  QString filename = KFileDialog::getOpenFileName(KUrl(), PURL::filter(PURL::Hex), this, i18n("Open Calibration Firmware"));
  PURL::Url url = KUrl(filename);
  if ( url.isEmpty() ) return;
  base().regenerateOsccal(url);
}

//----------------------------------------------------------------------------
::Programmer::ConfigWidget *Pickit2::GroupUI::createConfigWidget(QWidget *parent) const
{
  return new ::Programmer::ConfigWidget(static_cast<const ::Programmer::Group &>(group()), parent);
}

::Programmer::AdvancedDialog *Pickit2::GroupUI::createAdvancedDialog(::Programmer::Base &base, QWidget *parent) const
{
  return new Pickit::AdvancedDialog(static_cast<Base &>(base), parent, "pickit2_advanced_dialog");
}
