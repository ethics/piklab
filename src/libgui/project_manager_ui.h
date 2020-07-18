/***************************************************************************
 *   Copyright (C) 2005-2009 Nicolas Hadacek <hadacek@kde.org>             *
 *   Copyright (C) 2003 Alain Gibaud <alain.gibaud@free.fr>                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef PROJECT_MANAGER_UI_H
#define PROJECT_MANAGER_UI_H

#include <memory>

#include "common/global/purl.h"
#include "common/gui/misc_gui.h"
#include "common/gui/list_view.h"
#include "common/gui/key_gui.h"
#include "tools/list/tool_list.h"
#include "progs/list/prog_list.h"

//-----------------------------------------------------------------------------
class PopupMenu : public KMenu
{
Q_OBJECT
public:
  PopupMenu() : _index(1) {}
  void insertItem(const QString &icon, const QString &label, QObject *receiver = 0, const char *slot = 0);

private:
  uint _index;
};

//-----------------------------------------------------------------------------
namespace ProjectManager
{
  enum Rtti { RootRtti = 1000, HeaderRtti, FileRtti, DeviceRtti, RegisterRtti,
              DeviceInfoRtti, ToolchainRtti, ProgrammerRtti, LinkerScriptRtti };
  struct GroupData {
    const char* _label;
    const char* _icon;
  };
  BEGIN_DECLARE_ENUM(Group)
    Device, Toolchain, Programmer, Header, LinkerScript, Source,
    LinkerObject, Generated, Included
  END_DECLARE_ENUM_DATA_ONLY(Group, GroupData)

  Group group(PURL::FileType type);
  enum FileOrigin { Intrinsic, Generated, Included };

  BEGIN_DECLARE_ENUM(SpecialEditorType)
    DeviceInfos, Registers
  END_DECLARE_ENUM_KEY_LABEL(SpecialEditorType)

class UrlItem : public KListViewItem
{
public:
  UrlItem(KListView *listview) : KListViewItem(listview) {}
  UrlItem(KListViewItem *item) : KListViewItem(item) {}
  virtual PURL::Url url() const { return _url; }

protected:
  PURL::Url _url;
};

//-----------------------------------------------------------------------------
class RootItem : public UrlItem
{
public:
  RootItem(KListView *listview);
  void set(const PURL::Url &url);
  virtual int rtti() const { return RootRtti; }

protected:
  virtual void paintCell(QPainter *p, const QColorGroup &cg, int column, int width, int align);
};

//-----------------------------------------------------------------------------
class HeaderItem : public KListViewItem
{
public:
  HeaderItem(RootItem *item, Group group);
  virtual int rtti() const { return HeaderRtti; }
  Group group() const { return _group; }
  Q3ListViewItem *lastChild() const;

private:
  Group _group;
};

//-----------------------------------------------------------------------------
class FileItem : public UrlItem
{
public:
  FileItem(HeaderItem *item, PURL::FileType type, const PURL::Url &url, bool external);
  virtual int rtti() const { return FileRtti; }
  PURL::FileType ftype() const { return _ftype; }

private:
  PURL::FileType _ftype;
  bool           _external;

  void set(const PURL::Url &url);
  virtual void paintCell(QPainter *p, const QColorGroup &cg, int column, int width, int align);
};

//-----------------------------------------------------------------------------
class DeviceInfoItem : public KListViewItem
{
public:
  DeviceInfoItem(HeaderItem *item);
  virtual int rtti() const { return DeviceInfoRtti; }
};

//-----------------------------------------------------------------------------
class RegisterItem : public KListViewItem
{
public:
  RegisterItem(HeaderItem *item);
  virtual int rtti() const { return RegisterRtti; }
};

//-----------------------------------------------------------------------------
class DeviceItem : public EditListViewItem
{
public:
  DeviceItem(HeaderItem *item);
  void updateText();
  virtual int rtti() const { return DeviceRtti; }

private:
  mutable std::auto_ptr<QComboBox> _combo;

  virtual QWidget *editWidgetFactory(int) const;
  virtual bool alwaysAcceptEdit(int) const { return true; }
};

//-----------------------------------------------------------------------------
class ToolchainItem : public EditListViewItem
{
public:
  ToolchainItem(HeaderItem *item);
  void updateText();
  const Tool::Group* group() const { return _group; }
  virtual int rtti() const { return ToolchainRtti; }

private:
  const Tool::Group* _group;
  mutable std::auto_ptr<ListerComboBox<Tool::Lister> > _combo;

  virtual QWidget *editWidgetFactory(int) const;
  virtual bool alwaysAcceptEdit(int) const { return true; }
  virtual void editDone(int col, const QWidget *editWidget);
};

//-----------------------------------------------------------------------------
class ProgrammerItem : public EditListViewItem
{
public:
  ProgrammerItem(HeaderItem *item);
  void updateText();
  const Programmer::Group* group() const { return _group; }
  virtual int rtti() const { return ProgrammerRtti; }

private:
  const Programmer::Group* _group;
  mutable std::auto_ptr<ListerComboBox<Programmer::Lister> > _combo;

  virtual QWidget *editWidgetFactory(int) const;
  virtual bool alwaysAcceptEdit(int) const { return true; }
  virtual void editDone(int col, const QWidget *editWidget);
};

//-----------------------------------------------------------------------------
class LinkerScriptItem : public UrlItem
{
public:
  LinkerScriptItem(HeaderItem *item);
  void set(const PURL::Url &url);
  virtual PURL::Url url() const;
  void init();
  virtual int rtti() const { return LinkerScriptRtti; }
};

} // namespace

#endif
