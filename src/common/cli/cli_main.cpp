/***************************************************************************
 *   Copyright (C) 2007-2009 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "cli_main.h"
#include "cli_main.moc"

#include "cli_interactive.h"

//-----------------------------------------------------------------------------
const CLI::CommandData CLI::NullCommandData = { NULL, NoCommandProperty, 0, 0, NULL };
const CLI::PropertyData CLI::NullPropertyData = { 0, 0, 0, 0, 0, 0 };

namespace CLI {

const Piklab::CmdLineOption STANDARD_OPTIONS[] = {
  { "f",              0, 0 },
  { "force",          I18N_NOOP("Overwrite files and answer \"yes\" to questions."), 0 },
  { "i",              0, 0 },
  { "cli",            I18N_NOOP("Interactive mode"), 0 },
  { "c",              0, 0 },
  { "command <name>", I18N_NOOP("Perform the requested command."), 0 },
  { "command-list",   I18N_NOOP("Return the list of recognized commands."), 0 },
  { "debug",          I18N_NOOP("Display debug messages."), 0 },
  { "extra-debug",    I18N_NOOP("Display extra debug messages."), 0 },
  { "max-debug",      I18N_NOOP("Display all debug messages."), 0 },
  { "lowlevel-debug", I18N_NOOP("Display low level debug messages."), 0 },
  { "quiet",          I18N_NOOP("Do not display messages."), 0 },
  { "log <file>",     I18N_NOOP("Specify log file."), 0 },
  { "replay <file>",  I18N_NOOP("Replay commands in provided file."), 0 },
  { NULL, NULL, NULL }
};

const CLI::CommandData STANDARD_COMMAND_DATA[] = {
    NullCommandData
};

const CLI::CommandData STANDARD_INTERACTIVE_COMMAND_DATA[] = {
  { "help",          CLI::ListCommand,       0, 0, I18N_NOOP("Display help.") },
  { "exit",          CLI::NoCommandProperty, 0, 0, I18N_NOOP("Exit.") },
  { "quit",          CLI::NoCommandProperty, 0, 0, I18N_NOOP("Exit.") },
  { "set",           CLI::NoCommandProperty, 1, 2, I18N_NOOP("Set property value: \"set <property> <value>\" or \"<property> <value>\".") },
  { "unset",         CLI::NoCommandProperty, 1, 1, I18N_NOOP("Unset property value: \"unset <property>\".") },
  { "get",           CLI::NoCommandProperty, 1, 1, I18N_NOOP("Get property value: \"get <property>\" or \"<property>\".") },
  { "property-list", CLI::NoCommandProperty, 0, 0, I18N_NOOP("Display the list of available properties.") },
  { "replay",        CLI::NoCommandProperty, 1, 1, I18N_NOOP("Replay commands in provided file: \"replay <file>\".") },
  NullCommandData
};

} // namespace

//-----------------------------------------------------------------------------
CLI::MainBase* CLI::MainBase::_instance = NULL;

CLI::MainBase::MainBase(const char *executable, const char *name, const char *description)
: _aboutData(executable, name, description),
  _args(NULL),
  _force(false),
_mode(Mode::SingleCommand)
{
  ASSERT(_instance == NULL);
  _instance = this;

  setView(&_view);
}

CLI::MainBase::~MainBase()
{
  if (_mode == Mode::SingleCommand) _view.log(Log::LineType::Input, "exit");
  _instance = NULL;
}

void CLI::MainBase::initArgs(int argc, char** argv, const char* fileDescription)
{
  static Piklab::OptionList list = optionList(fileDescription); // danger!
  Piklab::initArgs(_aboutData, argc, argv, list);
  _args = KCmdLineArgs::parsedArgs();

  QString filepath = _args->getOption("log");
  if (!filepath.isEmpty()) {
    PURL::Url url = PURL::fromPathOrUrl(filepath, false);
    _view.setLogFile(url);
  }

  const bool gui = false;
  Piklab::createApplication(_aboutData.appName(), argc, argv, gui, &_view);
}

void CLI::MainBase::initListCommands()
{
  _listCommands += "command-list";
  CommandIterator iter(Mode::SingleCommand);
  while (const CommandData* data = iter.getNext())
    if (data->properties & ListCommand) _listCommands += data->name;
  for (uint i=0; propertyData(i).name != NULL; i++) {
    if (propertyData(i).list != NULL) _listCommands += propertyData(i).list;
  }
}

void CLI::MainBase::initListCommandsInteractive()
{
  _listCommands += "property-list";
  CommandIterator iter(Mode::Interactive);
  while (const CommandData* data = iter.getNext())
    if (data->properties & ListCommand) _listCommands += data->name;
}

Piklab::OptionList CLI::MainBase::optionList(const char *fileDescription) const
{
  Piklab::OptionList list;
  OptionIterator iter;
  while (const Piklab::CmdLineOption* data = iter.getNext())
    list.append(*data);

  Piklab::CmdLineOption opt;
  for (uint i=0; propertyData(i).name != NULL; i++) {
    opt.description = NULL;
    opt.def = NULL;
    if (propertyData(i).help == NULL) {
      ASSERT(QString(propertyData(i).name) != propertyData(i).optName);
      opt.name = propertyData(i).name; // alias
      list.append(opt);
    } else {
      if (propertyData(i).optName == NULL) continue; // interactive only
      if (propertyData(i).alias != NULL) {
        opt.name = propertyData(i).alias;
        list.append(opt);
      }
      opt.name = propertyData(i).optName;
      opt.description = propertyData(i).help;
      list.append(opt);
      if (propertyData(i).list != NULL) {
        opt.name = propertyData(i).list;
        opt.description = propertyData(i).listHelp;
        list.append(opt);
      }
    }
  }

  if (fileDescription != NULL) {
    opt.name = "+[file]";
    opt.description = fileDescription;
    opt.def = NULL;
  }
  list.append(opt);

  return list;
}

CLI::ExitCode CLI::MainBase::list(const QString& command)
{
  if (command == "command-list" || command == "help") return commandList();
  if (command == "property-list") return propertyList();
  return ARG_ERROR;
}

CLI::ExitCode CLI::MainBase::listFromArgs()
{
  for (QStringList::const_iterator it = _listCommands.begin(); it != _listCommands.end(); ++it) {
    const QString& listCommand = *it;
    if ( _args->isSet(listCommand.latin1()) ) {
      log(Log::LineType::Input, listCommand);
      return list(listCommand);
    }
  }
  return NOT_PROCESSED;
}

CLI::ExitCode CLI::MainBase::errorExit(const QString &message, ExitCode code)
{
  ASSERT( code!=OK );
  if (!_commandToLog.isEmpty()) {
    log(Log::LineType::Input, _commandToLog);
    _commandToLog = QString::null;
  }
  log(Log::LineType::SoftError, message);
  return code;
}

CLI::ExitCode CLI::MainBase::okExit(const QString &message)
{
  log(Log::LineType::Information, message);
  return OK;
}

const CLI::CommandData *CLI::MainBase::findCommandData(const QString &command) const
{
  {
    CommandIterator iter(Mode::SingleCommand);
    while (const CommandData* data = iter.getNext())
      if (data->name == command) return data;
  }
  if (_mode != Mode::SingleCommand) {
    CommandIterator iter(Mode::Interactive);
    while (const CommandData* data = iter.getNext())
      if (data->name == command) return data;
  }
  return NULL;
}

CLI::CommandProperties CLI::MainBase::allCommandProperties() const
{
  CommandProperties properties = NoCommandProperty;
  { CommandIterator iter(Mode::SingleCommand);
    while (const CommandData* data = iter.getNext())
      properties |= data->properties;
  }
  if (_mode != Mode::SingleCommand) {
    CommandIterator iter(Mode::Interactive);
    while (const CommandData* data = iter.getNext())
      properties |= data->properties;
  }
  return properties;
}

bool CLI::MainBase::isProperty(const QString &s) const
{
  for (uint i=0; propertyData(i).name != NULL; i++)
    if (s == propertyData(i).name) return true;
  CommandProperties properties = allCommandProperties();
  if ((properties & NeedInput) && s == "input") return true;
  if ((properties & NeedSecondInput) && s == "input2") return true;
  if ((properties & NeedOutput) && s == "output") return true;
  return false;
}

CLI::ExitCode CLI::MainBase::commandList()
{
  Log::KeyList keys(i18n("Supported commands:"));
  if (_mode != Mode::SingleCommand) {
    CommandIterator iter(Mode::Interactive);
    while (const CommandData* data = iter.getNext())
      keys.append(data->name, i18n(data->help));
    keys.append("", "");
  }
  CommandIterator iter(Mode::SingleCommand);
  while (const CommandData* data = iter.getNext())
    keys.append(data->name, i18n(data->help));
  keys.display(_view);
  return OK;
}

CLI::ExitCode CLI::MainBase::propertyList()
{
  Log::KeyList keys;
  for (uint i=0; propertyData(i).name != NULL; i++)
    keys.append(propertyData(i).name, i18n(propertyData(i).help));
  keys.display(_view);
  return OK;
}

CLI::ExitCode CLI::MainBase::setInput(const QString &filepath)
{
  _input = PURL::fromPathOrUrl(filepath, false);
  PURL::File file(_input, view());
  if ( !file.openForRead() ) return FILE_ERROR;
  return OK;
}

CLI::ExitCode CLI::MainBase::setSecondInput(const QString &filepath)
{
  _secondInput = PURL::fromPathOrUrl(filepath, false);
  PURL::File file(_secondInput, view());
  if ( !file.openForRead() ) return FILE_ERROR;
  return OK;
}

CLI::ExitCode CLI::MainBase::setOutput(const QString &filepath)
{
  _output = PURL::fromPathOrUrl(filepath, false);
  if (!force() && _output.exists()) return errorExit(i18n("Output file already exists."), FILE_ERROR);
  return OK;
}

CLI::ExitCode CLI::MainBase::executeSetCommand(const QString &property, const QString &value, bool logInput)
{
  if (logInput) log(Log::LineType::Input, QString("set ") + property + " " + value);
  ExitCode code = doExecuteSetCommand(property, value);
  if (code == NOT_PROCESSED) return errorExit(i18n("Unknown property \"%1\".").arg(property), ARG_ERROR);
  if (code != OK) return code;
  QString svalue;
  doExecuteGetCommand(property, svalue);
  log(Log::LineType::Information, svalue, Log::LogFileOnly);
  return OK;
}

CLI::ExitCode CLI::MainBase::doExecuteSetCommand(const QString &property, const QString &value)
{
  if (property == "input") return setInput(value);
  if (property == "input2") return setSecondInput(value);
  if (property == "output") return setOutput(value);
  return NOT_PROCESSED;
}

CLI::ExitCode CLI::MainBase::executeGetCommand(const QString& property, QString& value, bool logInput)
{
  value = QString::null;
  if (logInput) log(Log::LineType::Input, QString("get ") + property);
  ExitCode code = doExecuteGetCommand(property, value);
  if (code == NOT_PROCESSED) return errorExit(i18n("Unknown property \"%1\".").arg(property), ARG_ERROR);
  if (code != OK) return code;
  if (value.isNull()) value = i18n("<not set>");
  log(Log::LineType::Information, value);
  return OK;
}

CLI::ExitCode CLI::MainBase::doExecuteGetCommand(const QString& property, QString& value)
{
  if (property == "input") {
    if (!_input.isEmpty()) value = _input.filepath();
    return OK;
  }
  if (property == "input2") {
    if (!_secondInput.isEmpty()) value = _secondInput.filepath();
    return OK;
  }
  if (property == "output") {
    if (!_output.isEmpty()) value = _output.filepath();
    return OK;
  }
  return NOT_PROCESSED;
}

CLI::ExitCode CLI::MainBase::run()
{
  // process global options
  if (_args->isSet("replay")) _mode = Mode::Replay;
  else if (_args->isSet("cli")) _mode = Mode::Interactive;
  else _mode = Mode::SingleCommand;

  _force = _args->isSet("force");
  if (_force) log(Log::LineType::Input, "set force true");
  FOR_EACH(Log::DebugLevel, level) {
    if (_args->isSet(level.key().latin1())) _view.setDebugLevel(level);
  }
  Log::DebugLevel debugLevel = _view.debugLevel();
  if (debugLevel != Log::DebugLevel::Normal) {
    log(Log::LineType::Input, QString("set debuglevel ") + debugLevel.key());
  }

  // process specific options
  for (uint i=0; propertyData(i).name != NULL; i++) {
    if (propertyData(i).optName == NULL) continue; // alias or interactive only
    if (!_args->isSet(propertyData(i).name)) continue;
    QString option = _args->getOption(propertyData(i).name);
    ExitCode code = executeSetCommand(propertyData(i).name, option, true);
    if (code != OK) return code;
  }

  // process list command
  initListCommands();
  ExitCode code = listFromArgs();
  if (code != NOT_PROCESSED) return code;

  if (_mode != Mode::SingleCommand) initListCommandsInteractive();
  switch (_mode.type()) {
    case Mode::Replay:        return runReplay(_args->getOption("replay"));
    case Mode::Interactive:   return runInteractive();
    case Mode::SingleCommand: return runSingleCommand();
    case Mode::Nb_Types:      break;
  }
  ASSERT(false);
  return EXEC_ERROR;
}

CLI::ExitCode CLI::MainBase::prepareCommand(const CommandProperties& properties)
{
  int nbArgs = 0;
  if (properties & NeedInput) ++nbArgs;
  if (properties & NeedSecondInput) ++nbArgs;
  if (properties & NeedOutput) ++nbArgs;
  if (_args->count() < nbArgs) return errorExit(i18n("Too few arguments."), ARG_ERROR);
  if (_args->count() > nbArgs) return errorExit(i18n("Too many arguments."), ARG_ERROR);

  int argIndex = 0;
  if (properties & NeedInput) {
    QString filepath = _args->arg(argIndex);
    ++argIndex;
    setInput(filepath);
  }
  if (properties & NeedSecondInput) {
    QString filepath = _args->arg(argIndex);
    ++argIndex;
    setSecondInput(filepath);
  }
  if (properties & NeedOutput) {
    QString filepath = _args->arg(argIndex);
    argIndex++;
    setOutput(filepath);
  }
  return OK;
}

CLI::ExitCode CLI::MainBase::runSingleCommand()
{
  QString command = _args->getOption("command");
  _commandToLog = command;

  if (command.isEmpty()) return errorExit(i18n("No command specified"), ARG_ERROR);
  const CommandData *data = findCommandData(command);
  if (data == NULL) return errorExit(i18n("Unknown command: %1").arg(command), ARG_ERROR);
  ExitCode code = prepareCommand(data->properties);
  if (code != OK) return code;

  _commandToLog = QString::null;
  log(Log::LineType::Input, command);
  return executeCommand(command);
}

CLI::ExitCode CLI::MainBase::runInteractive()
{
  std::auto_ptr<Interactive> p(new Interactive(*this));
  log(Log::LineType::Normal, i18n("Interactive mode: type 'help' for the list of commands."));
  log(Log::LineType::Normal, QString::null);
  return ExitCode(QCoreApplication::exec());
}

CLI::ExitCode CLI::MainBase::runReplay(const QString &filepath)
{
  PURL::Url url = PURL::fromPathOrUrl(filepath, false);
  PURL::File file(url, _view);
  if (!file.openForRead()) return FILE_ERROR;
  uint i = 0;
  while (true) {
    QString line = file.readLine();
    if (line.isNull()) break; // EOF
    ++i;
    line = line.section('#', 0, 0); // everything after # is removed
    line = line.stripWhiteSpace();
    if (line.isEmpty()) continue;
    char fc = line[0].latin1();
    if ((fc != 'o' && fc != 'e' && fc != 'w' && fc != 'd' && fc != 'i')
        || (line.length() >= 2 && line[1] != ' ')) {
      log(Log::LineType::Error, i18n("Unrecognized line #%1 of replay file.").arg(i));
      continue;
    }
    if (fc != 'i') continue;
    line = line.mid(2).stripWhiteSpace();
    if (processLine(line) == EXITING) break;
  }
  return EXITING;
}

CLI::ExitCode CLI::MainBase::processLine(const QString& line)
{
  QStringList words;
  QString word;
  bool escaped = false;
  for (uint i=0; i<uint(line.length()); ++i) {
    if (!escaped) {
      if (line[i] == '\\') {
        escaped = true;
        continue;
      }
      if (line[i] == ' '
          && !word.isEmpty()) {
        words.append(word);
        word = QString::null;
        continue;
      }
    } else {
      if (line[i] != ' ') word += '\\';
      escaped = false;
    }
    word += line[i];
  }
  if (!word.isEmpty()) words.append(word);

  if (words.count() == 0) return ARG_ERROR;

  if (_listCommands.find(words[0]) != _listCommands.end()) {
    log(Log::LineType::Input, words[0]);
    return list(words[0]);
  }
  log(Log::LineType::Input, words.join(" "));
  ExitCode code = prepareProcessLine(words);
  if (code != OK) return code;
  code = processStandardLine(words);
  if (code != NOT_PROCESSED) return code;
  code = processLine(words);
  if (code != NOT_PROCESSED) return code;
  return executeCommand(words[0]);
}

CLI::ExitCode CLI::MainBase::prepareProcessLine(QStringList& words)
{
  if (isProperty(words[0])) {
    if (words.count() == 1) words.prepend("get");
    else words.prepend("set");
  }
  const CommandData* data = findCommandData(words[0]);
  if (data == NULL) return errorExit(i18n("Unrecognized command."), ARG_ERROR);
  uint nb = words.count() - 1;
  if (data->minNbArgs == data->maxNbArgs) {
    if (nb < data->minNbArgs || nb > data->maxNbArgs) {
      if (data->minNbArgs == 0) return errorExit(i18n("This command takes no argument."), ARG_ERROR);
      else if (data->minNbArgs == 1) return errorExit(i18n("This command takes one argument."), ARG_ERROR);
      else return errorExit(i18n("This command takes %1 arguments.").arg(data->minNbArgs), ARG_ERROR);
    }
  } else {
    if (nb < data->minNbArgs) {
      if (data->minNbArgs == 1) return errorExit(i18n("This command needs at least one argument."), ARG_ERROR);
      else return errorExit(i18n("This command needs at least %1 arguments.").arg(data->minNbArgs), ARG_ERROR);
    }
    if (nb > data->maxNbArgs) {
      if (data->maxNbArgs == 1) return errorExit(i18n("This command takes at most one argument."), ARG_ERROR);
      else return errorExit(i18n("This command takes at most %1 arguments.").arg(data->maxNbArgs), ARG_ERROR);
    }
  }
  return OK;
}

CLI::ExitCode CLI::MainBase::processStandardLine(const QStringList& words)
{
  if (words[0] == "exit" || words[0] == "quit") return EXITING;
  if (words[0] == "set") {
    if (words.count() == 2) return executeSetCommand(words[1], QString::null, false);
    return executeSetCommand(words[1], words[2], false);
  }
  if (words[0] == "unset") return executeSetCommand(words[1], QString::null, false);
  if (words[0] == "get") {
    QString s;
    return executeGetCommand(words[1], s, false);
  }
  if (words[0] == "replay") {
    Mode old = _mode;
    _mode = Mode::Replay;
    ExitCode code = runReplay(words[1]);
    _mode = old;
    return code;
  }
  return NOT_PROCESSED;
}

//-----------------------------------------------------------------------------
CLI::MainBase::CommandIterator::CommandIterator(Mode mode)
: _mode(mode),
  _standard(true),
  _index(0)
{
}

const CLI::CommandData* CLI::MainBase::CommandIterator::getNext()
{
  if (_standard) {
    const CommandData* data = (_mode == Mode::SingleCommand ? &STANDARD_COMMAND_DATA[_index]
                               : &STANDARD_INTERACTIVE_COMMAND_DATA[_index]);
    if (data->name == NULL) {
      _standard = false;
      _index = 0;
      return getNext();
    }
    ++_index;
    return data;
  } else {
    const CommandData* data = (_mode == Mode::SingleCommand ? &MainBase::instance().commandData(_index)
                               : &MainBase::instance().interactiveCommandData(_index));
    if (data->name == NULL) return NULL;
    ++_index;
    return data;
  }
}

//-----------------------------------------------------------------------------
CLI::MainBase::OptionIterator::OptionIterator()
: _standard(true),
  _index(0)
{
}

const Piklab::CmdLineOption* CLI::MainBase::OptionIterator::getNext()
{
  if (_standard) {
    const Piklab::CmdLineOption* data = &STANDARD_OPTIONS[_index];
    if (data->name == NULL) {
      _standard = false;
      _index = 0;
      return getNext();
    }
    ++_index;
    return data;
  } else {
    const Piklab::CmdLineOption* data = MainBase::instance().option(_index);
    if (data == NULL || data->name == NULL) return NULL;
    ++_index;
    return data;
  }
}
