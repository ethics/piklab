/***************************************************************************
 *   Copyright (C) 2007 Nicolas Hadacek <hadacek@kde.org>                  *
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
#include "devices/pic/base/pic.h"
#include "devices/pic/pic/pic_memory.h"
#include "devices/base/device_group.h"
#include "common/global/about.h"

//-----------------------------------------------------------------------------
const CLI::CommandData COMMAND_DATA[] = {
  { "check",    CLI::NeedInput | CLI::NeedCorrectInput, 0, 0, I18N_NOOP("Check hex file for correctness (if a device is specified, check compatibility).") },
  { "info",     CLI::NeedInput | CLI::NeedCorrectInput, 0, 0, I18N_NOOP("Return information about hex file.") },
  { "fix",      CLI::NeedInput | CLI::NeedOutput,       0, 0, I18N_NOOP("Clean hex file and fix errors (wrong CRC, truncated line, truncated file).") },
  { "compare",  CLI::NeedInput | CLI::NeedSecondInput  | CLI::NeedCorrectInput, 0, 0, I18N_NOOP("Compare two hex files.") },
  { "checksum", CLI::NeedInput | CLI::NeedCorrectInput | CLI::NeedDevice,       0, 0, I18N_NOOP("Return checksum.") },
  { "create",   CLI::NeedOutput | CLI::NeedDevice,      0, 0, I18N_NOOP("Create an hex file for the specified device.") },
  CLI::NullCommandData
};

const CLI::PropertyData PROPERTY_DATA[] = {
  { "input",  "input <file>",  0,   I18N_NOOP("First input hex file."), 0, 0 },
  { "input2", "input2 <file>", 0,   I18N_NOOP("Second input hex file."), 0, 0 },
  { "output", "output <file>", 0,   I18N_NOOP("Output hex file."), 0, 0 },
  { "device", "device <name>", "d", I18N_NOOP("Target device."), "device-list", I18N_NOOP("Return the list of supported devices.") },
  { "fill",   "fill <value>",  0,   I18N_NOOP("Fill option."),   "fill-list",   I18N_NOOP("Return the list of supported fill options.") },
    CLI::NullPropertyData
};

const CLI::FillOption CLI::NullFillOption = { 0, 0 };
const CLI::FillOption FILL_OPTIONS[] = {
  { "blank",          I18N_NOOP("Fill with blank values (default).") },
  { "zero",           I18N_NOOP("Fill with zeroes.") },
  { "checksum_check", I18N_NOOP("Fill for checksum verification (cf datasheets).") },
  CLI::NullFillOption
};

//-----------------------------------------------------------------------------
CLI::Main::Main()
  : MainBase("piklab-hex", I18N_NOOP("Piklab Hex Utility"),
             I18N_NOOP("Command-line utility to manipulate hex files.")),
  _device(NULL)
{}

const CLI::CommandData& CLI::Main::commandData(uint i) const
{
  return COMMAND_DATA[i];
}

const CLI::PropertyData& CLI::Main::propertyData(uint i) const
{
  return PROPERTY_DATA[i];
}

CLI::ExitCode CLI::Main::setInputHex(const CommandProperties& properties)
{
  PURL::File file(input(), view());
  if (!file.openForRead()) return FILE_ERROR;
  _errors = _source1.load(file.stream(), _format);
  if ((properties & NeedCorrectInput) && !_errors.isEmpty()) {
    QString s = (properties & NeedSecondInput ? i18n("First hex file: ") : QString::null);
    for (uint i=0; i<uint(_errors.count()); i++) log(Log::LineType::Error, s + _errors[i].message());
    return EXEC_ERROR;
  }
  return OK;
}

CLI::ExitCode CLI::Main::setSecondInputHex(const CommandProperties& properties)
{
  PURL::File file(secondInput(), view());
  if (!file.openForRead()) return FILE_ERROR;
  _errors = _source2.load(file.stream(), _format);
  if ((properties & NeedCorrectInput) && !_errors.isEmpty()) {
    QString s = i18n("Second hex file: ");
    for (uint i=0; i<uint(_errors.count()); i++) log(Log::LineType::Error, s + _errors[i].message());
    return EXEC_ERROR;
  }
  return OK;
}

CLI::ExitCode CLI::Main::prepareCommand(const CommandProperties& properties)
{
  ExitCode code = MainBase::prepareCommand(properties);
  if (code != OK) return code;
  if (properties & NeedInput) {
    code = setInputHex(properties);
    if (code != OK) return code;
  }
  if (properties & NeedSecondInput) {
    code = setSecondInputHex(properties);
    if (code != OK) return code;
  }
  if (_device == NULL
      && (properties & NeedDevice) ) return errorExit(i18n("Device not specified."), ARG_ERROR);
  return OK;
}

CLI::ExitCode CLI::Main::executeCommand(const QString &command)
{
  if (_device != NULL) _memory = _device->group().createMemory(*_device);

  if ( command=="check" ) {
    if (_device == NULL) return okExit(i18n("Hex file is valid."));
    QStringList warnings;
    Device::Memory::WarningTypes wtypes = _memory->fromHexBuffer(_source1, warnings);
    if ( wtypes==Device::Memory::NoWarning ) return okExit(i18n("Hex file is compatible with device \"%1\".").arg(_device->name()));
    return errorExit(warnings.join("\n"), EXEC_ERROR);
  }
  if ( command=="info" ) {
    Log::KeyList  keys;
    keys.append(i18n("Format:"), HexBuffer::FORMATS[_format]);
    uint nbWords = 0, start = 0, end = 0;
    HexBuffer::const_iterator it;
    for (it=_source1.begin(); it!=_source1.end(); ++it) {
      if ( !it.data().isInitialized() ) continue;
      nbWords++;
      if ( nbWords==1 ) {
        start = it.key();
        end = it.key();
      } else {
        start = qMin(start, it.key());
        end = qMax(end, it.key());
      }
    }
    keys.append(i18n("No. of Words:"), toHexLabelAbs(nbWords));
    if ( nbWords!=0 ) {
      uint nbc = nbChars(NumberBase::Hex, end);
      keys.append(i18n("Start:"), toHexLabel(start, nbc));
      keys.append(i18n("End:"), toHexLabel(end, nbc));
    }
    keys.display(view());
    return okExit(i18n("Hex file is valid."));
  }
  if ( command=="fix" ) {
    for (uint i=0; i<uint(_errors.count()); i++)
      if ( _errors[i].type==HexBuffer::UnrecognizedFormat ) return errorExit(i18n("Hex file cannot be fixed because the format was not recognized or is inconsistent."), EXEC_ERROR);
    if ( _format==HexBuffer::Nb_Formats ) _format = HexBuffer::IHX32;
    PURL::File dest(output(), view());
    if ( !dest.openForWrite() ) return FILE_ERROR;
    _source1.savePartial(dest.stream(), _format);
    _source1.saveEnd(dest.stream());
    if ( !dest.close() ) return FILE_ERROR;
    if ( _errors.isEmpty() ) return okExit(i18n("Hex file cleaned."));
    return okExit(i18n("Hex file cleaned and fixed."));
  }
  if ( command=="compare" ) {
    bool firstInSecond = true;
    for (HexBuffer::const_iterator it =_source1.begin(); it != _source1.end(); ++it) {
      if (it.data().maskWith(0xFFFF) != _source2[it.key()].maskWith(0xFFFF)) {
        firstInSecond = false;
        break;
      }
    }
    bool secondInFirst = true;
    uint address = 0;
    BitValue value1;
    BitValue value2;
    for (HexBuffer::const_iterator it = _source2.begin(); it != _source2.end(); ++it) {
      address = it.key();
      value2 = it.data().maskWith(0xFFFF);
      value1 = _source1[it.key()].maskWith(0xFFFF);
      if (value1 != value2) {
        secondInFirst = false;
        break;
      }
    }
    if ( firstInSecond && secondInFirst ) return okExit(i18n("The two hex files have the same content."));
    if (firstInSecond) log(Log::LineType::Information, i18n("The first hex file is a subset of the second one."));
    if (secondInFirst) log(Log::LineType::Information, i18n("The second hex file is a subset of the first one."));
    return errorExit(i18n("The two hex files are different at address %1: (%2 != %3).")
                     .arg(toHexLabel(address, 8)).arg(toHexLabel(value1, 4)).arg(toHexLabel(value2, 4)), EXEC_ERROR);
  }
  if ( command=="checksum" ) {
    QStringList warnings;
    _memory->fromHexBuffer(_source1, warnings);
    for (uint i=0; i<uint(warnings.count()); i++) log(Log::LineType::Warning, warnings[i]);
    log(Log::LineType::Warning, i18n("Checksum computation is experimental and is not always correct!")); // #### REMOVE ME
    BitValue cs = _memory->checksum();
    log(Log::LineType::Normal, i18n("Checksum: %1").arg(toHexLabel(cs, 4)));
    if ( _device->group().name()=="pic" ) {
      BitValue ucs = static_cast<const Pic::Memory&>(*_memory).unprotectedChecksum();
      if ( ucs!=cs ) log(Log::LineType::Information, i18n("Unprotected checksum: %1").arg(toHexLabel(ucs, 4)));
    }
    return OK;
  }
  if ( command=="create" ) {
    if ( _fill.isEmpty() || _fill=="blank" ) ; // default
    else if ( _fill=="zero" ) _memory->fill(0x0);
    else if ( _fill=="checksum_check" ) {
      if ( _device->group().name()=="pic" ) static_cast<Pic::Memory&>(*_memory).checksumCheckFill();
    } else {
      bool ok;
      uint value = fromAnyLabel(_fill, &ok);
      ASSERT(ok);
      _memory->fill(value);
    }
    PURL::File dest(output(), view());
    if ( !dest.openForWrite() ) return FILE_ERROR;
    _memory->save(dest.stream(), HexBuffer::IHX32);
    if ( !dest.close() ) return FILE_ERROR;
    return okExit(i18n("File created."));
  }
  ASSERT(false);
  return ARG_ERROR;
}

CLI::ExitCode CLI::Main::doExecuteSetCommand(const QString &property, const QString &value)
{
  ExitCode code = MainBase::doExecuteSetCommand(property, value);
  if (code != NOT_PROCESSED) return code;

  if (property == "device") {
    if ( value.isEmpty() ) {
      _device = NULL;
      return OK;
    }
    QString s = value.upper();
    _device = Device::Lister::instance().data(s);
    if (_device == NULL) return errorExit(i18n("Unknown device \"%1\".").arg(s), ARG_ERROR);
    return OK;
  }
  if ( property=="fill" ) {
    _fill = value;
    for (uint i=0; FILL_OPTIONS[i].name; i++)
      if ( value==FILL_OPTIONS[i].name ) return OK;
    bool ok;
    (void)fromAnyLabel(value, &ok);
    if ( !ok ) return errorExit(i18n("Number format not recognized."), ARG_ERROR);
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
  if ( property=="fill" ) {
    if (!_fill.isEmpty()) value = _fill;
    return OK;
  }
  return NOT_PROCESSED;
}

CLI::ExitCode CLI::Main::list(const QString &command)
{
  ExitCode code = MainBase::list(command);
  if (code != NOT_PROCESSED) return code;
  if ( command=="device-list" ) return deviceList();
  if ( command=="fill-list" ) return fillOptionList();
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

CLI::ExitCode CLI::Main::fillOptionList()
{
  Log::KeyList keys(i18n("Fill options:"));
  for (uint i=0; FILL_OPTIONS[i].name; i++)
    keys.append(FILL_OPTIONS[i].name, i18n(FILL_OPTIONS[i].description));
  keys.append(i18n("<value>"), i18n("Fill with the specified numeric value."));
  keys.display(view());
  return OK;
}

//-----------------------------------------------------------------------------
int main(int argc, char **argv)
{
  CLI::Main main;
  main.initArgs(argc, argv, I18N_NOOP("Hex filename(s)."));
  return main.run();
}
