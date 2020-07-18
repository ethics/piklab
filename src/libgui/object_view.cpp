/***************************************************************************
 *   Copyright (C) 2006-2009 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "object_view.h"
#include "object_view.moc"

#include "devices/base/device_group.h"
#include "devices/pic/pic/pic_memory.h"
#include "tools/gputils/gputils.h"
#include "hex_editor.h"
#include "coff/base/text_coff.h"
#include "coff/base/coff_archive.h"
#include "main_global.h"
#include "tools/list/compile_process.h"
#include "coff/base/disassembler.h"
#include "common/gui/misc_gui.h"

//-----------------------------------------------------------------------------
Coff::BaseEditor::BaseEditor(const PURL::Url &source, const Device::Data *data, QWidget *parent)
: SimpleTextEditor(true, parent),
  _source(source),
  _ok(false),
  _created(NULL),
  _device(data)
{
  setReadOnly(true);
  //  _view->setDynWordWrap(false);
  setView(&Main::compileLog());
}

Coff::BaseEditor::~BaseEditor()
{
  delete _created;
  _created = NULL;
}

//-----------------------------------------------------------------------------
Coff::CoffEditor::CoffEditor(const PURL::Url &source, const Device::Data &data, QWidget *parent)
: BaseEditor(source, &data, parent),
  _provided(NULL)
{}

Coff::CoffEditor::CoffEditor(const TextObject &object, QWidget *parent)
: BaseEditor(PURL::Url(), object.device(), parent),
  _provided(&object)
{}

bool Coff::CoffEditor::open(const PURL::Url &url)
{
  _url = url;
  if (_provided != NULL) return setText(_provided->disassembly());
  if (_created == NULL) {
    _created = new TextObject(_device, _source);
    _ok = _created->parse(*this);
  }
  return setText(static_cast<Coff::TextObject *>(_created)->disassembly());
}

//-----------------------------------------------------------------------------
Coff::ObjectEditor::ObjectEditor(const PURL::Url &source, QWidget *parent)
: BaseEditor(source, NULL, parent)
{}

bool Coff::ObjectEditor::open(const PURL::Url &url)
{
  _url = url;
  if (_created == NULL) {
    _created = new TextObject(NULL, _source);
    _ok = _created->parse(*this);
  }
  if ( !_ok ) return setText(i18n("Error parsing object:\n") + error());
  QString s = coff()->information().text() + "\n";
  return setText(s);
}

//-----------------------------------------------------------------------------
Coff::LibraryEditor::LibraryEditor(const PURL::Url &source, QWidget *parent)
: BaseEditor(source, NULL, parent)
{}

bool Coff::LibraryEditor::open(const PURL::Url &url)
{
  _url = url;
  if (_created == NULL) {
    _created = new Archive(_source);
    _ok = _created->parse(*this);
  }
  if ( !_ok ) return setText(i18n("Error parsing library:\n") + error());
  QString s = coff()->information().text() + "\n";
  if ( coff()->members().count()!=0 ) s += coff()->membersInformation().text() + "\n";
  if ( coff()->symbols().count()!=0 ) s += coff()->symbolsInformation().text() + "\n";
  return setText(s);
}

//-----------------------------------------------------------------------------
DisassemblyEditor::DisassemblyEditor(const HexEditor &e, const Device::Data &data, QWidget *parent)
: SimpleTextEditor("Disassembly", "disassembly", parent),
  _device(data),
  _editor(e)
{
  setFileType(PURL::AsmGPAsm);
  setReadOnly(true);
  // _view->setDynWordWrap(false);
  setView(&Main::compileLog());
}

bool DisassemblyEditor::open(const PURL::Url&)
{
  Pic::Architecture arch = (_device.group().name()=="pic" ? static_cast<const Pic::Data &>(_device).architecture() : Pic::Architecture(Pic::Architecture::Nb_Types));
  if ( arch==Pic::Architecture::Nb_Types ) {
    MessageBox::sorry(i18n("Disassembly not supported for the selected device."), Log::Show);
    return false;
  }

  log(Log::LineType::Information, i18n("Disassembling content of hex file editor."));
  const Device::Memory& memory = *_editor.memory();
  SourceLine::List list = GPUtils::disassemble(static_cast<const Pic::Memory&>(memory));
  log(Log::LineType::Information, i18n("Done."));
  return setText(SourceLine::text(PURL::SourceFamily::Asm, list, 4));
}
