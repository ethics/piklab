/***************************************************************************
 *   Copyright (C) 2005-2009 Nicolas Hadacek <hadacek@kde.org>             *
 *   Copyright (C) 2003 Alain Gibaud <alain.gibaud@free.fr>                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef PROJECT_MANAGER_H
#define PROJECT_MANAGER_H

#include <memory>

#include "common/global/purl.h"
#include "common/gui/misc_gui.h"
#include "common/gui/list_view.h"
#include "project_manager_ui.h"
#include "project_editor.h"
class Project;
class Editor;
namespace Coff { class SectionParser; }

namespace ProjectManager
{

class View : public ListView
{
Q_OBJECT
public:
  View(QWidget *parent);
  virtual ~View();

  bool configureProject(ProjectEditor::PageType type);
  bool newProject();
  bool openProject();
  bool openProject(const PURL::Url &url);
  void saveProject();
  void closeProject();
  Project* project() const { return _project.get(); }

  PURL::Url projectUrl() const { return _rootItem->url(); }
  PURL::Url linkerScriptUrl() const;
  PURL::Url selectedUrl() const;
  void removeFile(const PURL::Url &url);
  void select(const Editor *e);
  void insertFile(const PURL::Url &url);
  bool contains(const PURL::Url &url) const { return findFileItem(url); }
  void addExternalFile(const PURL::Url &url, FileOrigin fileOrigin);
  bool isExternalFile(const PURL::Url &url) const;
  void removeExternalFiles();

  bool isModified() const { return _modified; }
  void setModified(bool modified) { _modified = modified; }
  bool needsRecompile() const;

public slots:
  void insertSourceFiles();
  void insertObjectFiles();
  void insertCurrentFile();
  void updateGUI();

private slots:
  void init();
  void contextMenu(Q3ListViewItem *item, const QPoint &pos, int column);
  void clicked(int button, Q3ListViewItem *item, const QPoint &pos, int column);
  void renamed(Q3ListViewItem *item, int column, const QString &text);
  void modified(const PURL::Url &url);
  void filesReordered();

signals:
  void guiChanged();

private:
  std::auto_ptr<Project> _project;
  RootItem*              _rootItem;
  DeviceItem*            _deviceItem;
  ProgrammerItem*        _programmerItem;
  ToolchainItem*         _toolchainItem;
  LinkerScriptItem*      _linkerScriptItem;
  class ProjectData {
  public:
    PURL::FileType type;
    QMap<PURL::Url, FileOrigin> externals;
  };
  ProjectData            _projectData;
  bool                   _modified;

  HeaderItem *findHeaderItem(Group group) const;
  HeaderItem *headerItem(Group group);
  FileItem *findFileItem(const PURL::Url &url) const;
  FileItem *findFileItem(PURL::FileType type) const;
  Q3ListViewItem *findItem(const QString &name) const;
  virtual Q3DragObject *dragObject();
  virtual bool acceptDrag(QDropEvent* e) const;
  virtual QString tooltip(Q3ListViewItem *item, int col) const;
  void addExternalFiles();
  void rightClicked(Q3ListViewItem *item, const QPoint &pos);
  void leftClicked(Q3ListViewItem *item);
  void addFile(const PURL::Url &url, PURL::FileType type, FileOrigin origin);
  void initListView();
  ProjectData &projectData() { return _projectData; }
  const ProjectData &projectData() const { return _projectData; }
};

} // namespace

#endif
