/***************************************************************************
 *   Copyright (C) 2007 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef MAIN_TEST_H
#define MAIN_TEST_H

#include "common/global/about.h"

//----------------------------------------------------------------------------
#define TEST_SKIPPED_RETURN { skipped(); return false; }
#define TEST_FAILED_RETURN(message) { failed(message, __FILE__, __LINE__); return false; }
#define TEST_FAILED(message) { failed(message, __FILE__, __LINE__); }
#define TEST_PASSED { printf("."); fflush(stdout); passed(); }
#define TEST_MAIN(Type)  \
  int main(int argc, char **argv) \
  { \
  Type *check = new Type; \
  Piklab::OptionList opt = check->optionList(); \
  Piklab::AboutData about("test", NULL, NULL); \
  Piklab::initArgs(about , argc, argv, opt); \
  const bool gui = false; \
  Piklab::createApplication(about.appName(), argc, argv, gui, NULL); \
  printf("\n"); \
  return check->runTest(); \
  }

//----------------------------------------------------------------------------
class Test
{
public:
  Test();
  virtual ~Test() {}
  virtual Piklab::OptionList optionList() const { return Piklab::OptionList(); }
  virtual int runTest();

protected:
  KCmdLineArgs *_args;
  QString       _message;

  void passed() { _nbPassed++; }
  void failed(const QString &message, const char *file, int line);
  void skipped() { _nbSkipped++; }
  virtual void execute() = 0;
  virtual void checkArguments() {}

private:
  uint _nbPassed, _nbFailed, _nbSkipped;
};

#endif
