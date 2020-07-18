#include <unistd.h>
#include <qfile.h>

#include "common/common/number.h"
#include "devices/pic/base/pic.h"

//-----------------------------------------------------------------------------
template <class Type>
void extract(QFile &file, Type &type) {
  if ( read(file.handle(), &type, sizeof(Type))!=sizeof(Type) ) qFatal("truncated");
  //  qDebug(">%i: %s", 2*sizeof(Type), toHex(type, 2*sizeof(Type)).latin1());
}

template <>
void extract(QFile &file, float &f) {
  if ( read(file.handle(), &f, sizeof(float))!=sizeof(float) ) qFatal("float truncated");
  // qDebug(">%i: %f", 2*sizeof(float), f);
}

QString extractString(QFile &file, uint) {
  uchar c1, c2;
  extract(file, c1);
  uint len = c1;
  if ( len>=128 ) {
    extract(file, c2);
    len |= (c2/3) << 8; // ??
  }
  //qDebug("%i %i %i", len, c1, c2);
  QCString s(len+1);
  for (uint i=0; i<len; i++)
    if ( read(file.handle(), s.data()+i, sizeof(char))!=sizeof(char) ) qFatal("string truncated");
  //qDebug("string (%i): \"%s\"", len, s.data());
  return s;
}

template <class Type>
void extract(QFile &file, Type *type, uint count)
{
  for (uint i=0; i<count; i++) {
    if ( read(file.handle(), type+i, sizeof(Type))!=sizeof(Type) ) qFatal("truncated");
    //    qDebug(">%i: %s", 2*sizeof(Type), toHex(type[i], 2*sizeof(Type)).latin1());
  }
}

//-----------------------------------------------------------------------------
class FamilyData
{
public:
  FamilyData() {}
  FamilyData(QFile &file);

  ushort   id, type, searchPriority;
  QString  name;
  ushort   progEntryScript, progExitScript, readDevIdScript;
  uint     devIdMask, blankValue;
  uchar    bytesPerLocation, addressIncrement, partDetect;
  uint     unused1;
  uchar    eeMemBytesPerWord, eeMemAddressIncrement, userIdHexBytes, userIdBytes,
           progMemHexBytes, eeMemHexBytes, progMemShift;
  uint     testMemoryStart;
  ushort   testMemoryLength;
  float    vpp;

  Pic::Architecture arch;
};

//-----------------------------------------------------------------------------
class DeviceData
{
public:
  DeviceData() {}
  DeviceData(QFile &file);

  QString Name;
  ushort  Family;
  uint    DeviceId;
  uint    ProgramMem;
  ushort  EEMem;
  uint    EEAddr;
  uchar   ConfigWords;
  uint    ConfigAddr;
  uchar   UserIdWords;
  uint    UserIdAddr;
  uint    BandGapMask;
  ushort  ConfigMasks[8];
  ushort  ConfigBlank[8];
  ushort  CPMask;
  uchar   CPConfig;
  uchar   OsccalSave;
  uint    IgnoreAddress;
  float   VddMin, VddMax, VddErase;
  uchar   CalibrationWords;
  ushort  ChipEraseScript;
  ushort  ProgMemAddrSetScript;
  uchar   ProgMemAddrBytes;
  ushort  ProgMemRdScript;
  ushort  ProgMemRdWords;
  ushort  EERdPrepScript;
  ushort  EERdScript;
  ushort  EERdLocations;
  ushort  UserIDRdPrepScript;
  ushort  UserIDRdScript;
  ushort  ConfigRdPrepScript;
  ushort  ConfigRdScript;
  ushort  ProgMemWrPrepScript;
  ushort  ProgMemWrScript;
  ushort  ProgMemWrWords;
  uchar   ProgMemPanelBufs;
  uint    ProgMemPanelOffset;
  ushort  EEWrPrepScript;
  ushort  EEWrScript;
  ushort  EEWrLocations;
  ushort  UserIDWrPrepScript;
  ushort  UserIDWrScript;
  ushort  ConfigWrPrepScript;
  ushort  ConfigWrScript;
  ushort  OSCCALRdScript;
  ushort  OSCCALWrScript;
  ushort  DPMask;
  bool    WriteCfgOnErase;
  bool    BlankCheckSkipUsrIDs;
  ushort  IgnoreBytes;
  ushort  ChipErasePrepScript;
  uint    UNUSED3;
  uint    UNUSED4;
  ushort ProgMemEraseScript;
  ushort EEMemEraseScript;
  ushort ConfigMemEraseScript;
  ushort reserved1EraseScript;
  ushort reserved2EraseScript;
  ushort TestMemoryRdScript;
  ushort TestMemoryRdWords;
  ushort EERowEraseScript;
  ushort EERowEraseWords;
  bool ExportToMPLAB;
  ushort DebugHaltScript;
  ushort DebugRunScript;
  ushort DebugStatusScript;
  ushort DebugReadExecVerScript;
  ushort DebugSingleStepScript;
  ushort DebugBulkWrDataScript;
  ushort DebugBulkRdDataScript;
  ushort DebugWriteVectorScript;
  ushort DebugReadVectorScript;
  ushort DebugRowEraseScript;
  ushort DebugRowEraseSize;
  ushort DebugReserved5Script;
  ushort DebugReserved6Script;
  ushort DebugReserved7Script;
  ushort DebugReserved8Script;
  ushort DebugReserved9Script;
};

//-----------------------------------------------------------------------------
class ScriptData
{
public:
  ScriptData() {}
  ScriptData(QFile &file);

   ushort  Number;
   QString Name;
   ushort  Version;
   uint    UNUSED1;
   QValueVector<ushort> Script;
   QString Comment;
};
