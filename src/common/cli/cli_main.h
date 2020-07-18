/***************************************************************************
 *   Copyright (C) 2007-2009 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef CLI_MAIN_H
#define CLI_MAIN_H

#include "common/global/about.h"
#include "cli_log.h"

namespace CLI
{
class Interactive;

//-----------------------------------------------------------------------------
enum CommandProperty { NoCommandProperty = 0, NeedInput = 1, NeedSecondInput = 2,
                       NeedOutput = 4, NeedCorrectInput = 8,
                       NeedDevice = 16, NeedProgrammer = 32,
                       ListCommand = 64 };
Q_DECLARE_FLAGS(CommandProperties, CommandProperty)
Q_DECLARE_OPERATORS_FOR_FLAGS(CommandProperties)

enum ExitCode { EXITING = 1, OK = 0, ARG_ERROR = -1, NOT_SUPPORTED_ERROR = -2,
                FILE_ERROR = -3, EXEC_ERROR = -4, NOT_PROCESSED = -5 };

BEGIN_DECLARE_ENUM(Mode)
  SingleCommand = 0, Interactive, Replay
END_DECLARE_ENUM_NO_DATA(Mode)

extern const KCmdLineOptions NullCmdLineOption;

struct CommandData {
  const char*       name;
  CommandProperties properties;
  uint              minNbArgs;
  uint              maxNbArgs;
  const char*       help;
};
extern const CommandData NullCommandData;

struct PropertyData
{
  const char *name, *optName, *alias, *help, *list, *listHelp;
};
extern const PropertyData NullPropertyData;

//-----------------------------------------------------------------------------
class MainBase : public QObject, public Log::Base
{
Q_OBJECT
public:
  static MainBase& instance() { return *_instance; };

  MainBase(const char *executable, const char *name, const char *description);
  virtual ~MainBase();
  virtual void initArgs(int argc, char** argv, const char* fileDescription);
  virtual ExitCode run();

  bool force() const { return _force; }
  Mode mode() const { return _mode; }
  ExitCode errorExit(const QString &message, ExitCode code);
  ExitCode okExit(const QString &message);

protected:
  View& view() { return _view; }
  Piklab::OptionList optionList(const char *fileDescription) const;
  const PURL::Url& input() const { return _input; }
  const PURL::Url& secondInput() const { return _secondInput; }
  const PURL::Url& output() const { return _output; }

  virtual ExitCode list(const QString& command);
  virtual ExitCode prepareCommand(const CommandProperties& properties);
  virtual ExitCode doExecuteSetCommand(const QString& property, const QString& value);
  virtual ExitCode doExecuteGetCommand(const QString& property, QString& value);
  virtual ExitCode setInput(const QString& filepath);
  virtual ExitCode setSecondInput(const QString& filepath);
  virtual ExitCode setOutput(const QString& filepath);

private:
  static MainBase*        _instance;
  const Piklab::AboutData _aboutData;
  View                    _view;
  const KCmdLineArgs*     _args;

  bool                    _force;
  Mode                    _mode;
  QString                 _commandToLog;
  QStringList             _listCommands;
  PURL::Url               _input;
  PURL::Url               _secondInput;
  PURL::Url               _output;

  virtual const Piklab::CmdLineOption* option(uint i) const { Q_UNUSED(i); return NULL; }
  virtual const CommandData& commandData(uint i) const = 0;
  virtual const CommandData& interactiveCommandData(uint i) const { Q_UNUSED(i); return NullCommandData; }
  virtual const PropertyData& propertyData(uint i) const = 0;

  class CommandIterator
  {
  public:
    CommandIterator(Mode mode);
    const CommandData* getNext();

  private:
    Mode _mode;
    bool _standard;
    int  _index;
  };

  class OptionIterator
  {
  public:
    OptionIterator();
    const Piklab::CmdLineOption* getNext();

  private:
    bool _standard;
    int  _index;
  };

  void initListCommands();
  void initListCommandsInteractive();
  const CommandData *findCommandData(const QString &command) const;
  ExitCode listFromArgs();
  bool isProperty(const QString &s) const;
  ExitCode commandList();
  ExitCode propertyList();
  CommandProperties allCommandProperties() const;

  virtual ExitCode executeCommand(const QString &command) = 0;
  ExitCode executeSetCommand(const QString &property, const QString &value, bool logInput);
  ExitCode executeGetCommand(const QString &property, QString& value, bool logInput);

  ExitCode runSingleCommand();
  ExitCode runReplay(const QString& filepath);
  ExitCode runInteractive();
  ExitCode processLine(const QString& line);
  ExitCode prepareProcessLine(QStringList& words);
  ExitCode processStandardLine(const QStringList& words);
  virtual ExitCode processLine(const QStringList& words) { Q_UNUSED(words); return NOT_PROCESSED; }
  virtual void processInterrupt() {}

friend class Interactive;
};

} // namespace

#endif
