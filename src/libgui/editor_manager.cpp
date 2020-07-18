/***************************************************************************
 *   Copyright (C) 2005-2009 Nicolas Hadacek <hadacek@kde.org>             *
 *   Copyright (C) 2003-2004 Alain Gibaud <alain.gibaud@free.fr>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "editor_manager.h"
#include "editor_manager.moc"

#include <krecentfilesaction.h>

#include "text_editor.h"
#include "hex_editor.h"
#include "object_view.h"
#include "new_dialogs.h"
#include "main_global.h"
#include "gui_debug_manager.h"
#include "common/gui/purl_gui.h"
#include "device_gui.h"
#include "register_view.h"
#include "device_editor.h"
#include "project_manager.h"
#include "global_config.h"
#include "project.h"

//-----------------------------------------------------------------------------
SwitchToDialog::SwitchToDialog(const QStringList &names, QWidget *parent)
  : Dialog(parent, "switch_to_dialog", true, i18n("Switch to editor"), Ok | Cancel, Ok, false)
{
  QVBoxLayout *top = new QVBoxLayout(mainWidget(), 10, 10);
  _edit = new KLineEdit(mainWidget());
  _edit->setCompletedItems(names);
  top->addWidget(_edit);
}

//-----------------------------------------------------------------------------
void EditorTabBar::setReadOnly(uint index, bool readOnly)
{
#ifndef NO_KDE
  //  _readOnly[tabAt(index)] = readOnly;
#endif
}

#ifndef NO_KDE
void EditorTabBar::paintLabel(QPainter *p, const QRect &br, QTab *t, bool has_focus) const
{
  //  QFont f = p->font();
  //  f.setItalic(_readOnly[t]);
  //  p->setFont(f);
  //  QTabBar::paintLabel(p, br, t, has_focus);
}
#endif

//-----------------------------------------------------------------------------
QString EditorHistory::goBack()
{
  if ( !hasBack() ) return QString::null;
  _current--;
  return _names[_current];
}

QString EditorHistory::goForward()
{
  if ( !hasForward() ) return QString::null;
  _current++;
  return _names[_current];
}

void EditorHistory::add(const QString &name)
{
  if ( _names.count()!=0 ) {
    _current = qMin(_current, uint(_names.count()-1));
    if ( _names[_current]==name ) return;
    if ( _current!=0 && _names[_current-1]==name ) {
      _current--;
      return;
    }
    _current++;
    if ( _current<(uint)_names.count() && _names[_current]==name ) return;
  }
  _names.resize(_current+1);
  _names[_current] = name;
}

void EditorHistory::closedLast()
{
  if ( _names.count()==0 ) return;
  _names.resize(_current);
  if ( _current!=0 ) _current--;
}

//-----------------------------------------------------------------------------
EditorManager::EditorManager(QWidget *parent)
  : TabWidget(parent, "editor_manager"), _current(0)
{
  setTabBar(new EditorTabBar(this));
  connect(this, SIGNAL(currentChanged(QWidget *)), SLOT(showEditor(QWidget *)));
#ifndef NO_KDE
  setHoverCloseButton(readConfigEntry(BaseGlobalConfig::ShowTabCloseButton).toBool());
  setHoverCloseButtonDelayed(false);
#endif
}

bool EditorManager::openFile(const PURL::Url &url)
{
  if ( url.isEmpty() ) return false;
  Editor *e = findEditor(url);
  if (e) { // document already loaded
    KGuiItem warning(i18n("Warning"));
    if ( !MessageBox::askContinue(i18n("File \"%1\" already loaded. Reload?").arg(url.kurl().prettyUrl()),
                                 warning, i18n("Reload")) ) return true;
    if ( !e->slotLoad() ) {
      closeEditor(e, false);
      return false;
    }
    return true;
  }
  if ( !openEditor(url) ) {
    static_cast< KRecentFilesAction *>(Main::action("file_open_recent"))->removeUrl(url.kurl());
    return false;
  }
  static_cast<KRecentFilesAction *>(Main::action("file_open_recent"))->addUrl(url.kurl());
  return true;
}

void EditorManager::connectEditor(Editor *editor)
{
    disconnectEditor(currentEditor());
    if ( editor==0 ) return;
    editor->addGui();
    connect(editor, SIGNAL(modified()), SLOT(modifiedSlot()));
    connect(editor, SIGNAL(guiChanged()), SIGNAL(guiChanged()));
    connect(editor, SIGNAL(dropEventPass(QDropEvent *)), SLOT(slotDropEvent(QDropEvent *)));
    connect(editor, SIGNAL(statusTextChanged(const QString &)), SIGNAL(statusChanged(const QString &)));
}

void EditorManager::modifiedSlot()
{
  emit modified(currentEditor()->url());
}

void EditorManager::disconnectEditor(Editor *editor)
{
  if ( editor==0 ) return;
  editor->disconnect(this);
  editor->removeGui();
}

QString EditorManager::title(const Editor &e) const
{
  return (e.url().isEmpty() ? "<" + e.name() + ">" : e.url().filename());
}

void EditorManager::updateTitles()
{
  QPixmap def = IconLoader::loadIcon("piklab", IconLoader::Small);
  QPixmap modified = IconLoader::loadIcon("document-save", IconLoader::Small);
  QPixmap chip = IconLoader::loadIcon("media-flash", IconLoader::Small);
  for (QValueList<Editor *>::iterator it = _editors.begin(); it!=_editors.end(); ++it) {
    Editor* e = *it;
    static_cast<EditorTabBar*>(tabBar())->setReadOnly(indexOf(e), e->isReadOnly());
    QPixmap pixmap;
    if ( e->isModified() ) pixmap = modified;
    else if ( ::qobject_cast<DeviceEditor*>(e)==0 ) pixmap = PURL::icon(e->fileType());
    else pixmap = chip;
    changeTab(e, pixmap.isNull() ? def : pixmap, title(*e));
  }
}

bool EditorManager::closeCurrentEditor()
{
  if ( !closeEditor(_current, true) ) return false;
  emit guiChanged();
  return true;
}

bool EditorManager::closeAllEditors()
{
  if ( currentEditor()==0 ) return true;
  while ( currentEditor() )
    if ( !closeEditor(currentEditor(), true) ) break;
  emit guiChanged();
  return ( currentEditor()==0 );
}

bool EditorManager::closeAllOtherEditors()
{
  if ( nbEditors()==1 ) return true;
  QValueList<Editor *> list = _editors;
  list.remove(currentEditor());
  QValueList<Editor *>::iterator it = list.begin();
  bool ok = true;
  for (; it!=list.end(); ++it) {
    if ( !closeEditor(*it, true) ) {
      ok = false;
      break;
    }
  }
  emit guiChanged();
  return ok;
}

bool EditorManager::closeEditor(const PURL::Url &url)
{
  Editor *e = findEditor(url);
  if ( e==0 ) return true;
  if ( !closeEditor(e, true) ) return false;
  emit guiChanged();
  return true;
}

void EditorManager::closeRequest(int i)
{
  closeEditor(static_cast<Editor *>(page(i)), true);
  emit guiChanged();
}

bool EditorManager::closeEditor(Editor *e, bool ask)
{
  if ( e==0 ) return true;
  if ( ask && !e->checkSaved() ) return false;
  removePage(e);
  _editors.remove(e);
  Editor *g = static_cast<Editor *>(currentPage());
  changeToEditor(g);
  saveBookmarks(*e);
  delete e;
  return true;
}

void EditorManager::saveBookmarks(const Editor &e)
{
  if (Main::project() != NULL) {
    QValueList<uint> lines = e.bookmarkLines();
    Main::project()->setBookmarkLines(e.url(), lines);
  }
}

void EditorManager::restoreBookmarks(Editor &e)
{
  if (Main::project() != NULL) {
    QValueList<uint> lines = Main::project()->bookmarkLines(e.url());
    e.setBookmarkLines(lines);
  }
}

void EditorManager::showEditor(Editor *e)
{
  changeToEditor(e);
  emit guiChanged();
}

void EditorManager::changeToEditor(Editor *e)
{
  if ( e==_current ) return;
  connectEditor(e);
  _current = e;
  if (e) {
    showPage(e);
    e->clearFocus();   // force a got focus signal
    e->setFocus();
    e->statusChanged();
    _history.add(name(*e));
  } else {
    emit statusChanged(QString::null);
    _history.closedLast();
  }
}

Editor *EditorManager::findEditor(const QString &tag)
{
  QValueList<Editor *>::iterator it = _editors.begin();
  for (; it!=_editors.end(); ++it) if ( (*it)->tag()==tag ) return *it;
  return 0;
}

Editor *EditorManager::findEditor(const PURL::Url &url)
{
  QValueList<Editor *>::iterator it = _editors.begin();
  for (; it!=_editors.end(); ++it) if ( (*it)->url()==url ) return *it;
  return 0;
}

Editor *EditorManager::createEditor(PURL::FileType type, const PURL::Url &url)
{
  Editor *e = findEditor(url);
  if (e) closeEditor(e, false);
  switch (type.type()) {
    case PURL::Hex:
      e = new HexEditor(this);
      break;
    case PURL::CSource:
    case PURL::CppSource:
    case PURL::CHeader:
    case PURL::AsmGPAsm:
    case PURL::AsmPIC30:
    case PURL::AsmPICC:
    case PURL::Inc:
    case PURL::JalSource:
    case PURL::BasicSource:
      e = new TextEditor(true, this);
      break;
    case PURL::Lkr:
    case PURL::Gld:
    case PURL::Map:
    case PURL::Lst:
    case PURL::Cod:
    case PURL::Unknown:
      e = new TextEditor(false, this);
      break;
    case PURL::Coff:
      if ( Debugger::manager->coff()==0 ) return 0;
      e = new Coff::CoffEditor(*Debugger::manager->coff(), this);
      static_cast<Coff::CoffEditor *>(e)->setView(&Main::compileLog());
      break;
    case PURL::Object:
      e = new Coff::ObjectEditor(url, this);
      static_cast<Coff::ObjectEditor *>(e)->setView(&Main::compileLog());
      break;
    case PURL::Library:
      e = new Coff::LibraryEditor(url, this);
      static_cast<Coff::LibraryEditor *>(e)->setView(&Main::compileLog());
      break;
    case PURL::Elf:
    case PURL::Project:
    case PURL::PikdevProject:
    case PURL::Nb_FileTypes: break;
  }
  return e;
}

void EditorManager::addEditor(Editor *e)
{
  QValueList<Editor *>::iterator it = _editors.begin();
  for (; it!=_editors.end(); ++it) if ( *it==e ) return;
  _editors.append(e);
  addTab(e, QString::null);
  setTabEnabled(e, true);
  restoreBookmarks(*e);
  showEditor(e);
}

void EditorManager::slotDropEvent(QDropEvent *event)
{
  QStringList urls;
  if ( !Q3UriDrag::decodeLocalFiles(event, urls)) return;
  for(QStringList::const_iterator it = urls.begin(); it != urls.end(); ++it)
    openEditor(PURL::fromPathOrUrl(*it, true));
}

Editor *EditorManager::openEditor(const PURL::Url &url)
{
  Editor *e = findEditor(url);
  if ( e==0 ) {
    e = createEditor(url.fileType(), url);
    if ( e==0 ) return 0;
    if ( !e->open(url) ) {
      closeEditor(e, false);
      return 0;
    }
    addEditor(e);
  } else showEditor(e);
  return e;
}

void EditorManager::saveAllFiles()
{
  QValueList<Editor *>::iterator it = _editors.begin();
  for (; it!=_editors.end(); ++it) {
    if ( !(*it)->isModified() ) continue;
    (*it)->save();
  }
  emit guiChanged();
}

PURL::UrlList EditorManager::files() const
{
  PURL::UrlList names;
  QValueList<Editor *>::const_iterator it = _editors.begin();
  for(; it!=_editors.end(); ++it) {
    if ( (*it)->url().isEmpty() ) continue;
    names.push_back((*it)->url());
  }
  return names;
}

void EditorManager::contextMenu(int i, const QPoint &p)
{
  Editor *editor = static_cast<Editor *>(page(i));
  if ( editor==0 ) return;

  std::auto_ptr<KMenu> popup(new KMenu);
  popup->addTitle(title(*editor));
  QAction* close = popup->addAction(KIcon("document-close"), i18n("Close"));
  QAction* closeAllOthers = nbEditors()>1 ? popup->addAction(KIcon("document-close"), i18n("Close All Others")) : NULL;
  QAction* save = editor->isModified() ? popup->addAction(KIcon("document-save"), i18n("Save")) : NULL;
  QAction* saveAs = popup->addAction(KIcon("document-save-as"), i18n("Save As..."));
  QAction* reload = !editor->url().isEmpty() ? popup->addAction(KIcon("view-refresh"), i18n("Reload")) : NULL;
  QAction* action = popup->exec(p);
  if ( action==close ) closeEditor(editor, true);
  else if ( action==closeAllOthers ) closeAllOtherEditors();
  else if ( action==save ) editor->save();
  else if ( action==saveAs ) editor->saveAs();
  else if ( action==reload ) editor->reload();
  emit guiChanged();
}

void EditorManager::switchHeaderImplementation()
{
  if ( currentEditor()==NULL ) return;
  PURL::Url url = currentEditor()->url();
  PURL::FileType type = url.fileType();
  PURL::SourceFamily source = type.data().sourceFamily;
  if ( type.data().group==PURL::Source ) type = source.data().headerType;
  else {
    ASSERT( type.data().group==PURL::Source );
    type = Main::toolGroup().implementationType(source.data().toolType);
  }
  if ( type==PURL::Nb_FileTypes ) return;
  url = url.toFileType(type);
  if ( !url.exists() ) return;
  openEditor(url);
}

void EditorManager::switchToEditor()
{
  QStringList names;
  for (int i=0; i<_editors.count(); i++) names.append(title(*_editors[i]));
  SwitchToDialog dialog(names, this);
  if ( dialog.exec()!=QDialog::Accepted ) return;
  for (int i=0; i<names.count(); i++) {
    if ( dialog.name()!=names[i] && dialog.name()!=QString("%1").arg(i+1) ) continue;
    showEditor(_editors[i]);
    return;
  }
}

QString EditorManager::name(const Editor &e) const
{
  return (!e.name().isEmpty() ? e.name() : e.url().filepath());
}

void EditorManager::goBack()
{
  ASSERT( _history.hasBack() );
  QString s = _history.goBack();
  for (int i=0; i<_editors.count(); i++)
    if ( s==name(*_editors[i]) ) showEditor(_editors[i]);
}

void EditorManager::goForward()
{
  ASSERT( _history.hasForward() );
  QString s = _history.goForward();
  for (int i=0; i<_editors.count(); i++)
    if ( s==name(*_editors[i]) ) showEditor(_editors[i]);
}

Editor *EditorManager::openEditor(ProjectManager::SpecialEditorType type)
{
  bool created = false;
  Editor *e = NULL;
  switch (type.type()) {
      case ProjectManager::SpecialEditorType::DeviceInfos: {
    e = findEditor(type.key());
    if ( e==NULL ) {
      e = new DeviceChooser::Editor(type.label(), type.key(), this);
      static_cast<DeviceChooser::Editor *>(e)->setDevice(false);
      static_cast<DeviceChooser::Editor *>(e)->setDevice(true); // #### needed to fix GUI glitch ???
      created = true;
    }
    break;
  }
  case ProjectManager::SpecialEditorType::Registers: {
    e = findEditor(type.key());
    if ( e==0 ) {
      BusyCursorStarter bc;
      e = new Register::MainView(type.label(), type.key());
      static_cast<Debugger::GuiManager *>(Debugger::manager)->addRegisterView(*static_cast<Register::MainView *>(e));
      created = true;
    }
    break;
  }
  case ProjectManager::SpecialEditorType::Nb_Types: ASSERT(false); break;
  }
  if ( e==NULL ) return NULL;
  if (created) {
    if ( !e->slotLoad() ) {
      delete e;
      return NULL;
    }
    addEditor(e);
  } else showEditor(e);
  return e;
}
