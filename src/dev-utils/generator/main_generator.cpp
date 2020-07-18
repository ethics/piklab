/***************************************************************************
 *   Copyright (C) 2005 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "common/global/about.h"
#include "main_generator.h"

const Piklab::CmdLineOption OPTIONS[] = {
  { "+dir", I18N_NOOP("Output directory."), 0 },
  { 0, 0, 0}
};

int main(int argc, char **argv)
{
  const Piklab::AboutData about("generator", "generator", "");
  static Piklab::OptionList list;
  for (uint i = 0; OPTIONS[i].name != NULL; ++i)
    list.append(OPTIONS[i]);
  Piklab::initArgs(about, argc, argv, list);
  Piklab::createApplication("generator", argc, argv, true, NULL);

  KGlobal::locale()->setCountry("us", NULL); // consistent number format...

  KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
  if ( args->count()==0 ) qFatal("Output directory must be provided.");
  QDir dir(args->url(0).path());
  if ( !dir.exists() ) qFatal("Directory \"%s\" does not exists.", dir.absPath().latin1());

  return function(dir);
}
