/***************************************************************************
 *   Copyright (C) 2005-2006 Nicolas Hadacek <hadacek@kde.org>             *
 *   Copyright (C) 2004 Alain Gibaud <alain.gibaud@free.fr>                *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "tools_config_widget.h"
#include "tools_config_widget.moc"

#include "tool_list.h"
#include "tools/gui/tool_config_widget.h"
#include "tools/gui/tool_group_ui.h"
#include "tools/custom/custom_process.h"
#include "compile_config.h"
#include "compile_process.h"
#include "libgui/project.h"

//----------------------------------------------------------------------------
HelpDialog::HelpDialog(QWidget *parent, const CustomArgument::ArgList &args)
  : Dialog(parent, "help_dialog", false, i18n("Help Dialog"), Close, Close, false)
{
  QGridLayout *top = new QGridLayout(mainWidget(), 1, 1, 10, 10);
  uint k = 0;
  for (CustomArgument::ArgList::const_iterator it = args.begin();
       it != args.end();
       ++it) {
    const CustomArgument::Data &data = *it;
    QLabel *label = new QLabel(data.key, mainWidget());
    top->addWidget(label, k, 0);
    label = new QLabel(i18n(data.label), mainWidget());
    top->addWidget(label, k, 1);
    ++k;
  }
  top->setColStretch(2, 1);
  top->setRowStretch(k, 1);
}

//----------------------------------------------------------------------------
ToolsConfigWidget::ToolsConfigWidget(Project& project, QWidget *parent)
: ::ConfigWidget(parent), _project(project), _helpDialog(0)
{
  uint row = 0;

  QLabel *label = new QLabel(i18n("Toolchain:"), this);
  addWidget(label, row,row, 0,0);
  _tool.reset(new ListerComboBox<Tool::Lister>(this));
  connect(_tool->widget(), SIGNAL(activated(int)), SLOT(toolChanged()));
  addWidget(_tool->widget(), row,row, 1,1);
  label = new QLabel(i18n("Output type:"), this);
  addWidget(label, row,row, 2,2);
  _output.reset(new KeyComboBox<Tool::OutputType>(this));
  FOR_EACH(Tool::OutputType, type) _output->appendItem(type, type.label());
  addWidget(_output->widget(), row,row, 3,3);
  row++;

  _mainStack = new QStackedWidget(this);
  addWidget(_mainStack, row,row, 0,4);
  row++;
  _tabWidget = new QTabWidget(_mainStack);
  _mainStack->addWidget(_tabWidget);
  FOR_EACH(Tool::Category, category) {
    _stacks[category] = new KeyWidgetStack<QString>(_tabWidget);
    _stacks[category]->widget()->setMargin(10);
    for (Tool::Lister::ConstIterator it = Tool::Lister::instance().begin();
         it != Tool::Lister::instance().end();
         ++it) {
      if ( it.data()->isCustom() ) continue;
      if ( !it.data()->needs(category) ) continue;
      ToolConfigWidget *cw = static_cast<const Tool::GroupUI *>(it.data()->gui())->createConfigWidget(category, project);
      ASSERT(cw);
      _stacks[category]->appendItem(it.key(), cw);
      connect(cw, SIGNAL(displayHelp(Tool::Category)), SLOT(displayHelp(Tool::Category)));
    }
  }
  _customWidget = new QWidget(_mainStack);
  _mainStack->addWidget(_customWidget);
  QVBoxLayout *vbox = new QVBoxLayout(_customWidget);
  QHBoxLayout *hbox = new QHBoxLayout(vbox);
  label = new QLabel(i18n("Custom shell commands:"), _customWidget);
  hbox->addWidget(label, 1);
  KPushButton *button = new KPushButton(KGuiItem(QString::null, "help-about"), _customWidget);
  connect(button, SIGNAL(clicked()), SLOT(customDisplayHelp()));
  hbox->addWidget(button);
  _commandsEditor = new EditListBox(1, _customWidget, "command_editor", EditListBox::DuplicatesAllowed);
  vbox->addWidget(_commandsEditor);

  setColStretch(4, 1);
}

ToolsConfigWidget::~ToolsConfigWidget()
{
  for (StackMap::iterator it = _stacks.begin(); it != _stacks.end(); ++it) {
    delete it->second;
    it->second = NULL;
  }
}

void ToolsConfigWidget::toolChanged()
{
  const Tool::Group& group = _tool->group();
  bool canMakeLibrary = group.needs(Tool::Category::Librarian);
  _output->widget()->setEnabled(canMakeLibrary);
  if ( !canMakeLibrary ) _output->setCurrentItem(Tool::OutputType::Executable);
  QString name = group.name();
  if (name == Tool::Group::CUSTOM_NAME) _mainStack->setCurrentWidget(_customWidget);
  else {
    _mainStack->setCurrentWidget(_tabWidget);
    FOR_EACH(Tool::Category, k) {
      _tabWidget->removePage(_stacks[k]->widget());
      _stacks[k]->widget()->hide();
      if ( _stacks[k]->contains(name) ) {
        _stacks[k]->setCurrentItem(name);
        _stacks[k]->widget()->show();
        _tabWidget->addTab(_stacks[k]->widget(), k.label());
      }
    }
    _tabWidget->setCurrentPage(0);
  }
}

void ToolsConfigWidget::loadConfig()
{
  const Tool::Group &group = Compile::Config::toolGroup(&_project);
  _tool->setGroup(group);
  _output->setCurrentItem(_project.outputType());
  QStringList commands = Compile::Config::customCommands(&_project);
  _commandsEditor->setTexts(commands);
  toolChanged();
  FOR_EACH(Tool::Category, k) {
    for (KeyWidgetStack<QString>::const_iterator it = _stacks[k]->begin(); it != _stacks[k]->end(); ++it) {
      if ( it->first==Tool::Group::CUSTOM_NAME ) continue;
      static_cast<ToolConfigWidget *>(_stacks[k]->item(it->first))->loadConfig();
    }
  }
}

void ToolsConfigWidget::saveConfig()
{
  Compile::Config::setToolGroup(&_project, _tool->group());
  _project.setOutputType(_output->currentItem());
  Compile::Config::setCustomCommands(&_project, _commandsEditor->texts());
  QString name = _tool->group().name();
  FOR_EACH(Tool::Category, k) {
    if ( !_stacks[k]->contains(name) ) continue;
    QWidget *w = _stacks[k]->item(name);
    static_cast<ToolConfigWidget *>(w)->saveConfig();
  }
}

void ToolsConfigWidget::displayHelp(Tool::Category category)
{
  if ( _helpDialog.isNull() ) {
    Compile::Data data;
    data.category = category;
    Compile::Process* process = _tool->group().processFactory(data);
    CustomArgument::ArgList args = process->argList();
    delete process;
    _helpDialog = new HelpDialog(this, args);
  }
  _helpDialog->show();
}

void ToolsConfigWidget::customDisplayHelp()
{
  if ( _customHelpDialog.isNull() ) {
    Compile::CustomProcess process(QString::null);
    CustomArgument::ArgList args = process.argList();
    _customHelpDialog = new HelpDialog(this, args);
  }
  _customHelpDialog->show();
}
