/* GNU PIC processor definitions
   Copyright (C) 2001, 2002, 2003, 2004, 2005
   Craig Franklin

This file is part of gputils.

gputils is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

gputils is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with gputils; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#ifndef __GPPROCESSOR_H__
#define __GPPROCESSOR_H__

enum proc_class {
  PROC_CLASS_UNKNOWN,   /* Unknown device */
  PROC_CLASS_EEPROM8,   /* 8 bit EEPROM */
  PROC_CLASS_EEPROM16,  /* 16 bit EEPROM */
  PROC_CLASS_GENERIC,   /* 12 bit device */
  PROC_CLASS_PIC12,     /* 12 bit devices */
  PROC_CLASS_SX,        /* 12 bit devices */
  PROC_CLASS_PIC14,     /* 14 bit devices */
  PROC_CLASS_PIC16,     /* 16 bit devices */
  PROC_CLASS_PIC16E     /* enhanced 16 bit devices */
};

typedef struct px *pic_processor_t;
#define no_processor ((struct px *)0)

#define MAX_NAMES 3 /* Maximum number of names a processor can have */
#define MAX_BADROM 1*2 /* Maximum number of BADROM ranges a processor can be */
                       /* initialized with */

struct px {
  enum proc_class _class;
  const char *defined_as;
  const char *names[MAX_NAMES];
  unsigned long coff_type;
  int num_pages;
  int num_banks;
  long maxrom;
  long badrom[MAX_BADROM];
  long config_addrs[2];
  const char *script;
};

extern struct px pics[];

#endif
