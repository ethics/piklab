/***************************************************************************
 *   Copyright (C) 2005-2006 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "pic_prog.h"

#include "devices/list/device_list.h"
#include "progs/base/prog_config.h"
#include "progs/base/prog_group.h"
#include "pic_debug.h"

//-----------------------------------------------------------------------------
Debugger::DeviceSpecific* Programmer::PicGroup::createDebuggerDeviceSpecific(::Debugger::Base &base) const
{
  const Pic::Data *data = static_cast<const Pic::Data *>(base.device());
  if (data != NULL) {
    switch (data->architecture().type()) {
      case Pic::Architecture::Baseline:
      case Pic::Architecture::MidRange:
      case Pic::Architecture::EMidRange: return new ::Debugger::P16FSpecific(base);
      case Pic::Architecture::P18C:
      case Pic::Architecture::P18F:
      case Pic::Architecture::P18J: return new ::Debugger::P18FSpecific(base);
      case Pic::Architecture::P24F:
      case Pic::Architecture::P24H:
      case Pic::Architecture::P30F:
      case Pic::Architecture::P33F:
      case Pic::Architecture::P17C:
      case Pic::Architecture::Nb_Types: ASSERT(false); break;
    }
  }
  return NULL;
}

//-----------------------------------------------------------------------------
Programmer::PicBase::PicBase(const Group &group, const Pic::Data *data, const char *name)
  : Base(group, data, name), _deviceMemory(0), _hasProtectedCode(false), _hasProtectedEeprom(false)
{
  if (data != NULL) _deviceMemory = new Pic::Memory(*data);
}

Programmer::PicBase::~PicBase()
{
  delete _deviceMemory;
  _deviceMemory = NULL;
}

uint Programmer::PicBase::nbSteps(Task task, const Device::MemoryRange& range) const
{
  const Pic::MemoryRange& prange = static_cast<const Pic::MemoryRange&>(range);
  switch (task.type()) {
    case Task::Erase: return 1;
    case Task::Read:
    case Task::Verify:
    case Task::BlankCheck: {
      uint nb = 0;
      FOR_EACH(Pic::MemoryRangeType, type) {
        if ( type!=Pic::MemoryRangeType::Code && type!=Pic::MemoryRangeType::Eeprom ) continue;
        if ( !device()->isReadable(type) || !deviceSpecific()->canReadRange(type) ) continue;
        if ( !prange.all() && prange._type!=type ) continue;
        nb += device()->nbWords(type);
      }
      return QMAX(nb, uint(1));
    }
    case Task::Write: {
      uint nb = 0;
      FOR_EACH(Pic::MemoryRangeType, type) {
        if ( type!=Pic::MemoryRangeType::Code && type!=Pic::MemoryRangeType::Eeprom ) continue;
        if ( !device()->isWritable(type) || !deviceSpecific()->canWriteRange(type) ) continue;
        if ( !prange.all() && prange._type!=type ) continue;
        nb += device()->nbWords(type);
        if ( readConfigEntry(Config::VerifyAfterProgram).toBool() ) nb += device()->nbWords(type);
      }
      return QMAX(nb, uint(1));
    }
    case Task::Nb_Types: break;
  }
  ASSERT(false);
  return 0;
}

bool Programmer::PicBase::initProgramming(Task)
{
/*
  if ( vpp()!=UNKNOWN_VOLTAGE ) {
    const Pic::VoltageData &tvpp = device()->voltage(Pic::Vpp);
    if ( vpp()<tvpp.min )
      log(Log::LineType::Warning, i18n("Vpp (%1 V) is lower than the minimum required voltage (%2 V).")
                        .arg(vpp()).arg(tvpp.min));
    if ( vpp()>tvpp.max ) {
      QString s = i18n("Vpp (%1 V) is higher than the maximum voltage (%2 V). You may damage the device.")
                  .arg(vpp()).arg(tvpp.max);
      log(Log::LineType::Warning, s);
      if ( !askContinue(s) ) {
        logUserAbort();
        return false;
      }
    }
  }
  if ( vdd()!=UNKNOWN_VOLTAGE ) {
    ASSERT( type!=Pic::Vpp );
    const Pic::VoltageData &tvdd = device()->voltage(type);
    if ( vdd()<tvdd.min ) {
      if ( type==Pic::VddBulkErase && device()->voltage(Pic::VddWrite).min!=tvdd.min )
        log(Log::LineType::Warning, i18n("Vdd (%1 V) is too low for high-voltage programming\n(piklab only supports high-voltage programming at the moment).\nMinimum required is %2 V.")
                          .arg(vdd()).arg(tvdd.min));
      else if ( type==Pic::VddRead && device()->voltage(Pic::VddWrite).min!=tvdd.min )
        log(Log::LineType::Warning, i18n("Vdd (%1 V) is too low for reading\nMinimum required is %2 V.")
                          .arg(vdd()).arg(tvdd.min));
      else log(Log::LineType::Warning, i18n("Vdd (%1 V) is too low for programming\nMinimum required is %2 V.")
                            .arg(vdd()).arg(tvdd.min));
    } else if ( vdd()>tvdd.max ) {
      QString s = i18n("Vdd (%1 V) is higher than the maximum voltage (%2 V). You may damage the device.")
                  .arg(vdd()).arg(tvdd.max);
      log(Log::LineType::Warning, s);
      if ( !askContinue(s) ) {
        logUserAbort();
        return false;
      }
    }
  }
*/
  if ( deviceSpecific()->canReadRange(Pic::MemoryRangeType::Config) ) {
    // read config
    Device::Array data;
    if ( !deviceSpecific()->read(Pic::MemoryRangeType::Config, data, 0) ) return false;
    _deviceMemory->setArray(Pic::MemoryRangeType::Config, data);
    _hasProtectedCode = _deviceMemory->isProtected(Pic::Protection::ProgramProtected, Pic::MemoryRangeType::Code);
    _hasProtectedEeprom = _deviceMemory->isProtected(Pic::Protection::ProgramProtected, Pic::MemoryRangeType::Eeprom);
    log(Log::DebugLevel::Extra, QString("  protected: code=%1 data=%2")
                          .arg(_hasProtectedCode ? "true" : "false").arg(_hasProtectedEeprom ? "true" : "false"));
    // read calibration
    if ( !readCalibration() ) return false;
  }

  return Base::initProgramming();
}

bool Programmer::PicBase::preserveCode()
{
  if ( _hasProtectedCode && !askContinue(i18n("All or part of code memory is protected so it cannot be preserved. Continue anyway?")) )
    return false;
  return readRange(Pic::MemoryRangeType::Code, _deviceMemory, 0);
}

bool Programmer::PicBase::preserveEeprom()
{
  if ( _hasProtectedEeprom && !askContinue(i18n("All or part of data EEPROM is protected so it cannot be preserved. Continue anyway?")) )
    return false;
  return readRange(Pic::MemoryRangeType::Eeprom, _deviceMemory, 0);
}

//-----------------------------------------------------------------------------
BitValue Programmer::PicBase::readDeviceId()
{
  Device::Array data;
  if ( !deviceSpecific()->read(Pic::MemoryRangeType::DeviceId, data, 0) ) return 0;
  ASSERT( data.count()!=0 );
  BitValue id = 0x0;
  switch (device()->architecture().type()) {
    case Pic::Architecture::Baseline:
    case Pic::Architecture::MidRange:
    case Pic::Architecture::EMidRange:
    case Pic::Architecture::P17C: id = data[0]; break;
    case Pic::Architecture::P18C:
    case Pic::Architecture::P18F:
    case Pic::Architecture::P18J: id = data[0] | (data[1] << 8); break;
    case Pic::Architecture::P24F:
    case Pic::Architecture::P24H:
    case Pic::Architecture::P30F:
    case Pic::Architecture::P33F: id = data[1] | (data[0] << 16); break;
    case Pic::Architecture::Nb_Types: ASSERT(false); break;
  }
  return id;
}

bool Programmer::PicBase::verifyDeviceId()
{
  if ( !deviceSpecific()->canReadRange(Pic::MemoryRangeType::DeviceId ) ) return true;
  if ( !device()->isReadable(Pic::MemoryRangeType::DeviceId) ) {
    log(Log::LineType::Information, i18n("Device not autodetectable: continuing with the specified device name \"%1\"...").arg(device()->name()));
    return true;
  }
  BitValue rawId = readDeviceId();
  if ( hasError() ) return false;
  uint nbChars = device()->nbWords(Pic::MemoryRangeType::DeviceId) * device()->nbCharsWord(Pic::MemoryRangeType::DeviceId);
  if ( rawId==0x0 || rawId==device()->mask(Pic::MemoryRangeType::DeviceId) ) {
    log(Log::LineType::Error, i18n("Missing or incorrect device (Read id is %1).").arg(toHexLabel(rawId, nbChars)));
    return false;
  }
  QMap<QString, Device::IdData> ids;
  QStringList names = group().supportedDevices();
  QStringList::const_iterator it;
  for (it=names.begin(); it!=names.end(); ++it) {
    const Pic::Data *data = static_cast<const Pic::Data *>(group().deviceData(*it).data);
    if ( data->architecture()!=device()->architecture() ) continue;
    Device::IdData idata;
    if ( data->matchId(rawId, idata) ) ids[*it] = idata;
  }
  QString message;
  if ( ids.count()!=0 ) {
    log(Log::LineType::Information, i18n("Read id: %1").arg(device()->idNames(ids).join("; ")));
    if ( ids.contains(device()->name()) ) return true;
    message = i18n("Read id does not match the specified device name \"%1\".").arg(device()->name());
  } else {
    log(Log::LineType::Warning, i18n("  Unknown or incorrect device (Read id is %1).").arg(toHexLabel(rawId, nbChars)));
    message = i18n("Unknown device.");
  }
  if ( !askContinue(message) ) {
    logUserAbort();
    return false;
  }
  log(Log::LineType::Information, i18n("Continue with the specified device name: \"%1\"...").arg(device()->name()));
  return true;
}

//-----------------------------------------------------------------------------
QString Programmer::PicBase::prettyCalibration(const Device::Array &data) const
{
  QString s;
  for (uint i=0; i<data.count(); i++) {
    if ( i!=0 ) s += ", ";
    s += toHexLabel(data[i], device()->nbCharsWord(Pic::MemoryRangeType::Cal));
  }
  return s;
}

bool Programmer::PicBase::readCalibration()
{
  if ( device()->isReadable(Pic::MemoryRangeType::Cal) ) {
    if ( !deviceSpecific()->canReadRange(Pic::MemoryRangeType::Cal) ) {
      log(Log::LineType::Warning, i18n("Osccal cannot be read by the selected programmer"));
      return true;
    }
    Device::Array data;
    if ( !deviceSpecific()->read(Pic::MemoryRangeType::Cal, data, 0) ) return false;
    _deviceMemory->setArray(Pic::MemoryRangeType::Cal, data);
    log(Log::DebugLevel::Normal, QString("  Read osccal: %1").arg(prettyCalibration(data)));
    QString message;
    if ( !device()->checkCalibration(data, &message) ) log(Log::LineType::Warning, "  " + message);
    if ( device()->isReadable(Pic::MemoryRangeType::CalBackup) ) {
      if ( !deviceSpecific()->canReadRange(Pic::MemoryRangeType::CalBackup) ) {
        log(Log::LineType::Warning, i18n("Osccal backup cannot be read by the selected programmer"));
        return true;
      }
      if ( !deviceSpecific()->read(Pic::MemoryRangeType::CalBackup, data, 0) ) return false;
      _deviceMemory->setArray(Pic::MemoryRangeType::CalBackup, data);
      log(Log::DebugLevel::Normal, QString("  Read osccal backup: %1").arg(prettyCalibration(data)));
      if ( !device()->checkCalibration(data, &message) ) log(Log::LineType::Warning, "  " + message);
    }
  }
  return true;
}

bool Programmer::PicBase::restoreCalibration()
{
  if ( !deviceSpecific()->canReadRange(Pic::MemoryRangeType::Cal) || !deviceSpecific()->canWriteRange(Pic::MemoryRangeType::Cal) ) return true;
  if ( !device()->isWritable(Pic::MemoryRangeType::Cal) ) return true;
  Device::Array data = _deviceMemory->arrayForWriting(Pic::MemoryRangeType::Cal);
  Device::Array bdata = _deviceMemory->arrayForWriting(Pic::MemoryRangeType::CalBackup);
  if ( device()->isReadable(Pic::MemoryRangeType::CalBackup) && deviceSpecific()->canReadRange(Pic::MemoryRangeType::CalBackup) ) {
    if ( !device()->checkCalibration(data) && device()->checkCalibration(bdata) ) {
      log(Log::LineType::Information, i18n("  Replace invalid osccal with backup value."));
      data = bdata;
    }
  }
  Device::Array cdata;
  if ( !deviceSpecific()->read(Pic::MemoryRangeType::Cal, cdata, 0) ) return false;
  if ( cdata==data ) {
    log(Log::LineType::Information, i18n("  Osccal is unchanged."));
    return true;
  }
  if ( !programRange(Pic::MemoryRangeType::Cal, data) ) return false;
  if ( !deviceSpecific()->read(Pic::MemoryRangeType::Cal, cdata, 0) ) return false;
  if ( cdata==data ) log(Log::LineType::Information, i18n("  Osccal has been preserved."));

  if ( !device()->isWritable(Pic::MemoryRangeType::CalBackup) || !device()->checkCalibration(bdata) ) return true;
  if ( !deviceSpecific()->read(Pic::MemoryRangeType::CalBackup, cdata, 0) ) return false;
  if ( cdata.count()==0 ) {
    log(Log::LineType::Warning, i18n("Osccal backup cannot be read by selected programmer"));
    return true;
  }
  if ( cdata==bdata ) {
    log(Log::LineType::Information, i18n("  Osccal backup is unchanged."));
    return true;
  }
  if ( !programRange(Pic::MemoryRangeType::CalBackup, bdata) ) return false;
  if ( !deviceSpecific()->read(Pic::MemoryRangeType::CalBackup, cdata, 0) ) return false;
  if ( cdata==bdata ) log(Log::LineType::Information, i18n("  Osccal backup has been preserved."));
  return true;
}

bool Programmer::PicBase::restoreBandGapBits()
{
  if ( !deviceSpecific()->canReadRange(Pic::MemoryRangeType::Config) ) return true;
  bool hasProtectedBits = false;
  for (uint i=0; i<device()->nbWords(Pic::MemoryRangeType::Config); i++)
    if ( device()->config()._words[i].pmask!=0 ) hasProtectedBits = true;
  if ( !hasProtectedBits ) return true;
  Device::Array cdata;
  if ( !deviceSpecific()->read(Pic::MemoryRangeType::Config, cdata, 0) ) return false;
  Device::Array data = _deviceMemory->arrayForWriting(Pic::MemoryRangeType::Config);
  for (uint i=0; i<cdata.count(); i++) {
    BitValue pmask = device()->config()._words[i].pmask;
    if ( pmask==0 ) continue;
    cdata[i] = cdata[i].clearMaskBits(pmask);
    cdata[i] |= data[i].maskWith(pmask);
  }
  if ( !deviceSpecific()->canWriteRange(Pic::MemoryRangeType::Config) ) {
    log(Log::LineType::Warning, i18n("Could not restore band gap bits because programmer does not support writing config bits."));
    return true;
  }
  log(Log::DebugLevel::Normal, QString("  Write config with band gap bits: %2").arg(toHexLabel(cdata[0], device()->nbCharsWord(Pic::MemoryRangeType::Config))));
  if ( !programRange(Pic::MemoryRangeType::Config, cdata) ) return false;
  if ( !deviceSpecific()->read(Pic::MemoryRangeType::Config, data, 0) ) return false;
  if ( data==cdata ) log(Log::LineType::Information, i18n("  Band gap bits have been preserved."));
  return true;
}

bool Programmer::PicBase::eraseAll()
{
  if ( !(deviceSpecific()->capabilities() & CanEraseAll) ) {
    log(Log::LineType::SoftError, i18n("The selected programmer does not support erasing the whole device."));
    return false;
  }
  if ( !deviceSpecific()->erase(_hasProtectedCode || _hasProtectedEeprom) ) return false;
  if ( !restoreCalibration() ) return false;
  return true;
}

bool Programmer::PicBase::checkErase()
{
  if ( device()->memoryTechnology()==Device::MemoryTechnology::Rom || device()->memoryTechnology()==Device::MemoryTechnology::Romless
       || device()->memoryTechnology()==Device::MemoryTechnology::Eprom ) {
    log(Log::LineType::SoftError, i18n("Cannot erase ROM or EPROM device."));
    return false;
  }
  return true;
}

bool Programmer::PicBase::internalErase(const Device::MemoryRange &range)
{
  if ( !initProgramming(Task::Erase) ) return false;
  bool ok = true;
  if ( range.all() ) ok = eraseAll();
  else ok = eraseRange(static_cast<const Pic::MemoryRange &>(range)._type);
  if ( !restoreBandGapBits() ) return false;
  return ok;
}

bool Programmer::PicBase::eraseSingle(Pic::MemoryRangeType type)
{
  return erase(Pic::MemoryRange(type));
}

bool Programmer::PicBase::eraseRange(Pic::MemoryRangeType type)
{
  bool ok = internalEraseRange(type);
  if ( !restoreCalibration() ) return false;
  if ( ok && readConfigEntry(Config::BlankCheckAfterErase).toBool() ) {
    Pic::Memory memory(*device());
    VerifyData vdata(BlankCheckVerify, memory);
    return readRange(type, 0, &vdata);
  }
  return ok;
}

bool Programmer::PicBase::internalEraseRange(Pic::MemoryRangeType type)
{
  if ( !deviceSpecific()->canEraseRange(type) && !(deviceSpecific()->capabilities() & CanEraseAll) ) {
    log(Log::LineType::SoftError, i18n("The selected programmer does not support erasing neither the specified range nor the whole device."));
    return false;
  }
  if ( type==Pic::MemoryRangeType::Code && _hasProtectedCode ) {
    log(Log::LineType::SoftError, i18n("Cannot erase protected code memory. Consider erasing the whole chip."));
    return false;
  }
  if ( type==Pic::MemoryRangeType::Eeprom && _hasProtectedEeprom ) {
    log(Log::LineType::SoftError, i18n("Cannot erase protected data EEPROM. Consider erasing the whole chip."));
    return false;
  }
  if ( deviceSpecific()->canEraseRange(type) ) return deviceSpecific()->eraseRange(type);
  bool softErase = true;
  if ( type!=Pic::MemoryRangeType::Code && (!deviceSpecific()->canReadRange(Pic::MemoryRangeType::Code)
       || !deviceSpecific()->canWriteRange(Pic::MemoryRangeType::Code)) ) softErase = false;
  if ( type!=Pic::MemoryRangeType::Eeprom && (!deviceSpecific()->canReadRange(Pic::MemoryRangeType::Eeprom)
       || !deviceSpecific()->canWriteRange(Pic::MemoryRangeType::Eeprom)) ) softErase = false;
  if ( type!=Pic::MemoryRangeType::Config && (!deviceSpecific()->canReadRange(Pic::MemoryRangeType::Config)
       || !deviceSpecific()->canWriteRange(Pic::MemoryRangeType::Config)) ) softErase = false;
  if ( type!=Pic::MemoryRangeType::UserId && (!deviceSpecific()->canReadRange(Pic::MemoryRangeType::UserId)
       || !deviceSpecific()->canWriteRange(Pic::MemoryRangeType::UserId)) ) softErase = false;
  if ( !softErase ) {
    log(Log::LineType::SoftError, i18n("Cannot erase specified range because of programmer limitations."));
    return false;
  }
  if ( !askContinue(i18n("%1: Erasing this range only is not supported with this programmer. This will erase the whole chip and restore the other memory ranges.").arg(type.label())) ) {
    logUserAbort();
    return false;
  }
  if ( type!=Pic::MemoryRangeType::Code && !preserveCode() ) return false;
  if ( type!=Pic::MemoryRangeType::Eeprom && !preserveEeprom() ) return false;
  if ( type!=Pic::MemoryRangeType::UserId && !readRange(Pic::MemoryRangeType::UserId, _deviceMemory, 0) ) return false;
  deviceSpecific()->erase(_hasProtectedCode || _hasProtectedEeprom);
  if ( type!=Pic::MemoryRangeType::Code && !programAndVerifyRange(Pic::MemoryRangeType::Code, *_deviceMemory) ) return false;
  if ( type!=Pic::MemoryRangeType::Eeprom && !programAndVerifyRange(Pic::MemoryRangeType::Eeprom, *_deviceMemory) ) return false;
  if ( type!=Pic::MemoryRangeType::UserId && !programAndVerifyRange(Pic::MemoryRangeType::UserId, *_deviceMemory) ) return false;
  if ( !programAndVerifyRange(Pic::MemoryRangeType::Config, *_deviceMemory) ) return false;
  return true;
}

//-----------------------------------------------------------------------------
bool Programmer::PicBase::readSingle(Pic::MemoryRangeType type, Pic::Memory &memory)
{
  if ( !deviceSpecific()->canReadRange(type) ) {
    log(Log::LineType::SoftError, i18n("The selected programmer cannot read the specified memory range."));
    return false;
  }
  Pic::Memory tmp(*device());
  if ( !read(tmp, Pic::MemoryRange(type)) ) return false;
  memory.copyFrom(type, tmp);
  if ( type==Pic::MemoryRangeType::Cal ) memory.copyFrom(Pic::MemoryRangeType::CalBackup, tmp);
  return true;
}

bool Programmer::PicBase::readRange(Pic::MemoryRangeType type, Pic::Memory *memory, const VerifyData *vd)
{
  if ( !device()->isReadable(type) ) return true;
  if ( !deviceSpecific()->canReadRange(type) ) {
    log(Log::LineType::Information, i18n("The selected programmer cannot read %1: operation skipped.").arg(type.label()));
    return true;
  }
  std::auto_ptr<VerifyData> vdata;
  if (vd != NULL) vdata.reset(new VerifyData(*vd));
  if (vdata.get() != NULL) {
    log(Log::LineType::Information, i18n("  Verify memory: %1").arg(type.label()));
    if ( !(vdata->actions & IgnoreProtectedVerify) ) {
      vdata->protectedRanges = static_cast<const Pic::Memory &>(vdata->memory()).protectedRanges(Pic::Protection::ProgramProtected, type);
      if ( !vdata->protectedRanges.isEmpty() ) log(Log::LineType::Warning, i18n("  Part of device memory is protected (in %1) and cannot be verified.")
                                               .arg(type.label()));
    } else vdata->protectedRanges.clear();
  } else {
    log(Log::LineType::Information, i18n("  Read memory: %1").arg(type.label()));
    ASSERT(memory);
  }
  Device::Array data;
  if (!deviceSpecific()->read(type, data, vdata.get())) return false;
  if (memory != NULL) memory->setArray(type, data);
  return true;
}

bool Programmer::PicBase::checkRead()
{
  if ( device()->memoryTechnology()==Device::MemoryTechnology::Romless ) {
    log(Log::LineType::SoftError, i18n("Cannot read ROMless device."));
    return false;
  }
  return true;
}

void Programmer::PicBase::prepareMemoryForWriting(Pic::Memory& memory, bool issueWarnings)
{
  if (memory.hasDebugOn() != group().isDebugger()) {
    if (issueWarnings) {
      if (group().isDebugger()) log(Log::LineType::Warning, i18n("Enabling DEBUG configuration bit."));
      else log(Log::LineType::Warning, i18n("Disabling DEBUG configuration bit."));
    }
    memory.setDebugOn(group().isDebugger());
  }

  if (memory.hasJTagOn()) {
    if (issueWarnings) log(Log::LineType::Warning, i18n("Disabling JTAGEN configuration bit."));
    memory.setJTagOn(false);
  }
}

bool Programmer::PicBase::internalRead(Device::Memory *memory, const Device::MemoryRange &range, const VerifyData *pvdata)
{
  if ( !initProgramming(Task::Read) ) return false;
  Pic::Memory *pmemory = static_cast<Pic::Memory *>(memory);

  std::auto_ptr<VerifyData> vdata;
  if (pvdata != NULL) {
    vdata.reset(new VerifyData(*pvdata));
    prepareMemoryForWriting(static_cast<Pic::Memory&>(vdata->memory()), false);
  }

  if ( !range.all() ) {
    Pic::MemoryRangeType type = static_cast<const Pic::MemoryRange &>(range)._type;
    if ( type==Pic::MemoryRangeType::Cal ) {
      if ( !readRange(Pic::MemoryRangeType::Cal, pmemory, vdata.get()) ) return false;
      return readRange(Pic::MemoryRangeType::CalBackup, pmemory, vdata.get());
    }
    return readRange(type, pmemory, vdata.get());
  }
  if ( !readRange(Pic::MemoryRangeType::Config,    pmemory, vdata.get()) ) return false;
  if ( !readRange(Pic::MemoryRangeType::UserId,    pmemory, vdata.get()) ) return false;
  if (vdata.get() == NULL) if ( !readRange(Pic::MemoryRangeType::Cal,       pmemory, NULL) ) return false;
  if (vdata.get() == NULL) if ( !readRange(Pic::MemoryRangeType::CalBackup, pmemory, NULL) ) return false;
  if ( !readRange(Pic::MemoryRangeType::Code,      pmemory, vdata.get()) ) return false;
  if ( !readRange(Pic::MemoryRangeType::Eeprom,    pmemory, vdata.get()) ) return false;
  return true;
}

//-----------------------------------------------------------------------------
bool Programmer::PicBase::programSingle(Pic::MemoryRangeType type, const Pic::Memory &memory)
{
  if ( !deviceSpecific()->canWriteRange(type) ) {
    log(Log::LineType::SoftError, i18n("The selected programmer cannot read the specified memory range."));
    return false;
  }
  return program(memory, Pic::MemoryRange(type));
}

bool Programmer::PicBase::programRange(Pic::MemoryRangeType mtype, const Device::Array &data)
{
  log(Log::LineType::Information, i18n("  Write memory: %1").arg(mtype.label()));
  bool only = ( readConfigEntry(Config::OnlyProgramNonMask).toBool()
                && (mtype==Pic::MemoryRangeType::Code || mtype==Pic::MemoryRangeType::Eeprom) );
  return deviceSpecific()->write(mtype, data, !only);
}

bool Programmer::PicBase::programAndVerifyRange(Pic::MemoryRangeType type, const Pic::Memory &memory)
{
  if ( !device()->isWritable(type) || !deviceSpecific()->canWriteRange(type) ) return true;
  Device::Array data = memory.arrayForWriting(type);
  if ( !programRange(type, data) ) return false;
  if ( !readConfigEntry(Config::VerifyAfterProgram).toBool() ) return true;
  if ( !deviceSpecific()->canReadRange(type) ) return true;
  VerifyActions actions = IgnoreProtectedVerify;
  if ( type==Pic::MemoryRangeType::Code && readConfigEntry(Config::OnlyVerifyProgrammed).toBool() ) actions |= OnlyProgrammedVerify;
  VerifyData vdata(actions, memory);
  return readRange(type, 0, &vdata);
}

bool Programmer::PicBase::programAll(const Pic::Memory &memory)
{
  if ( !programAndVerifyRange(Pic::MemoryRangeType::Code, memory) ) return false;
  if ( readConfigEntry(Config::ProgramEeprom).toBool() ) {
    const Pic::Memory &tmp = (readConfigEntry(Config::PreserveEeprom).toBool() ? *_deviceMemory : memory);
    if ( !programAndVerifyRange(Pic::MemoryRangeType::Eeprom, tmp) ) return false;
  }
  if ( !programAndVerifyRange(Pic::MemoryRangeType::UserId, memory) ) return false;
  if ( memory.isProtected(Pic::Protection::WriteProtected, Pic::MemoryRangeType::Config) ) {
    log(Log::DebugLevel::Normal, "  Config write protection is on: first program without it and then with it");
    Pic::Memory tmp(memory.device());
    tmp.copyFrom(Pic::MemoryRangeType::Config, memory);
    tmp.setProtection(false, Pic::Protection::WriteProtected, Pic::MemoryRangeType::Config);
    if ( !programAndVerifyRange(Pic::MemoryRangeType::Config, tmp) ) return false;
  }
  if ( !programAndVerifyRange(Pic::MemoryRangeType::Config, memory) ) return false;
  return true;
}

bool Programmer::PicBase::checkProgram(const Device::Memory &)
{
  if ( device()->memoryTechnology()==Device::MemoryTechnology::Rom
       || device()->memoryTechnology()==Device::MemoryTechnology::Romless ) {
    log(Log::LineType::SoftError, i18n("Cannot write ROM or ROMless device."));
    return false;
  }
  return true;
}

bool Programmer::PicBase::internalProgram(const Device::Memory &memory, const Device::MemoryRange &range)
{
  if ( !initProgramming(Task::Erase) ) return false;
  std::auto_ptr<Pic::Memory> pmemory = static_cast<const Pic::Memory&>(memory).clone();
  prepareMemoryForWriting(*pmemory, true);

  // blank check if OTP device
  bool eprom = ( device()->memoryTechnology()==Device::MemoryTechnology::Eprom );
  if (eprom) {
    log(Log::LineType::Information, i18n("  EPROM device: blank checking first..."));
    Pic::Memory tmemory(*device());
    VerifyData vdata(BlankCheckVerify, tmemory);
    if ( !internalRead(0, range, &vdata) ) return false;
    log(Log::LineType::Information, i18n("  Blank check successful"));
    // check if protecting device
    bool protectedCode = pmemory->isProtected(Pic::Protection::ProgramProtected, Pic::MemoryRangeType::Code);
    bool protectedEeprom = pmemory->isProtected(Pic::Protection::ProgramProtected, Pic::MemoryRangeType::Eeprom);
    if ( protectedCode || protectedEeprom ) {
      log(Log::LineType::SoftError, i18n("Protecting code memory or data EEPROM on OTP devices is disabled as a security..."));
      return false;
    }
  }

  // programming
  bool ok = true;
  if ( !range.all() ) {
    Pic::MemoryRangeType type = static_cast<const Pic::MemoryRange &>(range)._type;
    if ( (type==Pic::MemoryRangeType::Code && _hasProtectedCode)
         || (type==Pic::MemoryRangeType::Eeprom && _hasProtectedEeprom) ) {
      log(Log::LineType::SoftError, i18n("This memory range is programming protected."));
      return false;
    }
    if ( deviceSpecific()->canEraseRange(type) ) {
      if ( !(deviceSpecific()->capabilities() & EmulatedErase) && !eraseRange(type) ) return false;
    } else log(Log::LineType::Warning, i18n("The range cannot be erased first by the selected programmer so programming may fail..."));
    ok = programRange(type, pmemory->arrayForWriting(type));
    VerifyData vdata(NormalVerify, *pmemory);
    if (ok) ok = readRange(type, 0, &vdata);
  } else {
    if ( !eprom ) {
      if ( deviceSpecific()->capabilities() & CanEraseAll ) {
        if ( !(deviceSpecific()->capabilities() & EmulatedErase) ) {
          log(Log::LineType::Information, i18n("  Erasing device"));
          ok = ( !readConfigEntry(Config::PreserveEeprom).toBool() || preserveEeprom() );
          if (ok) ok = eraseAll();
        }
      } else log(Log::LineType::Warning, i18n("The device cannot be erased first by the selected programmer so programming may fail..."));
    }
    if (ok) ok = programAll(*pmemory);
  }
  if ( !restoreBandGapBits() ) return false;
  return ok;
}

//-----------------------------------------------------------------------------
bool Programmer::PicBase::checkProgramCalibration(const Device::Array &data)
{
  QString message, s = prettyCalibration(data);
  if ( !device()->checkCalibration(data, &message) ) {
    sorry(i18n("The calibration word %1 is not valid.").arg(s), message);
    return false;
  }
  return askContinue(i18n("Do you want to overwrite the device calibration with %1?").arg(s));
}

bool Programmer::PicBase::tryProgramCalibration(const Device::Array &data, bool &success)
{
  log(Log::LineType::Information, i18n("  Write memory: %1").arg(Pic::MemoryRangeType(Pic::MemoryRangeType::Cal).label()));
  success = true;
  if ( !deviceSpecific()->write(Pic::MemoryRangeType::Cal, data, true) ) return false;
  Device::Array read;
  if ( !deviceSpecific()->read(Pic::MemoryRangeType::Cal, read, 0) ) return false;
  for (uint i=0; i<data.count(); i++)
    if ( data[i]!=read[i] ) success = false;
  if ( !success ) return true;
  if ( device()->isWritable(Pic::MemoryRangeType::CalBackup) ) {
    if ( !deviceSpecific()->read(Pic::MemoryRangeType::CalBackup, read, 0) ) return false;
    if ( device()->checkCalibration(read) ) return true; // do not overwrite correct backup value
    log(Log::LineType::Information, i18n("  Write memory: %1").arg(Pic::MemoryRangeType(Pic::MemoryRangeType::CalBackup).label()));
    if ( !deviceSpecific()->write(Pic::MemoryRangeType::CalBackup, data, true) ) return false;
    if ( !deviceSpecific()->read(Pic::MemoryRangeType::CalBackup, read, 0) ) return false;
    for (uint i=0; i<data.count(); i++)
      if ( data[i]!=read[i] ) success = false;
  }
  return true;
}

bool Programmer::PicBase::internalProgramCalibration(const Device::Array &data)
{
  if ( !initProgramming(Task::Write) ) return false;
  // try without erase
  bool success = false;
  if ( !tryProgramCalibration(data, success) ) return false;
  if (success) return true;
  if ( !askContinue(i18n("Programming calibration data needs a chip erase. Continue anyway?")) ) {
    logUserAbort();
    return false;
  }
  log(Log::LineType::Information, i18n("  Erasing device"));
  const bool ok = deviceSpecific()->erase(_hasProtectedCode || _hasProtectedEeprom);
  if ( !restoreBandGapBits() ) return false;
  if ( !ok ) return false;
  // retry
  if ( !tryProgramCalibration(data, success) ) return false;
  return success;
}

bool Programmer::PicBase::programCalibration(const Device::Array &data)
{
  _progressMonitor.clear();
  bool ok = doProgramCalibration(data);
  endProgramming();
  return ok;
}

bool Programmer::PicBase::doProgramCalibration(const Device::Array &data)
{
  if ( !checkProgramCalibration(data) ) return false;
  if ( !doConnectDevice() ) return false;
  log(Log::LineType::Information, i18n("Programming calibration..."));
  _progressMonitor.setActionMessage(i18n("Programming calibration..."));
  if ( !internalProgramCalibration(data) ) return false;
  log(Log::LineType::Information, i18n("Programming calibration successful"));
  return true;
}

//-----------------------------------------------------------------------------
bool Programmer::PicBase::verifySingle(Pic::MemoryRangeType type, const Pic::Memory &memory)
{
  return verify(memory, Pic::MemoryRange(type));
}

bool Programmer::PicBase::blankCheckSingle(Pic::MemoryRangeType type)
{
  return blankCheck(Pic::MemoryRange(type));
}
