/***************************************************************************
 *   Copyright (C) 2012 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <iostream>

#include "build_check.h"
#include "build_check.moc"
#include "libgui/toplevel.h"
#include "libgui/editor_manager.h"

//----------------------------------------------------------------------------
BuildCheckData DATA[] = {
  { "gputils", "blinker",     PURL::Project,       { "blinker.asm", NULL },            { NULL },                         { "o", "lst", NULL },                    { "cof", "cof", "map", "hex", NULL } },
  { "gputils", "blinker_18",  PURL::Project,       { "blinker_18.asm", NULL },         { NULL },                         { "o", "lst", NULL },                    { "cof", "cof", "map", "hex", NULL } },
  { "gputils", "blinker_lib", PURL::Project,       { "blinker.asm", NULL },            { NULL },                         { "o", "lst", NULL },                    { "lib", NULL }                      },
  { "gputils", "project",     PURL::Project,       { "test.asm",  "test2.asm", NULL }, { "test.inc", "test.lib", NULL }, { "o", "lst", NULL },                    { "cof", "cof", "map", "hex", NULL } },
  { "gputils", "project",     PURL::PikdevProject, { "test.asm",  "test2.asm", NULL }, { "test.inc", "test.lib", NULL }, { "o", "lst", NULL },                    { "cof", "cof", "map", "hex", NULL } },
  { "sdcc",    "blinker",     PURL::Project,       { "blinker.c", "sub/add.c", NULL }, { "blinker.h", NULL },            { "o", "lst", "asm", "adb", "p", NULL }, { "cof", "cof", "map", "hex", NULL } },
  { "sdcc",    "blinker_lib", PURL::Project,       { "blinker.c", "add.c", NULL },     { NULL },                         { "o", "lst", "asm", "adb", "p", NULL }, { "lib", NULL } },
  { "sdcc",    "test18",      PURL::Project,       { "test18.c", NULL },               { NULL },                         { "o", "lst", "asm", "adb", NULL },      { "cof", "cof", "map", "hex", NULL } },
  { "sdcc",    "usart",       PURL::Project,       { "usart_test.c", NULL },           { NULL },                         { "o", "lst", "asm", "adb", NULL },      { "cof", "cof", "map", "hex", NULL } },
  { "sdcc",    "usart_lib",   PURL::Project,       { "usart_test.c", NULL },           { NULL },                         { "o", "lst", "asm", "adb", NULL },      { "lib", NULL }                      },

  { NULL,      NULL,          PURL::Nb_FileTypes,  { NULL },                           { NULL },                         { NULL },                                { NULL }                             }
};

//----------------------------------------------------------------------------
bool BuildCheck::init(uint index)
{
  if (DATA[index].dir == NULL) {
    return false;
  }
  _dir = DATA[index].dir;
  _project = DATA[index].project;
  _projType = DATA[index].projType;
  _sources.clear();
  for (uint i = 0; DATA[index].sources[i] != NULL; ++i) {
    _sources.append(DATA[index].sources[i]);
  }
  _incFiles.clear();
  for (uint i = 0; DATA[index].incFiles[i] != NULL; ++i) {
    _incFiles.append(DATA[index].incFiles[i]);
  }
  _objExtensions.clear();
  for (uint i = 0; DATA[index].objExtensions[i] != NULL; ++i) {
    _objExtensions.append(DATA[index].objExtensions[i]);
  }
  _extensions.clear();
  for (uint i = 0; DATA[index].extensions[i] != NULL; ++i) {
    _extensions.append(DATA[index].extensions[i]);
  }
  return true;
}

PURL::Directory BuildCheck::testDir() const
{
  return PURL::currentDirectory().down("../../../../test/" + _dir + "/" + _project);
}

PURL::Url BuildCheck::getUrl(const QString& filename) const
{
  return PURL::Url(testDir(), filename);
}

void BuildCheck::checkFilesExist(bool exists) const
{
  for (QStringList::const_iterator it = _sources.begin(); it != _sources.end(); ++it) {
    for (QStringList::const_iterator it2 = _objExtensions.begin(); it2 != _objExtensions.end(); ++it2) {
      if (!exists) {
        PURL::Url url = PURL::Url(testDir(), *it).toExtension(*it2);
        QVERIFY2(url.exists() == exists, url.pretty().latin1());
      }
      QString filename = PURL::Url(PURL::Directory(), *it).filename();
      PURL::Url url = PURL::Url(testDir(), filename).toExtension(*it2);
      QVERIFY2(url.exists() == exists, url.pretty().latin1());
    }
  }
  for (QStringList::const_iterator it = _incFiles.begin(); it != _incFiles.end(); ++it) {
    PURL::Url url = PURL::Url(testDir(), *it);
    QVERIFY2(url.exists(), url.pretty().latin1());
  }
  for (QStringList::const_iterator it = _extensions.begin(); it != _extensions.end(); ++it) {
    PURL::Url url = getUrl(_project).toExtension(*it);
    QVERIFY2(url.exists() == exists, url.pretty().latin1());
  }
}

void BuildCheck::test1()
{
  std::cout << "TESTING: " << _dir.latin1() << "/" << _project.latin1()
            << "/" << _project.latin1() << "." << _projType.data().extensions[0] << std::endl;
  MainWindow::self().closeProject();
}

void BuildCheck::test2()
{
  QVERIFY(Main::project() == NULL);
  Main::editorManager().closeAllEditors();
}

void BuildCheck::test3()
{
  QVERIFY(Main::editorManager().currentEditor() == NULL);
  QVERIFY(testDir().exists());
  PURL::Url url = getUrl(_project).toFileType(_projType);
  QVERIFY(url.exists());
  QVERIFY(MainWindow::self().openProject(url));
}

void BuildCheck::test4()
{
  QVERIFY(Main::project() != NULL);
  Main::editorManager().closeAllEditors();
}

void BuildCheck::test5()
{
  QVERIFY(Main::editorManager().currentEditor() == NULL);
  QVERIFY(_sources.size() > 0);
  QVERIFY(Main::editorManager().openFile(getUrl(*_sources.begin())));
}

void BuildCheck::test6()
{
  QVERIFY(Main::editorManager().currentEditor() != NULL);
  QVERIFY(Main::editorManager().nbEditors() == 1);
  MainWindow::self().cleanBuild();
}

void BuildCheck::test7()
{
  checkFilesExist(false);
  MainWindow::self().buildProject();
}

void BuildCheck::test8()
{
  checkFilesExist(true);
  MainWindow::self().cleanBuild();
}

void BuildCheck::test9()
{
  checkFilesExist(false);
}

//----------------------------------------------------------------------------
void BuildCheckProxy::start()
{
    BuildCheck check;
    run(check);
}

//----------------------------------------------------------------------------
QTEST_APPLESS_MAIN(BuildCheckProxy);
