/***************************************************************************
 *   Copyright (C) 2005 Nicolas Hadacek <hadacek@kde.org>                  *
 *   Copyright (C) 2003-2004 Alain Gibaud <alain.gibaud@free.fr>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef GLOBAL_CONFIG_H
#define GLOBAL_CONFIG_H

#include "progs/base/generic_prog.h"
#include "common/global/purl.h"
#include "tools/base/tool_group.h"
#include "log_view.h"

BEGIN_DECLARE_CONFIG(BaseGlobalConfig)
  AutoRebuildModified, ProgramAfterBuild, UserIdSetToChecksum,
  ShowTabCloseButton, ParseDisassemblyListingAfterBuild
END_DECLARE_CONFIG(BaseGlobalConfig, "")

namespace GlobalConfig
{
extern PURL::Url openedProject();
extern void writeOpenedProject(const PURL::Url &p);
extern PURL::UrlList openedFiles();
extern void writeOpenedFiles(const PURL::UrlList &files);
extern void writeDebugLevel(Log::DebugLevel level);
extern Log::DebugLevel debugLevel();
extern void writeLogOutputType(Log::OutputType type);
extern Log::OutputType logOutputType();
extern void writeShowLineNumbers(bool show);
extern bool showLineNumbers();
} // namespace

#endif
