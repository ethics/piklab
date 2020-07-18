/***************************************************************************
 *   Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "jalv2.h"

#include "jalv2_compile.h"
#include "jalv2_config.h"
#include "jalv2_generator.h"
#include "devices/base/device_group.h"
#include "devices/pic/pic/pic_memory.h"
#include "devices/list/device_list.h"
#include "common/global/process.h"

//----------------------------------------------------------------------------
bool JALV2::Base::checkExecutableResult(Tool::ExecutableType, QStringList &lines) const
{
  QStringList tmp;
  for (int i=0; i<lines.count(); i++)
    if ( !lines[i].contains('\r') ) tmp += lines[i]; // ??
  lines = tmp;
  return ( lines.count()>0 && lines[0].startsWith("jal") );
}

//----------------------------------------------------------------------------
QString JALV2::Group::informationText() const
{
  return i18n("<a href=\"%1\">JAL V2</a> is a new compiler for the high-level language JAL.").arg("http://www.casadeyork.com/jalv2");
}

Tool::Base* JALV2::Group::baseFactory(Tool::Category category) const
{
  if ( category==Tool::Category::Compiler ) return new ::JALV2::Base;
  return NULL;
}

QValueList<const Device::Data *> JALV2::Group::getSupportedDevices(const QString &) const
{
  QValueList<const Device::Data*> list;
  QStringList devices = Device::Lister::instance().group("pic")->supportedDevices();
  for (QStringList::const_iterator it = devices.begin(); it != devices.end(); ++it) {
    const Pic::Data& data = static_cast<const Pic::Data&>(*Device::Lister::instance().data(*it));
    if (data.architecture() == Pic::Architecture::MidRange
        || data.architecture() == Pic::Architecture::EMidRange
        || data.is18Family()) list.append(&data);
  }
  return list;
}

Compile::Process *JALV2::Group::processFactory(const Compile::Data &data) const
{
  ASSERT( data.category==Tool::Category::Compiler );
  return new CompileFile;
}

Compile::Config *JALV2::Group::configFactory(::Project& project) const
{
  return new Config(project);
}

Tool::SourceGenerator *JALV2::Group::sourceGeneratorFactory() const
{
  return new JALV2::SourceGenerator;
}
