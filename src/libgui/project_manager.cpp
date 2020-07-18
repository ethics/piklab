/***************************************************************************
 *   Copyright (C) 2005-2009 Nicolas Hadacek <hadacek@kde.org>             *
 *   Copyright (C) 2003 Alain Gibaud <alain.gibaud@free.fr>                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "project_manager.h"
#include "project_manager.moc"

#include <krecentfilesaction.h>

#include "project.h"
#include "project_wizard.h"
#include "project_editor.h"
#include "toplevel.h"
#include "editor_manager.h"
#include "object_view.h"
#include "devices/list/device_list.h"
#include "tools/list/compile_config.h"
#include "register_view.h"
#include "hex_editor.h"
#include "main_global.h"
#include "common/gui/purl_gui.h"
#include "device_gui.h"
#include "progs/manager/debug_manager.h"
#include "tools/list/tool_list.h"
#include "progs/list/prog_list.h"

//----------------------------------------------------------------------------
ProjectManager::View::View(QWidget *parent)
: ListView(parent, "project_manager"),
  _modified(false)
{
  connect(this, SIGNAL(mouseButtonClicked(int, Q3ListViewItem *, const QPoint &, int)),
          SLOT(clicked(int, Q3ListViewItem *, const QPoint &, int)));
  connect(this, SIGNAL(contextMenuRequested(Q3ListViewItem *, const QPoint &, int)),
          SLOT(contextMenu(Q3ListViewItem *, const QPoint &, int)));
  connect(this, SIGNAL(itemRenamed(Q3ListViewItem *, int, const QString &)),
          SLOT(renamed(Q3ListViewItem *, int, const QString &)));
  connect(this, SIGNAL(moved()), SLOT(filesReordered()));

  header()->hide();
  setSorting(-1);
  addColumn(QString::null, 170);
  setFullWidth(true);
  setRootIsDecorated(false);
  setAcceptDrops(true);
  setDragEnabled(true);
  setDropVisualizer(true);
  QTimer::singleShot(0, this, SLOT(init()));;
}

ProjectManager::View::~View()
{
}

void ProjectManager::View::init()
{
  clear();

  _rootItem  = new RootItem(this);
  setCurrentItem(_rootItem);
  KListViewItem *item = new RegisterItem(headerItem(Group::Device));
  ensureItemVisible(item);
  item = new DeviceInfoItem(headerItem(Group::Device));
  ensureItemVisible(item);
  _deviceItem = new DeviceItem(headerItem(Group::Device));
  ensureItemVisible(_deviceItem);
  _programmerItem = new ProgrammerItem(headerItem(Group::Programmer));
  ensureItemVisible(_programmerItem);

  _linkerScriptItem = NULL;
  if (_project.get() != NULL) {
    _toolchainItem = new ToolchainItem(headerItem(Group::Toolchain));
    ensureItemVisible(_toolchainItem);
    (void)headerItem(Group::Source);
  } else {
    _toolchainItem = NULL;
  }
}

ProjectManager::HeaderItem *ProjectManager::View::findHeaderItem(Group group) const
{
  Q3ListViewItemIterator it(const_cast<View *>(this));
  for(; it.current(); ++it) {
    if ( it.current()->rtti()!=HeaderRtti ) continue;
    HeaderItem *hi = static_cast<HeaderItem *>(it.current());
    if ( hi->group()==group ) return hi;
  }
  return NULL;
}

ProjectManager::HeaderItem *ProjectManager::View::headerItem(Group group)
{
  HeaderItem *item = findHeaderItem(group);
  if (item != NULL) return item;
  item = new HeaderItem(_rootItem, group);
  // reorder groups...
  HeaderItem *items[Group::Nb_Types];
  FOR_EACH(Group, group) {
    HeaderItem* item = findHeaderItem(group);
    items[group.type()] = item;
    if (item != NULL) _rootItem->takeItem(item);
  }
  for (int i=Group::Nb_Types-1; i>=0; i--) {
    if (items[i] != NULL) _rootItem->insertItem(items[i]);
  }
  return item;
}

ProjectManager::FileItem* ProjectManager::View::findFileItem(const PURL::Url &url) const
{
  Q3ListViewItemIterator it(const_cast<View *>(this));
  for(; it.current(); ++it) {
    if ( it.current()->rtti()!=FileRtti ) continue;
    FileItem *fi = static_cast<FileItem *>(it.current());
    if ( fi->url()==url ) return fi;
  }
  return NULL;
}

ProjectManager::FileItem *ProjectManager::View::findFileItem(PURL::FileType type) const
{
  Q3ListViewItemIterator it(const_cast<View *>(this));
  for(; it.current(); ++it) {
    if ( it.current()->rtti()!=FileRtti ) continue;
    FileItem *fi = static_cast<FileItem *>(it.current());
    if ( fi->ftype()==type ) return fi;
  }
  return NULL;
}

Q3ListViewItem *ProjectManager::View::findItem(const QString &tag) const
{
  for(Q3ListViewItemIterator it(const_cast<View *>(this)); it.current() != NULL; ++it) {
    Rtti rtti = static_cast<Rtti>(it.current()->rtti());
    if (rtti == RegisterRtti
        && tag == SpecialEditorType(SpecialEditorType::Registers).key()) return it.current();
    if (rtti == DeviceInfoRtti
        && tag == SpecialEditorType(SpecialEditorType::DeviceInfos).key()) return it.current();
  }
  return NULL;
}

void ProjectManager::View::select(const Editor *e)
{
  Q3ListViewItem *item = NULL;
  if ( e->url().isEmpty() ) item = findItem(e->tag());
  else item = findFileItem(e->url());
  if (item) setSelected(item, true);
  else clearSelection();
}

void ProjectManager::View::contextMenu(Q3ListViewItem *item, const QPoint &p, int)
{
  if ( item==NULL ) return;

  PopupMenu pop;
  Group group;
  if ( item->rtti()==HeaderRtti ) group = static_cast<const HeaderItem *>(item)->group();

  if ( item->rtti()==LinkerScriptRtti || group==Group::LinkerScript ) {
    if ( Main::toolGroup().linkerScriptType()==PURL::Nb_FileTypes ) return;
    pop.addTitle(i18n("Linker Script"));
    QAction* addAction = pop.addAction(KIcon("document-import"), i18n("Set Custom..."));
    QAction* removeAction = !_project->customLinkerScript().isEmpty()
      ? pop.addAction(KIcon("edit-delete"), i18n("Set Default")) : NULL;
    QAction* action = pop.exec(p);
    if ( action==addAction ) {
      PURL::Url url = PURL::getOpenUrl("kfiledilog:///custom_linker_script", PURL::filter(Main::toolGroup().linkerScriptType()), this, i18n("Select Linker Script"));
      if ( !url.isEmpty() ) {
        _project->setCustomLinkerScript(url);
        _linkerScriptItem->init();
      }
    } else if ( removeAction!=NULL && action==removeAction ) {
      _project->setCustomLinkerScript(PURL::Url());
      _linkerScriptItem->init();
    }
  } else if ( item->rtti()==RootRtti ) {
    if (_project.get() != NULL) {
      pop.addTitle(i18n("Project"));
      pop.insertItem("document-properties", i18n("Options..."), &MainWindow::self(), SLOT(configureProject()));
      pop.insertItem("edit-find", i18n("Find Files..."), &MainWindow::self(), SLOT(runKfind()));
      pop.insertSeparator();
      pop.insertItem("run-build", i18n("Build Project"), &MainWindow::self(), SLOT(buildProject()));
      pop.insertItem("run-build-clean", i18n("Clean Project"), &MainWindow::self(), SLOT(cleanBuild()));
      pop.insertSeparator();
      pop.insertItem("document-new", i18n("New Source File..."), &MainWindow::self(), SLOT(newSourceFile()));
      pop.insertItem("document-import", i18n("Add Source Files..."), this, SLOT(insertSourceFiles()));
      pop.insertItem("document-import", i18n("Add Object Files..."), this, SLOT(insertObjectFiles()));
      if (Main::currentEditor() != NULL) pop.insertItem("document-import", i18n("Add Current File"), this, SLOT(insertCurrentFile()));
      pop.exec(p);
    }
  } else if ( item->rtti()==FileRtti ) {
    FileItem *fi = static_cast<FileItem *>(item);
    if ( isExternalFile(fi->url()) ) return;
    pop.addTitle(item->text(0));
    pop.insertItem("edit-delete", i18n("Remove From Project"));
    if ( pop.exec(p)!=NULL ) removeFile(fi->url());
  } else if ( item->rtti()==HeaderRtti ) {
    switch (group.type()) {
      case Group::LinkerObject:
        pop.addTitle(i18n("Objects"));
        pop.insertItem("document-import", i18n("Add Object Files..."), this, SLOT(insertObjectFiles()));
        pop.exec(p);
        break;
      case Group::Source:
      case Group::Header:
        pop.addTitle(i18n("Sources"));
        pop.insertItem("document-new", i18n("New File..."), &MainWindow::self(), SLOT(newSourceFile()));
        pop.insertItem("document-import", i18n("Add Source Files..."), this, SLOT(insertSourceFiles()));
        pop.exec(p);
        break;
      case Group::Device:
        pop.addTitle(i18n("Device"));
        pop.insertItem("media-flash", i18n("Select..."), &MainWindow::self(), SLOT(showDeviceInfo()));
        pop.exec(p);
        break;
      case Group::Toolchain:
        pop.addTitle(i18n("Toolchain"));
        pop.insertItem("document-properties", i18n("Options..."), &MainWindow::self(), SLOT(configureProjectToolchain()));
        pop.insertItem("system-run", i18n("Configure Toolchains..."), &MainWindow::self(), SLOT(configureToolchains()));
        pop.exec(p);
        break;
      case Group::Programmer:
        pop.addTitle(i18n("Programmer"));
        pop.insertItem("audio-card", i18n("Configure Programmers..."), &MainWindow::self(), SLOT(configureProgrammers()));
        pop.exec(p);
        break;
      case Group::LinkerScript:
      case Group::Generated:
      case Group::Included:
      case Group::Nb_Types:
        break;
    }
  }
}

void ProjectManager::View::saveProject()
{
  if (_project.get() == NULL) return;
  PURL::UrlList opened;
  PURL::UrlList files = Main::editorManager().files();
  for (PURL::UrlList::const_iterator it = files.begin(); it!=files.end(); ++it) {
    if ( !isExternalFile(*it) ) opened += *it;
  }
  _project->setOpenedFiles(opened);
  _project->setWatchedRegisters(Register::list().watched());
  QString error;
  if ( !_project->save(error) ) {
    MessageBox::detailedSorry(i18n("Could not save project file \"%1\".").arg(_project->url().pretty()), error, Log::Show);
  }
}

void ProjectManager::View::closeProject()
{
  if (_project.get() == NULL) return;
  saveProject();
  Main::editorManager().closeAllEditors();
  _project.reset(NULL);
  init();
}

void ProjectManager::View::addExternalFiles()
{
  const QMap<PURL::Url, FileOrigin> &ext = projectData().externals;
  for (QMap<PURL::Url, FileOrigin>::const_iterator it=ext.begin(); it!=ext.end(); ++it) {
    if ( !it.key().exists() ) continue;
    addFile(it.key(), it.key().fileType(), it.data());
  }
}

void ProjectManager::View::insertSourceFiles()
{
  PURL::UrlList list = PURL::getOpenUrls("kfiledialog:///sources", PURL::sourceFilter(PURL::CompleteFilter), this, i18n("Select Source File"));
  if ( list.isEmpty() ) return;
  for (PURL::UrlList::const_iterator it=list.begin(); it!=list.end(); ++it) insertFile(*it);
}

void ProjectManager::View::insertObjectFiles()
{
  PURL::UrlList list = PURL::getOpenUrls("kfiledialog:///objects", PURL::objectFilter(PURL::CompleteFilter), this, i18n("Select Object File"));
  if ( list.isEmpty() ) return;
  for (PURL::UrlList::const_iterator it=list.begin(); it!=list.end(); ++it) insertFile(*it);
}

void ProjectManager::View::insertFile(const PURL::Url &url)
{
  if ( !url.exists() ) {
    MessageBox::detailedSorry(i18n("Could not find file."), i18n("File: %1").arg(url.pretty()), Log::Show);
    return;
  }
  PURL::Url purl = url;
  MessageBox::Result copy = MessageBox::No;
  if ( !url.isInto(_project->directory()) ) {
    KGuiItem copyAdd(i18n("Copy and Add"));
    KGuiItem addOnly(i18n("Add only"));
    copy = MessageBox::questionYesNoCancel(i18n("File \"%1\" is not inside the project directory. Do you want to copy the file to your project directory?").arg(url.pretty()), copyAdd, addOnly);
    if ( copy==MessageBox::Cancel ) return;
    if ( copy==MessageBox::Yes ) purl = PURL::Url(_project->directory(), url.filename());
  }
  if ( _project->absoluteFiles().contains(purl) ) {
    MessageBox::detailedSorry(i18n("File is already in the project."), i18n("File: %1").arg(purl.pretty()), Log::Show);
    return;
  }
  if ( copy==MessageBox::Yes ) {
    Log::StringView sview;
    if ( !url.copyTo(purl, sview) ) {
      MessageBox::detailedSorry(i18n("Copying file to project directory failed."), i18n("File: %1\n").arg(url.pretty()) + sview.string(), Log::Show);
      return;
    }
  }
  _project->addFile(purl);
  addFile(purl, purl.fileType(), Intrinsic);
}

bool ProjectManager::View::openProject()
{
  PURL::Url url = PURL::getOpenUrl("kfiledialog:///open_project", PURL::projectFilter(PURL::CompleteFilter), this, i18n("Select Project file"));
  return openProject(url);
}

void ProjectManager::View::addExternalFile(const PURL::Url &url, FileOrigin origin)
{
  ASSERT( origin!=Intrinsic );
  addFile(url, url.fileType(), origin);
}

void ProjectManager::View::addFile(const PURL::Url &url, PURL::FileType type, FileOrigin origin)
{
  if ( contains(url) ) return;
  QMap<PURL::Url, FileOrigin> &ext = projectData().externals;
  if ( type.data().group==PURL::LinkerScript && _linkerScriptItem ) {
    _linkerScriptItem->set(url);
    ext[url] = Included;
    return;
  }
  Group grp;
  switch (origin) {
    case Intrinsic: grp = group(type); break;
    case Generated: grp = Group::Generated; break;
    case Included:  grp = Group::Included; break;
  }
  if (grp == Group::Nb_Types) grp = Group::Generated;
  HeaderItem *hitem = headerItem(grp);
  Q3ListViewItem *item = new FileItem(hitem, type, url, origin!=Intrinsic);
  item->moveItem(hitem->lastChild());
  ensureItemVisible(item);
  if ( origin!=Intrinsic ) ext[url] = origin;
  if ( origin==Intrinsic ) _modified = true;
}

void ProjectManager::View::removeExternalFiles()
{
  QMap<PURL::Url, FileOrigin> &ext = projectData().externals;
  QMap<PURL::Url, FileOrigin>::const_iterator it;
  for (it=ext.begin(); it!=ext.end(); ++it) {
    Main::editorManager().closeEditor(it.key());
    removeFile(it.key());
  }
  ext.clear();
  if (_linkerScriptItem) _linkerScriptItem->init();
}

void ProjectManager::View::removeFile(const PURL::Url &url)
{
  if ( !isExternalFile(url) ) _project->removeFile(url);
  FileItem *item = findFileItem(url);
  if ( item==NULL ) return;
  HeaderItem *group = static_cast<HeaderItem *>(item->parent());
  delete item;
  item = NULL;
  if ( group->childCount()==0 ) delete group;
  _modified = true;
  emit guiChanged();
}

void ProjectManager::View::clicked(int button, Q3ListViewItem *item, const QPoint &, int)
{
  if ( item==NULL ) return;
  if ( button!=Qt::LeftButton ) return;
  Rtti rtti = Rtti(item->rtti());
  Editor *e = NULL;
  BusyCursorStarter bc;
  switch (rtti) {
    case RootRtti:
      MainWindow::self().configureProject();
      break;
    case HeaderRtti: break;
    case DeviceInfoRtti:
      e = Main::editorManager().openEditor(SpecialEditorType::DeviceInfos);
      break;
    case RegisterRtti:
      e = Main::editorManager().openEditor(SpecialEditorType::Registers);
      break;
    case ToolchainRtti:
    case ProgrammerRtti:
    case DeviceRtti: return;
    case LinkerScriptRtti: {
      PURL::Url url = Main::projectManager().linkerScriptUrl();
      if ( url.isEmpty() ) break;
      e = Main::editorManager().findEditor(url);
      if ( e==NULL ) {
        e = Main::editorManager().createEditor(url.fileType(), url);
        if ( !e->open(url) ) {
          delete e;
          e = NULL;
          break;
        }
        if ( e != NULL && isExternalFile(url) ) e->setReadOnly(true);
        Main::editorManager().addEditor(e);
      } else Main::editorManager().showEditor(e);
      break;
    }
    case FileRtti: {
      FileItem *fi = static_cast<FileItem *>(item);
      if ( !(fi->ftype().data().properties & PURL::Editable) ) break;
      e = Main::editorManager().findEditor(fi->url());
      if ( e==NULL ) {
        if ( fi->ftype()==PURL::Coff
             && !Debugger::manager->isInited() ) {
           MainWindow::self().initDebugger();
        }
        e = Main::editorManager().createEditor(fi->url().fileType(), fi->url());
        if ( e==NULL ) break;
        if ( !e->open(fi->url()) ) {
          delete e;
          e = NULL;
          break;
        }
        if ( isExternalFile(fi->url()) ) e->setReadOnly(true);
        Main::editorManager().addEditor(e);
      } else Main::editorManager().showEditor(e);
      break;
    }
  }
  cancelRenaming();
  emit guiChanged();
}

void ProjectManager::View::insertCurrentFile()
{
  insertFile(Main::editorManager().currentEditor()->url());
}

bool ProjectManager::View::configureProject(ProjectEditor::PageType type)
{
  if (_project.get() != NULL) {
    ProjectEditor dialog(*_project, type, this);
    if ( dialog.exec()!=QDialog::Accepted ) return false;
    _modified = true;
    if (_linkerScriptItem != NULL) _linkerScriptItem->init();
  }
  return true;
}

bool ProjectManager::View::newProject()
{
  ProjectWizard wizard(this);
  if ( wizard.exec()==QDialog::Rejected ) return false;
  closeProject();
  QString error;
  if ( !wizard.project()->save(error) ) {
    MessageBox::detailedSorry(i18n("Failed to create new project file"), error, Log::Show);
    return false;
  }
  openProject(wizard.url());
  return true;
}

bool ProjectManager::View::openProject(const PURL::Url& url)
{
  if (url.isEmpty()) return false;
  static_cast<KRecentFilesAction *>(Main::action("project_open_recent"))->removeUrl(url.kurl());
  bool reload = (_project.get() != NULL && _project->url()==url);
  KGuiItem reloadItem(i18n("Reload"));
  if (reload
      && !MessageBox::askContinue(i18n("Project already loaded. Reload?"), reloadItem)) {
    static_cast<KRecentFilesAction *>(Main::action("project_open_recent"))->addUrl(url.kurl());
    return false;
  }
  Project *p = new Project(url);
  QString error;
  if (!p->load(error)) {
    MessageBox::detailedSorry(i18n("Could not open project file."), error, Log::Show);
    delete p;
    p = NULL;
    return false;
  }

  if (_project.get() != NULL && !reload) saveProject();
  Main::editorManager().closeAllEditors();
  _project.reset(p);
  init();

  _rootItem->set(p->url());
  if ( Main::toolGroup().linkerScriptType()!=PURL::Nb_FileTypes ) {
      _linkerScriptItem = new LinkerScriptItem(headerItem(Group::LinkerScript));
      ensureItemVisible(_linkerScriptItem);
  }
  PURL::UrlList files = _project->absoluteFiles();
  for(PURL::UrlList::const_iterator it=files.begin(); it!=files.end(); ++it)
      addFile(*it, (*it).fileType(), Intrinsic);
  _projectData.type = PURL::Project;
  _projectData.externals.clear();
  static_cast<KRecentFilesAction *>(Main::action("project_open_recent"))->addUrl(url.kurl());
  files = _project->openedFiles();
  for(PURL::UrlList::const_iterator it = files.begin(); it!=files.end(); ++it)
      Main::editorManager().openFile(*it);
  Register::list().init();
  QValueList<Register::TypeData> watched = _project->watchedRegisters();
  for (QValueList<Register::TypeData>::const_iterator wit=watched.begin(); wit!=watched.end(); ++wit)
      Register::list().setWatched(*wit, true);
  _modified = false;
  return true;
}

bool ProjectManager::View::isExternalFile(const PURL::Url &url) const
{
  if ( projectUrl().isEmpty() ) return false;
  return projectData().externals.contains(url);
}

void ProjectManager::View::modified(const PURL::Url &url)
{
  FileItem *item = findFileItem(url);
  if ( item != NULL && !isExternalFile(url) ) _modified = true;
}

void ProjectManager::View::renamed(Q3ListViewItem *item, int, const QString &text)
{
  switch (item->rtti()) {
    case DeviceRtti: {
      const Device::Data* data = Device::Lister::instance().data(text);
      if (data != NULL && data != &Main::deviceData()) {
        MainWindow::self().setDevice(*data);
        _modified = true;
      }
      break;
    }
    case ToolchainRtti: {
      const Tool::Group *group = _toolchainItem->group();
      if (group != NULL
          && group != &Main::toolGroup()) {
        MainWindow::self().selectTool(*group);
        _modified = true;
      }
      break;
    }
    case ProgrammerRtti: {
      const Programmer::Group* group = _programmerItem->group();
      if (group != NULL
          && group != &Main::programmerGroup()) {
        MainWindow::self().selectProgrammer(*group);
        _modified = true;
      }
      break;
    }
    default:
      ASSERT(false);
      break;
  }
}

void ProjectManager::View::updateGUI()
{
  _deviceItem->updateText();
  _programmerItem->updateText();
  if (_toolchainItem != NULL) _toolchainItem->updateText();
}

Q3DragObject* ProjectManager::View::dragObject()
{
  if ( currentItem()==NULL || currentItem()->rtti()!=FileRtti ) return NULL;
  const FileItem *item = static_cast<const FileItem *>(currentItem());
  const HeaderItem *hitem = static_cast<const HeaderItem *>(item->parent());
  if ( hitem->group()!=Group::Source ) return NULL;
  Q3StrList uris;
  uris.append(Q3UriDrag::localFileToUri(item->url().filepath()));
  return new Q3UriDrag(uris, viewport());
}

bool ProjectManager::View::acceptDrag(QDropEvent* e) const
{
  if ( e->source()!=viewport() ) return false;
  const Q3ListViewItem *item = itemAt(e->pos());
  if ( item==NULL || item->rtti()!=FileRtti ) return false;
  const HeaderItem *hitem = static_cast<const HeaderItem *>(item->parent());
  return ( hitem->group()==Group::Source );
}

void ProjectManager::View::filesReordered()
{
  _project->clearFiles();
  Q3ListViewItem *item = headerItem(Group::Source)->firstChild();
  for (;item; item=item->nextSibling())
    _project->addFile(static_cast<FileItem *>(item)->url());
}

QString ProjectManager::View::tooltip(Q3ListViewItem *item, int) const
{
  switch (Rtti(item->rtti())) {
    case RootRtti:
    case FileRtti:
    case LinkerScriptRtti: return static_cast<const UrlItem *>(item)->url().filepath();
    case ToolchainRtti:
    case ProgrammerRtti:
    case DeviceRtti:
    case DeviceInfoRtti:
    case RegisterRtti:
    case HeaderRtti: break;
  }
  return QString::null;
}

PURL::Url ProjectManager::View::linkerScriptUrl() const
{
  Q3ListViewItemIterator it(const_cast<View *>(this));
  for(; it.current(); ++it) {
    if ( it.current()->rtti()!=LinkerScriptRtti ) continue;
    return static_cast<LinkerScriptItem *>(it.current())->url();
  }
  return PURL::Url();
}

bool ProjectManager::View::needsRecompile() const
{
  // ### this could be perfected...
  PURL::Url output = projectUrl().toFileType(PURL::Hex);
  QDateTime outputLastModified;
  if ( !output.exists(&outputLastModified) ) return true;
  PURL::UrlList files = _project->absoluteFiles();
  for (PURL::UrlList::const_iterator it=files.begin(); it!=files.end(); ++it) {
    QDateTime lastModified;
    if ( !(*it).exists(&lastModified) ) continue;
    if ( lastModified>outputLastModified ) return true;
  }
  return false;
}

PURL::Url ProjectManager::View::selectedUrl() const
{
  Q3ListViewItem *item = currentItem();
  if ( item==0 ) return PURL::Url();
  Rtti rtti = Rtti(item->rtti());
  if ( rtti!=FileRtti ) return PURL::Url();
  return static_cast<FileItem *>(item)->url();
}
