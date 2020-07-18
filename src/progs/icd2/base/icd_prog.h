/***************************************************************************
 *   Copyright (C) 2005-2006 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef ICD_PROG_H
#define ICD_PROG_H

#include "icd.h"
namespace PURL { class File; }

namespace Icd
{
//----------------------------------------------------------------------------
class ProgrammerBase : public ::Programmer::PicBase
{
public:
  ProgrammerBase(const Programmer::Group &group, const Pic::Data *data, const char *name)
    : ::Programmer::PicBase(group, data, name) {}
  Hardware *hardware() const { return static_cast<Hardware *>(_hardware.get()); }
  virtual bool readFirmwareVersion();

protected:
  virtual bool doUploadFirmware(PURL::File &file);
};

//----------------------------------------------------------------------------
class Group : public ::Programmer::PicGroup
{
public:
};

} // namespace

#endif
