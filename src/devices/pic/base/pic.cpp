/***************************************************************************
 *   Copyright (C) 2005 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "pic.h"

#include "common/common/misc.h"
#include "common/global/purl.h"
#include "pic_register.h"
#include "pic_config.h"

//------------------------------------------------------------------------------
const Pic::ProgVoltageType::Data Pic::ProgVoltageType::DATA[Nb_Types] = {
  { "vpp"            },
  { "vdd_prog"       },
  { "vdd_prog_write" }
};

const Pic::MemoryRangeType::Data Pic::MemoryRangeType::DATA[Nb_Types] = {
  { "code",               I18N_NOOP("Code memory"),         Writable },
  { "calibration",        I18N_NOOP("Calibration"),         Writable },
  { "user_ids",           I18N_NOOP("User IDs"),            Writable },
  { "device_id",          I18N_NOOP("Device ID"),           ReadOnly },
  { "config",             I18N_NOOP("Configuration Bits"),  Writable },
  { "eeprom",             I18N_NOOP("Data EEPROM"),         Writable },
  { "debug_vector",       I18N_NOOP("Debug Vector"),        Writable },
  { "hardware_stack",     I18N_NOOP("Hardware Stack"),      ReadOnly },
  { "calibration_backup", I18N_NOOP("Calibration Backup"),  Writable },
  { "program_executive",  I18N_NOOP("Program Executive"),   Writable }
};

const Pic::Architecture::Data Pic::Architecture::DATA[Nb_Types] = {
//  name family_label nbBytesPC nbBytesWord nbBitsRegister registerBankLength
//  {Code, Cal, UserID, DevId, Conf, EEPROM, DebugVec, HardStack, CalBackup, Program Executive}
//  properties nominalVdd devIdMask
  { "baseline",  I18N_NOOP("Baseline Family"),            0, 2,  8, 0x020, { 12, 12, 12, 12, 12,  8, 12, 0, 12,  0 }, NoArchitectureProperty,                          5.0, 0xFFE0 }, // 9, 10, 11 or 12-bit program counter
  { "midrange",  I18N_NOOP("Mid-Range Family"),          13, 2,  8, 0x080, { 14, 14, 14, 14, 14,  8, 14, 0, 14,  0 }, MaybeSelfWrite,                                  5.0, 0xFFE0 }, // max eeprom: 256 words
  { "emidrange", I18N_NOOP("Enhanced Mid-Range Family"), 13, 2,  8, 0x080, { 14, 14, 14, 14, 14,  8, 14, 0, 14,  0 }, MaybeSelfWrite,                                  5.0, 0xFFE0 }, // max eeprom: 256 words
  { "17C",       I18N_NOOP("17C Family"),                16, 2,  8, 0x100, { 16,  0,  0,  0, 16,  8,  0, 0,  0,  0 }, RandomMemoryAccess,                              5.0, 0xFFE0 },
  { "18C",       I18N_NOOP("18C Family"),                21, 2,  8, 0x100, { 16,  8,  8,  8,  8,  8, 16, 0,  8,  0 }, PackedHex | RandomMemoryAccess,                  5.0, 0xFFE0 },
  { "18F",       I18N_NOOP("18F Family"),                21, 2,  8, 0x100, { 16,  8,  8,  8,  8,  8, 16, 0,  8,  0 }, PackedHex | RandomMemoryAccess | MaybeSelfWrite, 5.0, 0xFFE0 },
  { "18J",       I18N_NOOP("18J Family"),                21, 2,  8, 0x100, { 16,  8,  8,  8,  8,  8, 16, 0,  8,  0 }, PackedHex | RandomMemoryAccess | SelfWrite,      3.3, 0xFFE0 },
  { "24F",       I18N_NOOP("24F Family"),                23, 4, 16, 0x800, { 24,  0,  0, 16, 16,  0, 24, 0,  0, 24 }, RandomMemoryAccess | SelfWrite,                  3.3, 0xFFFF },
  { "24H",       I18N_NOOP("24H Family"),                23, 4, 16, 0x800, { 24,  0,  8, 16,  8,  0, 24, 0,  0, 24 }, RandomMemoryAccess | SelfWrite,                  3.3, 0xFFFF },
  { "30F",       I18N_NOOP("30F Family"),                23, 4, 16, 0xA00, { 24,  0, 24, 16, 16, 16, 24, 0,  0, 24 }, RandomMemoryAccess | SelfWrite,                  5.0, 0xFFFF }, // dsPIC: eeprom max = 2 kwords = 4 kbytes
  { "33F",       I18N_NOOP("33F Family"),                23, 4, 16, 0x800, { 24,  0, 16, 16, 16,  0, 24, 0,  0, 24 }, RandomMemoryAccess | SelfWrite,                  3.3, 0xFFFF }
};

const Pic::Checksum::Algorithm::Data Pic::Checksum::Algorithm::DATA[Nb_Types] = {
  { ""      },
  { "XOR4"  },
  { "XNOR7" },
  { "XNOR8" }
};

const Pic::Feature::Data Pic::Feature::DATA[Nb_Types] = {
  { "ccp", I18N_NOOP("CCP")      },
  { "adc", I18N_NOOP("ADC")      },
  { "ssp", I18N_NOOP("SSP")      },
  { "lvd", I18N_NOOP("Low Voltage Detect") },
  { "usb", I18N_NOOP("USB")      },
  { "usart",           I18N_NOOP("USART")    },
  { "can",             I18N_NOOP("CAN")      },
  { "ecan",            I18N_NOOP("ECAN")     },
  { "ethernet",        I18N_NOOP("Ethernet") },
  { "lcd",             I18N_NOOP("LCD")      },
  { "motor_control",   I18N_NOOP("Motor Control") },
  { "motion_feedback", I18N_NOOP("Motion Feeback") },
  { "self_write",      I18N_NOOP("Self-Write") }
};

//-----------------------------------------------------------------------------
Pic::Data::Data()
{
  _progFamilyIndex = -1;
  _registersData.reset(new RegistersData(*this));
  FOR_EACH(ProgVoltageType, type) {
    _voltages[type].min = 0.0;
    _voltages[type].max = 0.0;
    _voltages[type].nominal = 0.0;
  }
  FOR_EACH(MemoryRangeType, type) {
    _ranges[type].properties = NotPresent;
    _ranges[type].start = 0;
    _ranges[type].end = 0;
    _ranges[type].hexFileOffset = 0;
  }
  _config.reset(new Config(*this));
  _calibration.opcode = 0;
  _calibration.opcodeMask = 0;
}

bool Pic::Data::isReadable(MemoryRangeType type) const
{
  return ( range(type).properties & Programmable );
}

bool Pic::Data::isWritable(MemoryRangeType type) const
{
  return ( (type.data().properties & Writable) && (range(type).properties & Programmable) );
}

uint Pic::Data::addressIncrement(MemoryRangeType type) const
{
  uint inc = _architecture.data().nbBytesWord;
  if ( (_architecture.data().properties & PackedHex)
       && ( type==MemoryRangeType::Code || type==MemoryRangeType::DebugVector ) ) return inc;
  return inc / 2;
}

uint Pic::Data::nbWords(MemoryRangeType type) const
{
  if ( !isPresent(type) ) return 0;
  return nbAddresses(type) / addressIncrement(type);
}

uint Pic::Data::nbAddresses(MemoryRangeType type) const
{
  if ( !isPresent(type) ) return 0;
  return (range(type).end - range(type).start + 1);
}

QString Pic::Data::fname(Device::Special special) const
{
  QString s = name();
  switch (special.type()) {
    case Device::Special::Normal: break;
    case Device::Special::LowPower:
      // assume name is of form "NNX..."
      s.insert(2, 'L');
      break;
    case Device::Special::LowVoltage:
      // assume name is of form "NNXN..."
      s.replace(2, 1, "LV");
      break;
    case Device::Special::HighVoltage:
      // assume name is of form "NNXN..."
      s.replace(2, 1, "HV");
      break;
    case Device::Special::Nb_Types: ASSERT(false); break;
  }
  return s;
}

BitValue Pic::Data::devIdMask() const
{
  return architecture().data().devIdMask;
}

bool Pic::Data::matchId(BitValue rawId, Device::IdData &idata) const
{
  if ( !isPresent(MemoryRangeType::DeviceId) ) return false;
  QMap<Device::Special, BitValue>::const_iterator it;
  for (it=_ids.begin(); it!=_ids.end(); ++it) {
    idata.special = it.key();
    BitValue nid = 0x0;
    switch (architecture().type()) {
      case Architecture::Baseline:
      case Architecture::MidRange:
      case Architecture::EMidRange:
      case Architecture::P17C:
      case Architecture::P18C:
      case Architecture::P18F:
      case Architecture::P18J:
        nid = rawId;
        idata.revision = rawId.maskWith(0x1F);
        break;
      case Architecture::P24F:
        nid = (rawId >> 16);
        idata.revision = (rawId >> 6).maskWith(0x7);
        idata.minorRevision = rawId.maskWith(0x7);
        break;
      case Architecture::P30F:
        nid = (rawId >> 16);
        idata.revision = (rawId >> 6).maskWith(0x3F);
        idata.minorRevision = rawId.maskWith(0x3F);
        idata.process = (rawId >> 12).maskWith(0xF);
        break;
      case Architecture::P24H:
      case Architecture::P33F:
        nid = (rawId >> 16);
        idata.revision = rawId.maskWith(0xFFFF); // ??
        break;
      case Architecture::Nb_Types: ASSERT(false); break;
    }
    nid = nid.maskWith(devIdMask());
    if ( nid==it.data() ) return true;
  }
  return false;
}

QStringList Pic::Data::idNames(const QMap<QString, Device::IdData> &ids) const
{
  QStringList list;
  QMap<QString, Device::IdData>::const_iterator it;
  for (it=ids.begin(); it!=ids.end(); ++it) {
    switch (_architecture.type()) {
      case Architecture::Baseline:
      case Architecture::MidRange:
      case Architecture::EMidRange:
      case Architecture::P17C:
      case Architecture::P18C:
      case Architecture::P18F:
      case Architecture::P18J:
        list += i18n("%1 (rev. %2)").arg(it.key()).arg(toLabel(it.data().revision));
        break;
      case Architecture::P24F:
        list += i18n("%1 (rev. %2.%3)").arg(it.key()).arg(toLabel(it.data().revision)).arg(toLabel(it.data().minorRevision));
        break;
      case Architecture::P30F:
        list += i18n("%1 (proc. %2; rev. %3.%4)").arg(it.key()).arg(toLabel(it.data().process)).arg(toLabel(it.data().revision)).arg(toLabel(it.data().minorRevision));
        break;
      case Architecture::P24H:
      case Architecture::P33F:
        list += i18n("%1 (rev. %2)").arg(it.key()).arg(toLabel(it.data().revision));
        break;
      case Architecture::Nb_Types: ASSERT(false); break;
    }
  }
  return list;
}

bool Pic::Data::isCalibrationAtCodeEnd() const
{
  Address end = range(MemoryRangeType::Code).end + addressIncrement(MemoryRangeType::Code);
  return (range(MemoryRangeType::Cal).start == end);
}

bool Pic::Data::checkCalibration(const Device::Array &data, QString *message) const
{
  ASSERT( nbWords(MemoryRangeType::Cal)==data.count() );
  for (uint i=0; i<data.count(); i++) {
    QString address = toHexLabel(range(MemoryRangeType::Cal).start + i*addressIncrement(MemoryRangeType::Cal), nbCharsAddress());
    if ( data[i]==mask(MemoryRangeType::Cal) ) {
      if (message) *message = i18n("Calibration word at address %1 is blank.").arg(address);
      return false;
    }
  }
  if ( data.count()==1 ) {
    if ( data[0].maskWith(_calibration.opcodeMask)!=_calibration.opcode ) {
      if (message) *message = i18n("Calibration word is not a compatible opcode (%2).")
                              .arg(toHexLabel(_calibration.opcode, nbCharsWord(MemoryRangeType::Code)));
      return false;
    }
  }
  return true;
}

const Pic::RegistersData &Pic::Data::registersData() const
{
  return static_cast<const RegistersData &>(*_registersData);
}

bool Pic::Data::hasFeature(Feature feature, bool *unknown) const
{
  bool ok = ( registersData().nbBanks!=0 );
  if (unknown) *unknown = !ok;
  if (!ok) return false;
  switch (feature.type()) {
    case Feature::CCP: return registersData().sfrs.contains("CCP1CON");
    case Feature::ADC: return registersData().sfrs.contains("ADCON0");
    case Feature::SSP: return registersData().sfrs.contains("SSPCON");
    case Feature::LVD: return registersData().sfrs.contains("LVDCON");
    case Feature::USB: return registersData().sfrs.contains("UCON");
    case Feature::USART:
      return ( registersData().sfrs.contains("TXSTA")        // 16F
               || registersData().sfrs.contains("TXSTA1")    // 18F
               || registersData().sfrs.contains("U1MODE") ); // 30F
    case Feature::CAN: return registersData().sfrs.contains("CANCON") && !registersData().sfrs.contains("ECANCON");
    case Feature::ECAN: return registersData().sfrs.contains("ECANCON");
    case Feature::Ethernet: return registersData().sfrs.contains("ETHCON1");
    case Feature::LCD: return registersData().sfrs.contains("LCDCON");
    case Feature::MotorControl: return registersData().sfrs.contains("PWMCON0");
    case Feature::MotionFeedback: return registersData().sfrs.contains("CAP1CON");
    case Feature::SelfWrite: return _selfWrite;
    case Feature::Nb_Types: ASSERT(false); break;
  }
  return false;
}

Device::Array Pic::Data::gotoInstruction(Address address, bool withPageSelection) const
{
  ASSERT( address<addressIncrement(MemoryRangeType::Code)*nbWords(MemoryRangeType::Code) );
  Device::Array a;
  switch (_architecture.type()) {
  case Architecture::Baseline:
    if ( nbWords(MemoryRangeType::Code)>0x1FF && withPageSelection)
      a.append(0x4A3 | (address>0x1FF ? 0x100 : 0x000)); // bsf STATUS,PA0 or bcf STATUS,PA0
    a.append(0xA00 | (address.toUInt() & 0x1FF)); // goto
    break;
  case Architecture::MidRange:
  case Architecture::EMidRange:
    if ( nbWords(MemoryRangeType::Code)>0x7FF && withPageSelection ) {
      if ( address<=0x7FF ) a.append(0x018A); // clrf PCLATH
      else {
        a.append(0x3000 | (address.toUInt() >> 8)); // movl high address
        a.append(0x008A); // movwf PCLATH
      }
    }
    a.append(0x2800 | (address.toUInt() & 0x7FF));
    break;
  case Architecture::P17C:
    a.append(0xC000 | (address.toUInt() & 0x1FFF));
    break;
  case Architecture::P18C:
  case Architecture::P18F:
  case Architecture::P18J:
    a.append(0xEF00 | ((address.toUInt()/2) & 0xFF));
    a.append(0xF000 | ((address.toUInt()/2) >> 8));
    break;
  case Architecture::P24F:
  case Architecture::P24H:
  case Architecture::P30F:
  case Architecture::P33F:
    a.append(0x040000 | (address.toUInt() & 0x00FFFE));
    a.append(0X000000 | (address.toUInt() >> 16));
    break;
  case Architecture::Nb_Types: ASSERT(false); break;
  }
  return a;
}

bool Pic::Data::isGotoInstruction(BitValue instruction) const
{
  switch (_architecture.type()) {
  case Architecture::Baseline: return ( instruction.maskWith(0xE00)==0xA00 );
  case Architecture::MidRange:
  case Architecture::EMidRange: return ( instruction.maskWith(0xF800)==0x2800 );
  case Architecture::P17C: return ( instruction.maskWith(0xE000)==0xC000 );
  case Architecture::P18C:
  case Architecture::P18F:
  case Architecture::P18J: return ( instruction.maskWith(0xFF00)==0xEF00 );
  case Architecture::P24F:
  case Architecture::P24H:
  case Architecture::P30F:
  case Architecture::P33F: return ( instruction.maskWith(0xFF0000)==0x040000 );
  case Architecture::Nb_Types: ASSERT(false); break;
  }
  return false;
}

uint Pic::Data::nbWordsWriteAlignment(MemoryRangeType type) const
{
  if (type != MemoryRangeType::Code && type != MemoryRangeType::ProgramExecutive) return 1;
  if (progFamilyData() != NULL) return progFamilyData()->codeLatches / nbBytesWord(type);
  if (_nbWordsCodeWrite != 0) return _nbWordsCodeWrite;
  switch (architecture().type()) {
    case Architecture::P24F:
    case Architecture::P24H:
    case Architecture::P33F: return 64;
    case Architecture::P30F: return 32; // ?
    default:                 return 16; // default ??
  }
}

uint Pic::Data::nbWordsRowEraseAlignment(MemoryRangeType type) const
{
  if (type != MemoryRangeType::Code && type != MemoryRangeType::ProgramExecutive) return 1;
  if (progFamilyData() != NULL) return progFamilyData()->rowEraseLatches / nbBytesWord(type);
  if (_nbWordsCodeRowErase != 0) return _nbWordsCodeRowErase;
  switch (architecture().type()) {
    case Architecture::P24F:
    case Architecture::P24H:
    case Architecture::P33F: return 512;
    default:                 return 16; // default ??
  }
}

const Pic::ProgFamilyData* Pic::Data::progFamilyData() const
{
  if (_progFamilyIndex == -1) return NULL;
  return &PROG_FAMILY_DATA[_progFamilyIndex];
}

//----------------------------------------------------------------------------
QDataStream &operator <<(QDataStream &s, const Pic::VoltageData &vd)
{
  s << vd.min << vd.max << vd.nominal;
  return s;
}
QDataStream &operator >>(QDataStream &s, Pic::VoltageData &vd)
{
  s >> vd.min >> vd.max >> vd.nominal;
  return s;
}

QDataStream &operator <<(QDataStream &s, const Pic::MemoryRangeData &mrd)
{
  s << Q_UINT8(mrd.properties) << mrd.start << mrd.end << mrd.hexFileOffset;
  return s;
}
QDataStream &operator >>(QDataStream &s, Pic::MemoryRangeData &mrd)
{
  Q_UINT8 properties;
  s >> properties >> mrd.start >> mrd.end >> mrd.hexFileOffset;
  mrd.properties = Pic::MemoryRangeProperties(properties);
  return s;
}

QDataStream &operator <<(QDataStream &s, const Pic::Checksum::Data &cd)
{
  s << cd.constant << cd.bbsize << cd.algorithm << cd.protectedMaskNames;
  s << cd.blankChecksum << cd.checkChecksum;
  return s;
}
QDataStream &operator >>(QDataStream &s, Pic::Checksum::Data &cd)
{
  s >> cd.constant >> cd.bbsize >> cd.algorithm >> cd.protectedMaskNames;
  s >> cd.blankChecksum >> cd.checkChecksum;
  return s;
}

QDataStream &operator <<(QDataStream &s, const Pic::CalibrationData &cd)
{
  s << cd.opcode << cd.opcodeMask;
  return s;
}
QDataStream &operator >>(QDataStream &s, Pic::CalibrationData &cd)
{
  s >> cd.opcode >> cd.opcodeMask;
  return s;
}

QDataStream &Pic::operator <<(QDataStream &s, const Pic::Data &data)
{
  s << static_cast<const Device::Data &>(data);
  s << data._architecture << data._ids << data._nbBitsPC;
  s << data._voltages << data._ranges;
  s << data._userIdRecommendedMask;
  s << *data._config;
  s << data._checksums;
  s << data._calibration;
  s << static_cast<const Pic::RegistersData &>(*data._registersData);
  s << data._nbWordsCodeWrite << data._nbWordsCodeRowErase;
  s << Q_UINT8(data._selfWrite);
  s << Q_INT8(data._progFamilyIndex);
  return s;
}
QDataStream &Pic::operator >>(QDataStream &s, Pic::Data &data)
{
  s >> static_cast<Device::Data &>(data);
  s >> data._architecture >> data._ids >> data._nbBitsPC;
  s >> data._voltages >> data._ranges;
  s >> data._userIdRecommendedMask;
  s >> *data._config;
  s >> data._checksums;
  s >> data._calibration;
  s >> static_cast<Pic::RegistersData &>(*data._registersData);
  s >> data._nbWordsCodeWrite >> data._nbWordsCodeRowErase;
  Q_UINT8 v;
  s >> v;
  data._selfWrite = v;
  Q_INT8 sv;
  s >> sv;
  data._progFamilyIndex = sv;
  return s;
}
