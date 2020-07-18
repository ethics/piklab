/* Copyright (C) 2000 David Faure <faure@kde.org>, Alexander Neundorf <neundorf@kde.org>
                 2000, 2002 Carsten Pfeiffer <pfeiffer@kde.org>
   Copyright (C) 2006-2007 Nicolas Hadacek <hadacek@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#include "editlistbox.h"
#include "editlistbox.moc"

#include <qstringlist.h>
#include <qlabel.h>

#include "misc_gui.h"
#include "dialog.h"

EditListBox::EditListBox(uint nbColumns, QWidget *parent, const char *name, Mode mode, Buttons buttons)
  : Q3Frame(parent, name), _mode(mode), _buttons(buttons)
{
  m_lineEdit = new KLineEdit;
  init(nbColumns, m_lineEdit);
}

EditListBox::EditListBox(uint nbColumns, QWidget *view, KLineEdit *lineEdit, QWidget *parent, const char *name,
                         Mode mode, Buttons buttons)
  : Q3Frame(parent, name), _mode(mode), _buttons(buttons)
{
  m_lineEdit = lineEdit;
  init(nbColumns, view);
}

void EditListBox::init(uint nbColumns, QWidget *view)
{
    _addButton = 0;
    _removeButton = 0;
    _moveUpButton = 0;
    _moveDownButton = 0;
    _removeAllButton = 0;
    _resetButton = 0;
    setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));

  QGridLayout *grid = new QGridLayout(this, 1, 1, 0, Dialog::spacingHint());
    uint row = 0;
    if (view) {
      QHBoxLayout *hbox = new QHBoxLayout(Dialog::spacingHint());
      grid->addMultiCellLayout(hbox, row,row, 0,1);
      if (m_lineEdit) {
	KIcon icon("edit-clear-locationbar-ltr");
        KPushButton *button = new KPushButton(icon, QString::null, this);
        connect(button, SIGNAL(clicked()), SLOT(clearEdit()));
        hbox->addWidget(button);
      }
      view->reparent( this, QPoint(0,0) );
      hbox->addWidget(view);
      row++;
    }
    _listView = new ListView(this);
    for (uint i=0; i<nbColumns; i++) _listView->addColumn(QString::null);
    _listView->header()->hide();
    _listView->setSorting(-1);
    _listView->setResizeMode(Q3ListView::LastColumn);
    _listView->setColumnWidthMode(nbColumns-1, Q3ListView::Maximum);
    grid->addWidget(_listView, row, 0);
    QVBoxLayout *vbox = new QVBoxLayout(10);
    grid->addLayout(vbox, row, 1);
    _buttonsLayout = new QVBoxLayout(10);
    vbox->addLayout(_buttonsLayout);
    vbox->addStretch(1);

    setButtons(_buttons);

    if (m_lineEdit) {
      connect(m_lineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(typedSomething(const QString&)));
      m_lineEdit->setTrapReturnKey(true);
      connect(m_lineEdit, SIGNAL(returnPressed()), this, SLOT(addItem()));
    }
    connect(_listView, SIGNAL(selectionChanged()), SLOT(selectionChanged()));

    // maybe supplied lineedit has some text already
    typedSomething(m_lineEdit ? m_lineEdit->text() : QString::null);
}

void EditListBox::setButtons(Buttons buttons)
{
    _buttons = buttons;

    delete _addButton;
    _addButton = NULL;
    if ( buttons & Add ) {
        _addButton = new KPushButton(KStandardGuiItem::add(), this);
        _addButton->setEnabled(false);
        _addButton->show();
        connect(_addButton, SIGNAL(clicked()), SLOT(addItem()));
        _buttonsLayout->addWidget(_addButton);
    }

    delete _removeButton;
    _removeButton = 0;
    if ( buttons & Remove ) {
        _removeButton = new KPushButton(KGuiItem(i18n("Remove"), "list-remove"), this);
        _removeButton->setEnabled(false);
        _removeButton->show();
        connect(_removeButton, SIGNAL(clicked()), SLOT(removeItem()));
        _buttonsLayout->addWidget(_removeButton);
    }

    delete _removeAllButton;
    _removeAllButton = 0;
    if ( buttons & RemoveAll ) {
        _removeAllButton = new KPushButton(KGuiItem(i18n("Remove All"), "edit-clear-list"), this);
        _removeAllButton->show();
        connect(_removeAllButton, SIGNAL(clicked()), SLOT(clear()));
        _buttonsLayout->addWidget(_removeAllButton);
    }

    delete _moveUpButton;
    _moveUpButton = 0;
    delete _moveDownButton;
    _moveDownButton = 0;
    if ( buttons & UpDown ) {
        _moveUpButton = new KPushButton(KGuiItem(i18n("Move &Up"), "go-up"), this);
        _moveUpButton->setEnabled(false);
        _moveUpButton->show();
        connect(_moveUpButton, SIGNAL(clicked()), SLOT(moveItemUp()));
        _buttonsLayout->addWidget(_moveUpButton);
        _moveDownButton = new KPushButton(KGuiItem(i18n("Move &Down"), "go-down"), this);
        _moveDownButton->setEnabled(false);
        _moveDownButton->show();
        connect(_moveDownButton, SIGNAL(clicked()), SLOT(moveItemDown()));
        _buttonsLayout->addWidget(_moveDownButton);
    }

    delete _resetButton;
    _resetButton = 0;
    if ( buttons & Reset ) {
        _resetButton = new KPushButton(KStandardGuiItem::reset(), this);
        _resetButton->show();
        connect(_resetButton, SIGNAL(clicked()), SIGNAL(reset()));
        _buttonsLayout->addWidget(_resetButton);
    }
}

void EditListBox::typedSomething(const QString& text)
{
  Q3ListViewItem *item = _listView->selectedItem();
  if (item) {
    if( selectedText()!=text ) {
      item->setText(textColumn(), text);
      emit changed();
    }
  }
  updateButtons();
}

void EditListBox::moveItemUp()
{
  Q3ListViewItem *item = _listView->selectedItem();
  if ( item==0 || item->itemAbove()==0 ) return;
  item->itemAbove()->moveItem(item);
  updateButtons();
  emit changed();
}

void EditListBox::moveItemDown()
{
  Q3ListViewItem *item = _listView->selectedItem();
  if ( item==0 || item->itemBelow()==0 ) return;
  item->moveItem(item->itemBelow());
  updateButtons();
  emit changed();
}

void EditListBox::addItem()
{
    // when m_checkAtEntering is true, the add-button is disabled, but this
    // slot can still be called through Key_Return/Key_Enter. So we guard
    // against this.
    if ( !_addButton || !_addButton->isEnabled() || m_lineEdit==0 ) return;

    addItem(m_lineEdit->text());
}

void EditListBox::addItem(const QString &text)
{
    bool alreadyInList(false);
    //if we didn't check for dupes at the inserting we have to do it now
    if ( _mode==DuplicatesDisallowed ) alreadyInList = _listView->findItem(text, textColumn());

    if (m_lineEdit) {
      bool block = m_lineEdit->signalsBlocked();
      m_lineEdit->blockSignals(true);
      m_lineEdit->clear();
      m_lineEdit->blockSignals(block);
    }
    _listView->clearSelection();

    if (!alreadyInList) {
      Q3ListViewItem *item = createItem();
      item->setText(textColumn(), text);
      if ( _listView->lastItem() ) item->moveItem(_listView->lastItem());
      emit changed();
      emit added(text);
    }
    updateButtons();
}

void EditListBox::clearEdit()
{
  _listView->clearSelection();
  if (m_lineEdit) {
    m_lineEdit->clear();
    m_lineEdit->setFocus();
  }
  updateButtons();
}

void EditListBox::removeItem()
{
  Q3ListViewItem *item = _listView->selectedItem();
  if (item) {
    QString text = item->text(textColumn());
    delete item;
    emit changed();
    emit removed(text);
    updateButtons();
  }
}

void EditListBox::selectionChanged()
{
  if (m_lineEdit) m_lineEdit->setText(selectedText());
  updateButtons();
}

void EditListBox::clear()
{
  _listView->clear();
  if (m_lineEdit) {
    m_lineEdit->clear();
    m_lineEdit->setFocus();
  }
  updateButtons();
  emit changed();
}

uint EditListBox::count() const
{
  uint nb = 0;
  Q3ListViewItemIterator it(_listView);
  for (; it.current(); ++it) nb++;
  return nb;
}

const Q3ListViewItem *EditListBox::item(uint i) const
{
  uint k = 0;
  Q3ListViewItemIterator it(_listView);
  for (; it.current(); ++it) {
    if ( k==i ) return it.current();
    k++;
  }
  return NULL;
}

QStringList EditListBox::texts() const
{
  QStringList list;
  Q3ListViewItemIterator it(_listView);
  for (; it.current(); ++it) list.append(it.current()->text(textColumn()));
  return list;
}

void EditListBox::setTexts(const QStringList& items)
{
  _listView->clear();
  for (int i=items.count()-1; i>=0; i--) {
    Q3ListViewItem *item = createItem();
    item->setText(textColumn(), items[i]);
  }
  if (m_lineEdit) m_lineEdit->clear();
  updateButtons();
}

void EditListBox::updateButtons()
{
  Q3ListViewItem *item = _listView->selectedItem();
  if (_addButton) {
    if ( m_lineEdit==0 ) _addButton->setEnabled(true);
    else {
      QString text = m_lineEdit->text();
      if ( _mode!=DuplicatesCheckedAtEntering ) _addButton->setEnabled(!text.isEmpty());
      else if ( text.isEmpty() ) _addButton->setEnabled(false);
      else _addButton->setEnabled(!_listView->findItem(text, textColumn()));
    }
  }
  if (_removeButton) _removeButton->setEnabled(item);
  if (_moveUpButton) _moveUpButton->setEnabled(item && item->itemAbove());
  if (_moveDownButton) _moveDownButton->setEnabled(item && item->itemBelow());
  if (_removeAllButton) _removeAllButton->setEnabled(_listView->firstChild());
}

void EditListBox::setEditText(const QString &text)
{
  _listView->clearSelection();
  if (m_lineEdit) m_lineEdit->setText(text);
  updateButtons();
}

Q3ListViewItem *EditListBox::createItem()
{
  return new KListViewItem(_listView);
}

QString EditListBox::selectedText() const
{
  Q3ListViewItem *item = _listView->selectedItem();
  if (item == NULL) return QString::null;
  return item->text(textColumn());
}
