/***************************************************************************
 *   Copyright (C) 2012 Nicolas Hadacek <hadacek@kde.org>                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include <iostream>

#include <qfile.h>
#include <qfileinfo.h>
#include <qtextstream.h>
#include <qregexp.h>

#include <kcmdlineargs.h>
#include <klocale.h>

#include "common/common/misc.h"
#include "common/common/number.h"
#include "common/global/about.h"
#include "devices/base/generic_device.h"
#include "devices/base/device_group.h"
#include "devices/list/device_list.h"
#include "progs/base/prog_group.h"
#include "progs/pickit3/base/pickit3_prog.h"
#include "progs/pickit3/base/pickit3.h"
#include "progs/list/prog_list.h"

const Piklab::CmdLineOption OPTIONS[] = {
  { "+file", I18N_NOOP("DDS data as parsed by usb_snoop_filter"), 0 },
  { 0, 0, 0}
};

void compare(uint v, const ByteArray& ref, uint i, uint len, const char* label)
{
  uint rv = ref.getValue(i, len);
  if ( v != rv) qWarning("diff in '%s' at index %d: 0x%s != 0x%s (ref)",
                         label, i, toString(NumberBase::Hex, v, 2*len).latin1(),
                         toString(NumberBase::Hex, rv, 2*len).latin1());
}

void compare(const ByteArray& array, const ByteArray& ref, uint i, uint len, const char* label)
{
  uint v = array.getValue(i, len);
  compare(v, ref, i, len, label);
}

int main(int argc, char **argv)
{
  const Piklab::AboutData about("pickit3_dds_parser", "pickit3_dds_parser", "");
  static Piklab::OptionList list;
  for (uint i = 0; OPTIONS[i].name != NULL; ++i)
    list.append(OPTIONS[i]);
  Piklab::initArgs(about, argc, argv, list);

  KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
  if ( args->count()==0 ) args->usage();
  PURL::Url url(args->url(0));
  QString filename = url.filename();
  QRegExp regexp("dds_([0-9a-zA-Z]*)\\.txt");
  if (!regexp.exactMatch(filename)) qFatal("Filename should be \"dds_<device>.txt\"");
  QString device = regexp.cap(1);
  const Device::Data *ddata = Device::Lister::instance().data(device);
  if (ddata == NULL) qFatal("Unrecognized device \"%s\"", device.latin1());
  if (ddata->group().name() != "pic") qFatal("Device \"%s\" is not a PIC", device.latin1());
  const Programmer::Group *group = Programmer::Lister::instance().group("pickit3");
  ASSERT(group != NULL);
  if (!group->isSupported(device)) qFatal("Device \"%s\" not supported by pickit3", device.latin1());

  Log::Base log;
  PURL::File  in(url, log);
  if (!in.openForRead()) qFatal("Could not open file \"%s\".", url.pretty().latin1());
  QString dds = in.stream().readAll();

  ByteArray ref(384, 0x00, PrintEscapeAll);
  uint k = 0;
  uint n = 0;
  for (int i = 0; i < dds.length(); ++i) {
    if (dds[i] == 0x0A || dds[i] == 0x20) continue;
    if (k == 384) qFatal("Too long: %d", dds.length() - i);
    uint v = 0;
    if (dds[i] == '\\') {
      if (i+2 >= dds.length()) qFatal("Incorrect escape at index %d", i);
      bool ok = false;
      char s[3];
      s[0] = dds[i+1].ascii();
      s[1] = dds[i+2].ascii();
      s[2] = 0x00;
      v = fromString(NumberBase::Hex, s, 2, &ok);
      if (!ok) qFatal("Number not hexadecimal at index %d", i);
      i += 2;
    } else {
      v = dds[i].ascii();
    }
    if (n < 60 || n >= 64) {
      if (n > 0 && n%64 == 0) std::cout << std::endl;
      std::cout << toString(NumberBase::Hex, v, 2).latin1();
      ref.setValueInc(v, k, 1);
    }
    ++n;
  }
  std::cout << std::endl;
  if (n != 384) qFatal("Too short: %d/384", k);

  Pickit3::PicBase* base = new Pickit3::PicBase(*group, static_cast<const Pic::Data*>(ddata));
  Pickit3::Hardware* hardware = new Pickit3::Hardware(*base);
  ByteArray data = hardware->createConfigPacket();
  compare(data, ref, 0, 2, "id");
  compare(data, ref, 2, 2, "debugger");
  compare(data, ref, 4, 4, "devid_start");
  compare(data, ref, 8, 4, "devid_mask");
  compare(data, ref, 12, 4, "devid");
  compare(data, ref, 48, 2, "code_wait_time");
  compare(data, ref, 50, 2, "lv_code_wait_time");
  compare(data, ref, 52, 2, "eeprom_wait_time");
  compare(data, ref, 54, 2, "config_wait_time");
  compare(data, ref, 56, 2, "userids_wait_time");
  compare(data, ref, 58, 2, "erase_wait_time");
  compare(data, ref, 60, 2, "lv_erase_wait_time");
  compare(data, ref, 62, 2, "erase_algo");
  compare(data, ref, 64, 1, "code_latches");
  compare(data, ref, 65, 1, "eeprom_latches");
  compare(data, ref, 66, 1, "config_latches");
  compare(data, ref, 67, 1, "userids_latches");
  compare(data, ref, 68, 1, "row_erase_latches");
  compare(data, ref, 69, 1, "vdd_min");
  compare(data, ref, 70, 1, "vdd_max");
  compare(data, ref, 71, 1, "vdd_def");
  compare(data, ref, 72, 1, "vpp_min");
  compare(data, ref, 73, 1, "vpp_max");
  compare(data, ref, 74, 1, "vpp_def");
  compare(data, ref, 75, 1, "vdd_nominal_min");
  compare(data, ref, 76, 1, "vdd_nominal_max");
  compare(data, ref, 77, 1, "erase_min");
  compare(data, ref, 78, 4, "code_start");
  compare(data, ref, 82, 4, "code_size");
  compare(data, ref, 86, 4, "eeprom_start");
  compare(data, ref, 90, 4, "eeprom_size");
  compare(data, ref, 94, 4, "userids_start");
  compare(data, ref, 98, 4, "userids_size");
  compare(data, ref, 102, 4, "config_start");
  compare(data, ref, 106, 4, "config_size");
  compare(data, ref, 110, 4, "test_start");
  compare(data, ref, 114, 4, "test_size");
  compare(data, ref, 118, 4, "test_app_start");
  compare(data, ref, 122, 4, "test_app_size");
  compare(data, ref, 126, 2, "panel_count");
  compare(data, ref, 128, 2, "panel_size");
  for (uint i = 0 ; i < 20; ++i) compare(data, ref, 130+i, 1, "config_bvalue");
  for (uint i = 0 ; i < 20; ++i) compare(data, ref, 150+i, 1, "config_wmask");
  compare(data, ref, 170, 4, "cal_start");
  compare(data, ref, 174, 4, "debug_address");
  compare(data, ref, 178, 4, "debug_mask");
  compare(data, ref, 182, 4, "debug_on_value");
  compare(data, ref, 186, 2, "minor_algo");
  compare(data, ref, 188, 4, "config_boot_start");
  compare(data, ref, 192, 4, "config_boot_size");
  compare(data, ref, 196, 2, "debug_algo");
  compare(data, ref, 198, 2, "header_only");
  compare(data, ref, 200, 4, "header_id");
  compare(data, ref, 204, 4, "header_select_address");
  compare(data, ref, 208, 4, "header_select_value");
  compare(data, ref, 212, 4, "header_config_address");
  compare(data, ref, 216, 4, "header_cal_address");
  compare(data, ref, 220, 4, "header_debug_exec_address");
  compare(data, ref, 224, 2, "erase_algo_2");
  compare(data, ref, 226, 4, "flags");

  ByteArray array(334, 0x00, PrintEscapeAll);
  for (uint i = 0; i < 334; ++i) array[i] = ref[i];
  compare(hardware->computeChecksum(array), ref, 334, 2, "checksum");

  for (int i = 336; i < 380; ++i) compare('Z', ref, i, 1, "fill");

  return 0;
}
