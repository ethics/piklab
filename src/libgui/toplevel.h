/***************************************************************************
 *   Copyright (C) 2005-2007 Nicolas Hadacek <hadacek@kde.org>             *
 *   Copyright (C) 2003-2004 Alain Gibaud <alain.gibaud@free.fr>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef TOPLEVEL_H
#define TOPLEVEL_H

#include "config_center.h"
#include "tools/list/compile_process.h"
#include "toplevel_ui.h"
#include "progs/manager/prog_manager.h"
namespace Programmer { class Group; }
namespace Tool { class Group; }
class ConfigGenerator;
class HexEditor;
namespace Process { class Base; }

struct ExternalToolTypeData
{
  const char* key;
  const char* label;
  const char* icon;
  const char* url;
};
BEGIN_DECLARE_ENUM(ExternalToolType)
  Pikloops = 0, KFind, KHexTerm
END_DECLARE_ENUM(ExternalToolType, ExternalToolTypeData)

class MainWindow : public KXmlGuiWindow
{
Q_OBJECT
public:
  static MainWindow &self();
  virtual ~MainWindow();
  void initDebugger();

public slots:
  void newSourceFile();
  void newProject();
  void openProject();
  void buildProject(Programmer::ProgramAction action = Programmer::ProgramAction::Nb_Types);
  void cleanBuild();
  void configure(ConfigCenter::Type showType = ConfigCenter::General);
  void configureProject();
  void configureProjectToolchain();
  void showDeviceInfo();
  void configGenerator();
  void templateGenerator();
  void updateGUI();

  void setDevice(const Device::Data& data);
  void selectTool(const Tool::Group &group);
  void selectProgrammer(const Programmer::Group &group);

  bool openProject(const PURL::Url& url);
  void closeProject();

private slots:
  void newHexFile();
  bool openFile();
  void openRecentFile(const KUrl &url);

  void compileFile();
  void compileSuccess();
  void compileFailure();
  void updateFile(const Compile::FileData &data);
  void stopBuild();
  void disassembleFile();

  void erase();
  void blankCheck();
  bool program(bool allowRun = true);
  void verify();
  void read();
  void showProgress(bool show);
  void setTotalProgress(uint nbSteps);
  void setProgress(uint nbSteps);

  void configureToolchains();
  void configureProgrammers();
  void configureEditor();
  void showProgrammerSettings() { configure(ConfigCenter::ProgOptions); }
  void showDebuggerSettings() { configure(ConfigCenter::DebugOptions); }

  void runExternalTool();
  void externalToolDone();
  void externalToolError();
  void runKfind();

  void openRecentProject(const KUrl &url);

  void initialLoading();

signals:
  void deviceChanged();

private:
  QDockWidget            *_compileDock;
  QDockWidget            *_programDock;
  Log::Widget            *_programLog;
  QLabel                 *_actionStatus, *_debugStatus, *_editorStatus;
  VoltageStatusWidget    *_vddStatus, *_vppStatus;
  Q3ProgressBar          *_actionProgress;
  ConfigGenerator        *_configGenerator;
  ::Process::Base*        _externalToolProcesses[ExternalToolType::Nb_Types];
  Programmer::ProgramAction _actionAfterBuild;
  std::auto_ptr<BusyCursorStarter> _busyCursor;

  struct DockPos {
    DockPos() {}
    DockPos(const QString &pparent)
      : parent(pparent), pos(K3DockWidget::DockCenter), space(0) {}
    DockPos(K3DockWidget::DockPosition ppos, uint pspace)
      : parent("main_dock_widget"), pos(ppos), space(pspace) {}
    QString                   parent;
    K3DockWidget::DockPosition pos;
    uint                      space;
  };
  struct DockData {
    ViewMenuAction *action;
    K3DockWidget    *dock;
    QString         title;
    DockPos         position;
  };
  QValueList<DockData> _docks;
  K3DockWidget         *_mainDock;

  MainWindow();
  HexEditor *getHexEditor();
  virtual bool queryClose();
  bool stopOperations();
  void cleanBuild(bool singleFile);
  virtual void keyPressEvent(QKeyEvent *e);
  void doAfterBuild();
  void runExternalTool(ExternalToolType);
};

#endif
