/***************************************************************************
 *   Copyright (C) 2007 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "misc_check.h"
#include "misc_check.moc"

#include "common/global/purl.h"
#include "common/global/pfile.h"

//----------------------------------------------------------------------------
void MiscCheck::testUrl()
{
  {
    PURL::Url url = PURL::fromPathOrUrl(QString::null, false);
    QVERIFY(url.isEmpty());
  }
  {
    PURL::Url url = PURL::fromPathOrUrl("", false);
    QVERIFY(url.isEmpty());
  }
  {
    PURL::Url url = PURL::fromPathOrUrl("/", false);
    QVERIFY(!url.isEmpty());
    QVERIFY(!url.isRelative());
    QVERIFY(url.isLocal());
    QCOMPARE(url.path(), QString("/"));
    QCOMPARE(url.filepath(), QString("/"));
    QCOMPARE(url.path(PURL::Unterminated), QString("/"));
    QVERIFY(url.filename().isEmpty());
  }
  {
    PURL::Url url = PURL::fromPathOrUrl("test", true);
    QVERIFY(!url.isEmpty());
    QVERIFY(url.isRelative());
    QVERIFY(url.isLocal());
    QCOMPARE(url.filename(), QString("test"));
  }
  {
    PURL::Url url = PURL::fromPathOrUrl("test", false);
    QVERIFY(!url.isEmpty());
    QVERIFY(!url.isRelative());
    QVERIFY(url.isLocal());
    QCOMPARE(url.filename(), QString("test"));
  }
  {
    PURL::Url url = PURL::fromPathOrUrl("/test", false);
    QVERIFY(!url.isEmpty());
    QVERIFY(!url.isRelative());
    QVERIFY(url.isLocal());
    QCOMPARE(url.path(), QString("/"));
    QCOMPARE(url.path(PURL::Unterminated), QString("/"));
    QCOMPARE(url.filepath(), QString("/test"));
    QCOMPARE(url.filename(), QString("test"));
  }
  {
    PURL::Url url = PURL::fromPathOrUrl("/test/", false);
    QVERIFY(!url.isEmpty());
    QVERIFY(!url.isRelative());
    QVERIFY(url.isLocal());
    QCOMPARE(url.path(), QString("/test/"));
    QCOMPARE(url.path(PURL::Unterminated), QString("/test"));
    QCOMPARE(url.filepath(), QString("/test/"));
    QVERIFY(url.filename().isEmpty());
  }
  {
    PURL::Url url = PURL::fromPathOrUrl("c:/test", false);
    QVERIFY(!url.isEmpty());
    QVERIFY(!url.isRelative());
    QVERIFY(url.isLocal());
    PURL::Url url2 = PURL::fromPathOrUrl("c:/", false);
    QCOMPARE(url.path(), url2.path());
    QVERIFY(url != url2);
    QCOMPARE(url.filename(), QString("test"));
  }
  {
    PURL::Url url = PURL::fromPathOrUrl("d:/test/", false);
    QVERIFY(!url.isEmpty());
    QVERIFY(!url.isRelative());
    QVERIFY(url.isLocal());
    QVERIFY(url.filename().isEmpty());
  }
  {
    PURL::Url url(KUrl("http://test.net/test"));
    QVERIFY(!url.isEmpty());
    QVERIFY(!url.isRelative());
    QVERIFY(!url.isLocal());
    QCOMPARE(url.filename(), QString("test"));
  }
  {
    PURL::Url url= PURL::fromPathOrUrl("file://test/test", true);
    QVERIFY(!url.isEmpty());
    QVERIFY(url.isRelative());
    QVERIFY(url.isLocal());
    QCOMPARE(url.path(), QString("/test/"));
    QCOMPARE(url.filename(), QString("test"));
    QCOMPARE(url.filepath(), QString("/test/test"));
  }
}

void MiscCheck::testDir()
{
  {
    PURL::Directory dir;
    QVERIFY(dir.isEmpty());
    QVERIFY(dir.isLocal());
    QVERIFY(dir.isRelative());
    QCOMPARE(dir.path(), QString(""));
  }
  {
    PURL::Directory dir("");
    QVERIFY(dir.isEmpty());
    QVERIFY(dir.isLocal());
    QVERIFY(dir.isRelative());
    QCOMPARE(dir.path(), QString(""));
  }
  {
    PURL::Directory dir("/");
    QVERIFY(!dir.isEmpty());
    QVERIFY(dir.isLocal());
    QVERIFY(!dir.isRelative());
    QCOMPARE(dir.path(), QString("/"));
  }
  {
    PURL::Directory dir("test");
    QVERIFY(!dir.isEmpty());
    QVERIFY(dir.isLocal());
    QVERIFY(dir.isRelative());
    QCOMPARE(dir.path(), QString("/test/"));
  }
  {
    PURL::Directory dir("/test");
    QVERIFY(!dir.isEmpty());
    QVERIFY(dir.isLocal());
    QVERIFY(!dir.isRelative());
    QCOMPARE(dir.path(), QString("/test/"));
  }
  {
    PURL::Directory dir("/test/");
    QVERIFY(!dir.isEmpty());
    QVERIFY(dir.isLocal());
    QVERIFY(!dir.isRelative());
    QCOMPARE(dir.path(), QString("/test/"));
  }
  {
    PURL::Directory dir("file://test/test");
    QVERIFY(!dir.isEmpty());
    QVERIFY(!dir.isRelative());
    QVERIFY(dir.isLocal());
    QCOMPARE(dir.path(), QString("/test/test/"));
  }
}

void MiscCheck::testUrlOperations()
{
  Log::StringView log;
  PURL::Url url(PURL::currentDirectory(), "toto");
  url.del(log); // clean state
  QVERIFY(!url.exists());
  QVERIFY(url.create(log));
  QVERIFY(!url.create(log));
  QVERIFY(url.exists());
  QVERIFY(url.del(log));
  QVERIFY(!url.exists());
  QVERIFY(!url.del(log));
  QVERIFY(url.write("test", log));
  QVERIFY(url.exists());

  PURL::Url url2 = url.toExtension("cpp");
  QVERIFY(url2.fileType() == PURL::CppSource);
  url2.del(log); // clean start
  QVERIFY(!url2.exists());
  QVERIFY(url.copyTo(url2, log));
  QVERIFY(url2.exists());
  QVERIFY(url.del(log));
  QVERIFY(!url.exists());
  QVERIFY(url2.del(log));
  QVERIFY(!url2.exists());
}

void MiscCheck::testFile()
{
  Log::StringView log;
  PURL::TempFile file1(log);
  QVERIFY(!file1.isOpen());
  QVERIFY(file1.openForWrite());
  QVERIFY(file1.isOpen());
  file1.appendText("toto");
  QVERIFY(!file1.hasError());
  QVERIFY(file1.close());
  QVERIFY(!file1.isOpen());

  PURL::File file2(log);
  const bool keepRelative = false;
  PURL::Url url = PURL::fromPathOrUrl("toto.txt", keepRelative);
  file2.setUrl(url);
  QVERIFY(file2.openForWrite());
  QVERIFY(file2.isOpen());
  file2.appendText("toto");
  QVERIFY(!file2.hasError());
  QVERIFY(file2.close());
  QVERIFY(!file2.isOpen());
  QFileInfo info("toto.txt");
  QVERIFY(info.exists());
  QVERIFY(file2.openForRead());
  QVERIFY(file2.isOpen());
  QVERIFY(file2.remove());
  info.refresh();
  QVERIFY(!info.exists());
  QVERIFY(!file2.openForRead());
  QVERIFY(!file2.isOpen());
}

//----------------------------------------------------------------------------
QTEST_MAIN(MiscCheck)
