/***************************************************************************
 *   Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "breakpoint_view.h"
#include "breakpoint_view.moc"

#include "main_global.h"
#include "editor_manager.h"
#include "coff/base/text_coff.h"
#include "gui_debug_manager.h"

//----------------------------------------------------------------------------
Breakpoint::ListViewItem::ListViewItem(ListView *parent, const Data &data)
  : KListViewItem(parent), _data(data)
{}

//----------------------------------------------------------------------------
Breakpoint::View::View(QWidget *parent)
  : QWidget(parent, "breakpoints_view"), GenericView(Breakpoint::list()),
    _currentData(0)
{
  QVBoxLayout *top = new QVBoxLayout(this);
  _listview = new ListView(this);
  connect(_listview, SIGNAL(clicked(Q3ListViewItem *)), SLOT(itemClicked(Q3ListViewItem *)));
  connect(_listview, SIGNAL(contextMenuRequested(Q3ListViewItem *, const QPoint &, int)),
          SLOT(contextMenu(Q3ListViewItem *, const QPoint &, int)));
  _listview->setAllColumnsShowFocus(true);
  _listview->addColumn(i18n("Status"));
  _listview->addColumn(i18n("File"));
  _listview->addColumn(i18n("Line"));
  _listview->addColumn(i18n("Address"));
  _listview->setSorting(2);
  top->addWidget(_listview);
}

void Breakpoint::View::updateView()
{
  // #### flickering...
  _listview->clear();
  for (uint i=0; i<Breakpoint::list().count(); i++) {
    const Data &data = Breakpoint::list().data(i);
    KListViewItem *item = new ListViewItem(_listview, data);
    item->setPixmap(0, TextEditor::pixmap(Debugger::manager->breakpointType(data)));
    item->setText(1, data.url.filename());
    item->setText(2, QString::number(data.line));
    Address address = Breakpoint::list().address(data);
    if ( address.isValid() ) item->setText(3, toHexLabelAbs(address));
    else if ( Debugger::manager->coff() ) item->setText(3, i18n("Non-code breakpoint"));
    else item->setText(3, "---");
  }
}

void Breakpoint::View::itemClicked(Q3ListViewItem *item)
{
  if ( item==0 ) return;
  const Data &data = static_cast<ListViewItem *>(item)->data();
  Address address = Breakpoint::list().address(data);
  TextEditor *editor = ::qobject_cast<TextEditor *>(Main::currentEditor());
  const Coff::TextObject *coff = Debugger::manager->coff();
  int line = -1;
  if ( coff && editor && editor->fileType()==PURL::Coff && address.isValid() )
    line = coff->lineForAddress(editor->url(), address);
  if ( line==-1 ) {
    editor = ::qobject_cast<TextEditor *>(Main::editorManager().openEditor(data.url));
    line = data.line;
  }
  if ( editor==0 ) return;
  editor->show();
  editor->setCursorPosition(line, 0);
}

void Breakpoint::View::contextMenu(Q3ListViewItem *item, const QPoint &pos, int)
{
  _currentData = (item != NULL ? &static_cast<ListViewItem *>(item)->data() : NULL);
  PopupMenu pop;
  pop.addTitle(i18n("Breakpoints"));
  QAction* a  = pop.addAction(i18n("Remove breakpoint"), Debugger::manager, SLOT(toggleBreakpoint()));
  a->setEnabled(_currentData != NULL);
  const bool isActive = (_currentData != NULL ? Breakpoint::list().state(*_currentData)==Breakpoint::Active : false);
  a = pop.addAction(!isActive ? i18n("Enable breakpoint") : i18n("Disable breakpoint"), Debugger::manager, SLOT(toggleEnableBreakpoint()));
  const Coff::TextObject *coff = Debugger::manager->coff();
  a->setEnabled(_currentData != NULL && coff != NULL);
  a = pop.addAction(KIcon("edit-delete"), i18n("Clear"), Debugger::manager, SLOT(clearBreakpoints()));
  a->setEnabled(Breakpoint::list().count() > 0);
  pop.exec(pos);
  _currentData = NULL;
}
