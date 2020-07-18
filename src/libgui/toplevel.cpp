/***************************************************************************
 *   Copyright (C) 2005-2012 Nicolas Hadacek <hadacek@kde.org>             *
 *   Copyright (C) 2003-2004 Alain Gibaud <alain.gibaud@free.fr>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "toplevel.h"
#include "toplevel.moc"

#include <qeventloop.h>
#include <qmenubar.h>
#include <QDockWidget>

#include <kactioncollection.h>
#include <krecentfilesaction.h>
#include <kactionmenu.h>
#include <ktoolbar.h>
#include <kstandardguiitem.h>

#include "gui_debug_manager.h"
#include "hex_editor.h"
#include "project_manager.h"
#include "project.h"
#include "global_config.h"
#include "editor.h"
#include "device_gui.h"
#include "text_editor.h"
#include "new_dialogs.h"
#include "common/global/process.h"
#include "common/gui/misc_gui.h"
#include "common/gui/purl_gui.h"
#include "devices/list/device_list.h"
#include "progs/base/prog_config.h"
#include "progs/list/prog_list.h"
#include "progs/gui/prog_group_ui.h"
#include "progs/gui/prog_config_center.h"
#include "editor_manager.h"
#include "tools/list/compile_manager.h"
#include "object_view.h"
#include "config_gen.h"
#include "tools/list/compile_config.h"
#include "watch_view.h"
#include "coff/base/text_coff.h"
#include "tools/list/tool_list.h"
#include "breakpoint_view.h"
#include "main_global.h"
#include "gui_prog_manager.h"
#include "devices/gui/register_view.h"
#include "devices/base/device_group.h"
#include "tools/gui/toolchain_config_center.h"
#include "global_config.h"

#ifndef NO_KDE
#  include "console.h"
#endif

//----------------------------------------------------------------------------
const ExternalToolType::Data ExternalToolType::DATA[Nb_Types] = {
  { "pikloops", I18N_NOOP("&Pikloops..."),   "chronometer",        "http://pikloops.sourceforge.net" },
  { "kfind",    I18N_NOOP("&Find Files..."), "edit-find",          NULL },
  { "khexterm", I18N_NOOP("&KHexTerm..."),   "utilities-terminal", "http://khexterm.sourceforge.net" }
};

//----------------------------------------------------------------------------
MainWindow &MainWindow::self()
{
  static MainWindow *self = NULL;
  if ( self==NULL ) self = new MainWindow;
  return *self;
}

MainWindow::MainWindow()
: _configGenerator(NULL),
  _actionAfterBuild(Programmer::ProgramAction::Nb_Types)
{
  FOR_EACH(ExternalToolType, type) {
    _externalToolProcesses[type.type()] = NULL;
  }

// status bar
    _actionStatus = new QLabel(statusBar());
    statusBar()->addWidget(_actionStatus);
    _actionProgress = new Q3ProgressBar(statusBar());
    statusBar()->addWidget(_actionProgress);
    _debugStatus = new QLabel(statusBar());
    statusBar()->addWidget(_debugStatus, 0, true);
    _vddStatus = new VoltageStatusWidget(i18n("Vdd"), statusBar());
    statusBar()->addWidget(_vddStatus, 0, true);
    _vppStatus = new VoltageStatusWidget(i18n("Vpp"), statusBar());
    statusBar()->addWidget(_vppStatus, 0, true);
    _editorStatus = new QLabel(statusBar());
    statusBar()->addWidget(_editorStatus, 0, true);

// interface
    QDockWidget *dock1 = new QDockWidget(i18n("Project Manager"), this);
    dock1->setObjectName("project_manager_dock");
    Main::_projectManager = new ProjectManager::View(dock1);
    dock1->setWidget(Main::_projectManager);
    addDockWidget(Qt::LeftDockWidgetArea, dock1);
    connect(Main::_projectManager, SIGNAL(guiChanged()), SLOT(updateGUI()));

    QDockWidget* dock2 = new QDockWidget(i18n("Watch View") ,this);
    dock2->setObjectName("watch_view_dock");
    Main::_watchView = new Register::WatchView(dock2);
    dock2->setWidget(Main::_watchView);
    addDockWidget(Qt::LeftDockWidgetArea, dock2);
    tabifyDockWidget(dock1, dock2);
    dock1->raise();

    Main::_editorManager = new EditorManager(this);
    setCentralWidget(Main::_editorManager);
    connect(Main::_editorManager, SIGNAL(guiChanged()), SLOT(updateGUI()));
    connect(Main::_editorManager, SIGNAL(modified(const PURL::Url &)), Main::_projectManager, SLOT(modified(const PURL::Url &)));
    connect(Main::_editorManager, SIGNAL(statusChanged(const QString &)), _editorStatus, SLOT(setText(const QString &)));

    _compileDock = new QDockWidget(i18n("Compile Log"), this);
    _compileDock->setObjectName("compile_log_dock");
    Main::_compileLog = new Compile::LogWidget(_compileDock);
    Main::_compileLog->setFocusPolicy(Qt::NoFocus);
    _compileDock->setWidget(Main::_compileLog);
    addDockWidget(Qt::BottomDockWidgetArea, _compileDock);

    _programDock = new QDockWidget(i18n("Program Log"), this);
    _programDock->setObjectName("program_log_dock");
    _programLog = new Log::Widget(_programDock, "program_log");
    _programLog->setFocusPolicy(Qt::NoFocus);
    _programDock->setWidget(_programLog);
    addDockWidget(Qt::BottomDockWidgetArea, _programDock);

    QDockWidget* breakpointDock = new QDockWidget(i18n("Breakpoints"), this);
    breakpointDock->setObjectName("breakpoints_dock");
    Main::_breakpointsView = new Breakpoint::View(breakpointDock);
    Main::_breakpointsView->setFocusPolicy(Qt::NoFocus);
    breakpointDock->setWidget(Main::_breakpointsView);
    addDockWidget(Qt::BottomDockWidgetArea, breakpointDock);

    QDockWidget* consoleDock = new QDockWidget(i18n("Console"), this);
    consoleDock->setObjectName("console_dock");
    Main::_consoleView = new ConsoleView(consoleDock);
    consoleDock->setWidget(Main::_consoleView);
    addDockWidget(Qt::BottomDockWidgetArea, consoleDock);

    tabifyDockWidget(_compileDock, _programDock);
    tabifyDockWidget(_programDock, breakpointDock);
    tabifyDockWidget(breakpointDock, consoleDock);
    _compileDock->raise();

// managers
    Programmer::manager = new Programmer::GuiManager(this);
    Programmer::manager->setView(_programLog);
    connect(Programmer::manager, SIGNAL(actionMessage(const QString &)), _actionStatus, SLOT(setText(const QString &)));
    connect(Programmer::manager, SIGNAL(showProgress(bool)), SLOT(showProgress(bool)));
    connect(Programmer::manager, SIGNAL(setTotalProgress(uint)), SLOT(setTotalProgress(uint)));
    connect(Programmer::manager, SIGNAL(setProgress(uint)), SLOT(setProgress(uint)));

    Debugger::manager = new Debugger::GuiManager;
    connect(Debugger::manager, SIGNAL(targetStateChanged()), SLOT(updateGUI()));
    connect(Debugger::manager, SIGNAL(statusChanged(const QString &)), _debugStatus, SLOT(setText(const QString &)));
    connect(Debugger::manager, SIGNAL(actionMessage(const QString &)), _actionStatus, SLOT(setText(const QString &)));

    Main::_compileManager = new Compile::Manager(this);
    Main::_compileManager->setView(Main::_compileLog);
    connect(Main::_compileManager, SIGNAL(success()), SLOT(compileSuccess()));
    connect(Main::_compileManager, SIGNAL(failure()), SLOT(compileFailure()));
    connect(Main::_compileManager, SIGNAL(updateFile(const Compile::FileData &)), SLOT(updateFile(const Compile::FileData &)));

// actions
    // file actions
    KAction* a = actionCollection()->addAction(KStandardAction::New, this, SLOT(newSourceFile()));
    a->setText(i18n("&New Source File..."));
    a = actionCollection()->addAction("file_new_hex", this, SLOT(newHexFile()));
    a->setText(i18n("New hex File..."));
    a->setIcon(KIcon("document-new"));
    actionCollection()->addAction(KStandardAction::Open, this, SLOT(openFile()));
    KRecentFilesAction *recent = static_cast<KRecentFilesAction*>(actionCollection()->addAction(KStandardAction::OpenRecent, this, SLOT(openRecentFile(const KUrl&))));
    KConfigGroup configGroup = KGlobal::config()->group("recent-files");
    recent->loadEntries(configGroup);
    a = actionCollection()->addAction("file_save_all", Main::_editorManager, SLOT(saveAllFiles()));
    a->setText(i18n("Save All"));
    a->setIcon(KIcon("document-save"));
    actionCollection()->addAction(KStandardAction::Close, Main::_editorManager, SLOT(closeCurrentEditor()));
    a = actionCollection()->addAction("file_close_all", Main::_editorManager, SLOT(closeAllEditors()));
    a->setText(i18n("C&lose All"));
    a->setIcon(KIcon("window-close"));
    a = actionCollection()->addAction("file_close_other", Main::_editorManager, SLOT(closeAllOtherEditors()));
    a->setText(i18n("Close All Others"));
    a->setIcon(KIcon("window-close"));
    actionCollection()->addAction(KStandardAction::Quit, this, SLOT(close()));

    // window actions
    a = actionCollection()->addAction("window_history_back", Main::_editorManager, SLOT(goBack()));
    KGuiItem guiItem = KStandardGuiItem::back();
    a->setText(guiItem.text());
    a->setIcon(guiItem.icon());
    a = actionCollection()->addAction("window_history_forward",  Main::_editorManager, SLOT(goForward()));
    guiItem = KStandardGuiItem::forward();
    a->setText(guiItem.text());
    a->setIcon(guiItem.icon());
    a = actionCollection()->addAction("window_switch_to", Main::_editorManager, SLOT(switchToEditor()));
    a->setText(i18n("Switch to..."));
    a->setShortcut(Qt::CTRL + Qt::Key_Slash);
    a = actionCollection()->addAction("window_switch_source", Main::_editorManager, SLOT(switchHeaderImplementation()));
    a->setText(i18n("Switch Header/Implementation"));
    a->setShortcut(Qt::SHIFT + Qt::Key_F12);
    a = actionCollection()->addAction("window_show_disassembly_location", Debugger::manager, SLOT(showDisassemblyLocation()));
    a->setText(i18n("Show disassembly location"));

    // project actions
    a = actionCollection()->addAction("project_new", this, SLOT(newProject()));
    a->setText(i18n("New Project..."));
    a->setIcon(KIcon("document-new"));
    a = actionCollection()->addAction("project_open", this , SLOT(openProject()));
    a->setText(i18n("Open Project..."));
    a->setIcon(KIcon("document-open"));
    recent = new KRecentFilesAction(i18n("Open Recent Project"), this);
    actionCollection()->addAction("project_open_recent", recent);
    connect(recent, SIGNAL(urlSelected(const KUrl&)), this, SLOT(openRecentProject(const KUrl&)));
    configGroup = KGlobal::config()->group("recent-projects");
    recent->loadEntries(configGroup);
    recent->setIcon(KIcon("document-open-recent"));
    a = actionCollection()->addAction("project_options", this, SLOT(configureProject()));
    a->setText(i18n("Project Options..."));
    a->setIcon(KIcon("document-properties"));
    a = actionCollection()->addAction("project_close", this, SLOT(closeProject()));
    a->setText(i18n("Close Project"));
    a->setIcon(KIcon("document-close"));
    a = actionCollection()->addAction("project_add_source_file", Main::_projectManager, SLOT(insertSourceFiles()));
    a->setText(i18n("Add Source File..."));
    a->setIcon(KIcon("document-import"));
    a = actionCollection()->addAction("project_add_object_file", Main::_projectManager, SLOT(insertObjectFiles()));
    a->setText(i18n("Add Object File..."));
    a->setIcon(KIcon("document-import"));
    a = actionCollection()->addAction("project_add_current_file", Main::_projectManager, SLOT(insertCurrentFile()));
    a->setText(i18n("Add Current File"));
    a->setIcon(KIcon("document-import"));

    // build actions
    a = actionCollection()->addAction("build_build_project", this, SLOT(buildProject()));
    a->setText(i18n("&Build Project"));
    a->setIcon(KIcon("run-build"));
    a->setShortcut(Qt::Key_F8);
    a = actionCollection()->addAction("build_compile_file", this, SLOT(compileFile()));
    a->setText(i18n("&Compile File"));
    a->setIcon(KIcon("run-build-file"));
    a->setShortcut(Qt::SHIFT + Qt::Key_F8);
    a = actionCollection()->addAction("build_clean", this, SLOT(cleanBuild()));
    a->setText(i18n("Clean"));
    a->setIcon(KIcon("run-build-clean"));
    a = actionCollection()->addAction("build_stop", this, SLOT(stopBuild()));
    a->setText(i18n("Stop"));
    a->setIcon(KIcon("process-stop"));
    a = actionCollection()->addAction("build_disassemble", this, SLOT(disassembleFile()));
    a->setText(i18n("Disassemble"));
    a->setIcon(KIcon("document-edit-decrypt"));

    // programmer actions
    a = actionCollection()->addAction("prog_connect", Programmer::manager, SLOT(connectDevice()));
    a->setText(i18n("&Connect"));
    a->setIcon(KIcon("network-wired-activated"));
    a = actionCollection()->addAction("prog_power", Programmer::manager, SLOT(toggleDevicePower()));
    a->setText(i18n("Device Power"));
    a->setIcon(KIcon("battery-charging"));
    a = actionCollection()->addAction("prog_disconnect", Programmer::manager, SLOT(disconnectDevice()));
    a->setText(i18n("&Disconnect"));
    a->setIcon(KIcon("network-wired"));
    a = actionCollection()->addAction("prog_program", this , SLOT(program()));
    a->setText(i18n("&Program"));
    a->setIcon(KIcon("arrow-down"));
    a->setShortcut(Qt::SHIFT + Qt::Key_F5);
    a = actionCollection()->addAction("prog_verify", this , SLOT(verify()));
    a->setText(i18n("&Verify"));
    a->setIcon(KIcon("dialog-ok-apply"));
    a->setShortcut(Qt::SHIFT + Qt::Key_F6);
    a = actionCollection()->addAction("prog_read", this , SLOT(read()));
    a->setText(i18n("&Read"));
    a->setIcon(KIcon("arrow-up"));
    a->setShortcut(Qt::SHIFT + Qt::Key_F7);
    a = actionCollection()->addAction("prog_erase", this, SLOT(erase()));
    a->setText(i18n("&Erase"));
    a->setIcon(KIcon("draw-eraser"));
    a = actionCollection()->addAction("prog_blank_check", this, SLOT(blankCheck()));
    a->setText(i18n("&Blank Check"));
    a->setIcon(KIcon("dialog-ok"));
    a = actionCollection()->addAction("prog_run", Programmer::manager, SLOT(run()));
    a->setText(i18n("&Run"));
    a->setIcon(KIcon("system-run"));
    a->setShortcut(Qt::SHIFT + Qt::Key_F9);
    a = actionCollection()->addAction("prog_stop", Programmer::manager, SLOT(halt()));
    a->setText(i18n("&Stop"));
    a->setIcon(KIcon("media-playback-stop"));
    a = actionCollection()->addAction("prog_restart", Programmer::manager, SLOT(restart()));
    a->setText(i18n("R&estart"));
    a->setIcon(KIcon("view-refresh"));
    a = actionCollection()->addAction("prog_advanced", Programmer::manager , SLOT(showAdvancedDialog()));
    a->setText(i18n("&Advanced..."));
    a = actionCollection()->addAction("prog_settings", this , SLOT(showProgrammerSettings()));
    a->setText(i18n("Settings..."));
    a->setIcon(KIcon("configure"));

    // debugger actions
    a = actionCollection()->addAction("debug_next", Programmer::manager, SLOT(step()));
    a->setText(i18n("&Start/Step"));
    a->setIcon(KIcon("debug-step-instruction"));
    a = actionCollection()->addAction("debug_run", Programmer::manager, SLOT(run()));
    a->setText(i18n("&Run"));
    a->setIcon(KIcon("debug-run"));
    a->setShortcut(Qt::SHIFT + Qt::Key_F9);
    //(void)new KAction(i18n("Step &In"), "piklab_debug_stepin",
    //             0, this, SLOT(debugStepIn()), actionCollection(), "debug_step_in");
    //(void)new KAction(i18n("Step &Out"), "piklab_debug_stepout",
    //             0, this, SLOT(debugStepOut()), actionCollection(), "debug_step_out");
    a = actionCollection()->addAction("debug_halt", Programmer::manager, SLOT(halt()));
    a->setText(i18nc("it is the verb", "&Halt"));
    a->setIcon(KIcon("media-playback-pause"));
    a = actionCollection()->addAction("debug_stop",  Programmer::manager, SLOT(disconnectDevice()));
    a->setText(i18n("&Disconnect/Stop"));
    a->setIcon(KIcon("media-playback-stop"));
    a = actionCollection()->addAction("debug_reset", Programmer::manager, SLOT(restart()));
    a->setText(i18n("R&eset"));
    a->setIcon(KIcon("view-refresh"));
    a = actionCollection()->addAction("debug_show_pc", Debugger::manager, SLOT(showPC()));
    a->setText(i18n("Show Program Counter"));
    a->setIcon(KIcon("view-process-all"));
    a = actionCollection()->addAction("debug_clear_breakpoints", Debugger::manager, SLOT(clearBreakpoints()));
    a->setText(i18n("Clear All Breakpoints"));
    a->setIcon(KIcon("edit-delete"));
    a = actionCollection()->addAction("debug_settings", this , SLOT(showDebuggerSettings()));
    a->setText(i18n("Settings..."));
    a->setIcon(KIcon("configure"));

    // tools
    FOR_EACH(ExternalToolType, type) {
      QString s = "tools_" + type.key();
      a = actionCollection()->addAction(s.latin1(), this , SLOT(runExternalTool()));
      a->setText(type.label());
      a->setIcon(KIcon(type.data().icon));
    }
    a = actionCollection()->addAction("tools_device_information", this , SLOT(showDeviceInfo()));
    a->setText(i18n("&Device Selector..."));
    a->setIcon(KIcon("media-flash"));
    a = actionCollection()->addAction("tools_config_generator", this , SLOT(configGenerator()));
    a->setText(i18n("&Config Generator..."));
    a = actionCollection()->addAction("tools_template_generator", this , SLOT(templateGenerator()));
    a->setText(i18n("&Template Generator..."));

    // settings actions
    a = actionCollection()->addAction("options_configure_toolchains", this, SLOT(configureToolchains()));
    a->setText(i18n("Configure Toolchains..."));
    a->setIcon(KIcon("system-run"));
    a = actionCollection()->addAction("options_configure_programmers", this, SLOT(configureProgrammers()));
    a->setText(i18n("Configure Programmers..."));
    a->setIcon(KIcon("audio-card"));
    a = actionCollection()->addAction("options_configure_editor", this, SLOT(configureEditor()));
    a->setText(i18n("Configure Editor..."));
    a->setIcon(KIcon("preferences-other"));
    actionCollection()->addAction(KStandardAction::Preferences, this, SLOT(configure()));

    setupGUI(ToolBar|Keys|StatusBar|Create);
    configGroup = KGlobal::config()->group("MainWindow");
    applyMainWindowSettings(configGroup);

    QTimer::singleShot(0, this, SLOT(initialLoading()));
}

MainWindow::~MainWindow()
{}

void MainWindow::initialLoading()
{
  BusyCursorStarter bc;
  KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
  if ( args->count()!=0 ) { // opened urls provided on the command line
    for (int i = 0; i<args->count(); i++) {
      PURL::Url url(args->url(i));
      if ( url.fileType()==PURL::Project ) {
        Main::_projectManager->openProject(url);
      } else Main::_editorManager->openEditor(url);
    }
  } else { // otherwise reopen last project/files
    Main::_projectManager->openProject(GlobalConfig::openedProject());
    PURL::UrlList files = GlobalConfig::openedFiles();
    for (PURL::UrlList::const_iterator it = files.begin(); it != files.end(); ++it)
      Main::_editorManager->openEditor(*it);
  }
  updateGUI();
}

void MainWindow::openRecentFile(const KUrl &kurl)
{
  Main::_editorManager->openFile(PURL::Url(kurl));
}

void MainWindow::configure(ConfigCenter::Type showType)
{
  stopOperations();
  ConfigCenter dialog(showType, this);
  dialog.exec();
  Programmer::manager->clear();
  updateGUI();
  Debugger::manager->update(true);
}

void MainWindow::configureToolchains()
{
  stopOperations();
  ToolchainsConfigCenter dialog(Main::toolGroup(), this);
  dialog.exec();
  Programmer::manager->clear();
  updateGUI();
  Debugger::manager->update(true);
}

void MainWindow::configureProgrammers()
{
  stopOperations();
  Programmer::ConfigCenter dialog(Main::programmerGroup(), this);
  dialog.exec();
  Programmer::manager->clear();
  updateGUI();
  Debugger::manager->update(true);
}

void MainWindow::configureEditor()
{
  TextEditor::configure();
}

void MainWindow::selectProgrammer(const Programmer::Group &group)
{
  if ( group.name()==Main::programmerGroup().name() ) return;
  bool debugInitialized = Debugger::manager->coff();
  stopOperations();
  Compile::Config::setProgrammerGroup(Main::project(), group);
  Programmer::manager->clear();
  updateGUI();
  if (debugInitialized) Debugger::manager->init();
  else Debugger::manager->update(true);
}

void MainWindow::selectTool(const Tool::Group &group)
{
  if ( group.name()==Compile::Config::toolGroup(Main::project()).name() ) return;
  bool debugInitialized = Debugger::manager->coff();
  stopOperations();
  Compile::Config::setToolGroup(Main::project(), group);
  updateGUI();
  if (debugInitialized) Debugger::manager->init();
  else Debugger::manager->update(true);
}

void MainWindow::setDevice(const Device::Data& data)
{
  Compile::Config::setDevice(Main::project(), data);
  updateGUI();
}

void MainWindow::showDeviceInfo()
{
  DeviceChooser::Dialog d(Main::deviceData().name(), this);
  if ( d.exec() ) {
    const Device::Data* data = Device::Lister::instance().data(d.device());
    if (data != NULL) {
      setDevice(*data);
      updateGUI();
    }
  }
}

void MainWindow::configGenerator()
{
  PURL::FileType ftype = (Main::currentEditor() != NULL ? Main::currentEditor()->fileType() : PURL::Nb_FileTypes);
  PURL::SourceFamily family = (ftype!=PURL::Nb_FileTypes ? ftype.data().sourceFamily : PURL::SourceFamily(PURL::SourceFamily::Nb_Types));
  PURL::ToolType type = (family!=PURL::SourceFamily::Nb_Types ? family.data().toolType : PURL::ToolType(PURL::ToolType::Assembler));
  ConfigGenerator dialog(this);
  dialog.set(&Main::deviceData(), Main::toolGroup(), type);
  dialog.exec();
}

void MainWindow::templateGenerator()
{
  PURL::FileType ftype = (Main::currentEditor() != NULL ? Main::currentEditor()->fileType() : PURL::Nb_FileTypes);
  PURL::SourceFamily family = (ftype!=PURL::Nb_FileTypes ? ftype.data().sourceFamily : PURL::SourceFamily(PURL::SourceFamily::Nb_Types));
  PURL::ToolType type = (family!=PURL::SourceFamily::Nb_Types ? family.data().toolType : PURL::ToolType(PURL::ToolType::Assembler));
  TemplateGenerator dialog(this);
  dialog.set(&Main::deviceData(), Main::toolGroup(), type);
  dialog.exec();
}

bool MainWindow::queryClose()
{
  if ( !stopOperations() ) return false;
  Main::setState(Main::Closing);
  // save list of opened editors
  PURL::UrlList toSave;
  const PURL::UrlList files = Main::_editorManager->files();
  for (PURL::UrlList::const_iterator it=files.begin(); it!=files.end(); ++it)
    if ( !Main::_projectManager->isExternalFile(*it) ) toSave.append(*it);
  GlobalConfig::writeOpenedFiles(toSave);
  // close editors
  if ( !Main::_editorManager->closeAllEditors() ) {
    Main::setState(Main::Idle);
    return false;
  }

  // save other settings
  BusyCursorStarter bc;
  KConfigGroup configGroup = KGlobal::config()->group("recent-projects");
  static_cast<KRecentFilesAction *>(Main::action("project_open_recent"))->saveEntries(configGroup);
  configGroup = KGlobal::config()->group("recent-files");
  static_cast<KRecentFilesAction *>(Main::action("file_open_recent"))->saveEntries(configGroup);
  GlobalConfig::writeOpenedProject(Main::_projectManager->projectUrl());
  Main::_projectManager->saveProject();

  configGroup = KGlobal::config()->group("MainWindow");
  saveMainWindowSettings(configGroup);
  return true;
}

void MainWindow::newSourceFile()
{
  NewFileDialog dialog(Main::project(), this);
  if ( dialog.exec()!=QDialog::Accepted ) return;
  if ( !dialog.url().exists() && !dialog.url().create(*Main::_compileLog) ) return;
  Main::_editorManager->openEditor(dialog.url());
  if ( dialog.addToProject() ) Main::_projectManager->insertFile(dialog.url());
}

void MainWindow::newHexFile()
{
  QString s;
  for (uint i=0; true; i++) {
    s = i18n("Hex") + (i==0 ? QString::null : QString::number(i));
    if (Main::_editorManager->findEditor(s) == NULL) break;
  }
  HexEditor *editor = new HexEditor(s, Main::_editorManager);
  editor->memoryRead();
  Main::_editorManager->addEditor(editor);
}

bool MainWindow::openFile()
{
  QString filter;
  filter += PURL::sourceFilter(PURL::SimpleFilter);
  filter += "\n" + PURL::filter(PURL::Hex);
  filter += "\n" + PURL::projectFilter(PURL::SimpleFilter);
  filter += "\n*|" + i18n("All Files");
  PURL::Url url = PURL::getOpenUrl("kfiledialog:///open_file", filter, this ,i18n("Open File"));
  if ( url.fileType()==PURL::Project || url.fileType()==PURL::PikdevProject ) {
    return openProject(url);
  }
  return Main::_editorManager->openFile(url);
}

bool MainWindow::openProject(const PURL::Url& url)
{
  stopOperations();
  if ( !Main::_projectManager->openProject(url) ) return false;
  updateGUI();
  return true;
}
void MainWindow::updateGUI()
{
  static bool updating = false;
  if (updating) return;
  updating = true;

  const bool idle = ( Main::_state==Main::Idle );
  switch (Main::_state) {
  case Main::Closing: return;
  case Main::Idle:
    showProgress(false);
    break;
  case Main::Compiling:
    _actionStatus->setText(Main::_compileManager->label());
    showProgress(true);
    _compileDock->raise();
    break;
  case Main::Programming:
    _programDock->raise();
    break;
  }

  // update editor actions
  Main::_editorManager->updateTitles();
  Main::action("file_save_all")->setEnabled(Main::currentEditor());
  Main::action("file_close")->setEnabled(Main::currentEditor());
  Main::action("file_close_all")->setEnabled(Main::currentEditor());
  Main::action("file_close_other")->setEnabled(Main::_editorManager->nbEditors()>1);
  Main::action("options_configure")->setEnabled(idle);
  const PURL::FileType currentType = (Main::currentEditor() != NULL ? Main::currentEditor()->fileType() : PURL::Nb_FileTypes);
  const bool isSource = (currentType==PURL::Nb_FileTypes ? false : currentType.data().group==PURL::Source);
  const bool isHeader = (currentType==PURL::Nb_FileTypes ? false : currentType.data().group==PURL::Header);
  Main::action("window_switch_source")->setEnabled(isSource || isHeader);
  Main::action("window_history_back")->setEnabled(Main::editorManager().history().hasBack());
  Main::action("window_history_forward")->setEnabled(Main::editorManager().history().hasForward());
  Main::action("window_show_disassembly_location")->setEnabled(Debugger::manager->coff()!=0 && (isSource || isHeader));


  // update project
  const bool hasProject = (Main::project() != NULL);
  const bool idleProject = (idle && hasProject);
  const bool inProject = (Main::currentEditor() != NULL
                          && (currentType==PURL::Nb_FileTypes
                              || Main::currentEditor()->url().isEmpty()
                              || Main::_projectManager->contains(Main::currentEditor()->url())));
  if (Main::currentEditor() != NULL) Main::_projectManager->select(Main::currentEditor());

  // update project actions
  Main::action("project_new")->setEnabled(idle);
  Main::action("project_open")->setEnabled(idle);
  Main::action("project_close")->setEnabled(idleProject);
  Main::action("project_options")->setEnabled(idleProject);
  Main::action("project_add_source_file")->setEnabled(idleProject);
  Main::action("project_add_object_file")->setEnabled(idleProject);
  Main::action("project_add_current_file")->setEnabled(idle && inProject && isSource);

  // update build actions
  const bool hexProject = ( Main::_projectManager->projectUrl().fileType()==PURL::Hex );
  const bool customTool = (hasProject ? Main::toolGroup().isCustom() : false);
  Main::action("build_build_project")->setEnabled(!hexProject && idleProject);
  const PURL::Url selected = Main::_projectManager->selectedUrl();
  const bool isSelectedSource = ( !selected.isEmpty() && selected.data().group==PURL::Source );
  Main::action("build_compile_file")->setEnabled(!hexProject && (isSource || isSelectedSource) && idleProject && !customTool);
  Main::action("build_clean")->setEnabled(idleProject && !customTool);
  Main::action("build_stop")->setEnabled(Main::_state==Main::Compiling);
  Main::action("build_disassemble")->setEnabled(idle && currentType==PURL::Hex);

  // update programmer status
  const bool isProgrammer = bool(Main::programmerGroup().properties() & Programmer::Programmer);
  const PURL::Url& purl = Main::_projectManager->projectUrl();
  const bool hasHex = ( currentType==PURL::Hex || Main::_projectManager->contains(purl.toFileType(PURL::Hex)) );
  const bool hasConnected = ( isProgrammer && Main::programmerGroup().properties() & Programmer::HasConnectedState );
  Main::action("prog_connect")->setEnabled(idle && hasConnected);
  Main::action("prog_read")->setEnabled(idle && isProgrammer);
  Main::action("prog_program")->setEnabled(idle && isProgrammer && hasHex);
  Main::action("prog_verify")->setEnabled(idle && isProgrammer && hasHex);
  Main::action("prog_erase")->setEnabled(idle && isProgrammer);
  Main::action("prog_blank_check")->setEnabled(idle && isProgrammer);
  Programmer::State pstate = Programmer::NotConnected;
  if (Main::programmer() != NULL && !Main::programmer()->hasError() ) pstate = Main::programmer()->state();
  Main::action("prog_power")->setEnabled(idle && hasConnected && pstate!=Programmer::NotConnected);
  Main::action("prog_disconnect")->setEnabled(idle && hasConnected && pstate!=Programmer::NotConnected);
  const bool isDebugger = (hasProject ? bool(Main::programmerGroup().properties() & ::Programmer::Debugger) : false);
  const bool resetAvailable = (Main::programmerGroup().properties() & Programmer::CanReleaseReset);
  Main::action("prog_run")->setEnabled(idle && !isDebugger && resetAvailable && pstate!=Programmer::Running);
  Main::action("prog_stop")->setEnabled(idle && !isDebugger && pstate==Programmer::Running);
  Main::action("prog_restart")->setEnabled(idle && !isDebugger && pstate==Programmer::Running);
  const Programmer::GroupUI *pgui = static_cast<const Programmer::GroupUI *>(Main::programmerGroup().gui());
  Main::action("prog_advanced")->setEnabled(idle && pgui->hasAdvancedDialog());
  Device::VoltageValue vdd, vpp;
  if (Main::programmer() != NULL && !(Main::programmerGroup().properties() & Programmer::Custom)) {
    vdd = static_cast<Programmer::Base *>(Main::programmer())->voltage(Device::VoltageType::TargetVdd);
    vpp = static_cast<Programmer::Base *>(Main::programmer())->voltage(Device::VoltageType::TargetVpp);
  }
  _vddStatus->setVoltage(vdd);
  _vppStatus->setVoltage(vpp);

  // update debugger status
  Debugger::manager->updateDevice();
  const bool isStepping = ( pstate==Programmer::Halted && Debugger::manager->isStepping());
  const bool stoppedDebugger = (isDebugger && pstate!=Programmer::Running && !isStepping);
  Main::action("debug_run")->setEnabled(idleProject && stoppedDebugger);
  Main::action("debug_next")->setEnabled(idleProject && stoppedDebugger);
  Main::action("debug_halt")->setEnabled(idleProject && isDebugger && (pstate==Programmer::Running || isStepping));
  Main::action("debug_stop")->setEnabled(idleProject && isDebugger && (pstate==Programmer::Running || pstate==Programmer::Halted));
  Main::action("debug_reset")->setEnabled(idleProject && isDebugger && (pstate==Programmer::Running || pstate==Programmer::Halted));
  Main::action("debug_show_pc")->setEnabled(idleProject && isDebugger && Debugger::manager->coff()!=NULL && Debugger::manager->pc().isInitialized() && pstate==Programmer::Halted && !isStepping);

  Main::_projectManager->updateGUI();

  // caption
  QString caption = (hasProject ? Main::project()->name() : "<no project>");
  if (Main::currentEditor() != NULL) {
    caption += " - ";
    caption += Main::currentEditor()->url().filepath();
  }
  setCaption(caption + " - Piklab");

  emit deviceChanged();

  updating = false;
}

void MainWindow::runExternalTool()
{
  FOR_EACH(ExternalToolType, type) {
    QString name = "tools_" + type.key();
    if (sender() == actionCollection()->action(name.latin1())) {
      runExternalTool(type);
      break;
    }
  }
}

void MainWindow::runKfind()
{
  runExternalTool(ExternalToolType::KFind);
}

void MainWindow::runExternalTool(ExternalToolType type)
{
  if (_externalToolProcesses[type.type()] != NULL) return;
  ::Process::StringOutput* process = new ::Process::StringOutput;
  _externalToolProcesses[type.type()] = process;
  QStringList args;
  if (type == ExternalToolType::KFind) {
    QString path;
    PURL::Url url = Main::projectManager().projectUrl();
    if ( !url.isEmpty() ) path = url.path();
    args.append(path);
  }
  process->setup(type.key(), args, false);
  connect(process, SIGNAL(done()), SLOT(externalToolDone()));
  connect(process, SIGNAL(error()), SLOT(externalToolError()));
  process->start();
}

void MainWindow::externalToolDone()
{
  FOR_EACH(ExternalToolType, type) {
    if (sender() == _externalToolProcesses[type.type()]) {
      _externalToolProcesses[type.type()]->deleteLater();
      _externalToolProcesses[type.type()] = NULL;
      break;
    }
  }
}

void MainWindow::externalToolError()
{
  FOR_EACH(ExternalToolType, type) {
    if (sender() == _externalToolProcesses[type.type()]) {
      if (type.data().url != NULL) {
        MessageBox::detailedSorry(i18n("Could not run \"%1\"").arg(type.key()), type.data().url, Log::Show);
      } else {
        MessageBox::sorry(i18n("Could not run \"%1\"").arg(type.key()), Log::Show);
      }
      _externalToolProcesses[type.type()]->deleteLater();
      _externalToolProcesses[type.type()] = NULL;
      break;
    }
  }
}

//-----------------------------------------------------------------------------
void MainWindow::compileFile()
{
  Editor* editor = Main::currentEditor();
  if (editor != NULL && editor->isModified()) editor->save(); // buffer is systematically saved
  stopOperations();
  Main::_compileLog->clear();
  PURL::Url url = (editor != NULL ? editor->url() : Main::_projectManager->selectedUrl());
  bool generated = (editor != NULL ? Main::_projectManager->isExternalFile(url) : false);
  if ( !generated ) Main::_projectManager->removeExternalFiles();
  Compile::Item item;
  item.url = (!generated ? url : Main::_projectManager->projectUrl());
  item.type = (generated ? Compile::Item::Generated : Compile::Item::NotGenerated);
  if ( Main::_compileManager->compileFile(item) ) Main::setState(Main::Compiling);
  else compileFailure();
}

void MainWindow::buildProject(Programmer::ProgramAction action)
{
  _actionAfterBuild = action;
  // save modified buffers
  PURL::UrlList files = Main::project()->absoluteFiles();
  for (PURL::UrlList::const_iterator it = files.begin(); it != files.end(); ++it) {
    // save modified editors
    Editor* editor = Main::_editorManager->findEditor(*it);
    if (editor != NULL && editor->isModified()) editor->save();
  }
  Programmer::ProgramAction tmp = _actionAfterBuild;
  stopOperations();
  _actionAfterBuild = tmp;
  Main::_compileLog->clear();
  Main::_projectManager->removeExternalFiles();
  if ( Main::_compileManager->buildProject() ) Main::setState(Main::Compiling);
  else compileFailure();
}

void MainWindow::compileFailure()
{
  _actionAfterBuild = Programmer::ProgramAction::Nb_Types;
  Main::setState(Main::Idle);
}

void MainWindow::doAfterBuild()
{
  if ( _actionAfterBuild==Programmer::ProgramAction::Nb_Types && !readConfigEntry(BaseGlobalConfig::ProgramAfterBuild).toBool() ) return;
  bool allowRun = ( _actionAfterBuild==Programmer::ProgramAction::Nb_Types && _actionAfterBuild==Programmer::ProgramAction::ProgramOnly );
  if ( !program(allowRun) ) return;
  switch (_actionAfterBuild.type()) {
    case Programmer::ProgramAction::Run: Programmer::manager->run(); break;
    case Programmer::ProgramAction::Step: Programmer::manager->run(); break;
    case Programmer::ProgramAction::ProgramOnly:
    case Programmer::ProgramAction::Halt:
    case Programmer::ProgramAction::Nb_Types: break;
  }
}

void MainWindow::initDebugger()
{
  if ( Debugger::manager->init() ) {
    const QStringList &included = Debugger::manager->coff()->filenames();
    for (QStringList::const_iterator it=included.begin(); it!=included.end(); ++it) {
      PURL::Directory dir = Main::project()->directory();
      PURL::Url url = PURL::fromPathOrUrl(*it, true).toAbsolute(dir);
      if ( !url.exists() ) continue;
      Main::_projectManager->addExternalFile(url, ProjectManager::Included);
    }
  }
}

void MainWindow::compileSuccess()
{
  if ( !Main::_compileManager->compileOnly() ) {
    Main::_projectManager->setModified(false);
    if ( Main::project()->outputType()==Tool::OutputType::Executable ) {
      if ( readConfigEntry(BaseGlobalConfig::ParseDisassemblyListingAfterBuild).toBool() ) {
        initDebugger();
      }
      doAfterBuild();
    }
  }
  _actionAfterBuild = Programmer::ProgramAction::Nb_Types;
  Main::setState(Main::Idle);
}

void MainWindow::updateFile(const Compile::FileData &fdata)
{
  if ( fdata.actions & Compile::InProject ) {
    if ( fdata.actions & Compile::Generated ) Main::_projectManager->addExternalFile(fdata.url, ProjectManager::Generated);
    else if ( fdata.actions & Compile::Included ) Main::_projectManager->addExternalFile(fdata.url, ProjectManager::Included);
    else ASSERT(false);
  }
  if ( fdata.actions & Compile::Show ) {
    Editor *editor = Main::_editorManager->openEditor(fdata.url);
    if (editor != NULL) editor->setReadOnly(true);
  }
}

void MainWindow::cleanBuild()
{
  stopOperations();
  Main::_compileLog->clear();
  Main::_compileManager->cleanProject();
  Main::_projectManager->removeExternalFiles();
}

void MainWindow::stopBuild()
{
  Main::_compileManager->kill();
}

void MainWindow::disassembleFile()
{
  const HexEditor *he = ::qobject_cast<const HexEditor*>(Main::currentEditor());
  if (he == NULL) return;
  stopOperations();
  Main::_compileLog->clear();
  _compileDock->raise();
  const Device::Data& data = Main::deviceData();
  std::auto_ptr<Editor> editor(new DisassemblyEditor(*he, data, this));
  if (editor->open(PURL::Url()) ) {
    Main::editorManager().addEditor(editor.release());
  }
}

//-----------------------------------------------------------------------------
void MainWindow::keyPressEvent(QKeyEvent *e)
{
  if ( e->key()==Qt::Key_Escape ) stopOperations();
}

bool MainWindow::stopOperations()
{
  if ( Main::_state==Main::Programming ) {
    _programLog->log(Log::LineType::Warning, i18n("Programming in progress. Cannot be aborted."));
    return false;
  }
  stopBuild();
  Programmer::manager->stop();
  Debugger::manager->clear();
  return true;
}

void MainWindow::newProject()
{
  stopOperations();
  Main::_projectManager->newProject();
  updateGUI();
  Main::_compileLog->clear();
}

void MainWindow::openProject()
{
  stopOperations();
  Main::_projectManager->openProject();
  updateGUI();
  Main::_compileLog->clear();
}

void MainWindow::openRecentProject(const KUrl &url)
{
  stopOperations();
  Main::_projectManager->openProject(PURL::Url(url));
  updateGUI();
  Main::_compileLog->clear();
}

void MainWindow::configureProject()
{
  stopOperations();
  Main::_projectManager->configureProject(ProjectEditor::Main);
  updateGUI();
}

void MainWindow::configureProjectToolchain()
{
  stopOperations();
  Main::_projectManager->configureProject(ProjectEditor::Toolchain);
  updateGUI();
}

void MainWindow::closeProject()
{
  stopOperations();
  Main::_projectManager->closeProject();
  updateGUI();
  Main::_compileLog->clear();
}

//----------------------------------------------------------------------------
HexEditor *MainWindow::getHexEditor()
{
  if (Main::project() != NULL) {
    if ( Main::_projectManager->isModified() || Main::_projectManager->needsRecompile() ) {
      MessageBox::Result res = MessageBox::Yes;
      if ( !readConfigEntry(BaseGlobalConfig::AutoRebuildModified).toBool() ) {
	KGuiItem item1(i18n("Recompile First"));
	KGuiItem item2(i18n("Continue Anyway"));
        res = MessageBox::questionYesNoCancel(i18n("The project hex file may not be up-to-date since some project files have been modified."), item1, item2);
        if ( res==MessageBox::Cancel ) return NULL;
      }
      if ( res==MessageBox::Yes ) {
        buildProject(Programmer::ProgramAction::ProgramOnly);
        return NULL;
      }
    }
  }
  HexEditor* editor = ::qobject_cast<HexEditor*>(Main::currentEditor());
  if (editor != NULL) return editor;
  PURL::Url purl = Main::_projectManager->projectUrl();
  editor = static_cast<HexEditor *>(Main::_editorManager->openEditor(purl.toFileType(PURL::Hex)));
  if (editor != NULL) editor->setReadOnly(true);
  return editor;
}

void MainWindow::erase()
{
  Programmer::manager->erase(Device::MemoryRange());
}

void MainWindow::blankCheck()
{
  Programmer::manager->blankCheck(Device::MemoryRange());
}

bool MainWindow::program(bool allowRun)
{
  const HexEditor *editor = getHexEditor();
  if (editor == NULL) return false;
  if ( Main::programmerGroup().isDebugger() && !Main::_projectManager->contains(editor->url()) ) {
    MessageBox::sorry(i18n("It is not possible to start a debugging session with an hex file not generated with the current project."), Log::Show);
    return false;
  }
  return Programmer::manager->program(*editor->memory(), Device::MemoryRange(), allowRun);
}

void MainWindow::verify()
{
  const HexEditor *editor = getHexEditor();
  if (editor == NULL) return;
  Programmer::manager->verify(*editor->memory(), Device::MemoryRange());
}

void MainWindow::read()
{
  QString s = i18n("Read");
  Editor *e = Main::_editorManager->findEditor(s);
  if (e != NULL) Main::_editorManager->closeEditor(e, true);
  std::auto_ptr<HexEditor> editor(new HexEditor(s, Main::_editorManager));
  editor->setDevice();
  if ( Programmer::manager->read(*editor->memory(), Device::MemoryRange()) ) {
    editor->memoryRead();
    Main::_editorManager->addEditor(editor.release());
  };
}

void MainWindow::showProgress(bool show)
{
  if (show) {
    _busyCursor.reset(new BusyCursorStarter);
    _actionStatus->show();
    _actionProgress->show();
  } else {
    _busyCursor.reset(NULL);
    _actionStatus->hide();
    _actionProgress->hide();
  }
}

void MainWindow::setTotalProgress(uint nb)
{
  _actionProgress->setTotalSteps(nb);
  _actionProgress->setProgress(0);
  QApplication::processEvents(QEventLoop::ExcludeUserInput); // #### DANGER !!!!
}

void MainWindow::setProgress(uint nb)
{
  _actionProgress->setProgress(nb);
  QApplication::processEvents(QEventLoop::ExcludeUserInput); // #### DANGER !!!!
}
