/***************************************************************************
 *   Copyright (C) 2006-2007 Nicolas Hadacek <hadacek@kde.org>             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "pickit2_to_data.h"

#include "devices/list/device_list.h"
#include "devices/pic/pic/pic_group.h"
#include "devices/pic/base/pic_config.h"

//-----------------------------------------------------------------------------
FamilyData::FamilyData(QFile &file)
{
  arch = Pic::Architecture::Nb_Types;

  extract(file, id);
  extract(file, type);
  extract(file, searchPriority);
  name = extractString(file, 16);
  qDebug("family #%i: %s type=%i spriority=%i", id, name.latin1(), type, searchPriority);
  extract(file, progEntryScript);
  extract(file, progExitScript);
  extract(file, readDevIdScript);
  extract(file, devIdMask);
  extract(file, blankValue);
  extract(file, bytesPerLocation);
  extract(file, addressIncrement);
  extract(file, partDetect);
  extract(file, unused1);
  extract(file, eeMemBytesPerWord);
  extract(file, eeMemAddressIncrement);
  extract(file, userIdHexBytes);
  extract(file, userIdBytes);
  extract(file, progMemHexBytes);
  extract(file, eeMemHexBytes);
  extract(file, progMemShift);
  extract(file, testMemoryStart);
  extract(file, testMemoryLength);
  extract(file, vpp);
}

DeviceData::DeviceData(QFile &file)
{
  Name = extractString(file, 28);
  extract(file, Family);
  extract(file, DeviceId);
  extract(file, ProgramMem);
  extract(file, EEMem);
  extract(file, EEAddr);
  extract(file, ConfigWords);
  extract(file, ConfigAddr);
  extract(file, UserIdWords);
  extract(file, UserIdAddr);
  extract(file, BandGapMask);
  extract(file, ConfigMasks, 8);
  extract(file, ConfigBlank, 8);
  extract(file, CPMask);
  extract(file, CPConfig);
  extract(file, OsccalSave);
  extract(file, IgnoreAddress);
  extract(file, VddMin);
  extract(file, VddMax);
  extract(file, VddErase);
  extract(file, CalibrationWords);
  extract(file, ChipEraseScript);
  extract(file, ProgMemAddrSetScript);
  extract(file, ProgMemAddrBytes);
  extract(file, ProgMemRdScript);
  extract(file, ProgMemRdWords);
  extract(file, EERdPrepScript);
  extract(file, EERdScript);
  extract(file, EERdLocations);
  extract(file, UserIDRdPrepScript);
  extract(file, UserIDRdScript);
  extract(file, ConfigRdPrepScript);
  extract(file, ConfigRdScript);
  extract(file, ProgMemWrPrepScript);
  extract(file, ProgMemWrScript);
  extract(file, ProgMemWrWords);
  extract(file, ProgMemPanelBufs);
  extract(file, ProgMemPanelOffset);
  extract(file, EEWrPrepScript);
  extract(file, EEWrScript);
  extract(file, EEWrLocations);
  extract(file, UserIDWrPrepScript);
  extract(file, UserIDWrScript);
  extract(file, ConfigWrPrepScript);
  extract(file, ConfigWrScript);
  extract(file, OSCCALRdScript);
  extract(file, OSCCALWrScript);
  extract(file, DPMask);
  extract(file, WriteCfgOnErase);
  extract(file, BlankCheckSkipUsrIDs);
  extract(file, IgnoreBytes);
  extract(file, ChipErasePrepScript);
  extract(file, UNUSED3);
  extract(file, UNUSED4);
  extract(file, ProgMemEraseScript);
  extract(file, EEMemEraseScript);
  extract(file, ConfigMemEraseScript);
  extract(file, reserved1EraseScript);
  extract(file, reserved2EraseScript);
  extract(file, TestMemoryRdScript);
  extract(file, TestMemoryRdWords);
  extract(file, EERowEraseScript);
  extract(file, EERowEraseWords);
  extract(file, ExportToMPLAB);
  extract(file, DebugHaltScript);
  extract(file, DebugRunScript);
  extract(file, DebugStatusScript);
  extract(file, DebugReadExecVerScript);
  extract(file, DebugSingleStepScript);
  extract(file, DebugBulkWrDataScript);
  extract(file, DebugBulkRdDataScript);
  extract(file, DebugWriteVectorScript);
  extract(file, DebugReadVectorScript);
  extract(file, DebugRowEraseScript);
  extract(file, DebugRowEraseSize);
  extract(file, DebugReserved5Script);
  extract(file, DebugReserved6Script);
  extract(file, DebugReserved7Script);
  extract(file, DebugReserved8Script);
  extract(file, DebugReserved9Script);
}

ScriptData::ScriptData(QFile &file)
{
  extract(file, Number);
  Name = extractString(file, 32);
  extract(file, Version);
  extract(file, UNUSED1);
  ushort length;
  extract(file, length);
  Script.resize(length);
  for (ushort i=0; i<length; i++) extract(file, Script[i]);
  Comment = extractString(file, 128);
  //  qDebug("script %i: \"%s\" version=%i length=%i comment=\"%s\"", Number, Name.data(), Version, length, Comment.data());
}

//-----------------------------------------------------------------------------
int main(int argc, char **argv)
{
  if ( argc!=2 ) qFatal("Need file as argument.\n");

  QFile file(argv[1]);
  if ( !file.open(IO_ReadOnly) ) qFatal("Could not open file \"%s\".\n", argv[1]);

  QFile dfile("pickit2v2_data.cpp");
  if ( !dfile.open(IO_WriteOnly) ) qFatal("Could not open \"%s\" for writing", dfile.name().latin1());
  QTextStream ds(&dfile);
  ds << "// this file is autogenerated, do not edit !" << endl << endl;
  ds << "#include \"devices/list/device_list.h\"" << endl;
  ds << "#include \"pickit2v2_prog.h\"" << endl;
  ds << "#include \"pickit2v2_data.h\"" << endl << endl;
  ds << "namespace Pickit2V2" << endl;
  ds << "{" << endl;

  // header
  int vmajor;
  extract(file, vmajor);
  int vminor;
  extract(file, vminor);
  int vdot;
  extract(file, vdot);
  qDebug("info: %i.%i.%i", vmajor, vminor, vdot);
  qDebug("--------------------------------------------------------------------");
  QString vnotes = extractString(file, 512);
  //qDebug("%s", vnotes.latin1());
  int nbFamilies;
  extract(file, nbFamilies);
  qDebug("nbFamilies: %i", nbFamilies);
  int nbParts;
  extract(file, nbParts);
  qDebug("nbParts: %i", nbParts);
  int nbScripts;
  extract(file, nbScripts);
  qDebug("nbScripts: %i", nbScripts);
  uchar compatibility;
  extract(file, compatibility);
  qDebug("compatibility: %i", compatibility);
  uchar unused1A;
  extract(file, unused1A);
  ushort unused1B;
  extract(file, unused1B);
  uint unused2;
  extract(file, unused2);

// families
  qDebug("--------------------------------------------------------------------");
  QMap<uint, FamilyData> families;
  for (int i=0; i<nbFamilies; i++) {
    FamilyData fdata(file);
    families[fdata.id] = fdata;
  }

// devices
  qDebug("--------------------------------------------------------------------");
  const Pic::Group &group = static_cast<const Pic::Group &>(*Device::Lister::instance().group("pic"));
  QMap<QString, DeviceData> devices;
  for (int i=0; i<nbParts; i++) {
    DeviceData ddata(file);
    if ( ddata.Name=="PIC16F636 (639)" ) {
      ddata.Name = "16F636";
      devices[ddata.Name] = ddata;
      ddata.Name = "16F639";
      devices[ddata.Name] = ddata;
    } else {
      ddata.Name = ddata.Name.stripWhiteSpace();
      if ( ddata.Name.startsWith("PIC" ) ) ddata.Name = ddata.Name.mid(3);
      else if ( ddata.Name.startsWith("dsPIC") ) ddata.Name = ddata.Name.mid(5);
      else {
        qDebug("%s: ignored", ddata.Name.latin1());
        continue;
      }
      devices[ddata.Name] = ddata;
    }
  }

  // check with data in piklab
  QMap<QString, DeviceData>::const_iterator it;
  for (it=devices.begin(); it!=devices.end(); ++it) {
    QString name = (*it).Name;
    const Pic::Data *pdata = static_cast<const Pic::Data *>(group.deviceData(name).data);
    if ( pdata==0 ) {
      qDebug("%s: unsupported", name.latin1());
      continue;
    }
    if ( pdata->architecture()==Pic::Architecture::P18J || pdata->architecture()==Pic::Architecture::P24F
         || pdata->architecture()==Pic::Architecture::P24H || pdata->architecture()==Pic::Architecture::P30F ) { // ### REMOVE ME
      qDebug("%s: no support for this device for now...", name.latin1());
      continue;
    }
    FamilyData &fdata = families[(*it).Family];
    fdata.arch = pdata->architecture();
    qDebug("%s", name.latin1());
    if ( fdata.partDetect==0 ) {
      if ( pdata->ids()[Device::Special::Normal]!=0x0000 ) qFatal("no detect but id %s", toHexLabel(pdata->ids()[Device::Special::Normal], 4).latin1());
    } else {
      BitValue id = pdata->ids()[Device::Special::Normal];
      if ( id!=(*it).DeviceId ) qFatal("id different %s (%s)", toHexLabel((*it).DeviceId, 4).latin1(), toHexLabel(id, 4).latin1());
    }
    if ( pdata->nbBytesWord(Pic::MemoryRangeType::Code)!=fdata.bytesPerLocation ) qFatal("code: byte per word different");
    if ( pdata->addressIncrement(Pic::MemoryRangeType::Code)!=fdata.addressIncrement ) qFatal("code: address increment different");
    if ( XOR((*it).ProgMemAddrBytes, fdata.arch.data().properties & Pic::RandomMemoryAccess) ) qFatal("Mismatch in memory access type");
    uint len = pdata->nbWords(Pic::MemoryRangeType::Code);
    if ( pdata->isPresent(Pic::MemoryRangeType::Cal) && pdata->range(Pic::MemoryRangeType::Cal).start==len ) len++;
    if ( pdata->architecture()==Pic::Architecture::P18J ) len += 4; // config at end of code
    if ( len!=(*it).ProgramMem ) qFatal("code: lengths different %i (%i)", (*it).ProgramMem, len);
    if ( pdata->isPresent(Pic::MemoryRangeType::Eeprom) ) {
      //      if ( pdata->nbBytesWord(Pic::MemoryEeprom)!=fdata.eeMemBytesPerWord ) qFatal("eeprom: byte per word different: %i (%i)", fdata.eeMemBytesPerWord, pdata->nbBytesWord(Pic::MemoryEeprom));
      if ( pdata->addressIncrement(Pic::MemoryRangeType::Eeprom)!=fdata.eeMemAddressIncrement ) qFatal("eeprom: address increment different");
      if ( pdata->nbWords(Pic::MemoryRangeType::Eeprom)!=(*it).EEMem ) qFatal("eeprom: lengths different %i (%i)", (*it).EEMem, pdata->nbWords(Pic::MemoryRangeType::Eeprom));
      uint offset = (pdata->range(Pic::MemoryRangeType::Eeprom).start + pdata->range(Pic::MemoryRangeType::Eeprom).hexFileOffset).toUInt();
      if ( !(pdata->architecture().data().properties & Pic::PackedHex) ) offset *= 2;
      if ( offset!=(*it).EEAddr ) qFatal("eeprom: hex offset different %s (%s)", toHexLabel((*it).EEAddr, 8).latin1(), toHexLabel(offset, 8).latin1());
    }
    //qDebug("%i %i %i %i %i", fdata.userIdHexBytes, fdata.userIdBytes, fdata.progMemHexBytes, fdata.eeMemHexBytes, fdata.progMemShift);
    len = pdata->nbWords(Pic::MemoryRangeType::Config);
    if ( pdata->architecture().data().properties & Pic::PackedHex ) len /= 2;
    if ( len!=(*it).ConfigWords ) qFatal("config: lengths different %i (%i)", (*it).ConfigWords, len);
    if ( pdata->nbWords(Pic::MemoryRangeType::UserId)!=(*it).UserIdWords ) qFatal("user id: lengths different %i (%i)", (*it).UserIdWords, pdata->nbWords(Pic::MemoryRangeType::UserId));
    uint offset = pdata->range(Pic::MemoryRangeType::Config).hexFileOffset;
    if ( pdata->architecture()!=Pic::Architecture::Baseline || offset==0 ) offset += pdata->range(Pic::MemoryRangeType::Config).start.toUInt();
    if ( !(pdata->architecture().data().properties & Pic::PackedHex) ) offset *= 2;
    if ( offset!=(*it).ConfigAddr ) qFatal("config: hex offset different %s (%s)", toHexLabel((*it).ConfigAddr, 8).latin1(), toHexLabel(offset, 8).latin1());
    offset = pdata->range(Pic::MemoryRangeType::UserId).hexFileOffset + pdata->range(Pic::MemoryRangeType::UserId).start.toUInt();
    if ( !(pdata->architecture().data().properties & Pic::PackedHex) ) offset *= 2;
    if ( offset!=(*it).UserIdAddr ) qWarning("user id: hex offset different %s (%s)", toHexLabel((*it).UserIdAddr, 8).latin1(), toHexLabel(offset, 8).latin1());
    //const Pic::Config &config = pdata->config();
    //for (uint i=0; i<pdata->nbWords(Pic::MemoryRangeType::Config); i++) {
      //const Pic::Config::Word &cword = config._words[i];
      //if ( cword.cmask!=(*it).ConfigMasks[i] ) qWarning("config word %i: mask different %s (%s)", i, toHexLabel((*it).ConfigMasks[i], 4).latin1(), toHexLabel(cword.cmask, 4).latin1());
      //if ( cword.bvalue!=(*it).ConfigBlank[i] ) qWarning("config word %i: blank value different %s (%s)", i, toHexLabel((*it).ConfigBlank[i], 4).latin1(), toHexLabel(cword.bvalue, 4).latin1());
    //}
    //const Pic::VoltageData &vpp = pdata->voltage(Pic::ProgVoltageType::Vpp);
    //if ( fdata.vpp>vpp.max || fdata.vpp<vpp.min ) qWarning("Vpp %f out of range (min=%f max=%f)", fdata.vpp, vpp.min, vpp.max);
    if ( fdata.vpp!=12.0 ) qFatal("Vpp is not 12 V");
    if ( (*it).VddMin<pdata->vddMin() ) qFatal("Vdd min %f is less than %f", (*it).VddMin, pdata->vddMin());
    if ( (*it).VddMax>pdata->vddMax() ) qFatal("Vdd max %f is more than %f", (*it).VddMax, pdata->vddMax());
    if ( (*it).VddMax!=pdata->architecture().data().nominalVdd ) qFatal("Vdd max %f is not nominal value (%f)", (*it).VddMax, pdata->architecture().data().nominalVdd);
    const Pic::VoltageData &vddErase = pdata->voltage(Pic::ProgVoltageType::VddBulkErase);
    if ( (*it).VddErase<vddErase.min-0.06 ) qFatal("Vdd erase %f is less than %f", (*it).VddErase, vddErase.min);
    if ( (*it).VddErase>vddErase.max ) qFatal("Vdd erase %f is more than %f", (*it).VddErase, vddErase.max);
  }

  // write device data to file
  ds << "struct CData {" << endl;
  ds << "  const char *name;" << endl;
  ds << "  Data data;" << endl;
  ds << "};" << endl << endl;
  for (it=devices.begin(); it!=devices.end(); ++it) {
    QString name = (*it).Name;
    const FamilyData &fdata = families[(*it).Family];
    ds << "const CData PIC" << name << "_DATA = { \"" << name << "\", { ";
    ds << (*it).ProgMemRdWords << ", " << (*it).EERdLocations;
    ds << ", " << (*it).ProgMemWrWords << ", " << (*it).EEWrLocations << ", " << (*it).DebugRowEraseSize;
    ds << "," << endl;
    ds << "  { ";
    ds << fdata.progEntryScript << ", " << fdata.progExitScript << ", " << fdata.readDevIdScript;
    ds << ", " << (*it).ProgMemRdScript << ", " << (*it).ChipErasePrepScript;
    ds << ", " << (*it).ProgMemAddrSetScript << ", " << (*it).ProgMemWrPrepScript;
    ds << ", " << (*it).ProgMemWrScript << ", " << (*it).EERdPrepScript << ", " << (*it).EERdScript;
    ds << ", " << (*it).EEWrPrepScript << ", " << (*it).EEWrScript << ", " << (*it).ConfigRdPrepScript;
    ds << ", " << (*it).ConfigRdScript << ", " << (*it).ConfigWrPrepScript << ", " << (*it).ConfigWrScript;
    ds << ", " << (*it).UserIDRdPrepScript << ", " << (*it).UserIDRdScript << ", " << (*it).UserIDWrPrepScript;
    ds << ", " << (*it).UserIDWrScript << ", " << (*it).OSCCALRdScript << ", " << (*it).OSCCALWrScript;
    ds << ", " << (*it).ChipEraseScript << ", " << (*it).ProgMemEraseScript << ", " << (*it).EEMemEraseScript;
    ds << ", " << (*it).ConfigMemEraseScript << ", " << (*it).DebugRowEraseScript;
    ds << ", " << (*it).TestMemoryRdScript << ", " << (*it).EERowEraseScript;
    ds << " } } };" << endl;
  }
  ds << endl;
  ds << "const CData *DATA_LIST[] = {";
  uint i = 0;
  for (it=devices.begin(); it!=devices.end(); ++it) {
    if ( (i%20)==0 ) ds << endl;
    QString name = (*it).Name;
    ds << "&PIC" << name << "_DATA,";
    i++;
  }
  ds << 0 << endl;
  ds << "};" << endl << endl;
  ds << "const CData *cdata(const QString &device)" << endl;
  ds << "{" << endl;
  ds << "for(uint i=0; DATA_LIST[i]; i++)" << endl;
  ds << "  if ( device==DATA_LIST[i]->name ) return DATA_LIST[i];" << endl;
  ds << "return 0;" << endl;
  ds << "}" << endl;
  ds << "::Group::Support support(const QString &)" << endl;
  ds << "{" << endl;
  ds << "  return ::Group::Support::Untested;" << endl;
  ds << "}" << endl;
  ds << "const Data &data(const QString &device)" << endl;
  ds << "{" << endl;
  ds << "  return cdata(device)->data;" << endl;
  ds << "}" << endl;
  ds << "void Group::initSupported()" << endl;
  ds << "{" << endl;
  ds << "  for (uint i=0; DATA_LIST[i]; i++) {" << endl;
  ds << "    const Device::Data *data = Device::lister().data(DATA_LIST[i]->name);" << endl;
  ds << "    if ( data==0 ) continue; // in case there are some changes..." << endl;
  ds << "    addDevice(data->name(), data, ::Group::Support::Untested);" << endl;
  ds << "  }" << endl;
  ds << "}" << endl;

  // write family data for search
  ds << endl;
  ds << "const FamilyData FAMILY_DATA[] = {" << endl;
  QMap<uint, FamilyData>::const_iterator fit;
  for (fit=families.begin(); fit!=families.end(); ++fit) {
    if ( (*fit).arch==Pic::Architecture::Nb_Types ) continue; // unsupported family
    ds << "  { Pic::Architecture::Type(" << (*fit).arch.type() << "), ";
    ds << (*fit).vpp << ", ";
    ds << toHexLabel((*fit).progEntryScript, 4) << ", " << toHexLabel((*fit).progExitScript, 4) << ", ";
    ds << toHexLabel((*fit).readDevIdScript, 4) << ", " << ((*fit).progMemShift ? "true" : "false");
    ds << " }, " << endl;
  }
  ds << "  { Pic::Architecture::Nb_Types, 0.0, 0x0000, 0x0000, 0x0000, false }" << endl;
  ds << "};" << endl;

// scripts
  qDebug("--------------------------------------------------------------------");
  QMap<uint, ScriptData> scripts;
  for (int i=0; i<nbScripts; i++) {
    ScriptData sdata(file);
    scripts[sdata.Number] = sdata;
    qDebug("script %i: %s", sdata.Number, sdata.Name.latin1());
  }

  ds << endl;
  ds << "const ScriptData SCRIPT_DATA[] = {" << endl;
  for (int k=1; k<=scripts.count(); k++) {
    Q_ASSERT( scripts.contains(k) );
    QString name = scripts[k].Name.replace("\"", "\\\"");
    ds << "  { \"" << name << "\", " << scripts[k].Version
       << ", " << scripts[k].Script.count() << ", { ";
    for (int i=0; i<scripts[k].Script.count(); i++) {
      if ( i!=0 ) ds << ", ";
      ds << toHexLabel(scripts[k].Script[i], 4);
    }
    ds << " } }," << endl;
  }
  ds << "  { 0, 0, 0, {} }" << endl;
  ds << "};" << endl;

  ds << "} // namespace" << endl;

  return 0;
}
