/***************************************************************************
 *   Copyright (C) 2005-2007 Nicolas Hadacek <hadacek@kde.org>             *
 *   Copyright (C) 2003 Alain Gibaud <alain.gibaud@free.fr>                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "hex_editor.h"
#include "hex_editor.moc"

#include <kactioncollection.h>
#include <ktoggleaction.h>
#include <kxmlguifactory.h>

#include "devices/base/device_group.h"
#include "devices/gui/device_group_ui.h"
#include "devices/gui/hex_view.h"
#include "toplevel.h"
#include "common/global/pfile.h"
#include "main_global.h"

//-----------------------------------------------------------------------------
HexEditorPart::HexEditorPart(HexEditor *editor)
 : KParts::ReadWritePart(editor)
{
  setObjectName("hex_editor_part");
  setXMLFile("hexeditorpartui.rc");

  actionCollection()->addAction(KStandardAction::Save, editor, SLOT(save()));
  actionCollection()->addAction(KStandardAction::SaveAs, editor, SLOT(saveAs()));
  QAction* a = action("tools_toggle_write_lock");
  delete a;
  a = new KToggleAction(i18n("Read Only Mode"), this);
  actionCollection()->addAction("tools_toggle_write_lock", a);
  connect(a, SIGNAL(toggled(bool)), editor, SLOT(toggleReadOnly()));
}

void HexEditorPart::setReadWrite(bool rw)
{
  KParts::ReadWritePart::setReadWrite(rw);
  QAction* a = action("tools_toggle_write_lock");
  a->blockSignals(true);
  a->setChecked(!rw);
  a->blockSignals(false);
}

//-----------------------------------------------------------------------------
HexEditor::HexEditor(const QString &name, QWidget *parent)
  : DeviceEditor(name, QString::null, parent, "hex_editor")
{
  init();
}

HexEditor::HexEditor(QWidget *parent)
  : DeviceEditor(parent, "hex_editor")
{
  init();
}

void HexEditor::init()
{
  _modified = false;
  _top->addStretch(1);
  _part = new HexEditorPart(this);
  setReadOnly(false);
}

QWidget *HexEditor::createView(const Device::Data& data, QWidget *parent)
{
  _originalMemory = data.group().createMemory(data);
  _memory = data.group().createMemory(data);
  Device::HexView *hv = Device::groupui(data).createHexView(*this, parent);
  connect(hv, SIGNAL(modified()), SLOT(slotModified()));
  _dirty = true;
  QTimer::singleShot(0, this, SLOT(simpleLoad()));
  return hv;
}

bool HexEditor::simpleLoad()
{
  if ( !_dirty ) return true;
  _dirty = false;
  if (_memory.get() != NULL) {
    QStringList warnings;
    if ( _memory->fromHexBuffer(_hexBuffer, warnings)!=Device::Memory::NoWarning ) {
      _labelWarning->setText(i18n("<b>Warning:</b> hex file seems to be incompatible with the selected device %1:<br>%2")
                             .arg(_memory->device().name()).arg(warnings.join("<br>")));
      _labelWarning->show();
    } else _labelWarning->hide();
    display();
  }
  return true;
}

void HexEditor::setReadOnlyInternal(bool readOnly)
{
  _part->setReadWrite(!readOnly);
  if (_memory.get() != NULL) static_cast<Device::HexView*>(_view.get())->setReadOnly(readOnly);
}

void HexEditor::addGui()
{
  MainWindow::self().guiFactory()->addClient(_part);
}

void HexEditor::removeGui()
{
  MainWindow::self().guiFactory()->removeClient(_part);
}

bool HexEditor::open(const PURL::Url &url)
{
  _url = url;
  PURL::File file(url, Main::compileLog());
  if ( !file.openForRead() ) return false;
  QStringList errors;
  if ( !_hexBuffer.load(file.stream(), errors) ) {
    MessageBox::detailedSorry(i18n("Error(s) reading hex file."), errors.join("\n"), Log::Show);
    return false;
  }
  _dirty = true;
  return simpleLoad();
}

bool HexEditor::save(const PURL::Url &url)
{
  return save(url, i18n("File URL: \"%1\".").arg(url.pretty()));
}

bool HexEditor::save(const PURL::Url &url, const QString &fileErrorString)
{
  PURL::File file(url, Main::compileLog());
  if ( !file.openForWrite() ) return false;
  if ( !_memory->save(file.stream(), HexBuffer::IHX32) ) {
    MessageBox::detailedSorry(i18n("Error while writing file \"%1\".").arg(url.pretty()), fileErrorString, Log::Show);
    return false;
  }
  _originalMemory->copyFrom(*_memory);
  _url = url;
  return file.close();
}

void HexEditor::display()
{
  _modified = false;
  if (_memory.get() != NULL) {
    _originalMemory->copyFrom(*_memory);
    static_cast<Device::HexView*>(_view.get())->display(_memory.get());
    static_cast<Device::HexView*>(_view.get())->setReadOnly(isReadOnly());
    static_cast<Device::HexView*>(_view.get())->updateDisplay();
  }
  statusChanged();
}

void HexEditor::memoryRead()
{
  display();
  emit guiChanged();
}

void HexEditor::slotModified()
{
  static_cast<Device::HexView*>(_view.get())->updateDisplay();
  _modified = true;
  statusChanged();
  emit guiChanged();
}

void HexEditor::statusChanged()
{
  QString s;
  if (_memory.get() != NULL) {
    BitValue cs = static_cast<Device::HexView*>(_view.get())->checksum();
    s = i18n("Checksum: %1").arg(toHexLabel(cs, 4));
  }
  emit statusTextChanged(s);
}
