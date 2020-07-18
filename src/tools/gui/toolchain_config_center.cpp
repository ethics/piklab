/***************************************************************************
 *   Copyright (C) 2005-2007 Nicolas Hadacek <hadacek@kde.org>             *
 *   Copyright (C) 2004 Alain Gibaud <alain.gibaud@free.fr>                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "toolchain_config_center.h"
#include "toolchain_config_center.moc"

#include "tools/list/tools_config_widget.h"
#include "tools/list/compile_config.h"
#include "tools/list/tool_list.h"
#include "toolchain_config_widget.h"
#include "tool_group_ui.h"

//-----------------------------------------------------------------------------
ToolchainsConfigCenter::ToolchainsConfigCenter(const Tool::Group &sgroup, QWidget *parent)
  : TreeListDialog(parent, "configure_toolchains_dialog", true,
                   i18n("Configure Toolchains"), Ok|User1|User2|User3|Cancel, Cancel, false)
{
  setButtonGuiItem(User1, KStandardGuiItem::reset());
  setButtonGuiItem(User2, KGuiItem(i18n("Update"), "view-refresh"));
  setButtonGuiItem(User3, KGuiItem(i18n("Info"), "help-about"));

  KPageWidgetItem *current = 0;
  FOR_EACH(PURL::SourceFamily, family) {
    KPageWidgetItem* familyWidget = addPage(NULL, family.label());
    for (Tool::Lister::ConstIterator it = Tool::Lister::instance().begin();
         it != Tool::Lister::instance().end();
         ++it) {
      PURL::FileType type = it.data()->implementationType(family.data().toolType);
      if ( type==PURL::Nb_FileTypes || type.data().sourceFamily!=family ) continue;
      if ( family==PURL::SourceFamily::Asm && it.data()->implementationType(PURL::ToolType::Compiler)!=PURL::Nb_FileTypes ) continue;
      QStringList names;
      names += family.label();
      names += it.data()->label();
      ToolchainConfigWidget* tcw = static_cast<const ::Tool::GroupUI *>(it.data()->gui())->toolchainConfigWidgetFactory(NULL);
      _configWidgets.push_back(tcw);
      tcw->init();
      tcw->loadConfig();
      KPageWidgetItem* page = addSubPage(familyWidget, tcw, it.data()->label());
      if ( it.key()==sgroup.name() ) current = page;
    }
  }
  setCurrentPage(current);
  currentPageChangedSlot(current);

  connect(this, SIGNAL(currentPageChanged(KPageWidgetItem*, KPageWidgetItem*)), SLOT(currentPageChangedSlot(KPageWidgetItem*)));
  connect(this, SIGNAL(okClicked()), SLOT(slotOk()));
  connect(this, SIGNAL(applyClicked()), SLOT(slotApply()));
  connect(this, SIGNAL(user1Clicked()), SLOT(slotUser1()));
  connect(this, SIGNAL(user2Clicked()), SLOT(slotUser2()));
  connect(this, SIGNAL(user3Clicked()), SLOT(slotUser3()));
}

void ToolchainsConfigCenter::currentPageChangedSlot(KPageWidgetItem*)
{
  ToolchainConfigWidget* tcw = static_cast<ToolchainConfigWidget*>(currentPage()->widget());
  if (tcw != NULL) {
    QTimer::singleShot(0, tcw, SLOT(detect()));
  }
}

void ToolchainsConfigCenter::slotApply()
{
  for (ConfigWidgetVector::const_iterator it = _configWidgets.begin(); it != _configWidgets.end(); ++it) (*it)->saveConfig();
}

void ToolchainsConfigCenter::slotOk()
{
  slotApply();
  accept();
}

void ToolchainsConfigCenter::slotUser1()
{
  ToolchainConfigWidget* tcw = static_cast<ToolchainConfigWidget*>(currentPage()->widget());
  if (tcw != NULL) {
    tcw->loadConfig();
    tcw->forceDetect();
  }
}

void ToolchainsConfigCenter::slotUser2()
{
  ToolchainConfigWidget* tcw = static_cast<ToolchainConfigWidget*>(currentPage()->widget());
  if (tcw != NULL) {
    tcw->forceDetect();
  }
}

void ToolchainsConfigCenter::slotUser3()
{
  const ToolchainConfigWidget* tcw = static_cast<const ToolchainConfigWidget*>(currentPage()->widget());
  if (tcw != NULL) {
    TextEditorDialog dialog(tcw->group().informationText(), tcw->group().label(), true, this);
    dialog.exec();
  }
}
