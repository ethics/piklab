/***************************************************************************
 *   Copyright (C) 2007-2009 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "main.h"
#include "main.moc"

#include "common/global/pfile.h"
#include "common/cli/cli_log.h"
#include "devices/list/device_list.h"
#include "devices/base/device_group.h"
#include "common/global/about.h"
#include "coff/base/text_coff.h"
#include "coff/base/coff_archive.h"

//-----------------------------------------------------------------------------
const CLI::CommandData COMMAND_DATA[] = {
  { "info",      CLI::NeedInput, 0, 0, I18N_NOOP("Return general informations.") },
  { "variables", CLI::NeedInput, 0, 0, I18N_NOOP("Return informations about variables (for object).") },
  { "symbols",   CLI::NeedInput, 0, 0, I18N_NOOP("Return informations about symbols.") },
  { "sections",  CLI::NeedInput, 0, 0, I18N_NOOP("Return informations about sections (for object).") },
  { "lines",     CLI::NeedInput, 0, 0, I18N_NOOP("Return informations about code lines (for object).") },
  { "files",     CLI::NeedInput, 0, 0, I18N_NOOP("Return informations about files.") },
  CLI::NullCommandData
};

const CLI::PropertyData PROPERTY_DATA[] = {
  { "input",  "input <file>",  0,   I18N_NOOP("Input COFF filepath."), 0, 0 },
  { "device", "device <name>", "d", I18N_NOOP("Target device."), "device-list", I18N_NOOP("Return the list of supported devices.") },
  CLI::NullPropertyData
};

//-----------------------------------------------------------------------------
CLI::Main::Main()
: MainBase("piklab-coff", I18N_NOOP("Piklab COFF Utility"),
           I18N_NOOP("Command-line utility to view COFF files.")),
  _device(NULL)
{
}

const CLI::CommandData& CLI::Main::commandData(uint i) const
{
  return COMMAND_DATA[i];
}

const CLI::PropertyData& CLI::Main::propertyData(uint i) const
{
  return PROPERTY_DATA[i];
}

CLI::ExitCode CLI::Main::prepareCommand(const CommandProperties& properties)
{
  ExitCode code = MainBase::prepareCommand(properties);
  if (code == OK) {
    if (_device == NULL
        && (properties & NeedDevice) ) return errorExit(i18n("Device not specified."), ARG_ERROR);
  }
  return code;
}

QString CLI::Main::prettyAuxSymbol(const Coff::AuxSymbol &aux)
{
  QString s = (aux.type()==Coff::AuxSymbolType::Nb_Types ? "?" : aux.type().label());
  switch (aux.type().type()) {
    case Coff::AuxSymbolType::Direct: break;
    case Coff::AuxSymbolType::File: s += "=" + static_cast<const Coff::AuxSymbolFile &>(aux).filename(); break;
    case Coff::AuxSymbolType::Identifier: s += "=" + static_cast<const Coff::AuxSymbolIdentifier &>(aux).string(); break;
    case Coff::AuxSymbolType::Section: break;
    case Coff::AuxSymbolType::Nb_Types: break;
  }
  return s;
}

QString CLI::Main::prettySymbol(const Coff::Symbol &sym)
{
  QStringList saux;
  for (uint i=0; i<uint(sym.auxSymbols().count()); i++) saux += prettyAuxSymbol(*sym.auxSymbols()[i]);
  QString s = (sym.auxSymbols().count()!=0 ? " aux=[" + saux.join(" ") + "]" : QString::null);
  QStringList slist;
  if ( sym.sectionType()!=Coff::SymbolSectionType::Nb_Types ) slist += QString("sectionType=\"%1\"").arg(sym.sectionType().label());
  if ( sym.symbolClass()!=Coff::SymbolClass::Nb_Types ) slist += QString("class=\"%1\"").arg(sym.symbolClass().label());
  if ( sym.type()!=Coff::SymbolType::Nb_Types ) {
    slist += QString("type=\"%1\"").arg(sym.type().label());
    if ( sym.derivedType()!=Coff::SymbolDerivedType::Nb_Types ) slist += QString("/\"%1\"").arg(sym.derivedType().label());
  }
  return slist.join(" ") + s;
}

CLI::ExitCode CLI::Main::executeCommandArchive(const QString &command, Log::KeyList &keys)
{
  Coff::Archive coff(input());
  if ( !coff.parse(*this) ) return ARG_ERROR;

  if ( command=="info" ) {
    keys = coff.information();
    keys.setTitle(QString::null);
    return OK;
  }

  if ( command=="symbols" ) {
    keys = coff.symbolsInformation();
    return OK;
  }

  if ( command=="files" ) {
    keys = coff.membersInformation();
    return OK;
  }

  return errorExit(i18n("Command not available for COFF of type Archive."), ARG_ERROR);
}

CLI::ExitCode CLI::Main::executeCommandObject(const QString &command, Log::KeyList &keys)
{
  Coff::TextObject coff(_device, input());
  if ( !coff.parse(*this) ) return ARG_ERROR;
  ASSERT( coff.device()==0 || coff.device()->group().name()=="pic" );
  const Pic::Data *pdata = static_cast<const Pic::Data *>(coff.device());
  uint nbCharsAddress = (pdata ? pdata->nbCharsAddress() : 8);

  if ( command=="info" ) {
    keys = coff.information();
    keys.setTitle(QString::null);
    return OK;
  }

  if ( command=="variables" ) {
    keys.setTitle(i18n("Variables:"));
    QMap<QString, Address>::const_iterator it;
    for (it=coff.variables().begin(); it!=coff.variables().end(); ++it)
      keys.append(it.key(), toHexLabelAbs(it.data()));
    return OK;
  }

  if ( command=="symbols" ) {
    keys.setTitle(i18n("Symbols:"));
    for (uint i=0; i<coff.nbSymbols(); i++) {
      if ( coff.symbol(i)==0 || coff.symbol(i)->isAuxSymbol() ) continue;
      const Coff::Symbol &sym = static_cast<const Coff::Symbol &>(*coff.symbol(i));
      keys.append(sym.name(), prettySymbol(sym));
    }
    return OK;
  }

  if ( command=="sections" ) {
    keys.setTitle(i18n("Sections:"));
    for (uint i=0; i<coff.nbSections(); i++) {
      const Coff::Section *s = coff.Coff::Object::section(i);
      keys.append(s->name(), i18n("type=\"%1\" address=%2 size=%3 flags=%4")
        .arg(s->type()==Coff::SectionType::Nb_Types ? "?" : s->type().label())
        .arg(toHexLabel(s->address(), nbCharsAddress)).arg(toHexLabel(s->size(), nbCharsAddress)).arg(toHexLabel(s->flags(), 8)));
    }
    return OK;
  }

  if ( command=="lines" ) {
    keys.setTitle(i18n("Source Lines:"));
    keys.append(i18n(" Filename:Line"), i18n("Address"));
    for (uint i=0; i<coff.nbSections(); i++) {
      const Coff::Section *s = coff.Coff::Object::section(i);
      bool first = true;
      for (uint k=0; k<uint(s->lines().count()); k++) {
        if (first) {
          first = false;
          keys.append(i18n("section \"%1\":").arg(s->name()), QString::null);
        }
        const Coff::CodeLine *cl = s->lines()[k];
        QString key = cl->filename() + ":"  + QString::number(cl->line());
        if ( !cl->address().isValid() ) {
          const Coff::Symbol &sym = *cl->symbol();
          keys.append(key, i18n("symbol \"%1\"").arg(sym.name()) + prettySymbol(sym));
        } else keys.append(key, toHexLabel(cl->address(), nbCharsAddress));
      }
    }
    return OK;
  }

  if ( command=="files" ) {
    keys.setTitle(i18n("Files:"));
    QStringList::const_iterator it;
    for (it=coff.filenames().begin(); it!=coff.filenames().end(); ++it)
      keys.append(*it, QString::null);
    return OK;
  }

  return errorExit(i18n("Command not available for COFF of type Object."), ARG_ERROR);
}

CLI::ExitCode CLI::Main::executeCommand(const QString &command)
{
  CoffType type = Coff::identify(input(), *this);
  if ( type==CoffType::Nb_Types ) return ARG_ERROR;
  Log::KeyList keys;
  if ( command=="info" ) keys.append(i18n("COFF type:"), type.label());
  ExitCode code = ARG_ERROR;
  switch (type.type()) {
    case CoffType::Archive: code = executeCommandArchive(command, keys); break;
    case CoffType::Object: code = executeCommandObject(command, keys); break;
    case CoffType::Nb_Types: ASSERT(false); break;
  }
  if ( code==OK ) keys.display(view());
  return code;
}

CLI::ExitCode CLI::Main::doExecuteSetCommand(const QString &property, const QString &value)
{
  ExitCode code = MainBase::doExecuteSetCommand(property, value);
  if (code != NOT_PROCESSED) return code;

  if (property == "device") {
    if (value.isEmpty()) {
      _device = NULL;
      return OK;
    }
    QString s = value.upper();
    _device = Device::Lister::instance().data(s);
    if (_device == NULL) return errorExit(i18n("Unknown device \"%1\".").arg(s), ARG_ERROR);
    return OK;
  }
  return NOT_PROCESSED;
}

CLI::ExitCode CLI::Main::doExecuteGetCommand(const QString &property, QString& value)
{
  ExitCode code = MainBase::doExecuteGetCommand(property, value);
  if (code != NOT_PROCESSED) return code;

  if (property == "device") {
    if (_device != NULL) value = _device->name();
    return OK;
  }
  return NOT_PROCESSED;
}

CLI::ExitCode CLI::Main::list(const QString &command)
{
  ExitCode code = MainBase::list(command);
  if (code != NOT_PROCESSED) return code;
  if (command == "device-list") return deviceList();
  ASSERT(false);
  return NOT_PROCESSED;
}

CLI::ExitCode CLI::Main::deviceList()
{
  log(Log::LineType::Normal, i18n("Supported devices:"));
  QStringList devices = Device::Lister::instance().supportedDevices();
  qSort(devices);
  QString s;
  QStringList::const_iterator it;
  for (it=devices.begin(); it!=devices.end(); ++it) s += " " + *it;
  log(Log::LineType::Normal, s + "\n");
  return OK;
}

//-----------------------------------------------------------------------------
int main(int argc, char **argv)
{
  CLI::Main main;
  main.initArgs(argc, argv, I18N_NOOP("COFF filepath."));
  return main.run();
}
