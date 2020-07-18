/***************************************************************************
 *   Copyright (C) 2005-2009 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "cmdline.h"
#include "cmdline.moc"

#include <signal.h>
#include <memory>

#include "devices/list/device_list.h"
#include "devices/base/device_group.h"
#include "common/global/about.h"
#include "progs/base/prog_config.h"
#include "progs/base/hardware_config.h"
#include "devices/pic/pic/pic_memory.h"
#include "devices/pic/prog/pic_prog.h"
#include "progs/list/prog_list.h"
#include "coff/base/text_coff.h"
#include "devices/pic/base/pic_register.h"
#include "devices/pic/prog/pic_debug.h"
#include "common/cli/cli_log.h"
#include "cli_prog_manager.h"
#include "cli_debug_manager.h"

//-----------------------------------------------------------------------------
const CLI::CommandData COMMAND_DATA[] = {
  { "connect",         CLI::NeedProgrammer | CLI::NeedDevice, 0, 0, I18N_NOOP("Connect programmer.") },
  { "run",             CLI::NeedProgrammer | CLI::NeedDevice, 0, 0, I18N_NOOP("Run device (release reset).") },
  { "stop",            CLI::NeedProgrammer | CLI::NeedDevice, 0, 0, I18N_NOOP("Stop device (hold reset).") },
  { "program",         CLI::NeedProgrammer | CLI::NeedInput  | CLI::NeedDevice, 0, 3, I18N_NOOP("Program device memory: \"program [-r <range] [<hexfilename>]\".") },
  { "verify",          CLI::NeedProgrammer | CLI::NeedInput  | CLI::NeedDevice, 0, 3, I18N_NOOP("Verify device memory: \"verify [-r <range] [<hexfilename>]\".") },
  { "read",            CLI::NeedProgrammer | CLI::NeedOutput | CLI::NeedDevice, 0, 3, I18N_NOOP("Read device memory: \"read [-r <range] [<hexfilename>]\".") },
  { "erase",           CLI::NeedProgrammer | CLI::NeedDevice, 0, 2, I18N_NOOP("Erase device memory: \"erase [-r <range]\".") },
  { "blank_check",     CLI::NeedProgrammer | CLI::NeedDevice, 0, 2, I18N_NOOP("Blank check device memory: \"blank_check [-r <range>]\".") },
  { "upload_firmware", CLI::NeedProgrammer | CLI::NeedInput,  1, 1, I18N_NOOP("Upload firmware to programmer: \"upload_firmware <hexfilename>\".") },
  CLI::NullCommandData
};

const CLI::CommandData INTERACTIVE_COMMAND_DATA[] = {
  { "register-list",   CLI::ListCommand,    0, 0, I18N_NOOP("Display the list of registers.") },
  { "variable-list",   CLI::ListCommand,    0, 0, I18N_NOOP("Display the list of variables.") },
  { "disconnect",      CLI::NeedProgrammer, 0, 0, I18N_NOOP("Disconnect programmer.") },
  { "start",           CLI::NeedDevice | CLI::NeedProgrammer, 0, 0, I18N_NOOP("Start or restart debugging session.") },
  { "step",            CLI::NeedDevice | CLI::NeedProgrammer, 0, 0, I18N_NOOP("Step one instruction.") },
  { "x",               CLI::NeedDevice | CLI::NeedProgrammer, 1, 2, I18N_NOOP("Read or set a register or variable: \"x PORTB\" or \"x W 0x1A\".") },
  { "break",           CLI::NeedDevice | CLI::NeedProgrammer, 0, 2, I18N_NOOP("Set a breakpoint \"break e 0x04\" or list all breakpoints \"break\".") },
  { "clear",           CLI::NeedDevice | CLI::NeedProgrammer, 1, 1, I18N_NOOP("Clear a breakpoint \"clear 2\", or clear all breakpoints \"clear all\".") },
  { "raw-com",         CLI::NeedDevice | CLI::NeedProgrammer, 1, 1, I18N_NOOP("Write and read raw commands to port from given file \"raw-com file\".") },
  CLI::NullCommandData
};

const CLI::PropertyData PROPERTY_DATA[] = {
  { "programmer",   "programmer <name>",  "p", I18N_NOOP("Programmer to use."), "programmer-list", I18N_NOOP("Return the list of supported programmers.") },
  { "hardware",     "hardware <name>",    "h", I18N_NOOP("Programmer hardware configuration to use (for direct programmer)."), "hardware-list", I18N_NOOP("Return the list of supported programmer hardware configurations.") },
  { "device",       "device <name>",      "d", I18N_NOOP("Target device."), "device-list", I18N_NOOP("Return the list of supported devices.") },
  { "format",       "format <name>",      "f", I18N_NOOP("Hex output file format."), "format-list", I18N_NOOP("Return the list of supported hex file formats.") },
  { "port",         "port <name>",        "t", I18N_NOOP("Programmer port (\"usb\" or device such as \"/dev/ttyS0\")"), "port-list", I18N_NOOP("Return the list of detected ports.") },
  { "range",        "range <name>",       "r", I18N_NOOP("Memory range to operate on."), "range-list", I18N_NOOP("Return the list of memory ranges.") },
  { "firmware-dir", "firmware-dir <dir>", 0,   I18N_NOOP("Firmware directory."), 0, 0 },
  { "target-self-powered", "target-self-powered <true|false>", 0, I18N_NOOP("Set if target device is self-powered."), 0, 0 },
  { "input",        0,                    0,   I18N_NOOP("Hex file to be used for programming or verifying."), 0, 0 },
  { "output",       0,                    0,   I18N_NOOP("Hex file to be used for reading."), 0, 0 },
  { "coff",         0,                    0,   I18N_NOOP("COFF file to be used for debugging."), 0, 0 },
  CLI::NullPropertyData
};

//-----------------------------------------------------------------------------
const Programmer::Group*      CLI::_progGroup = NULL;
const Device::Data*           CLI::_device = NULL;
HexBuffer::Format             CLI::_format = HexBuffer::IHX32;
QString                       CLI::_port;
QString                       CLI::_targetSelfPowered;
QString                       CLI::_hardware;
PURL::Directory               CLI::_firmwareDir;
PURL::Url                     CLI::_coffUrl;
std::auto_ptr<Device::Memory> CLI::_memory;

//-----------------------------------------------------------------------------
const CLI::CommandData& CLI::Main::commandData(uint i) const
{
  return COMMAND_DATA[i];
}

const CLI::CommandData& CLI::Main::interactiveCommandData(uint i) const
{
  return INTERACTIVE_COMMAND_DATA[i];
}

const CLI::PropertyData& CLI::Main::propertyData(uint i) const
{
  return PROPERTY_DATA[i];
}

CLI::ExitCode CLI::Main::formatList()
{
  log(Log::LineType::Normal, i18n("Supported hex file formats:"));
  for (uint i=0; i<HexBuffer::Nb_Formats; i++)
    log(Log::LineType::Normal, QString(" ") + HexBuffer::FORMATS[i]);
  return OK;
}

CLI::ExitCode CLI::Main::programmerList()
{
  log(Log::LineType::Normal, i18n("Supported programmers:"));
  for (Programmer::Lister::ConstIterator it = Programmer::Lister::instance().begin();
       it != Programmer::Lister::instance().end();
       ++it)
    log(Log::LineType::Normal, " " + QString(it.data()->name()));
  return OK;
}

CLI::ExitCode CLI::Main::hardwareList()
{
  log(Log::LineType::Normal, i18n("Supported hardware configuration for programmers:"));
  for (Programmer::Lister::ConstIterator it = Programmer::Lister::instance().begin();
       it != Programmer::Lister::instance().end();
       ++it) {
    std::auto_ptr< ::Hardware::Config> config(it.data()->hardwareConfig());
    if (config.get() == NULL) continue;
    FOR_EACH(PortType, type) {
      if ( !it.data()->isPortSupported(type) ) continue;
      log(Log::LineType::Normal, "-" + QString(it.data()->name()) + " [" + type.label() + "]:");
      QStringList list = config->hardwareNames(type);
      for (uint k=0; k<uint(list.count()); k++) log(Log::LineType::Normal, "  " + list[k]);
    }
  }
  return OK;
}

CLI::ExitCode CLI::Main::deviceList()
{
  QStringList devices;
  if ( _progGroup==0 ) {
    log(Log::LineType::Normal, i18n("Supported devices:"));
    devices = Programmer::Lister::instance().supportedDevices();
  } else {
    log(Log::LineType::Normal, i18n("Supported devices for \"%1\":").arg(_progGroup->label()));
    devices = _progGroup->supportedDevices();
  }
  qSort(devices);
  QString s;
  for (QStringList::const_iterator it = devices.begin();
       it != devices.end();
       ++it)
    s += " " + *it;
  log(Log::LineType::Normal, s + "\n");
  return OK;
}

CLI::ExitCode CLI::Main::portList()
{
  if (_progGroup) log(Log::LineType::Normal, i18n("Detected ports supported by \"%1\":").arg(_progGroup->label()));
  else log(Log::LineType::Normal, i18n("Detected ports:"));
  FOR_EACH(PortType, type) {
    if ( _progGroup && !_progGroup->isPortSupported(type) ) continue;
    QString s = "- " + type.label() + ":";
    if ( !Port::isAvailable(type) ) {
      log(Log::LineType::Normal, s + i18n(" support disabled."));
      continue;
    }
    QStringList list = Port::probedDeviceList(type);
    if ( list.count()==0 ) log(Log::LineType::Normal, s + i18n(" no port detected."));
    else {
      log(Log::LineType::Normal, s);
      for (uint k=0; k<uint(list.count()); k++) log(Log::LineType::Normal, "   " + list[k]);
    }
  }
  return OK;
}

CLI::ExitCode CLI::Main::rangeList()
{
  log(Log::LineType::Normal, i18n("Memory ranges for PIC/dsPIC devices:"));
  FOR_EACH(Pic::MemoryRangeType, type) log(Log::LineType::Normal, QString(" %1").arg(type.key()));
  return OK;
}

CLI::ExitCode CLI::Main::prepareCommand(const CommandProperties& properties)
{
  ExitCode code = MainBase::prepareCommand(properties);
  if (code != OK) return code;

  if (_device == NULL && (properties & NeedDevice)) return errorExit(i18n("Device not specified."), ARG_ERROR);
  if (_progGroup == NULL && (properties & NeedProgrammer)) return errorExit(i18n("Programmer not specified."), ARG_ERROR);

  if ( (properties & NeedInput) || (properties & NeedOutput) ) {
    _memory.reset(NULL);
    if (properties & NeedDevice) _memory = _device->group().createMemory(*_device);
  }

  if (properties & NeedInput) {
    if (_memory.get() != NULL) {
      PURL::File file(input(), view());
      if ( !file.openForRead() ) return FILE_ERROR;
      QStringList errors, warnings;
      Device::Memory::WarningTypes warningTypes;
      if ( !_memory->load(file.stream(), errors, warningTypes, warnings) )
        return errorExit(i18n("Could not load hex file \"%1\".").arg(errors[0]), FILE_ERROR);
      if ( warningTypes!=Device::Memory::NoWarning )
        log(Log::LineType::Warning, i18n("Hex file seems incompatible with device \"%1\".").arg(warnings.join(" ")));
    }
  }

  return OK;
}

CLI::Main::Main()
: MainBase("piklab-prog", I18N_NOOP("Piklab Programmer Utility"),
           I18N_NOOP("Command-line programmer/debugger."))
{
  _range.reset(new Device::MemoryRange);
  Programmer::manager = new Programmer::CliManager(this);
  Programmer::manager->setView(&view());
  Debugger::manager = new Debugger::CliManager;
}

CLI::ExitCode CLI::Main::registerList()
{
  if (_device == NULL) return errorExit(i18n("No device specified."), NOT_SUPPORTED_ERROR);
  if (_device->group().name() != "pic") return errorExit(i18n("No register."), NOT_SUPPORTED_ERROR);
  const Pic::Data &data = static_cast<const Pic::Data &>(*_device);
  const Coff::Object *coff = Debugger::manager->coff();
  const Pic::RegistersData &rdata = data.registersData();
  log(Log::LineType::Normal, i18n("Special Function Registers:"));
  QValueVector<Pic::RegisterNameData> list = Pic::sfrList(data);
  for (uint i=0; i<uint(list.count()); i++) log(Log::LineType::Normal, QString("  %1: %2").arg(toHexLabel(list[i].data().address(), rdata.nbCharsAddress())).arg(list[i].label()));
  log(Log::LineType::Normal, i18n("General Purpose Registers:"));
  for (uint i=0; i<rdata.nbRegisters(); i++) {
    Pic::RegisterNameData rnd = Pic::gprNameData(rdata, i, coff);
    if ( rnd.label().isEmpty() ) continue;
    log(Log::LineType::Normal, QString("  %1: %2").arg(toHexLabel(rnd.data().address(), rdata.nbCharsAddress())).arg(rnd.label()));
  }
  return OK;
}

CLI::ExitCode CLI::Main::variableList()
{
  if (_device == NULL) return errorExit(i18n("No device specified."), NOT_SUPPORTED_ERROR);
  const Coff::Object *coff = Debugger::manager->coff();
  if (coff == NULL) return errorExit(i18n("No COFF file specified."), NOT_SUPPORTED_ERROR);
  if (_device->group().name() != "pic") return errorExit(i18n("No register."), NOT_SUPPORTED_ERROR);
  const Pic::Data &data = static_cast<const Pic::Data &>(*_device);
  const Pic::RegistersData &rdata = data.registersData();
  QValueVector<Pic::RegisterNameData> list = Pic::variableList(data, *coff);
  if (list.count() == 0) log(Log::LineType::Normal, i18n("No variable."));
  for (uint i=0; i<uint(list.count()); i++) log(Log::LineType::Normal, QString("  %1: %2").arg(toHexLabel(list[i].data().address(), rdata.nbCharsAddress())).arg(list[i].label()));
  return OK;
}

CLI::ExitCode CLI::Main::list(const QString &command)
{
  if ( MainBase::list(command)==OK ) return OK;
  if ( command=="format-list" ) return formatList();
  if ( command=="programmer-list" ) return programmerList();
  if ( command=="hardware-list" ) return hardwareList();
  if ( command=="port-list" ) return portList();
  if ( command=="device-list" ) return deviceList();
  if ( command=="range-list" ) return rangeList();
  if ( command=="register-list" ) return registerList();
  if ( command=="variable-list" ) return variableList();
  return ARG_ERROR;
}

//-----------------------------------------------------------------------------
CLI::ExitCode CLI::Main::extractRange(const QString &range)
{
  _range.reset(NULL);
  if ( !range.isEmpty() ) {
    if (_device == NULL) return errorExit(i18n("Cannot specify range without specifying device."), ARG_ERROR);
    if ( _device->group().name()=="pic" ) {
      FOR_EACH(Pic::MemoryRangeType, type) {
        if ( range!=type.key() ) continue;
        if ( !static_cast<const Pic::Data *>(_device)->isReadable(type) ) return errorExit(i18n("Memory range not present on this device."), ARG_ERROR);
        _range.reset(new Pic::MemoryRange(type));
        break;
      }
      if (_range.get() == NULL) return errorExit(i18n("Memory range not recognized."), ARG_ERROR);
    } else return errorExit(i18n("Memory ranges are not supported for the specified device."), ARG_ERROR);
  } else _range.reset(new Device::MemoryRange);
  return OK;
}

CLI::ExitCode CLI::Main::executeCommand(const QString &command)
{
  Programmer::Generic *programmer = Programmer::manager->programmer();
  if ( command=="connect" ) return (Programmer::manager->connectDevice() ? OK : EXEC_ERROR);
  if ( command=="disconnect" ) {
    if ( programmer==NULL || programmer->state()==Programmer::NotConnected )
      return okExit(i18n("Programmer is already disconnected."));
    return (Programmer::manager->disconnectDevice() ? OK : EXEC_ERROR);
  }
  if ( command=="run" ) {
    if ( programmer && programmer->state()==Programmer::Running ) return okExit(i18n("Programmer is already running."));
    return (Programmer::manager->run() ? OK : EXEC_ERROR);
  }
  if ( command=="stop" ) {
    if ( programmer != NULL && programmer->state()!=Programmer::Running ) return okExit(i18n("Programmer is already stopped."));
    return (Programmer::manager->halt() ? OK : EXEC_ERROR);
  }
  if ( command=="step" ) {
    if ( !_progGroup->isDebugger() ) return errorExit(i18n("Debugging is not supported for specified programmer."), NOT_SUPPORTED_ERROR);
    if ( programmer && programmer->state()==Programmer::Running ) return (Programmer::manager->halt() ? OK : EXEC_ERROR);
    return (Programmer::manager->step() ? OK : EXEC_ERROR);
  }
  if ( command=="start" ) {
    if ( !_progGroup->isDebugger() ) return errorExit(i18n("Debugging is not supported for specified programmer."), NOT_SUPPORTED_ERROR);
    return (Programmer::manager->restart() ? OK : EXEC_ERROR);
  }
  if ( command=="program" ) {
    if ( _progGroup->isSoftware() ) return errorExit(i18n("Reading device memory not supported for specified programmer."), NOT_SUPPORTED_ERROR);
    return (Programmer::manager->program(*_memory, *_range, true) ? OK : EXEC_ERROR);
  }
  if ( command=="verify" ) {
    if ( _progGroup->isSoftware() )
      return errorExit(i18n("Reading device memory not supported for specified programmer."), NOT_SUPPORTED_ERROR);
    return (Programmer::manager->verify(*_memory, *_range) ? OK : EXEC_ERROR);
  }
  if ( command=="read" ) {
    if ( _progGroup->isSoftware() )
      return errorExit(i18n("Reading device memory not supported for specified programmer."), NOT_SUPPORTED_ERROR);
    if ( !Programmer::manager->read(*_memory, *_range) ) return EXEC_ERROR;
    PURL::File file(output(), view());
    if ( !file.openForWrite() ) return FILE_ERROR;
    if ( !_memory->save(file.stream(), _format) )
      return errorExit(i18n("Error while writing file \"%1\".").arg(output().pretty()), FILE_ERROR);
    return OK;
  }
  if ( command=="erase" ) {
    if ( _progGroup->isSoftware() )
      return errorExit(i18n("Erasing device memory not supported for specified programmer."), NOT_SUPPORTED_ERROR);
    return (Programmer::manager->erase(*_range) ? OK : EXEC_ERROR);
  }
  if ( command=="blank_check" ) {
    if ( _progGroup->isSoftware() )
      return errorExit(i18n("Blank-checking device memory not supported for specified programmer."), NOT_SUPPORTED_ERROR);
    return (Programmer::manager->blankCheck(*_range) ? OK : EXEC_ERROR);
  }
  if ( command=="upload_firmware" ) {
    if ( !(_progGroup->properties() & ::Programmer::CanUploadFirmware) )
      return errorExit(i18n("Uploading firmware is not supported for the specified programmer."), NOT_SUPPORTED_ERROR);
    if (programmer == NULL) programmer = Programmer::manager->createProgrammer(NULL); // no device specified
    return (static_cast<Programmer::Base *>(programmer)->uploadFirmware(input()) ? OK : EXEC_ERROR);
  }

  return NOT_PROCESSED;
}

CLI::ExitCode CLI::Main::checkProgrammer()
{
  if ( _progGroup==0 ) return OK;
  if ( _progGroup->isSoftware() && _progGroup->supportedDevices().isEmpty() )
    return errorExit(i18n("Please check installation of selected software debugger."), NOT_SUPPORTED_ERROR);
  if ( _device && !_progGroup->isSupported(_device->name()) )
    return errorExit(i18n("The selected device \"%1\" is not supported by the selected programmer.").arg(_device->name()), NOT_SUPPORTED_ERROR);
  if ( !_hardware.isEmpty() ) {
    std::auto_ptr< ::Hardware::Config> config(_progGroup->hardwareConfig());
    Port::Description pd = static_cast<Programmer::CliManager *>(Programmer::manager)->portDescription();
    bool ok = (config.get() == NULL || config->hardwareNames(pd.type).contains(_hardware));
    if ( !ok ) return errorExit(i18n("The selected programmer does not supported the specified hardware configuration (\"%1\").").arg(_hardware), NOT_SUPPORTED_ERROR);
  }
  return OK;
}

CLI::ExitCode CLI::Main::doExecuteSetCommand(const QString &property, const QString &value)
{
  ExitCode code = MainBase::doExecuteSetCommand(property, value);
  if (code != NOT_PROCESSED) return code;

  if (property == "range") {
    return extractRange(value);
  }
  if (property == "programmer") {
    _progGroup = NULL;
    if ( value.isEmpty() ) return OK;
    _progGroup = Programmer::Lister::instance().group(value.lower());
    if (_progGroup != NULL) return checkProgrammer();
    return errorExit(i18n("Unknown programmer \"%1\".").arg(value.lower()), ARG_ERROR);
  }
  if (property == "hardware") {
    _hardware = value;
    return OK;
  }
  if (property == "device") {
    if ( value.isEmpty() ) {
      _device = NULL;
      return OK;
    }
    QString s = value.upper();
    _device = Device::Lister::instance().data(s);
    Debugger::manager->updateDevice();
    if (_device == NULL) return errorExit(i18n("Unknown device \"%1\".").arg(s), ARG_ERROR);
    Debugger::manager->init();
    return checkProgrammer();
  }
  if (property == "format") {
    if (value.isEmpty()) {
      _format = HexBuffer::IHX32;
      return OK;
    }
    QString s = value.lower();
    for (uint i=0; i<HexBuffer::Nb_Formats; i++)
      if ( s==HexBuffer::FORMATS[i] ) {
        _format = HexBuffer::Format(i);
        return OK;
      }
    return errorExit(i18n("Unknown hex file format \"%1\".").arg(s), ARG_ERROR);
  }
  if ( property=="port" ) {
    _port = value;
    return OK;
  }
  if ( property=="firmware-dir" ) {
    _firmwareDir = value;
    return OK;
  }
  if ( property=="target-self-powered" ) {
    _targetSelfPowered = value.lower();
    return OK;
  }
  if ( property=="coff" ) {
    _coffUrl = PURL::fromPathOrUrl(value, false);
    if (_device != NULL && !Debugger::manager->init()) return ARG_ERROR;
    return OK;
  }
  return NOT_PROCESSED;
}

CLI::ExitCode CLI::Main::doExecuteGetCommand(const QString &property, QString& value)
{
  ExitCode code = MainBase::doExecuteGetCommand(property, value);
  if (code != NOT_PROCESSED) return code;

  if (property == "range") {
    value = "<all>";
    if (!_range->all() && _device != NULL
        && _device->group().name() == "pic") {
      value = static_cast<Pic::MemoryRange&>(*_range)._type.key();
    }
    return OK;
  }
  if ( property=="programmer" ) {
    if (_progGroup != NULL) value = _progGroup->name();
    return OK;
  }
  if ( property=="hardware" ) {
    if (!_hardware.isEmpty()) value = _hardware;
    else if (_progGroup != NULL) {
      Port::Description pd = static_cast<Programmer::CliManager *>(Programmer::manager)->portDescription();
      std::auto_ptr< ::Hardware::Config> config(_progGroup->hardwareConfig());
      if (config.get() != NULL) value = config->currentHardware(pd.type) + " " + i18n("<from config>");
    }
    return OK;
  }
  if (property == "device") {
    if (_device != NULL) value = _device->name();
    return OK;
  }
  if (property == "format") {
    if (_format != HexBuffer::Nb_Formats) value = HexBuffer::FORMATS[_format];
    return OK;
  }
  if (property == "port") {
    if (!_port.isEmpty()) value = _port;
    else if (_progGroup != NULL) {
      Port::Description pd = Programmer::GroupConfig::portDescription(*_progGroup);
      QString s = pd.type.key();
      if (pd.type.data().withDevice) s += " (" + pd.device + ")";
      value = s + " " + i18n("<from config>");
    }
    return OK;
  }
  if (property == "firmware-dir") {
    if (!_firmwareDir.isEmpty()) value = _firmwareDir.pretty();
    else if (_progGroup != NULL) value = Programmer::GroupConfig::firmwareDirectory(*_progGroup) + " " + i18n("<from config>");
    return OK;
  }
  if (property == "target-self-powered") {
    if (!_targetSelfPowered.isEmpty()) value = _targetSelfPowered;
    else value = QString(readConfigEntry(Programmer::Config::TargetSelfPowered).toBool() ? "true" : "false") + " " + i18n("<from config>");
    return OK;
  }
  if (property == "coff") {
    if (!_coffUrl.isEmpty()) value = _coffUrl.pretty();
    return OK;
  }

  return NOT_PROCESSED;
}

CLI::ExitCode CLI::Main::processLine(const QStringList &words)
{
  if ( words[0]=="x" ) return executeRegister(words[1], words.count()==3 ? words[2] : QString::null);
  if ( words[0]=="break" ) {
    if ( words.count()==3 ) {
      if ( words[1]=="e" ) {
        bool ok;
        ulong address = fromAnyLabel(words[2], &ok);
        if ( !ok ) return errorExit(i18n("Number format not recognized."), ARG_ERROR);
        PURL::Url dummy;
        Breakpoint::Data data(dummy, address);
        if ( Breakpoint::list().contains(data) ) return okExit(i18n("Breakpoint already set at %1.").arg(toHexLabel(address, nbChars(NumberBase::Hex, address))));
        Breakpoint::list().append(data);
        Breakpoint::list().setAddress(data, address);
        Breakpoint::list().setState(data, Breakpoint::Active);
        return okExit(i18n("Breakpoint set at %1.").arg(toHexLabel(address, nbChars(NumberBase::Hex, address))));
      }
      return errorExit(i18n("The first argument should be \"e\"."), ARG_ERROR);
    }
    if ( words.count()==1 ) {
      uint nb = Breakpoint::list().count();
      if ( nb==0 ) return okExit(i18n("No breakpoint set."));
      log(Log::LineType::Normal, i18n("Breakpoints:"));
      uint nbc = 0;
      if (_device) nbc = _device->nbCharsAddress();
      else for (uint i=0; i<nb; i++) {
        Address address = Breakpoint::list().address(Breakpoint::list().data(i));
        nbc = QMAX(nbc, nbChars(NumberBase::Hex, address.toUInt()));
      }
      for (uint i=0; i<nb; i++) {
        Address address = Breakpoint::list().address(Breakpoint::list().data(i));
        log(Log::LineType::Normal, QString("  #%1: %2").arg(i).arg(toHexLabel(address, nbc)));
      }
      return OK;
    }
    if (words.count() == 2) return errorExit(i18n("This command takes no or two argument."), ARG_ERROR);
    return errorExit(i18n("Arguments not recognized."), ARG_ERROR);
  }
  if ( words[0]=="clear" ) {
    if ( words[1]=="all" ) {
      Breakpoint::list().clear();
      return okExit(i18n("All breakpoints removed."));
    }
    bool ok;
    uint i = words[1].toUInt(&ok);
    if ( !ok ) return errorExit(i18n("Argument should be breakpoint index."), ARG_ERROR);
    if ( i>=Breakpoint::list().count() ) return errorExit(i18n("Breakpoint index too large."), ARG_ERROR);
    Breakpoint::Data data = Breakpoint::list().data(i);
    Address address = Breakpoint::list().address(data);
    Breakpoint::list().remove(data);
    return okExit(i18n("Breakpoint at %1 removed.").arg(toHexLabelAbs(address)));
  }
  if ( words[0]=="raw-com" ) return executeRawCommands(words[1]);

  uint hexi = 1;
  if (words[0]=="program" || words[0]=="read" || words[0]=="verify"
      || words[0]=="erase" || words[0]=="blank_check") {
    if ( words.count()>=2 && words[1]=="-r" ) {
      hexi += 2;
      if ( words.count()==2 ) return errorExit(i18n("You need to specify the range."), ARG_ERROR);
      ExitCode code = extractRange(words[2]);
      if ( code!=OK ) return code;
    }
  }
  if (words[0] == "program") {
    ++hexi;
    if (uint(words.count()) == hexi) setInput(words[hexi]);
  }
  if (words[0] == "read" || words[0] == "verify") {
    ++hexi;
    if (uint(words.count()) == hexi) setOutput(words[hexi]);
  }
  if (words[0] == "upload_firmware") setInput(words[1]);
  return executeCommand(words[0]);
}

Address CLI::Main::findRegisterAddress(const QString &name)
{
  const Pic::Data &data = static_cast<const Pic::Data &>(*_device);
  const Coff::Object *coff = Debugger::manager->coff();
  const Pic::RegistersData &rdata = data.registersData();
  bool ok;
  Address address = fromAnyLabel(name, &ok);
  if (ok) {
    if ( address>rdata.addressFromIndex(rdata.nbRegisters()-1) ) return Address();
    return address;
  }
  QValueVector<Pic::RegisterNameData> sfrs = Pic::sfrList(data);
  for (uint i=0; i<uint(sfrs.count()); i++) if ( name.lower()==sfrs[i].label().lower() ) return sfrs[i].data().address();
  if ( coff==0 ) return Address();
  QMap<QString, Address> variables = coff->variables();
  if ( variables.contains(name) ) return variables[name];
  return Address();
}

CLI::ExitCode CLI::Main::executeRegister(const QString &name, const QString &value)
{
  if (Debugger::manager->debugger() == NULL) return errorExit(i18n("You need to start the debugging session first (with \"start\")."), ARG_ERROR);
  const Pic::Data &data = static_cast<const Pic::Data &>(*_device);
  const Pic::RegistersData &rdata = data.registersData();
  uint nbChars = rdata.nbChars();
  bool ok;
  ulong v = fromAnyLabel(value, &ok);
  if ( !ok ) return errorExit(i18n("Number format not recognized."), ARG_ERROR);
  if ( v>maxValue(NumberBase::Hex, nbChars) ) return errorExit(i18n("The given value is too large (max: %1).").arg(toHexLabel(maxValue(NumberBase::Hex, nbChars), nbChars)), ARG_ERROR);
  Register::TypeData rtd;
  if ( name.lower()=="w" || name.lower()=="wreg" )
    rtd = static_cast<Debugger::PicBase *>(Debugger::manager->debugger())->deviceSpecific()->wregTypeData();
  else if ( name.lower()=="pc" )
    rtd = Debugger::manager->debugger()->pcTypeData();
  else {
    Address address = findRegisterAddress(name);
    if ( !address.isValid() ) return errorExit(i18n("Unknown register or variable name."), ARG_ERROR);
    rtd = Register::TypeData(address, rdata.nbChars());
  }
  if ( value.isEmpty() ) {
    if ( !Debugger::manager->readRegister(rtd) ) return ARG_ERROR;
    return okExit(i18n("%1 = %2").arg(name.upper()).arg(toHexLabel(Register::list().value(rtd), nbChars)));
  }
  return (Debugger::manager->writeRegister(rtd, v) ? OK : EXEC_ERROR);
}

CLI::ExitCode CLI::Main::executeRawCommands(const QString &filename)
{
  QFile file(filename);
  if ( !file.open(IO_ReadOnly) ) return errorExit(i18n("Could not open filename \"%1\".").arg(filename), ARG_ERROR);
  Programmer::Generic *generic = Programmer::manager->programmer();
  if ( generic==0 ) generic = Programmer::manager->createProgrammer(_device);
  if ( generic->group().properties() & Programmer::Custom ) return errorExit(i18n("The selected programmer does not support sending raw command."), ARG_ERROR);
  Programmer::Base *base = static_cast<Programmer::Base *>(generic);
  if ( base->state()==Programmer::NotConnected ) {
    if ( !base->simpleConnectHardware() ) return EXEC_ERROR;
  }
  for (;;) {
    QString s;
    char buffer[1000];
    if ( file.readLine(buffer, 1000)==-1 ) break;
    s += buffer;
    bool write = !s.startsWith(" ");
    s = s.stripWhiteSpace();
    if ( s.isEmpty() ) continue;
    if (write) {
      if ( !base->hardware()->rawWrite(s) ) return EXEC_ERROR;
    } else {
      QString rs;
      if ( !base->hardware()->rawRead(s.length(), rs) ) return EXEC_ERROR;
      if ( rs!=s ) log(Log::LineType::Warning, i18n("Read string is different than expected: %1 (%2).").arg(rs).arg(s));
    }
  }
  return okExit(i18n("End of command file reached."));
}

void CLI::Main::processInterrupt()
{
  Programmer::Generic *prog = Programmer::manager->programmer();
  if (prog != NULL && prog->state() == Programmer::Running) Programmer::manager->halt();
}

//-----------------------------------------------------------------------------
int main(int argc, char **argv)
{
  CLI::Main main;
  main.initArgs(argc, argv, I18N_NOOP("Hex filename for programming."));
  return main.run();
}
