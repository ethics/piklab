/***************************************************************************
 *   Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "html_generator.h"

#include <qdir.h>
#include <qfile.h>
#include <qtextstream.h>
#include <kapplication.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>

#include "devices/list/device_list.h"
#include "progs/list/prog_list.h"
#include "tools/list/tool_list.h"
#include "libgui/device_gui.h"
#include "devices/base/device_group.h"
#include "progs/list/device_info.h"

//----------------------------------------------------------------------------
void writePixmap(const QDir& dir, const QString& name, const QPixmap& pixmap)
{
  if (pixmap.isNull()) return;
  QImage image = pixmap.toImage();
  QString filepath = dir.filePath(name);
  QImage old;
  if ( !old.load(filepath) || old!=image ) {
    qDebug("  add image %s", name.latin1());
    if ( !image.save(filepath, "PNG") ) qFatal("Could not write \"%s\".", filepath.latin1());
  }
}

int function(const QDir &dir)
{
  QStringList devices = Device::Lister::instance().supportedDevices();
  for (int i=0; i<devices.count(); i++) {
    // html file
    QFile file(dir.filePath(devices[i] + ".htm"));
    qDebug("%s", file.name().latin1());
    if ( !file.open(IO_WriteOnly) ) qFatal("Could not open \"%s\" for writing.", file.name().latin1());
    QTextStream fs(&file);
    const Device::Data *data = Device::Lister::instance().data(devices[i]);
    Q_ASSERT(data);
    fs << "<table><tr><td>";
    QString s = htmlTableRow("Information:", Device::webpageHtml(*data));
    fs << Device::htmlInfo(*data, "devices.php?device=%1", s);
    fs << Device::supportedHtmlInfo(*data);
    fs << "<hr />\n";
    fs << Device::voltageFrequencyHtml(*data, "devices/");
    fs << "</td><td valign=\"top\">";
    fs << Device::pinsHtml(*data, "devices/");
    fs << "</td></tr></table";

    for (int k=0; k<data->frequencyRanges().count(); k++) {
      const Device::FrequencyRange &fr = data->frequencyRanges()[k];
      QString name = Device::voltagePixmapLabel(*data, fr);
      QPixmap pixmap = Device::voltagePixmap(fr);
      writePixmap(dir, name, pixmap);
    }

    writePixmap(dir, Device::pinsPixmapLabel(*data), Device::pinsPixmap(*data));
  }

  return 0;
}
