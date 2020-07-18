/***************************************************************************
 *   Copyright (C) 2005-2009 Nicolas Hadacek <hadacek@kde.org>             *
 *   Copyright (C) 2003 Alain Gibaud <alain.gibaud@free.fr>                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "project_manager_ui.h"
#include "project_manager_ui.moc"

#include "project.h"
#include "devices/list/device_list.h"
#include "main_global.h"
#include "common/gui/purl_gui.h"
#include "device_gui.h"
#include "tools/list/tool_list.h"
#include "progs/list/prog_list.h"

//----------------------------------------------------------------------------
void PopupMenu::insertItem(const QString &icon, const QString &label, QObject *receiver, const char *slot)
{
  QPixmap pixmap = IconLoader::loadIcon(icon, IconLoader::Small);
  KMenu::insertItem(pixmap, label, receiver, slot, 0, _index);
  _index++;
}

//----------------------------------------------------------------------------
const ProjectManager::Group::Data ProjectManager::Group::DATA[Nb_Types] = {
  { I18N_NOOP("Device"),        "media-flash" },
  { I18N_NOOP("Toolchain"),     "system-run" },
  { I18N_NOOP("Programmer"),    "audio-card" },
  { I18N_NOOP("Headers"),       NULL },
  { I18N_NOOP("Linker Script"), NULL },
  { I18N_NOOP("Sources"),       NULL },
  { I18N_NOOP("Objects"),       NULL },
  { I18N_NOOP("Generated"),     NULL },
  { I18N_NOOP("Included"),      NULL }
};

const ProjectManager::SpecialEditorType::Data ProjectManager::SpecialEditorType::DATA[Nb_Types] = {
    { "device",    I18N_NOOP("Device") },
    { "registers", I18N_NOOP("Registers") }
};

//----------------------------------------------------------------------------
ProjectManager::Group ProjectManager::group(PURL::FileType type)
{
  switch (type.data().group) {
    case PURL::Source:        return Group::Source;
    case PURL::Header:        return Group::Header;
    case PURL::LinkerScript:  return Group::LinkerScript;
    case PURL::LinkerObject:  return Group::LinkerObject;
    case PURL::Nb_FileGroups: break;
  }
  return Group::Nb_Types;
}

ProjectManager::RootItem::RootItem(KListView *listview)
  : UrlItem(listview)
{
  setSelectable(false);
  setPixmap(0, PURL::icon(PURL::Project));
  set(PURL::Url());
}

void ProjectManager::RootItem::set(const PURL::Url &url)
{
  _url = url;
  if ( _url.isEmpty() ) setText(0, i18n("<no project>"));
  else setText(0, _url.basename());
}

void ProjectManager::RootItem::paintCell(QPainter *p, const QColorGroup &cg,
                                         int column, int width, int align)
{
    QFont f = p->font();
    f.setItalic(_url.isEmpty());
    p->setFont(f);
    UrlItem::paintCell(p, cg, column, width, align);
}

//----------------------------------------------------------------------------
ProjectManager::HeaderItem::HeaderItem(RootItem *item, Group group)
  : KListViewItem(item), _group(group)
{
  setSelectable(false);
  setText(0, i18n(group.data()._label));
  const char* icon = group.data()._icon;
  if (icon != NULL) {
    QPixmap chip = IconLoader::loadIcon(icon, IconLoader::Small);
    setPixmap(0, chip);
  }
}

Q3ListViewItem *ProjectManager::HeaderItem::lastChild() const
{
  Q3ListViewItem *item = firstChild();
  if (item == NULL) return NULL;
  for (;;) {
    if (item->nextSibling() == NULL) break;
    item = item->nextSibling();
  }
  return item;
}

//----------------------------------------------------------------------------
ProjectManager::FileItem::FileItem(HeaderItem *item, PURL::FileType ftype,
                                   const PURL::Url &url, bool external)
  : UrlItem(item), _ftype(ftype), _external(external)
{
  setPixmap(0, PURL::icon(ftype));
  set(url);
}

void ProjectManager::FileItem::set(const PURL::Url &url)
{
  _url = url;
  QString s = url.filename();
  switch (_ftype.type()) {
    case PURL::Hex:  s = i18n("Hex File") + " (" + s + ")"; break;
    case PURL::Coff: s = i18n("Disassembly Listing"); break;
    case PURL::Lst:  s = i18n("List"); break;
    case PURL::Map:  s = i18n("Memory Map"); break;
    case PURL::Project:
    case PURL::Nb_FileTypes: ASSERT(false); break;
    default:
      if ( _external && group(_ftype)==Group::LinkerScript )
        s += i18n(" (default)");
      break;
  }
  setText(0, s);
}

void ProjectManager::FileItem::paintCell(QPainter *p, const QColorGroup &cg,
                                         int column, int width, int align)
{
  QFont f = p->font();
  f.setItalic(_external);
  p->setFont(f);
  UrlItem::paintCell(p, cg, column, width, align);
}

//----------------------------------------------------------------------------
ProjectManager::DeviceInfoItem::DeviceInfoItem(HeaderItem *item)
    : KListViewItem(item)
{
  setText(0, i18n("Infos"));
}

//----------------------------------------------------------------------------
ProjectManager::RegisterItem::RegisterItem(HeaderItem *item)
  : KListViewItem(item)
{
  setText(0, i18n("Registers"));
}

//----------------------------------------------------------------------------
ProjectManager::DeviceItem::DeviceItem(HeaderItem *item)
: EditListViewItem(item)
{}

QWidget *ProjectManager::DeviceItem::editWidgetFactory(int) const
{
  _combo.reset(new DeviceChooser::ComboBox(NULL));
  _combo->setCurrentText(Main::deviceData().name());
  QObject::connect(_combo.get(), SIGNAL(activated(int)), listView(), SLOT(finishRenaming()));
  return _combo.get();
}

void ProjectManager::DeviceItem::updateText()
{
  setText(0, Main::deviceData().name());
}

//----------------------------------------------------------------------------
ProjectManager::ToolchainItem::ToolchainItem(HeaderItem *item)
: EditListViewItem(item),
  _group(NULL)
{
}

QWidget *ProjectManager::ToolchainItem::editWidgetFactory(int) const
{
  _combo.reset(new ListerComboBox<Tool::Lister>(NULL));
  _combo->setGroup(Main::toolGroup());
  QObject::connect(_combo->widget(), SIGNAL(activated(int)), listView(), SLOT(finishRenaming()));
  return _combo->widget();
}

void ProjectManager::ToolchainItem::editDone(int col, const QWidget* widget)
{
  _group = &_combo->group();
  EditListViewItem::editDone(col, widget);
}

void ProjectManager::ToolchainItem::updateText()
{
  _group = &Main::toolGroup();
  setText(0, Main::toolGroup().label());
}

//----------------------------------------------------------------------------
ProjectManager::ProgrammerItem::ProgrammerItem(HeaderItem *item)
: EditListViewItem(item),
  _group(NULL)
{}

QWidget *ProjectManager::ProgrammerItem::editWidgetFactory(int) const
{
  _combo.reset(new ListerComboBox<Programmer::Lister>(NULL));
  _combo->setGroup(Main::programmerGroup());
  QObject::connect(_combo->widget(), SIGNAL(activated(int)), listView(), SLOT(finishRenaming()));
  return _combo->widget();
}

void ProjectManager::ProgrammerItem::editDone(int col, const QWidget* widget)
{
  _group = &_combo->group();
  EditListViewItem::editDone(col, widget);
}

void ProjectManager::ProgrammerItem::updateText()
{
  _group = &Main::programmerGroup();
  setText(0, Main::programmerGroup().label());
}

//----------------------------------------------------------------------------
ProjectManager::LinkerScriptItem::LinkerScriptItem(HeaderItem *item)
  : UrlItem(item)
{
  init();
}

void ProjectManager::LinkerScriptItem::init()
{
  _url = PURL::Url();
  PURL::Url lkr;
  lkr = Main::project()->customLinkerScript();
  setText(0, lkr.isEmpty() ? i18n("<default>") : lkr.filename());
  setPixmap(0, lkr.isEmpty() ? QPixmap() : PURL::icon(PURL::Lkr));
}

void ProjectManager::LinkerScriptItem::set(const PURL::Url &url)
{
  _url = url;
  QString s = url.filename();
  PURL::Url lkr;
  lkr = Main::project()->customLinkerScript();
  if ( lkr.isEmpty() ) s += i18n(" (default)");
  setText(0, s);
  setPixmap(0, PURL::icon(PURL::Lkr));
}

PURL::Url ProjectManager::LinkerScriptItem::url() const
{
  if ( !_url.isEmpty() ) return _url;
  return Main::project()->customLinkerScript();
}
