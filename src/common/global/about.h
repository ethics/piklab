/***************************************************************************
 *   Copyright (C) 2005-2006 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#ifndef ABOUT_H
#define ABOUT_H

#include "common/common/global.h"
#include "common/common/key_enum.h"
#include "log.h"

namespace Piklab
{
//-----------------------------------------------------------------------------
struct UrlTypeData {
  const char *url;
};
BEGIN_DECLARE_ENUM(UrlType)
  Homepage = 0, Support, BugReport
END_DECLARE_ENUM_DATA_ONLY(UrlType, UrlTypeData)

//-----------------------------------------------------------------------------
struct CmdLineOption {
  const char* name;
  const char* description;
  const char* def;
};

class OptionList : public KCmdLineOptions
{
public:
  OptionList(const CmdLineOption* options = NULL);
  void append(const CmdLineOption& option);
};

//-----------------------------------------------------------------------------
void initArgs(const KAboutData& aboutData, int argc, char** argv, const KCmdLineOptions& options);
void createApplication(const QString& executable, int argc, char **argv, bool gui, Log::Generic* log);

//-----------------------------------------------------------------------------
class AboutData : public KAboutData
{
public:
  AboutData(const char *executable, const char *name, const char *description);
};

} // namespace

#endif
