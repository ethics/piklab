/***************************************************************************
 *   Copyright (C) 2012 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef BUILD_CHECK_H
#define BUILD_CHECK_H

#include "gui_check.h"
#include "common/global/purl.h"

//----------------------------------------------------------------------------
struct BuildCheckData {
  const char* dir;
  const char* project;
  PURL::FileType projType;
  const char* sources[3];
  const char* incFiles[3];
  const char* objExtensions[6];
  const char* extensions[5];
};

class BuildCheck : public GuiCheck
{
  Q_OBJECT

  private slots:
    void test1();
    void test2();
    void test3();
    void test4();
    void test5();
    void test6();
    void test7();
    void test8();
    void test9();

  private:
    QString _dir;
    QString _project;
    PURL::FileType _projType;
    QStringList _sources;
    QStringList _incFiles;
    QStringList _objExtensions;
    QStringList _extensions;

    PURL::Directory testDir() const;
    PURL::Url getUrl(const QString& filename) const;
    void checkFilesExist(bool exists) const;
    virtual bool init(uint index);
};

//----------------------------------------------------------------------------
class BuildCheckProxy : public GuiCheckProxy
{
  Q_OBJECT

  private slots:
    void start();
};

#endif
