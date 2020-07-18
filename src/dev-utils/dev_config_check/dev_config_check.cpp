/***************************************************************************
 *   Copyright (C) 2006 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include <qdir.h>
#include <qstringlist.h>
#include <qregexp.h>
#include <qtextstream.h>

#include "dev-utils/dev_parser/dev_parser.h"
#include "devices/list/device_list.h"
#include "devices/base/device_group.h"
#include "devices/pic/base/pic.h"
#include "devices/pic/base/pic_config.h"

class ConfigChecker : public DevParser
{
public:
  ConfigChecker(const Pic::Data &data) : DevParser(data.name()), _data(data) {}
  virtual void doAction() const;

private:
  const Pic::Data &_data;

  void checkMemoryRange(Pic::MemoryRangeType type) const;
  QStringList findDevConfigMask(const QString &xmask) const;
  QString configName(uint i) const;
};

DevParser *DevParser::factory(const QString &name)
{
  const Device::Data *data = Device::Lister::instance().data(name);
  if ( data==0 ) {
    ::qWarning("Device \"%s\" not supported.", name.latin1());
    return 0;
  }
  if ( data->group().name()!="pic" ) return 0;
  return new ConfigChecker(static_cast<const Pic::Data &>(*data));
}

const char * const MEMORY_TECH[Device::MemoryTechnology::Nb_Types] = {
  "ee", "eprom", "rom", "romless"
};
const char * const MEMORY_RANGES[Pic::MemoryRangeType::Nb_Types] = {
  "pgmmem", "calmem", "userid", "devid", "cfgmem", "eedata", "bkbgvectmem", 0, 0
};

void ConfigChecker::checkMemoryRange(Pic::MemoryRangeType type) const
{
  if ( MEMORY_RANGES[type.type()]==0 ) return;
  const Section *mem = findUniqueSection(MEMORY_RANGES[type.type()]);
  if ( XOR(mem, _data.isPresent(type)) ) qFatal(QString("Memory range %1 presence does not match").arg(type.label()));
  if ( mem==0 ) return;
  if ( !mem->attributes.contains("region") ) qFatal(QString("Memory range %1 presence does not match").arg(type.label()));
  Address start, end;
  extractRange(mem->attributes["region"], start, end);
  if ( type==Pic::MemoryRangeType::Config && _data.architecture()==Pic::Architecture::Baseline && start==end && start==0xFFF ) return; // dev contains hexfile offset (?)
  if ( start!=_data.range(type).start ) qFatal(QString("Memory range %1 start does not match").arg(type.label()));
  if ( type==Pic::MemoryRangeType::DeviceId && _data.architecture()==Pic::Architecture::P30F ) end++; // #### not sure why...
  if ( type==Pic::MemoryRangeType::Code && _data.isPresent(Pic::MemoryRangeType::Cal) && _data.range(Pic::MemoryRangeType::Cal).start==_data.range(Pic::MemoryRangeType::Code).end+1 ) end--;
  if ( end!=_data.range(type).end )  qFatal(QString("Memory range %1 end does not match (%2 - %3)").arg(type.label()).arg(toHexLabel(end, 8)).arg(toHexLabel(_data.range(type).end, 8)));
  if ( type==Pic::MemoryRangeType::DeviceId ) {
    BitValue idmask;
    BitValue ridmask = fromHexLabel(mem->attributes["idmask"], NULL);
    BitValue id = _data.ids()[Device::Special::Normal];
    BitValue rid = fromHexLabel(mem->attributes["id"], NULL);
    switch (_data.architecture().type()) {
      case Pic::Architecture::Baseline:
      case Pic::Architecture::MidRange:
      case Pic::Architecture::EMidRange:
      case Pic::Architecture::P17C:
        idmask = 0x3FE0;
        break;
      case Pic::Architecture::P18C:
      case Pic::Architecture::P18F:
      case Pic::Architecture::P18J:
        idmask = 0xFFE0;
        break;
      case Pic::Architecture::P24F:
      case Pic::Architecture::P24H:
      case Pic::Architecture::P30F:
      case Pic::Architecture::P33F:
        idmask = 0xFFFF0000;
        rid = rid >> 16;
        break;
      case Pic::Architecture::Nb_Types: break;
    }
    if (ridmask != idmask) qFatal(QString("Id mask is not %1: %2").arg(toHexLabel(idmask, 8)).arg(mem->attributes["idmask"]));
    if (rid != id) qFatal(QString("Id does not match (%1 - %2)").arg(toHexLabel(id, 4)).arg(mem->attributes["id"]));
  }
}

struct ConfigData {
  const char *xmask, *dmask;
};

const ConfigData CONFIG_DATA[] = {
  { "CP",  0 },
  { "CPD", 0 },
  { "CPC", 0 },
  { "CPB", 0 },

  { "WRT",  "WRT_ENABLE" },
  { "WRTD", 0 },
  { "WRTB", 0 },
  { "WRTC", 0 },

  { "EBTR",  0 },
  { "EBTRB", 0 },

  { "WDTE",  "WDT" },
  { "WDTE",  "WDTEN" },
  { "MCLRE", "MCLRE" },
  { "FOSC",  "OSC" },
//    { "EXTRC", I18N_NOOP("External RC oscillator") }, { "EXTRC_CLKOUT", I18N_NOOP("External RC oscillator with CLKOUT") }, { "EXTRC_IO", I18N_NOOP("External RC oscillator (no CLKOUT)") },
//    { "INTRC", I18N_NOOP("Internal oscillator") }, { "INTRC_CLKOUT", I18N_NOOP("Internal oscillator with CLKOUT") }, { "INTRC_IO", I18N_NOOP("Internal oscillator (no CLKOUT)") },
//    { "XT", I18N_NOOP("Crystal/resonator") }, { "LP", I18N_NOOP("Low power crystal") },
//    { "EC", I18N_NOOP("External clock") }, { "EC_CLKOUT", I18N_NOOP("External clock with CLKOUT") }, { "EC_IO", I18N_NOOP("External clock (no CLKOUT)") },
//    { "E4_CLKOUT", I18N_NOOP("External clock with 4x PLL and with CLKOUT") },
//    { "E4_IO", I18N_NOOP("External clock with 4x PLL (no CLKOUT)") },
//    { "E4S_IO", I18N_NOOP("External clock with software controlled 4x PLL (no CLKOUT)") },
//    { "ER", I18N_NOOP("External resistor") }, { "ER_CLKOUT", I18N_NOOP("External resistor with CLKOUT") }, { "ER_IO", I18N_NOOP("External resistor (no CLKOUT)") },
//    { "HS", I18N_NOOP("High speed crystal/resonator") }, { "H4", I18N_NOOP("High speed crystal/resonator with 4x PLL") },
//    { "H4S", I18N_NOOP("High speed crystal/resonator with software controlled 4x PLL") },
//    { 0, 0 } } },
  { "PWRTE", "PUT" },
  { "PWRTE", "PWRTEN" },
  { "BG",    0 },
  { "BODEN", "BOREN" },
  { "FCMEN", 0 },
  { "IESO",  0 },
  { "WUREN",  "WUR" },
  { "DEBUG", "BACKBUG" },
  { "MPEEN", "PARITY" },
  { "BORV",  "BODENV" },
  { "LVP",   0 },
  { "CCP2MX", "CCP2MUX" },
  { "CCP1MX", "CCP1MUX" },
  { "BORSEN", 0 },
  { "WDT", "WDTEN" },
  { "WDTPS", "WDPS" },
  { "PM",    "PMODE" },
//    {"Extended microcontroller", I18N_NOOP("Extended microcontroller") }, { "Microcontroller", I18N_NOOP("Microcontroller") },
//    { "Microprocessor", I18N_NOOP("Microprocessor") }, { "Code protected microcontroller", I18N_NOOP("Code protected microcontroller") },
//    { "Microprocessor with boot", I18N_NOOP("Microprocessor with boot block") }, {0, 0} } },
  { "OSCSEN", "OSCS" },
  { "STVREN", "STVR" },
  { "BW",     "BUS"  },
  { "PBADEN", 0 },
  { "WINEN", 0 },
  { "HPOL", 0 },
  { "LPOL", 0 },
  { "PWMPIN", 0 },
  { "T1OSCMX", 0 },
  { "EXCLKMX", 0 },
  { "FLTAMX", 0 },
  { "PWM4MX", 0 },
  { "SSPMX", 0 },
  { "LPT1OSC", 0 },
  { "XINST", "ENHCPU" },
  { "BBSIZ", 0 },
  { "ICPORT", "ENICPORT" },
  { "VREGEN", 0 },
  { "WAIT", 0 },
  { "ABW", 0 },

  { "FCKSM", "FCKSMEN" },
//    { "Switching off, monitor off", I18N_NOOP("Switching off, monitor off") }, { "Switching off, monitor off", I18N_NOOP("Switching off, monitor off") },
//    { "Switching on, monitor off", I18N_NOOP("Switching on, monitor off") }, { "Switching on, monitor on", I18N_NOOP("Switching on, monitor on") }, {0, 0} } },
  { "FOS", 0 },
//    { "INTRC_F", I18N_NOOP("Internal fast RC") }, { "INTRC_LP", I18N_NOOP("Internal low-power RC") },
//    { "PRIM", I18N_NOOP("Primary") }, { "TMR1", I18N_NOOP("Timer1") }, {0, 0} } },
//  { { "FPR", I18N_NOOP("Primary oscillator mode") }, Fixed, {
//    { "XTL", I18N_NOOP("Low-power/low-frequency crystal") }, { "HS", I18N_NOOP("High speed crystal") },
//    { "XT", I18N_NOOP("Crystal") }, { "X4", I18N_NOOP("Crystal with 4x PLL") }, { "X8", I18N_NOOP("Crystal with 8x PLL") }, { "X16", I18N_NOOP("Crystal with 16x PLL") },
//    { "EC_CLKOUT", I18N_NOOP("External clock with CLKOUT") }, { "EC_IO", I18N_NOOP("External clock (no CLKOUT)") },
//    { "E4_CLKOUT", I18N_NOOP("External clock with 4x PLL and with CLKOUT") }, { "E8_CLKOUT", I18N_NOOP("External clock with 8x PLL and with CLKOUT") }, { "E16_CLKOUT", I18N_NOOP("External clock with 16x PLL and with CLKOUT") },
//    { "EXTRC_CLKOUT", I18N_NOOP("External RC oscillator with CLKOUT") }, { "EXTRC_IO", I18N_NOOP("External RC oscillator (no CLKOUT)") }, {0, 0} } },
  { "FWPSA",  0 },
  { "FWPSB",  0 },
  { "FWDTEN", "WDT" },
  { "FPWRT",  0 },
  { "GCP",    0 },
  { "GWRP",   0 },
  { "COE",    0 },
  { "ICS",    0 },

  { "USBDIV", "USBPLL" },
  { "CPUDIV", 0 },
  { "PLLDIV", 0 },

  { "MCPU", 0 },
  { "IOSCFS", "IOFSCS" },

  { 0, 0 }
};

QStringList ConfigChecker::findDevConfigMask(const QString &xmask) const
{
  QStringList list;
  if ( _data.name().startsWith("17") && xmask=="WDTPS" ) {
    list.append("WDT");
  } else {
    QString mask = xmask;
    mask.replace('_', QString::null);
    for (uint i=0; CONFIG_DATA[i].xmask; i++) {
      if ( CONFIG_DATA[i].xmask!=mask ) continue;
      if ( CONFIG_DATA[i].dmask!=0 ) list.append(CONFIG_DATA[i].dmask);
    }
    list.append(xmask);
    list.append(mask);
    if ( xmask=="CP" ) list.append("CP_0");
    if ( xmask=="WRT" ) list.append("WRT_0");
    if ( xmask=="EBTR" ) list.append("EBTR_0");
  }
  return list;
}

QString ConfigChecker::configName(uint i) const
{
  const Pic::Config &c = _data.config();
  if ( _data.name().startsWith("18") ) return "CONFIG" + QString::number(i/2+1) + (i%2 ? "H" : "L");
  if ( (_data.name()=="16F87" || _data.name()=="16F88") && i==0 ) return "CONFIG";
  if ( !c._words[i].name.isEmpty() ) return c._words[i].name;
  return "CONFIG";
}

void ConfigChecker::doAction() const
{
  const Section *pgming = findUniqueSection("pgming");
  if ( pgming==0 ) qFatal("No programming section");
  if ( pgming->attributes["memtech"]!=MEMORY_TECH[_data.memoryTechnology().type()] ) qFatal("Memory technology different.");

  FOR_EACH(Pic::MemoryRangeType, i) checkMemoryRange(i);

  //uint cmask = _data.range(Device::MemoryConfig).mask;
  const Pic::Config &c = _data.config();
  for (int i=0; i<c._words.count(); i++) {
    const Pic::Config::Word &w = c._words[i];
    QStringList keys;
    keys.append(configName(i));
    const Section *cw = findSection(_sections, "cfgbits", keys);
    if ( cw==0 ) {
      if ( c._words[i].masks.count() ) qFatal(QString("Config %1 not present in .dev file").arg(configName(i)));
      continue;
    }
    //if ( fromHexLabel(cw->attributes["unused"], 0)!=(cmask & ~w.usedMask() ) ) qWarning(QString("Unused bits do not match in %1 (%2 - %3)").arg(w.name).arg(toHexLabel(cmask & ~w.usedMask(), 4)).arg(cw->attributes["unused"]));
    for (int k=0; k<w.masks.count(); k++) {
      const Pic::Config::Mask &m = w.masks[k];
      QStringList masks = findDevConfigMask(m.name);
      const Section *cm = findSection(cw->sections, "field", masks);
      if ( cm==0 ) qFatal(QString("Mask %1 does not exist in .dev file").arg(m.name));
      // #### TODO
    }
    for (int k=0; k<cw->sections.count(); k++) {
      if ( cw->sections[k].name!="field" ) continue;
      if ( cw->sections[k].attributes["flags"].find("h")!=-1 ) continue;
      bool in = false;
      for (int l=0; l<w.masks.count(); l++) {
        QStringList masks = findDevConfigMask(w.masks[l].name);
        if ( masks.find(cw->sections[k].attributes["key"])==masks.end() ) continue;
        in = true;
        break;
      }
      if ( !in ) qFatal(QString("Mask %1 does not exist in xml file").arg(cw->sections[k].attributes["key"]));
    }
  }
  for (int i=0; i<_sections.count(); i++) {
    if ( _sections[i].name!="cfgbits" ) continue;
    bool in = false;
    for (int k=0; k<c._words.count(); k++) {
      QStringList masks = findDevConfigMask(configName(k));
      if ( masks.find(_sections[i].attributes["key"])==masks.end() ) continue;
      in = true;
      break;
    }
    if ( !in ) qFatal(QString("Config %1 does not exist in xml file").arg(_sections[i].attributes["key"]));
  }
}

//-----------------------------------------------------------------------------
int main(int argc, char **argv)
{
  QStringList devices;
  DevParser::loopFiles(argc, argv, devices);

  QStringList all = Device::Lister::instance().supportedDevices();
  for (int i=0; i<all.count(); i++)
    if ( devices.find(all[i])==devices.end() ) qFatal("Device \"%s\" not in any .dev file.", all[i].latin1());

  return 0;
}
