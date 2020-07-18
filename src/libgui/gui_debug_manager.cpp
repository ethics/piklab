/***************************************************************************
 *   Copyright (C) 2006-2012 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "gui_debug_manager.h"
#include "gui_debug_manager.moc"

#include "toplevel.h"
#include "text_editor.h"
#include "project.h"
#include "tools/list/compile_config.h"
#include "tools/list/tool_list.h"
#include "register_view.h"
#include "watch_view.h"
#include "project_manager.h"
#include "editor_manager.h"
#include "main_global.h"
#include "coff/base/text_coff.h"
#include "progs/base/generic_prog.h"
#include "gui_prog_manager.h"
#include "breakpoint_view.h"
#include "progs/base/prog_group.h"

bool Debugger::GuiManager::addEditor(Editor &editor)
{
  if ( _editors.find(&editor)!=_editors.end() ) return false;
  connect(&editor, SIGNAL(destroyed()), SLOT(editorDestroyed()));
  _editors.append(&editor);
  return true;
}

void Debugger::GuiManager::addRegisterView(Register::MainView &view)
{
  if ( !addEditor(view) ) return;
  updateRegisters();
}

void Debugger::GuiManager::addTextEditor(TextEditor &editor)
{
  if ( !addEditor(editor) ) return;
  internalUpdateView(false);
}

void Debugger::GuiManager::clearEditors()
{
  QValueList<Editor *>::iterator it = _editors.begin();
  for (; it!=_editors.end(); ++it) (*it)->disconnect(this);
  _editors.clear();
}

void Debugger::GuiManager::editorDestroyed()
{
  QValueList<Editor *>::iterator it = _editors.begin();
  for (; it!=_editors.end(); ++it) {
    if ( (*it)!=sender() ) continue;
    _editors.remove(it);
    break;
  }
}

void Debugger::GuiManager::updateDevice()
{
  Manager::updateDevice();
  Main::watchView().init(false);
}

PURL::Url Debugger::GuiManager::coffUrl() const
{
  return Main::projectManager().projectUrl().toFileType(PURL::Coff);
}

bool Debugger::GuiManager::internalInit()
{
  if ( !Manager::internalInit() ) return false;
  if ( !Main::projectManager().contains(coffUrl()) )
    Main::projectManager().addExternalFile(coffUrl(), ProjectManager::Generated);
  Main::watchView().init(true);
  if ( registerView() ) registerView()->view()->updateView();
  return true;
}

bool Debugger::GuiManager::checkState(bool &first)
{
  if ( !Debugger::Manager::checkState(first) ) return false;
  if ( first && !Main::toolGroup().generateCompleteDebugInfo(Main::deviceData().name()) )
    MessageBox::information(i18n("The toolchain in use does not generate all necessary debugging information with the selected device. This may reduce debugging capabilities."), Log::Show, "doesnt_generate_debug_information");
  return true;
}

Breakpoint::Data Debugger::GuiManager::currentBreakpointData()
{
  const Breakpoint::Data *data = Main::breakpointsView().currentData();
  if (data != NULL) return *data;
  TextEditor *editor = ::qobject_cast<TextEditor *>(Main::currentEditor());
  ASSERT(editor != NULL);
  return Breakpoint::Data(editor->url(), editor->cursorLine());
}

void Debugger::GuiManager::toggleBreakpoint()
{
  if ( !Main::programmerGroup().isDebugger() ) {
    MessageBox::sorry(i18n("You cannot set breakpoints when a debugger is not selected."), Log::Show);
    return;
  }
  Breakpoint::Data data = currentBreakpointData();
  if ( Breakpoint::list().contains(data) ) {
    Breakpoint::list().remove(data);
    return;
  }
  Address address;
  if ( !checkBreakpoint(data, false, address) ) return;
  Breakpoint::list().append(data);
  Breakpoint::list().setAddress(data, address);
  if (_coff != NULL) toggleEnableBreakpoint();
  else {
    Editor* editor = Main::currentEditor();
    if (editor != NULL) editor->statusChanged();
  }
}

void Debugger::GuiManager::toggleEnableBreakpoint()
{
  ASSERT(_coff != NULL);
  Breakpoint::Data data = currentBreakpointData();
  bool isActive = ( Breakpoint::list().state(data)==Breakpoint::Active );
  if ( !Breakpoint::list().address(data).isValid() ) {
    if ( !isActive ) MessageBox::sorry(i18n("Breakpoint at non-code line cannot be activated."), Log::Show);
  } else {
    if ( !isActive ) {
      freeActiveBreakpoint();
      Breakpoint::list().setState(data, Breakpoint::Active);
    } else Breakpoint::list().setState(data, Breakpoint::Disabled);
  }
  Editor* editor = Main::currentEditor();
  if (editor != NULL) editor->statusChanged();
}

void Debugger::GuiManager::clearBreakpoints()
{
  Breakpoint::list().clear();
  Editor* editor = Main::currentEditor();
  if (editor != NULL) editor->statusChanged();
}

void Debugger::GuiManager::updateEditorMarks(TextEditor &editor) const
{
  editor.clearBreakpointMarks();
  // update breakpoints
  bool reached = false;
  for (uint i=0; i<Breakpoint::list().count(); i++) {
    const Breakpoint::Data &data = Breakpoint::list().data(i);
    int line = -1;
    Address address = Breakpoint::list().address(data);
    if ( _coff && address.isValid() ) line = _coff->lineForAddress(editor.url(), address);
    else if ( data.url==editor.url() ) line = data.line;
    Breakpoint::MarkType type = breakpointType(data);
    if ( line!=-1 ) editor.setMark(line, type);
    if ( type==Breakpoint::BreakpointReached ) reached = true;
  }
  // update pc
  if ( _coff && programmer() && programmer()->isActive() && pc().isInitialized() && !reached
        && _currentSourceLines.contains(editor.url()) ) {
    int pcline = _currentSourceLines[editor.url()];
    if ( programmer()->state()==Programmer::Halted ) editor.setMark(pcline, Breakpoint::ProgramCounterActive);
    else editor.setMark(pcline, Breakpoint::ProgramCounterDisabled);
  }
}

void Debugger::GuiManager::clear()
{
  Manager::clear();
  Main::watchView().init(true);
}

bool Debugger::GuiManager::update(bool gotoPC)
{
  return Manager::update(gotoPC);
}

bool Debugger::GuiManager::checkIfContinueStepping(bool &continueStepping)
{
  if ( Main::toolGroup().generateCompleteDebugInfo(Main::deviceData().name()) )
    return Debugger::Manager::checkIfContinueStepping(continueStepping);
  if ( !update(false) ) return false;
  continueStepping = false;
  return true;
}

void Debugger::GuiManager::internalUpdateView(bool gotoPC)
{
  Main::breakpointsView().updateView();
  bool currentHasPC = false;
  QValueList<Editor *>::iterator ite;
  for (ite=_editors.begin(); ite!=_editors.end(); ++ite) {
    TextEditor *e = ::qobject_cast<TextEditor *>(*ite);
    if ( e==NULL ) continue;
    updateEditorMarks(*e);
    if ( !_currentSourceLines.contains(e->url()) ) continue;
    if (gotoPC) e->setCursorPosition(_currentSourceLines[e->url()], 0);
    if ( e==Main::currentEditor() ) currentHasPC = true;
  }

  MainWindow::self().updateGUI();

  if ( programmer()==NULL
       || programmer()->state()!=Programmer::Halted
       || !pc().isInitialized()
       || currentHasPC ) return;

  // 1: look at files inside project and not generated
  // 2: look at files inside project
  // 3: look at existing files
  for (uint i=0; i<3; i++) {
    QMap<PURL::Url, uint>::const_iterator it;
    for (it=_currentSourceLines.begin(); it!=_currentSourceLines.end(); ++it) {
      switch (i) {
        case 0: if ( !Main::projectManager().contains(it.key()) || Main::projectManager().isExternalFile(it.key()) ) continue; break;
        case 1: if ( !Main::projectManager().contains(it.key()) ) continue; break;
        case 2: if ( !it.key().exists() ) continue; break;
      }
      TextEditor *e = ::qobject_cast<TextEditor *>(Main::editorManager().findEditor(it.key()));
      if ( e==NULL ) {
        if (gotoPC) e = ::qobject_cast<TextEditor *>(Main::editorManager().openEditor(it.key()));
        if ( e==NULL ) continue;
      }
      updateEditorMarks(*e);
      if (gotoPC) {
        e->setCursorPosition(_currentSourceLines[e->url()], 0);
        Main::editorManager().showEditor(e);
      }
      break;
    }
  }
}

Register::MainView *Debugger::GuiManager::registerView() const
{
  for (QValueList<Editor *>::const_iterator it = _editors.begin(); it!=_editors.end(); ++it) {
    Register::MainView *rv = ::qobject_cast<Register::MainView *>(*it);
    if (rv) return rv;
  }
  return NULL;
}

bool Debugger::GuiManager::isProjectSource(const PURL::Url &url) const
{
  return ( Main::projectManager().contains(url) && !Main::projectManager().isExternalFile(url) );
}

void Debugger::GuiManager::showDisassemblyLocation()
{
  TextEditor *editor = ::qobject_cast<TextEditor *>(Main::currentEditor());
  ASSERT(editor);
  ASSERT(_coff);
  QValueVector<Address> addresses = _coff->addresses(editor->url(), editor->cursorLine());
  if ( addresses.isEmpty() ) {
    MessageBox::sorry(i18n("Cannot show disassembly location for non-code line."), Log::Show);
    return;
  }
  int line = _coff->lineForAddress(_coff->url(), addresses[0]);
  if ( line==-1 ) return; // possible ?
  TextEditor *e = ::qobject_cast<TextEditor *>(Main::editorManager().openEditor(_coff->url()));
  if (e) e->setCursorPosition(line, 0);
}
