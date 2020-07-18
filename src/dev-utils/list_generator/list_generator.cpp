/***************************************************************************
 *   Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include <qdir.h>
#include <qfile.h>
#include <qtextstream.h>
#include <kapplication.h>
#include <kcmdlineargs.h>
#include <kaboutdata.h>

#include "devices/base/device_group.h"
#include "devices/list/device_list.h"
#include "devices/pic/base/pic.h"
#include "progs/list/prog_list.h"
#include "tools/list/tool_list.h"
#include "progs/list/device_info.h"
#include "libgui/device_gui.h"
#include "progs/bootloader/base/bootloader_prog.h"
#include "libgui/toplevel.h"
#include "libgui/project_manager.h"

//----------------------------------------------------------------------------
QString imageHtml(Group::Support support)
{
  switch (support.type()) {
    case Group::Support::Untested: return "<img src=\\\"untested.png\\\" alt=\\\"untested\\\"/>";
    case Group::Support::None:     return "<img src=\\\"unsupported.png\\\" alt=\\\"not supported\\\"/>";
    case Group::Support::Tested:   return "<img src=\\\"tested.png\\\" alt=\\\"tested\\\"/>";
    case Group::Support::Nb_Types: break;
  }
  return "?";
}

QString toolSupportHtml(const QString &name, const Device::Data &data)
{
  const Group::Base *group = Tool::Lister::instance().group(name);
  if (group == NULL) qFatal("No group \"%s\"", name.latin1());
  Group::Support support = group->deviceData(data.name()).support;
  return "echo \"<td class=\\\"devices_value\\\">" + imageHtml(support) + "</td>\";\n";
}

QString progSupportHtml(const QString &name, const Device::Data &data)
{
  const Group::Base *group = Programmer::Lister::instance().group(name);
  if (group == NULL) qFatal("No group \"%s\"", name.latin1());
  Group::Support support = group->deviceData(data.name()).support;
  return "echo \"<td class=\\\"devices_value\\\">" + imageHtml(support) + "</td>\";\n";
}

int function(const QDir &dir)
{
  QFile phpFile(dir.filePath("devices.php"));
  if ( !phpFile.open(IO_WriteOnly) ) qFatal("Could not open \"%s\" for writing.", phpFile.name().latin1());
  QTextStream phps(&phpFile);
  phps << "<?php" << endl;

  QStringList devices = Device::Lister::instance().supportedDevices();
  int k = 1;
  FOR_EACH(Pic::Architecture, arch) {
    bool hasDevice = false;
    for (int i=0; i<devices.count(); i++) {
      const Device::Data *data = Device::Lister::instance().data(devices[i]);
      Q_ASSERT(data);
      if ( data->group().name()!="pic" ) continue;
      if ( static_cast<const Pic::Data *>(data)->architecture()!=arch ) continue;
      if ( !hasDevice ) {
        hasDevice = true;
        phps << "tableStart(\"" << arch.key() << "\", \"" << arch.label() << "\");" << endl;
      }
      phps << "echo \"<tr class=\\\"" << ((k%2)==0 ? "even_row" : "odd_row") /*<< "\" id=\"" << Pic::ARCHITECTURE_DATA[k].name << "\" style=\"display:none\"" */ << "\\\">\";" << endl;
      phps << "echo \"<td class=\\\"devices\\\"><a class=\\\"devices\\\" href=\\\"devices.php?device=" << data->name() << "\\\">" << data->name() << "</a></td>\";" << endl;
      phps << "echo \"<td class=\\\"empty_col\\\"></td>\";" << endl;
      phps << toolSupportHtml("gputils", *data);
      phps << toolSupportHtml("sdcc", *data);
      phps << "echo \"<td class=\\\"empty_col\\\"></td>\";" << endl;
      phps << progSupportHtml("direct", *data);
      phps << progSupportHtml("icd1", *data);
      phps << progSupportHtml("icd2", *data);
      phps << progSupportHtml("pickit1", *data);
      phps << progSupportHtml("pickit2", *data);
      phps << progSupportHtml("psp", *data);
      phps << "echo \"<td class=\\\"empty_col\\\"></td>\";" << endl;
      phps << progSupportHtml("icd2_debugger", *data);
      phps << progSupportHtml("gpsim", *data);
      phps << "echo \"<td class=\\\"empty_col\\\"></td>\";" << endl;
      phps << progSupportHtml("tbl_bootloader", *data);
      phps << progSupportHtml("picdem_bootloader", *data);
      phps << progSupportHtml("pickit2_bootloader", *data);
      phps << "echo \"</tr>\\n\";" << endl;
      phps << endl;
      ++k;
    }
    if (hasDevice) phps << "echo \"</table>\\n\";" << endl;
  }

  (void)new ProjectManager::View(0); // ### DUMMY: needed for linking !!??

  phps << "?>" << endl;
  return 0;
}
