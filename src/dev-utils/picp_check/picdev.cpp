//-----------------------------------------------------------------------------
//
//	PIC device definitions and initialization data
//
//
//	name field must be all upper case
//	array of initialization data does not include checksum -- checksum is pasted
//		onto the end by DoInitPIC()
//
//-----------------------------------------------------------------------------
//
//	Copyright (C) 1999-2002 Cosmodog, Ltd.
// Copyright (c) 2004-2005 Jeffery L. Post
//
// Maintained at
// http://home.pacbell.net/theposts/picmicro
//
// Please send bug reports for version 0.6.0 or higher to
// j_post <AT> pacbell <DOT> net.
//
//	This program is free software; you can redistribute it and/or
//	modify it under the terms of the GNU General Public License
//	as published by the Free Software Foundation; either version 2
//	of the License, or (at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with this program; if not, write to the Free Software
//	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
//-----------------------------------------------------------------------------
//
//	[TODO]:
//	need to add code protect and watchdog masks to definition structures
//		for most parts
//
//-----------------------------------------------------------------------------

#include "picdev.h"

#include <stdlib.h> // for NULL.

const static unsigned char def_PIC10F200[] =
{
	0x00, 0xff,		// size of program space
	0x0f, 0xff,		// width of address word
	0x0f, 0xff,		// width of data word
	0x00, 0x0f,		// width of ID
	0x00, 0x0f,		// ID mask
	0x00, 0x1c,		// width of configuration word
	0x00, 0x1c,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x0f, 0xff,		// Calibration width
	0x0f, 0xff,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0xff,		// ??
	0x01, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x0f, 0xff,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0xff,		// address of internal clock calibration value
	0x00, 0x01,		// size of clock calibration space
	0x01,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x1d, 0x17,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC10F200[] =
{
	0x00, 0x1c, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x1c, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC10F200 =
{
	"10F200",			// device name
	def_PIC10F200,		// definition
	defx_PIC10F200,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_JUPIC),	// bit map of supporting programmers
};

const static unsigned char def_PIC10F202[] =
{
	0x01, 0xff,		// size of program space
	0x0f, 0xff,		// width of address word
	0x0f, 0xff,		// width of data word
	0x00, 0x0f,		// width of ID
	0x00, 0x0f,		// ID mask
	0x00, 0x1c,		// width of configuration word
	0x00, 0x1c,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x0f, 0xff,		// Calibration width
	0x0f, 0xff,		// Calibration mask
	0x00, 0x00,		// ??
	0x01, 0xff,		// ??
	0x02, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x0f, 0xff,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x01, 0xff,		// address of internal clock calibration value
	0x00, 0x01,		// size of clock calibration space
	0x01,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x1d, 0x17,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC10F202[] =
{
	0x00, 0x1c, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x1c, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC10F202 =
{
	"10F202",			// device name
	def_PIC10F202,		// definition
	defx_PIC10F202,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_JUPIC),	// bit map of supporting programmers
};

const static unsigned char def_PIC10F204[] =
{
	0x00, 0xff,		// size of program space
	0x0f, 0xff,		// width of address word
	0x0f, 0xff,		// width of data word
	0x00, 0x0f,		// width of ID
	0x00, 0x0f,		// ID mask
	0x00, 0x1f,		// width of configuration word
	0x00, 0x1f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x0f, 0xff,		// Calibration width
	0x0f, 0xff,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x02, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x01, 0xff,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0xff,		// address of internal clock calibration value
	0x00, 0x01,		// size of clock calibration space
	0x0b,				// additional programming pulses for C devices
	0x08,				// main programming pulses for C devices
	0x0a, 0x16,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC10F204[] =
{
	0x00, 0x1f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x1f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC10F204 =
{
	"10F204",			// device name
	def_PIC10F204,		// definition
	defx_PIC10F204,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0x100, 3,			// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_JUPIC),	// bit map of supporting programmers
};

const static unsigned char def_PIC10F206[] =
{
	0x01, 0xff,		// size of program space
	0x0f, 0xff,		// width of address word
	0x0f, 0xff,		// width of data word
	0x00, 0x0f,		// width of ID
	0x00, 0x0f,		// ID mask
	0x00, 0x1f,		// width of configuration word
	0x00, 0x1f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x0f, 0xff,		// Calibration width
	0x0f, 0xff,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x02, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x03, 0xff,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x01, 0xff,		// address of internal clock calibration value
	0x00, 0x01,		// size of clock calibration space
	0x0b,				// additional programming pulses for C devices
	0x08,				// main programming pulses for C devices
	0x0a, 0x16,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC10F206[] =
{
	0x00, 0x1f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x1f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC10F206 =
{
	"10F206",			// device name
	def_PIC10F206,		// definition
	defx_PIC10F206,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0x200, 3,			// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC12C508[] =
{
	0x01, 0xff,		// size of program space
	0x0f, 0xff,		// width of address word
	0x0f, 0xff,		// width of data word
	0x00, 0x0f,		// width of ID
	0x00, 0x0f,		// ID mask
	0x00, 0x1f,		// width of configuration word
	0x00, 0x1f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x0f, 0xff,		// Calibration width
	0x0f, 0xff,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x02, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x0f, 0xff,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x01, 0xff,		// address of internal clock calibration value
	0x00, 0x01,		// size of clock calibration space
	0x0b,				// additional programming pulses for C devices
	0x08,				// main programming pulses for C devices
	0x0a, 0x16,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC12C508[] =
{
	0x00, 0x1f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x1f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC12C508 =
{
	"12C508",			// device name
	def_PIC12C508,		// definition
	defx_PIC12C508,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC12C508A[] =
{
	0x01, 0xff,		// size of program space
	0x0f, 0xff,		// width of address word
	0x0f, 0xff,		// width of data word
	0x00, 0x0f,		// width of ID
	0x00, 0x0f,		// ID mask
	0x00, 0x1f,		// width of configuration word
	0x00, 0x1f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x0f, 0xff,		// Calibration width
	0x0f, 0xff,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x02, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x0f, 0xff,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x01, 0xff,		// address of internal clock calibration value
	0x00, 0x01,		// size of clock calibration space
	0x0b,				// additional programming pulses for C devices
	0x08,				// main programming pulses for C devices
	0x0a, 0x16,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC12C508A[] =
{
	0x00, 0x1f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x1f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC12C508A =
{
	"12C508A",			// device name
	def_PIC12C508A,	// definition
	defx_PIC12C508A,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC12C509[] =
{
	0x03, 0xff,		// size of program space
	0x0f, 0xff,		// width of address word
	0x0f, 0xff,		// width of data word
	0x00, 0x0f,		// width of ID
	0x00, 0x0f,		// ID mask
	0x00, 0x1f,		// width of configuration word
	0x00, 0x1f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x0f, 0xff,		// Calibration width
	0x0f, 0xff,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x04, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x0f, 0xff,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x03, 0xff,		// address of internal clock calibration value
	0x00, 0x01,		// size of clock calibration space
	0x0b,				// additional programming pulses for C devices
	0x08,				// main programming pulses for C devices
	0x0a, 0x16,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC12C509[] =
{
	0x00, 0x1f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x1f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC12C509 =
{
	"12C509",			// device name
	def_PIC12C509,		// definition
	defx_PIC12C509,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC12C509A[] =
{
	0x03, 0xff,		// size of program space
	0x0f, 0xff,		// width of address word
	0x0f, 0xff,		// width of data word
	0x00, 0x0f,		// width of ID
	0x00, 0x0f,		// ID mask
	0x00, 0x1f,		// width of configuration word
	0x00, 0x1f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x0f, 0xff,		// Calibration width
	0x0f, 0xff,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x04, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x0f, 0xff,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x03, 0xff,		// address of internal clock calibration value
	0x00, 0x01,		// size of clock calibration space
	0x0b,				// additional programming pulses for C devices
	0x08,				// main programming pulses for C devices
	0x0a, 0x16,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC12C509A[] =
{
	0x00, 0x1f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x1f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC12C509A =
{
	"12C509A",			// device name
	def_PIC12C509A,	// definition
	defx_PIC12C509A,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC12CE518[] =
{
	0x01, 0xff,		// size of program space
	0x0f, 0xff,		// width of address word
	0x0f, 0xff,		// width of data word
	0x00, 0x0f,		// width of ID
	0x00, 0x0f,		// ID mask
	0x00, 0x1f,		// width of configuration word
	0x00, 0x1f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x0f, 0xff,		// Calibration width
	0x0f, 0xff,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x02, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x0f, 0xff,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x01, 0xff,		// address of internal clock calibration value
	0x00, 0x01,		// size of clock calibration space
	0x0b,				// additional programming pulses for C devices
	0x08,				// main programming pulses for C devices
	0x0a, 0x16,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC12CE518[] =
{
	0x00, 0x1f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x1f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC12CE518 =
{
	"12CE518",			// device name
	def_PIC12CE518,	// definition
	defx_PIC12CE518,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC12CE519[] =
{
	0x03, 0xff,		// size of program space
	0x0f, 0xff,		// width of address word
	0x0f, 0xff,		// width of data word
	0x00, 0x0f,		// width of ID
	0x00, 0x0f,		// ID mask
	0x00, 0x1f,		// width of configuration word
	0x00, 0x1f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x0f, 0xff,		// Calibration width
	0x0f, 0xff,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x04, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x0f, 0xff,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x03, 0xff,		// address of internal clock calibration value
	0x00, 0x01,		// size of clock calibration space
	0x0b,				// additional programming pulses for C devices
	0x08,				// main programming pulses for C devices
	0x0a, 0x16,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC12CE519[] =
{
	0x00, 0x1f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x1f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC12CE519 =
{
	"12CE519",			// device name
	def_PIC12CE519,	// definition
	defx_PIC12CE519,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC12F508[] =
{
	0x01, 0xff,		// size of program space
	0x0f, 0xff,		// width of address word
	0x0f, 0xff,		// width of data word
	0x00, 0x0f,		// width of ID
	0x00, 0x0f,		// ID mask
	0x00, 0x1f,		// width of configuration word
	0x00, 0x1f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x0f, 0xff,		// Calibration width
	0x0f, 0xff,		// Calibration mask
	0x00, 0x00,		// ??
	0x01, 0xff,		// ??
	0x02, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x0f, 0xff,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x01, 0xff,		// address of internal clock calibration value
	0x00, 0x01,		// size of clock calibration space
	0x01,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x01c, 0x17,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC12F508[] =
{
	0x00, 0x1f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x1f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC12F508 =
{
	"12F508",			// device name
	def_PIC12F508,		// definition
	defx_PIC12F508,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	1,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC12F509[] =
{
	0x01, 0xff,		// size of program space
	0x0f, 0xff,		// width of address word
	0x0f, 0xff,		// width of data word
	0x00, 0x0f,		// width of ID
	0x00, 0x0f,		// ID mask
	0x00, 0x1f,		// width of configuration word
	0x00, 0x1f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x0f, 0xff,		// Calibration width
	0x0f, 0xff,		// Calibration mask
	0x00, 0x00,		// ??
	0x01, 0xff,		// ??
	0x02, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x0f, 0xff,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x01, 0xff,		// address of internal clock calibration value
	0x00, 0x01,		// size of clock calibration space
	0x01,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x01c, 0x17,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC12F509[] =
{
	0x00, 0x1f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x1f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC12F509 =
{
	"12F509",			// device name
	def_PIC12F509,		// definition
	defx_PIC12F509,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	1,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC12F629[] =
{
	0x03, 0xff,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x01, 0xff,		// width of configuration word
	0x01, 0xff,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x3f, 0xff,		// Calibration width
	0x3f, 0xff,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x80,		// size of data space
	0x03, 0xff,		// address of internal clock calibration value
	0x00, 0x01,		// size of clock calibration space
	0x00,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x0f, 0x1f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC12F629[] =
{
	0x01, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x01, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC12F629 =
{
	"12F629",			// device name
	def_PIC12F629,		// definition
	defx_PIC12F629,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0x2100,				// Data eeprom address
	1,						// number of words in cfg bits with factory set bits
	{0x3000, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC12C671[] =
{
	0x03, 0xff,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0xff,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x3f, 0xff,		// Calibration width
	0x3f, 0xff,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x03, 0xff,		// address of internal clock calibration value
	0x00, 0x01,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x09, 0x16,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC12C671[] =
{
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC12C671 =
{
	"12C671",			// device name
	def_PIC12C671,		// definition
	defx_PIC12C671,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC12C672[] =
{
	0x07, 0xff,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0xff,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x3f, 0xff,		// Calibration width
	0x3f, 0xff,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x07, 0xff,		// address of internal clock calibration value
	0x00, 0x01,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x09, 0x16,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC12C672[] =
{
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC12C672 =
{
	"12C672",			// device name
	def_PIC12C672,		// definition
	defx_PIC12C672,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC12CE673[] =
{
	0x03, 0xff,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0xff,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x3f, 0xff,		// Calibration width
	0x3f, 0xff,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x03, 0xff,		// address of internal clock calibration value
	0x00, 0x01,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x09, 0x16,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC12CE673[] =
{
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC12CE673 =
{
	"12CE673",			// device name
	def_PIC12CE673,	// definition
	defx_PIC12CE673,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC12CE674[] =
{
	0x07, 0xff,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0xff,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x3f, 0xff,		// Calibration width
	0x3f, 0xff,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x07, 0xff,		// address of internal clock calibration value
	0x00, 0x01,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x09, 0x16,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC12CE674[] =
{
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC12CE674 =
{
	"12CE674",			// device name
	def_PIC12CE674,	// definition
	defx_PIC12CE674,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC12F675[] =
{
	0x03, 0xff,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x31, 0xff,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x3f, 0xff,		// Calibration width
	0x3f, 0xff,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x80,		// size of data space
	0x03, 0xff,		// address of internal clock calibration value
	0x00, 0x01,		// size of clock calibration space
	0x00,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x0f, 0x1f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC12F675[] =
{
	0x31, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x01, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC12F675 =
{
	"12F675",			// device name
	def_PIC12F675,		// definition
	defx_PIC12F675,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0x2100,				// Data eeprom address
	1,						// number of words in cfg bits with factory set bits
	{0x3000, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC12F683[] =
{
	0x08, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x0f, 0xff,		// width of configuration word
	0x00, 0xff,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x08, 0x00,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x01, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x01,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x0f, 0x0f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC12F683[] =
{
	0x0f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x0f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC12F683 =
{
	"12F683",			// device name
	def_PIC12F683,		// definition
	defx_PIC12F683,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0x2000,				// Configuration memory start address
	0x2000, 0x2003,	// ID Locations addr
	0x2100,				// Data eeprom address
	1,						// number of words in cfg bits with factory set bits
	{0x3000, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC14000[] =
{
	0x0f, 0xc0,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0xbd,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x3f, 0xff,		// Calibration width
	0x3f, 0xff,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x0f, 0xc0,		// address of internal clock calibration value
	0x00, 0x40,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x01, 0x16,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC14000[] =
{
	0x3f, 0xbd, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0xbd, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC14000 =
{
	"14000",				// device name
	def_PIC14000,		// definition
	defx_PIC14000,		// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C505[] =
{
	0x03, 0xff,		// size of program space
	0x0f, 0xff,		// width of address word
	0x0f, 0xff,		// width of data word
	0x00, 0x0f,		// width of ID
	0x00, 0x0f,		// ID mask
	0x0f, 0xff,		// width of configuration word
	0x0f, 0xff,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x0f, 0xff,		// Calibration width
	0x0f, 0xff,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x04, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x0f, 0xff,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x03, 0xff,		// address of internal clock calibration value
	0x00, 0x01,		// size of clock calibration space
	0x0b,				// additional programming pulses for C devices
	0x08,				// main programming pulses for C devices
	0x0a, 0x16,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C505[] =
{
	0x0f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x0f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C505 =
{
	"16C505",			// device name
	def_PIC16C505,		// definition
	defx_PIC16C505,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C52[] =
{
	0x01, 0x80,		// size of program space
	0x0f, 0xff,		// width of address word
	0x0f, 0xff,		// width of data word
	0x00, 0x0f,		// width of ID
	0x00, 0x0f,		// ID mask
	0x00, 0x0f,		// width of configuration word
	0x00, 0x0b,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x02, 0x00,		// address of ID locations
	0x00,				// size of ID locations
	0x0f, 0xff,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x0b,				// additional programming pulses for C devices
	0x08,				// main programming pulses for C devices
	0x05, 0x04,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C52[] =
{
	0x00, 0x0b, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x0b, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C52 =
{
	"16C52",				// device name
	def_PIC16C52,		// definition
	defx_PIC16C52,		// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C54[] =
{
	0x02, 0x00,		// size of program space
	0x0f, 0xff,		// width of address word
	0x0f, 0xff,		// width of data word
	0x00, 0x0f,		// width of ID
	0x00, 0x0f,		// ID mask
	0x00, 0x0f,		// width of configuration word
	0x00, 0x0f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x02, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x0f, 0xff,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x0b,				// additional programming pulses for C devices
	0x08,				// main programming pulses for C devices
	0x05, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C54[] =
{
	0x00, 0x0f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x0f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C54 =
{
	"16C54",				// device name
	def_PIC16C54,		// definition
	defx_PIC16C54,		// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C54A[] =
{
	0x02, 0x00,		// size of program space
	0x0f, 0xff,		// width of address word
	0x0f, 0xff,		// width of data word
	0x00, 0x0f,		// width of ID
	0x00, 0x0f,		// ID mask
	0x00, 0x0f,		// width of configuration word
	0x00, 0x0f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x02, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x0f, 0xff,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x0b,				// additional programming pulses for C devices
	0x08,				// main programming pulses for C devices
	0x05, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C54A[] =
{
	0x00, 0x0f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x0f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C54A =
{
	"16C54A",			// device name
	def_PIC16C54A,		// definition
	defx_PIC16C54A,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C54B[] =
{
	0x02, 0x00,		// size of program space
	0x0f, 0xff,		// width of address word
	0x0f, 0xff,		// width of data word
	0x00, 0x0f,		// width of ID
	0x00, 0x0f,		// ID mask
	0x0f, 0xff,		// width of configuration word
	0x0f, 0xff,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x02, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x0f, 0xff,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x0b,				// additional programming pulses for C devices
	0x08,				// main programming pulses for C devices
	0x05, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C54B[] =
{
	0x0f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x0f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C54B =
{
	"16C54B",			// device name
	def_PIC16C54B,		// definition
	defx_PIC16C54B,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C54C[] =
{
	0x02, 0x00,		// size of program space
	0x0f, 0xff,		// width of address word
	0x0f, 0xff,		// width of data word
	0x00, 0x0f,		// width of ID
	0x00, 0x0f,		// ID mask
	0x0f, 0xff,		// width of configuration word
	0x0f, 0xff,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x02, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x0f, 0xff,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x0b,				// additional programming pulses for C devices
	0x08,				// main programming pulses for C devices
	0x05, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C54C[] =
{
	0x0f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x0f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C54C =
{
	"16C54C",			// device name
	def_PIC16C54C,		// definition
	defx_PIC16C54C,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16HV540[] =
{
	0x02, 0x00,		// size of program space
	0x0f, 0xff,		// width of address word
	0x0f, 0xff,		// width of data word
	0x00, 0x0f,		// width of ID
	0x00, 0x0f,		// ID mask
	0x00, 0x0f,		// width of configuration word
	0x00, 0x0f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x02, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x0f, 0xff,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x0b,				// additional programming pulses for C devices
	0x08,				// main programming pulses for C devices
	0x05, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16HV540[] =
{
	0x00, 0x0f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x0f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16HV540 =
{
	"16HV540",			// device name
	def_PIC16HV540,	// definition
	defx_PIC16HV540,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C55[] =
{
	0x02, 0x00,		// size of program space
	0x0f, 0xff,		// width of address word
	0x0f, 0xff,		// width of data word
	0x00, 0x0f,		// width of ID
	0x00, 0x0f,		// ID mask
	0x00, 0x0f,		// width of configuration word
	0x00, 0x0f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x02, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x0f, 0xff,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x0b,				// additional programming pulses for C devices
	0x08,				// main programming pulses for C devices
	0x06, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C55[] =
{
	0x00, 0x0f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x0f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C55 =
{
	"16C55",				// device name
	def_PIC16C55,		// definition
	defx_PIC16C55,		// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C55A[] =
{
	0x02, 0x00,		// size of program space
	0x0f, 0xff,		// width of address word
	0x0f, 0xff,		// width of data word
	0x00, 0x0f,		// width of ID
	0x00, 0x0f,		// ID mask
	0x0f, 0xff,		// width of configuration word
	0x0f, 0xff,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x02, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x0f, 0xff,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x0b,				// additional programming pulses for C devices
	0x08,				// main programming pulses for C devices
	0x06, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C55A[] =
{
	0x0f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x0f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C55A =
{
	"16C55A",			// device name
	def_PIC16C55A,		// definition
	defx_PIC16C55A,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C554[] =
{
	0x02, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x3f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x02, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C554[] =
{
	0x3f, 0x3f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x3f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C554 =
{
	"16C554",			// device name
	def_PIC16C554,		// definition
	defx_PIC16C554,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C558[] =
{
	0x08, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x3f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x02, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C558[] =
{
	0x3f, 0x3f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x3f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C558 =
{
	"16C558",			// device name
	def_PIC16C558,		// definition
	defx_PIC16C558,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C56[] =
{
	0x04, 0x00,		// size of program space
	0x0f, 0xff,		// width of address word
	0x0f, 0xff,		// width of data word
	0x00, 0x0f,		// width of ID
	0x00, 0x0f,		// ID mask
	0x00, 0x0f,		// width of configuration word
	0x00, 0x0f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x04, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x0f, 0xff,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x0b,				// additional programming pulses for C devices
	0x08,				// main programming pulses for C devices
	0x05, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C56[] =
{
	0x00, 0x0f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x0f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C56 =
{
	"16C56",				// device name
	def_PIC16C56,		// definition
	defx_PIC16C56,		// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C56A[] =
{
	0x04, 0x00,		// size of program space
	0x0f, 0xff,		// width of address word
	0x0f, 0xff,		// width of data word
	0x00, 0x0f,		// width of ID
	0x00, 0x0f,		// ID mask
	0x0f, 0xff,		// width of configuration word
	0x0f, 0xff,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x04, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x0f, 0xff,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x0b,				// additional programming pulses for C devices
	0x08,				// main programming pulses for C devices
	0x05, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C56A[] =
{
	0x0f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x0f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C56A =
{
	"16C56A",			// device name
	def_PIC16C56A,		// definition
	defx_PIC16C56A,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C57[] =
{
	0x08, 0x00,		// size of program space
	0x0f, 0xff,		// width of address word
	0x0f, 0xff,		// width of data word
	0x00, 0x0f,		// width of ID
	0x00, 0x0f,		// ID mask
	0x00, 0x0f,		// width of configuration word
	0x00, 0x0f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x08, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x0f, 0xff,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x0b,				// additional programming pulses for C devices
	0x08,				// main programming pulses for C devices
	0x06, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C57[] =
{
	0x00, 0x0f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x0f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C57 =
{
	"16C57",				// device name
	def_PIC16C57,		// definition
	defx_PIC16C57,		// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C57C[] =
{
	0x08, 0x00,		// size of program space
	0x0f, 0xff,		// width of address word
	0x0f, 0xff,		// width of data word
	0x00, 0x0f,		// width of ID
	0x00, 0x0f,		// ID mask
	0x0f, 0xff,		// width of configuration word
	0x0f, 0xff,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x08, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x0f, 0xff,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x0b,				// additional programming pulses for C devices
	0x08,				// main programming pulses for C devices
	0x06, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C57C[] =
{
	0x0f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x0f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C57C =
{
	"16C57C",			// device name
	def_PIC16C57C,		// definition
	defx_PIC16C57C,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C58A[] =
{
	0x08, 0x00,		// size of program space
	0x0f, 0xff,		// width of address word
	0x0f, 0xff,		// width of data word
	0x00, 0x0f,		// width of ID
	0x00, 0x0f,		// ID mask
	0x00, 0x0f,		// width of configuration word
	0x00, 0x0f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x08, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x0f, 0xff,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x0b,				// additional programming pulses for C devices
	0x08,				// main programming pulses for C devices
	0x05, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C58A[] =
{
	0x00, 0x0f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x0f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C58A =
{
	"16C58A",			// device name
	def_PIC16C58A,		// definition
	defx_PIC16C58A,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C58B[] =
{
	0x08, 0x00,		// size of program space
	0x0f, 0xff,		// width of address word
	0x0f, 0xff,		// width of data word
	0x00, 0x0f,		// width of ID
	0x00, 0x0f,		// ID mask
	0x0f, 0xff,		// width of configuration word
	0x0f, 0xff,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x08, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x0f, 0xff,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x0b,				// additional programming pulses for C devices
	0x08,				// main programming pulses for C devices
	0x05, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C58B[] =
{
	0x0f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x0f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C58B =
{
	"16C58B",			// device name
	def_PIC16C58B,		// definition
	defx_PIC16C58B,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C61[] =
{
	0x04, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x00, 0x1f,		// width of configuration word
	0x00, 0x1f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x02, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C61[] =
{
	0x00, 0x1f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x1f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C61 =
{
	"16C61",				// device name
	def_PIC16C61,		// definition
	defx_PIC16C61,		// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C62[] =
{
	0x08, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x00, 0x3f,		// width of configuration word
	0x00, 0x3f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x03, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C62[] =
{
	0x00, 0x3f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x3f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C62 =
{
	"16C62",				// device name
	def_PIC16C62,		// definition
	defx_PIC16C62,		// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C62A[] =
{
	0x08, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x03, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C62A[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C62A =
{
	"16C62A",			// device name
	def_PIC16C62A,		// definition
	defx_PIC16C62A,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C62B[] =
{
	0x08, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x03, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C62B[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C62B =
{
	"16C62B",			// device name
	def_PIC16C62B,		// definition
	defx_PIC16C62B,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C620[] =
{
	0x02, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x02, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C620[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C620 =
{
	"16C620",			// device name
	def_PIC16C620,		// definition
	defx_PIC16C620,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C620A[] =
{
	0x02, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x02, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C620A[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C620A =
{
	"16C620A",			// device name
	def_PIC16C620A,	// definition
	defx_PIC16C620A,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C621[] =
{
	0x04, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x02, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C621[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C621 =
{
	"16C621",			// device name
	def_PIC16C621,		// definition
	defx_PIC16C621,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C621A[] =
{
	0x04, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x02, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C621A[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C621A =
{
	"16C621A",			// device name
	def_PIC16C621A,	// definition
	defx_PIC16C621A,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C622[] =
{
	0x08, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x02, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C622[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C622 =
{
	"16C622",			// device name
	def_PIC16C622,		// definition
	defx_PIC16C622,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C622A[] =
{
	0x08, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x02, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C622A[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C622A =
{
	"16C622A",			// device name
	def_PIC16C622A,	// definition
	defx_PIC16C622A,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16CE623[] =
{
	0x02, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x02, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16CE623[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16CE623 =
{
	"16CE623",			// device name
	def_PIC16CE623,	// definition
	defx_PIC16CE623,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16CE624[] =
{
	0x04, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x02, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16CE624[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16CE624 =
{
	"16CE624",			// device name
	def_PIC16CE624,	// definition
	defx_PIC16CE624,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16CE625[] =
{
	0x08, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x02, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16CE625[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16CE625 =
{
	"16CE625",			// device name
	def_PIC16CE625,	// definition
	defx_PIC16CE625,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16F627[] =
{
	0x04, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3d, 0xff,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x80,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x00,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x0c, 0x0f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16F627[] =
{
	0x3d, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3d, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16F627 =
{
	"16F627",			// device name
	def_PIC16F627,		// definition
	defx_PIC16F627,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16F627A[] =
{
	0x04, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3d, 0xff,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x80,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x00,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x0c, 0x0f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16F627A[] =
{
	0x3d, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3d, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16F627A =
{
	"16F627A",			// device name
	def_PIC16F627A,	// definition
	defx_PIC16F627A,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16F628[] =
{
	0x08, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3d, 0xff,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x80,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x00,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x0c, 0x0f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16F628[] =
{
	0x3d, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3d, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16F628 =
{
	"16F628",			// device name
	def_PIC16F628,		// definition
	defx_PIC16F628,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16F628A[] =
{
	0x08, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x21, 0xff,		// width of configuration word
	0x21, 0xff,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x08, 0x00,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x80,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x00,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x13, 0x0f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16F628A[] =
{
	0x21, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x21, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 
};

const static PIC_DEFINITION PIC16F628A =
{
	"16F628A",			// device name
	def_PIC16F628A,	// definition
	defx_PIC16F628A,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address    
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C63[] =
{
	0x10, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x03, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C63[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C63 =
{
	"16C63",				// device name
	def_PIC16C63,		// definition
	defx_PIC16C63,		// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C63A[] =
{
	0x10, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x03, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C63A[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C63A =
{
	"16C63A",			// device name
	def_PIC16C63A,		// definition
	defx_PIC16C63A,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C63B[] =
{
	0x10, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x03, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C63B[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C63B =
{
	"16C63B",			// device name
	def_PIC16C63B,		// definition
	defx_PIC16C63B,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16F630[] =
{
	0x03, 0xff,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x01, 0xff,		// width of configuration word
	0x01, 0xff,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x3f, 0xff,		// Calibration width
	0x3f, 0xff,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x80,		// size of data space
	0x03, 0xff,		// address of internal clock calibration value
	0x00, 0x01,		// size of clock calibration space
	0x00,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x0f, 0x1f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16F630[] =
{
	0x01, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x01, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16F630 =
{
	"16F630",			// device name
	def_PIC16F630,		// definition
	defx_PIC16F630,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0x2100,				// Data eeprom address
	1,						// number of words in cfg bits with factory set bits
	{0x3000, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C64[] =
{
	0x08, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x00, 0x3f,		// width of configuration word
	0x00, 0x3f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x04, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C64[] =
{
	0x00, 0x3f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x3f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C64 =
{
	"16C64",				// device name
	def_PIC16C64,		// definition
	defx_PIC16C64,		// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C64A[] =
{
	0x08, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x04, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C64A[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C64A =
{
	"16C64A",			// device name
	def_PIC16C64A,		// definition
	defx_PIC16C64A,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C642[] =
{
	0x10, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0xff,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x03, 0x26,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C642[] =
{
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C642 =
{
	"16C642",			// device name
	def_PIC16C642,		// definition
	defx_PIC16C642,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------

const static unsigned char def_PIC16F648[] =
{
	0x10, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x21, 0xff,		// width of configuration word
	0x21, 0xff,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x10, 0x00,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x01, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x01,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x13, 0x0f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16F648[] =
{
	0x21, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x21, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16F648 =
{
	"16F648",			// device name
	def_PIC16F648,		// definition
	defx_PIC16F648,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13),	// bit map of supporting programmers
};

//-----------------------------------------------------------

const static unsigned char def_PIC16F648A[] =
{
	0x10, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x21, 0xff,		// width of configuration word
	0x21, 0xff,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x10, 0x00,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x01, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x01,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x13, 0x0f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16F648A[] =
{
	0x21, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x21, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16F648A =
{
	"16F648A",			// device name
	def_PIC16F648A,	// definition
	defx_PIC16F648A,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C65[] =
{
	0x10, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x00, 0x3f,		// width of configuration word
	0x00, 0x3f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x04, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C65[] =
{
	0x00, 0x3f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x3f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C65 =
{
	"16C65",				// device name
	def_PIC16C65,		// definition
	defx_PIC16C65,		// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C65A[] =
{
	0x10, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x04, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C65A[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C65A =
{
	"16C65A",			// device name
	def_PIC16C65A,		// definition
	defx_PIC16C65A,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C65B[] =
{
	0x10, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x04, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C65B[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C65B =
{
	"16C65B",			// device name
	def_PIC16C65B,		// definition
	defx_PIC16C65B,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C66[] =
{
	0x20, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x03, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C66[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C66 =
{
	"16C66",				// device name
	def_PIC16C66,		// definition
	defx_PIC16C66,		// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C66A[] =
{
	0x20, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x03, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C66A[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C66A =
{
	"16C66A",			// device name
	def_PIC16C66A,		// definition
	defx_PIC16C66A,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C662[] =
{
	0x10, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0xff,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x04, 0x26,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C662[] =
{
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C662 =
{
	"16C662",			// device name
	def_PIC16C662,		// definition
	defx_PIC16C662,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C67[] =
{
	0x20, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x04, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C67[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C67 =
{
	"16C67",				// device name
	def_PIC16C67,		// definition
	defx_PIC16C67,		// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C67A[] =
{
	0x20, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x04, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C67A[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C67A =
{
	"16C67A",			// device name
	def_PIC16C67A,		// definition
	defx_PIC16C67A,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16F676[] =
{
	0x03, 0xff,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x01, 0xff,		// width of configuration word
	0x01, 0xff,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x3f, 0xff,		// Calibration width
	0x3f, 0xff,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x80,		// size of data space
	0x03, 0xff,		// address of internal clock calibration value
	0x00, 0x01,		// size of clock calibration space
	0x00,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x0f, 0x1f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16F676[] =
{
	0x01, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x01, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16F676 =
{
	"16F676",			// device name
	def_PIC16F676,		// definition
	defx_PIC16F676,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0x2100,				// Data eeprom address
	1,						// number of words in cfg bits with factory set bits
	{0x3000, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C71[] =
{
	0x04, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x00, 0x1f,		// width of configuration word
	0x00, 0x1f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x02, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C71[] =
{
	0x00, 0x1f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x1f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C71 =
{
	"16C71",				// device name
	def_PIC16C71,		// definition
	defx_PIC16C71,		// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C710[] =
{
	0x02, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0xff,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x02, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C710[] =
{
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C710 =
{
	"16C710",			// device name
	def_PIC16C710,		// definition
	defx_PIC16C710,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C711[] =
{
	0x04, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0xff,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x02, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C711[] =
{
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C711 =
{
	"16C711",			// device name
	def_PIC16C711,		// definition
	defx_PIC16C711,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C712[] =
{
	0x04, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x02, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C712[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C712 =
{
	"16C712",			// device name
	def_PIC16C712,		// definition
	defx_PIC16C712,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C715[] =
{
	0x08, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0xff,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x02, 0x26,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C715[] =
{
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C715 =
{
	"16C715",			// device name
	def_PIC16C715,		// definition
	defx_PIC16C715,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C716[] =
{
	0x08, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x02, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C716[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C716 =
{
	"16C716",			// device name
	def_PIC16C716,		// definition
	defx_PIC16C716,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C717[] =
{
	0x08, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x0c, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C717[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C717 =
{
	"16C717",			// device name
	def_PIC16C717,		// definition
	defx_PIC16C717,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C72[] =
{
	0x08, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x03, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C72[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C72 =
{
	"16C72",				// device name
	def_PIC16C72,		// definition
	defx_PIC16C72,		// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C72A[] =
{
	0x08, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x03, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C72A[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C72A =
{
	"16C72A",			// device name
	def_PIC16C72A,		// definition
	defx_PIC16C72A,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16F72[] =
{
	0x08, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x00, 0x7f,		// width of configuration word
	0x00, 0x5f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x00,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x0d, 0x07,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16F72[] =
{
	0x00, 0x5f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x5f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16F72 =
{
	"16F72",				// device name
	def_PIC16F72,		// definition
	defx_PIC16F72,		// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C73[] =
{
	0x10, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x00, 0x3f,		// width of configuration word
	0x00, 0x3f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x03, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C73[] =
{
	0x00, 0x3f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x3f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C73 =
{
	"16C73",				// device name
	def_PIC16C73,		// definition
	defx_PIC16C73,		// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C73A[] =
{
	0x10, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x03, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C73A[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C73A =
{
	"16C73A",			// device name
	def_PIC16C73A,		// definition
	defx_PIC16C73A,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C73B[] =
{
	0x10, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x03, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C73B[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C73B =
{
	"16C73B",			// device name
	def_PIC16C73B,		// definition
	defx_PIC16C73B,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16F73[] =
{
	0x10, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x00, 0x7f,		// width of configuration word
	0x00, 0x5f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x00,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x0d, 0x07,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16F73[] =
{
	0x00, 0x5f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x5f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16F73 =
{
	"16F73",				// device name
	def_PIC16F73,		// definition
	defx_PIC16F73,		// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C74[] =
{
	0x10, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x00, 0x7f,		// width of configuration word
	0x00, 0x3f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x04, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C74[] =
{
	0x00, 0x3f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x3f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C74 =
{
	"16C74",				// device name
	def_PIC16C74,		// definition
	defx_PIC16C74,		// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C74A[] =
{
	0x10, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x04, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C74A[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C74A =
{
	"16C74A",			// device name
	def_PIC16C74A,		// definition
	defx_PIC16C74A,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C74B[] =
{
	0x10, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x04, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C74B[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C74B =
{
	"16C74B",			// device name
	def_PIC16C74B,		// definition
	defx_PIC16C74B,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C74C[] =
{
	0x10, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x04, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C74C[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C74C =
{
	"16C74C",			// device name
	def_PIC16C74C,		// definition
	defx_PIC16C74C,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16F74[] =
{
	0x10, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x00, 0x7f,		// width of configuration word
	0x00, 0x5f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x00,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x0d, 0x07,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16F74[] =
{
	0x00, 0x5f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x5f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16F74 =
{
	"16F74",				// device name
	def_PIC16F74,		// definition
	defx_PIC16F74,		// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C745[] =
{
	0x20, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x3f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x03, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C745[] =
{
	0x3f, 0x3f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x3f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C745 =
{
	"16C745",			// device name
	def_PIC16C745,		// definition
	defx_PIC16C745,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C76[] =
{
	0x20, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x03, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C76[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C76 =
{
	"16C76",				// device name
	def_PIC16C76,		// definition
	defx_PIC16C76,		// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16F76[] =
{
	0x20, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x00, 0x7f,		// width of configuration word
	0x00, 0x5f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x00,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x0d, 0x07,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16F76[] =
{
	0x00, 0x5f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x5f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16F76 =
{
	"16F76",				// device name
	def_PIC16F76,		// definition
	defx_PIC16F76,		// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C765[] =
{
	0x20, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x3f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x04, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C765[] =
{
	0x3f, 0x3f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x3f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C765 =
{
	"16C765",			// device name
	def_PIC16C765,		// definition
	defx_PIC16C765,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C77[] =
{
	0x20, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x04, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C77[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C77 =
{
	"16C77",				// device name
	def_PIC16C77,		// definition
	defx_PIC16C77,		// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16F77[] =
{
	0x20, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x00, 0x7f,		// width of configuration word
	0x00, 0x5f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x00,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x0d, 0x07,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16F77[] =
{
	0x00, 0x5f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x5f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16F77 =
{
	"16F77",				// device name
	def_PIC16F77,		// definition
	defx_PIC16F77,		// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C770[] =
{
	0x08, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x08, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C770[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C770 =
{
	"16C770",			// device name
	def_PIC16C770,		// definition
	defx_PIC16C770,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C771[] =
{
	0x10, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x08, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C771[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C771 =
{
	"16C771",			// device name
	def_PIC16C771,		// definition
	defx_PIC16C771,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C773[] =
{
	0x10, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x04, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C773[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C773 =
{
	"16C773",			// device name
	def_PIC16C773,		// definition
	defx_PIC16C773,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C774[] =
{
	0x10, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x04, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C774[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C774 =
{
	"16C774",			// device name
	def_PIC16C774,		// definition
	defx_PIC16C774,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C781[] =
{
	0x04, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x08, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C781[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C781 =
{
	"16C781",			// device name
	def_PIC16C781,		// definition
	defx_PIC16C781,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C782[] =
{
	0x08, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x08, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C782[] =
{
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C782 =
{
	"16C782",			// device name
	def_PIC16C782,		// definition
	defx_PIC16C782,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16F818[] =
{
	0x04, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0xff,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x80,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x00,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x10, 0x0f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16F818[] =
{
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16F818 =
{
	"16F818",			// device name
	def_PIC16F818,		// definition
	defx_PIC16F818,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	4,						// Word alignment for writing to this device
	0x400e,				// Configuration memory start address
	0x4000, 4,			// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16F819[] =
{
	0x08, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0xff,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x01, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x00,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x10, 0x0f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16F819[] =
{
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16F819 =
{
	"16F819",			// device name
	def_PIC16F819,		// definition
	defx_PIC16F819,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	4,						// Word alignment for writing to this device
	0x400e,				// Configuration memory start address
	0x4000, 4,			// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16F83[] =
{
	0x02, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0xff,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// address of ID locations
	0x00, 0x01,		// size of ID locations
	0x20, 0x00,		// ??
	0x04,				// ??
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x40,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x02, 0x0f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16F83[] =
{
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16F83 =
{
	"16F83",				// device name
	def_PIC16F83,		// definition
	defx_PIC16F83,		// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C84[] =
{
	0x04, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x00, 0x7f,		// width of configuration word
	0x00, 0x1f,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x40,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x02, 0x0f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C84[] =
{
	0x00, 0x1f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x1f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C84 =
{
	"16C84",				// device name
	def_PIC16C84,		// definition
	defx_PIC16C84,		// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16F84[] =
{
	0x04, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0xff,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x40,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x02, 0x0f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16F84[] =
{
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16F84 =
{
	"16F84",				// device name
	def_PIC16F84,		// definition
	defx_PIC16F84,		// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16F84A[] =
{
	0x04, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0xff,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x40,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x00,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x02, 0x0f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16F84A[] =
{
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16F84A =
{
	"16F84A",			// device name
	def_PIC16F84A,		// definition
	defx_PIC16F84A,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16F87[] =
{
	0x10, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0xff,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x10, 0x00,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x02,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x01, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x00,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x14, 0x0f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16F87[] =
{
	0x3f, 0xff, 0x00, 0x03,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0xff, 0x00, 0x03,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16F87 =
{
	"16F87",				// device name
	def_PIC16F87,		// definition
	defx_PIC16F87,		// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16F88[] =
{
	0x10, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0xff,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x10, 0x00,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x02,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x01, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x00,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x14, 0x0f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16F88[] =
{
	0x3f, 0xff, 0x00, 0x03,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0xff, 0x00, 0x03,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16F88 =
{
	"16F88",				// device name
	def_PIC16F88,		// definition
	defx_PIC16F88,		// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16F870[] =
{
	0x08, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0xff,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x40,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x00,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x03, 0x0f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16F870[] =
{
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16F870 =
{
	"16F870",			// device name
	def_PIC16F870,		// definition
	defx_PIC16F870,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16F871[] =
{
	0x08, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0xff,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x40,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x00,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x04, 0x0f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16F871[] =
{
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16F871 =
{
	"16F871",			// device name
	def_PIC16F871,		// definition
	defx_PIC16F871,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16F872[] =
{
	0x08, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0xff,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x40,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x00,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x03, 0x0f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16F872[] =
{
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16F872 =
{
	"16F872",			// device name
	def_PIC16F872,		// definition
	defx_PIC16F872,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16F873[] =
{
	0x10, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0xff,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x80,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x00,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x04, 0x0f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16F873[] =
{
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16F873 =
{
	"16F873",			// device name
	def_PIC16F873,		// definition
	defx_PIC16F873,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16F873A[] =
{
	0x10, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0xff,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x80,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x00,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x11, 0x0f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16F873A[] =
{
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16F873A =
{
	"16F873A",			// device name
	def_PIC16F873A,	// definition
	defx_PIC16F873A,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16F874[] =
{
	0x10, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0xff,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x80,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x00,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x04, 0x0f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16F874[] =
{
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16F874 =
{
	"16F874",			// device name
	def_PIC16F874,		// definition
	defx_PIC16F874,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16F874A[] =
{
	0x10, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0xff,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x80,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x00,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x11, 0x0f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16F874A[] =
{
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16F874A =
{
	"16F874A",			// device name
	def_PIC16F874A,	// definition
	defx_PIC16F874A,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	4,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16F876[] =
{
	0x20, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0xff,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x01, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x00,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x04, 0x0f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16F876[] =
{
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16F876 =
{
	"16F876",			// device name
	def_PIC16F876,		// definition
	defx_PIC16F876,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16F876A[] =
{
	0x20, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x2f, 0xcf,		// width of configuration word
	0x2f, 0xcf,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x20, 0x00,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x01, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x00,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x11, 0x0f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16F876A[] =
{
	0x2f, 0xcf, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x2f, 0xcf, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16F876A =
{
	"16F876A",			// device name
	def_PIC16F876A,	// definition
	defx_PIC16F876A,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	4,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16F877[] =
{
	0x20, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0xff,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x01, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x00,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x04, 0x0f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16F877[] =
{
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16F877 =
{
	"16F877",			// device name
	def_PIC16F877,		// definition
	defx_PIC16F877,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16F877A[] =
{
	0x20, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0xff,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x01, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x00,				// additional programming pulses for C devices
	0x01,				// main programming pulses for C devices
	0x11, 0x0f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16F877A[] =
{
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0xff, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16F877A =
{
	"16F877A",			// device name
	def_PIC16F877A,	// definition
	defx_PIC16F877A,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	4,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C923[] =
{
	0x10, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x3f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x04, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C923[] =
{
	0x3f, 0x3f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x3f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C923 =
{
	"16C923",			// device name
	def_PIC16C923,		// definition
	defx_PIC16C923,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C924[] =
{
	0x10, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x3f, 0x3f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x04, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C924[] =
{
	0x3f, 0x3f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x3f, 0x3f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C924 =
{
	"16C924",			// device name
	def_PIC16C924,		// definition
	defx_PIC16C924,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C925[] =
{
	0x10, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x00, 0x7f,		// width of configuration word
	0x00, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x04, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C925[] =
{
	0x00, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C925 =
{
	"16C925",			// device name
	def_PIC16C925,		// definition
	defx_PIC16C925,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC16C926[] =
{
	0x20, 0x00,		// size of program space
	0x3f, 0xff,		// width of address word
	0x3f, 0xff,		// width of data word
	0x00, 0x7f,		// width of ID
	0x00, 0x7f,		// ID mask
	0x00, 0x7f,		// width of configuration word
	0x00, 0x7f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x20, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x20, 0x07,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x04, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC16C926[] =
{
	0x00, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x7f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC16C926 =
{
	"16C926",			// device name
	def_PIC16C926,		// definition
	defx_PIC16C926,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC17C42[] =
{
	0x08, 0x00,		// size of program space
	0xff, 0xff,		// width of address word
	0xff, 0xff,		// width of data word
	0x00, 0x00,		// width of ID
	0x00, 0x00,		// ID mask
	0x00, 0x7f,		// width of configuration word
	0x00, 0x5f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x00, 0x00,		// address of ID locations
	0x00,				// size of ID locations
	0xfe, 0x00,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x07, 0x04,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC17C42[] =
{
	0x00, 0x5f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x5f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC17C42 =
{
	"17C42",				// device name
	def_PIC17C42,		// definition
	defx_PIC17C42,		// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART),		// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC17C42A[] =
{
	0x08, 0x00,		// size of program space
	0xff, 0xff,		// width of address word
	0xff, 0xff,		// width of data word
	0x00, 0x00,		// width of ID
	0x00, 0x00,		// ID mask
	0xff, 0xff,		// width of configuration word
	0x80, 0x5f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x00, 0x00,		// address of ID locations
	0x00,				// size of ID locations
	0xfe, 0x00,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x07, 0x04,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC17C42A[] =
{
	0x80, 0x5f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x80, 0x5f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC17C42A =
{
	"17C42A",			// device name
	def_PIC17C42A,		// definition
	defx_PIC17C42A,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART),		// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC17C43[] =
{
	0x10, 0x00,		// size of program space
	0xff, 0xff,		// width of address word
	0xff, 0xff,		// width of data word
	0x00, 0x00,		// width of ID
	0x00, 0x00,		// ID mask
	0xff, 0xff,		// width of configuration word
	0x80, 0x5f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x00, 0x00,		// address of ID locations
	0x00,				// size of ID locations
	0xfe, 0x00,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x07, 0x04,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC17C43[] =
{
	0x80, 0x5f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x80, 0x5f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC17C43 =
{
	"17C43",				// device name
	def_PIC17C43,		// definition
	defx_PIC17C43,		// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART),		// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC17C44[] =
{
	0x20, 0x00,		// size of program space
	0xff, 0xff,		// width of address word
	0xff, 0xff,		// width of data word
	0x00, 0x00,		// width of ID
	0x00, 0x00,		// ID mask
	0xff, 0xff,		// width of configuration word
	0x80, 0x5f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x00, 0x00,		// address of ID locations
	0x00,				// size of ID locations
	0xfe, 0x00,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x07, 0x04,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC17C44[] =
{
	0x80, 0x5f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x80, 0x5f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC17C44 =
{
	"17C44",				// device name
	def_PIC17C44,		// definition
	defx_PIC17C44,		// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART),		// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC17C752[] =
{
	0x20, 0x00,		// size of program space
	0xff, 0xff,		// width of address word
	0xff, 0xff,		// width of data word
	0x00, 0x00,		// width of ID
	0x00, 0x00,		// ID mask
	0xff, 0xff,		// width of configuration word
	0xc0, 0x5f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x00, 0x00,		// address of ID locations
	0x00,				// size of ID locations
	0xfe, 0x00,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x07, 0x04,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC17C752[] =
{
	0xc0, 0x5f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0xc0, 0x5f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC17C752 =
{
	"17C752",			// device name
	def_PIC17C752,		// definition
	defx_PIC17C752,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART),		// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC17C756[] =
{
	0x40, 0x00,		// size of program space
	0xff, 0xff,		// width of address word
	0xff, 0xff,		// width of data word
	0x00, 0x00,		// width of ID
	0x00, 0x00,		// ID mask
	0xff, 0xff,		// width of configuration word
	0xc0, 0x5f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x00, 0x00,		// address of ID locations
	0x00,				// size of ID locations
	0xfe, 0x00,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x07, 0x04,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC17C756[] =
{
	0xc0, 0x5f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0xc0, 0x5f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC17C756 =
{
	"17C756",			// device name
	def_PIC17C756,		// definition
	defx_PIC17C756,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART),		// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC17C756A[] =
{
	0x40, 0x00,		// size of program space
	0xff, 0xff,		// width of address word
	0xff, 0xff,		// width of data word
	0x00, 0x00,		// width of ID
	0x00, 0x00,		// ID mask
	0xff, 0xff,		// width of configuration word
	0xc0, 0x5f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x00, 0x00,		// address of ID locations
	0x00,				// size of ID locations
	0xfe, 0x00,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x07, 0x04,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC17C756A[] =
{
	0xc0, 0x5f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0xc0, 0x5f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC17C756A =
{
	"17C756A",			// device name
	def_PIC17C756A,	// definition
	defx_PIC17C756A,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART),		// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC17C762[] =
{
	0x20, 0x00,		// size of program space
	0xff, 0xff,		// width of address word
	0xff, 0xff,		// width of data word
	0x00, 0x00,		// width of ID
	0x00, 0x00,		// ID mask
	0xff, 0xff,		// width of configuration word
	0xc0, 0x5f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x00, 0x00,		// address of ID locations
	0x00,				// size of ID locations
	0xfe, 0x00,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x07, 0x04,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC17C762[] =
{
	0xc0, 0x5f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0xc0, 0x5f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC17C762 =
{
	"17C762",			// device name
	def_PIC17C762,		// definition
	defx_PIC17C762,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART),		// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC17C766[] =
{
	0x40, 0x00,		// size of program space
	0xff, 0xff,		// width of address word
	0xff, 0xff,		// width of data word
	0x00, 0x00,		// width of ID
	0x00, 0x00,		// ID mask
	0xff, 0xff,		// width of configuration word
	0xc0, 0x5f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x00, 0x00,		// address of ID locations
	0x00,				// size of ID locations
	0xfe, 0x00,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x07, 0x04,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC17C766[] =
{
	0xc0, 0x5f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0xc0, 0x5f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC17C766 =
{
	"17C766",			// device name
	def_PIC17C766,		// definition
	defx_PIC17C766,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART),		// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC18C242[] =
{
	0x20, 0x00,		// size of program space
	0xff, 0xff,		// width of address word
	0xff, 0xff,		// width of data word
	0x0f, 0x0f,		// width of ID
	0x0f, 0x0f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x27, 0xff,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x00, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x00, 0x00,		// address of configuration bits
	0x04,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x0b, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC18C242[] =
{
	0x27, 0xff, 0x0f, 0x0f,
	0x01, 0x00, 0x00, 0x01,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x27, 0xff, 0x0f, 0x0f,
	0x01, 0x00, 0x00, 0x01,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC18C242 =
{
	"18C242",			// device name
	def_PIC18C242,		// definition
	defx_PIC18C242,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC18F242[] =
{
	0x20, 0x00,		// size of program space
	0xff, 0xff,		// width of address word
	0xff, 0xff,		// width of data word
	0x0f, 0x0f,		// width of ID
	0x0f, 0x0f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x27, 0x00,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x00, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x00, 0x00,		// address of configuration bits
	0x07,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x01, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x0e, 0x0f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC18F242[] =
{
	0x27, 0x00, 0x0f, 0x0f,
	0x01, 0x00, 0x00, 0x85,
	0xc0, 0x03, 0xe0, 0x03,
	0x40, 0x03, 0x00, 0x00,
	0x27, 0x00, 0x0f, 0x0f,
	0x01, 0x00, 0x00, 0x85,
	0xc0, 0x03, 0xe0, 0x03,
	0x40, 0x03, 0x00, 0x00,
};

const static PIC_DEFINITION PIC18F242 =
{
	"18F242",			// device name
	def_PIC18F242,		// definition
	defx_PIC18F242,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	4,						// Word alignment for writing to this device
	0x300000,			// Configuration memory start address
	0x200000, 0,		// ID Locations addr
	0xF00000,			// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC18F248[] =
{
	0x20, 0x00,		// size of program space
	0xff, 0xff,		// width of address word
	0xff, 0xff,		// width of data word
	0x0f, 0x0f,		// width of ID
	0x0f, 0x0f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x27, 0x00,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x00, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x00, 0x00,		// address of configuration bits
	0x07,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x01, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x0e, 0x0f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC18F248[] =
{
	0x27, 0x00, 0x0f, 0x0f,
	0x00, 0x00, 0x00, 0x85,
	0xc0, 0x03, 0xe0, 0x03,
	0x40, 0x03, 0x00, 0x00,
	0x27, 0x00, 0x0f, 0x0f,
	0x00, 0x00, 0x00, 0x85,
	0xc0, 0x03, 0xe0, 0x03,
	0x40, 0x03, 0x00, 0x00,
};

const static PIC_DEFINITION PIC18F248 =
{
	"18F248",			// device name
	def_PIC18F248,		// definition
	defx_PIC18F248,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	4,						// Word alignment for writing to this device
	0x300000,			// Configuration memory start address
	0x200000, 0,		// ID Locations addr
	0xF00000,			// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC18C252[] =
{
	0x40, 0x00,		// size of program space
	0xff, 0xff,		// width of address word
	0xff, 0xff,		// width of data word
	0x0f, 0x0f,		// width of ID
	0x0f, 0x0f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x27, 0xff,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x00, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x00, 0x00,		// address of configuration bits
	0x04,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x0b, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC18C252[] =
{
	0x27, 0xff, 0x0f, 0x0f,
	0x01, 0x00, 0x00, 0x01,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x27, 0xff, 0x0f, 0x0f,
	0x01, 0x00, 0x00, 0x01,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC18C252 =
{
	"18C252",			// device name
	def_PIC18C252,		// definition
	defx_PIC18C252,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC18F252[] =
{
	0x40, 0x00,		// size of program space
	0xff, 0xff,		// width of address word
	0xff, 0xff,		// width of data word
	0x0f, 0x0f,		// width of ID
	0x0f, 0x0f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x27, 0x00,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x00, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x00, 0x00,		// address of configuration bits
	0x07,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x01, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x0e, 0x0f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC18F252[] =
{
	0x27, 0x00, 0x0f, 0x0f,
	0x01, 0x00, 0x00, 0x85,
	0xc0, 0x0f, 0xe0, 0x0f,
	0x40, 0x0f, 0x00, 0x00,
	0x27, 0x00, 0x0f, 0x0f,
	0x01, 0x00, 0x00, 0x85,
	0xc0, 0x0f, 0xe0, 0x0f,
	0x40, 0x0f, 0x00, 0x00,
};

const static PIC_DEFINITION PIC18F252 =
{
	"18F252",			// device name
	def_PIC18F252,		// definition
	defx_PIC18F252,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	4,						// Word alignment for writing to this device
	0x300000,			// Configuration memory start address
	0x200000, 0,		// ID Locations addr
	0xF00000,			// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC18F258[] =
{
	0x40, 0x00,		// size of program space
	0xff, 0xff,		// width of address word
	0xff, 0xff,		// width of data word
	0x0f, 0x0f,		// width of ID
	0x0f, 0x0f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x27, 0x00,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x00, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x00, 0x00,		// address of configuration bits
	0x07,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x01, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x0e, 0x0f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC18F258[] =
{
	0x27, 0x00, 0x0f, 0x0f,
	0x00, 0x00, 0x00, 0x85,
	0xc0, 0x0f, 0xe0, 0x0f,
	0x40, 0x0f, 0x00, 0x00,
	0x27, 0x00, 0x0f, 0x0f,
	0x00, 0x00, 0x00, 0x85,
	0xc0, 0x0f, 0xe0, 0x0f,
	0x40, 0x0f, 0x00, 0x00,
};

const static PIC_DEFINITION PIC18F258 =
{
	"18F258",			// device name
	def_PIC18F258,		// definition
	defx_PIC18F258,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	4,						// Word alignment for writing to this device
	0x300000,			// Configuration memory start address
	0x200000, 0,		// ID Locations addr
	0xF00000,			// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC18C442[] =
{
	0x20, 0x00,		// size of program space
	0xff, 0xff,		// width of address word
	0xff, 0xff,		// width of data word
	0x0f, 0x0f,		// width of ID
	0x0f, 0x0f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x27, 0xff,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x00, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x00, 0x00,		// address of configuration bits
	0x04,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x0b, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC18C442[] =
{
	0x27, 0xff, 0x0f, 0x0f,
	0x01, 0x00, 0x00, 0x01,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x27, 0xff, 0x0f, 0x0f,
	0x01, 0x00, 0x00, 0x01,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC18C442 =
{
	"18C442",			// device name
	def_PIC18C442,		// definition
	defx_PIC18C442,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC18F442[] =
{
	0x20, 0x00,		// size of program space
	0xff, 0xff,		// width of address word
	0xff, 0xff,		// width of data word
	0x0f, 0x0f,		// width of ID
	0x0f, 0x0f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x27, 0x00,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x00, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x00, 0x00,		// address of configuration bits
	0x07,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x01, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x0e, 0x0f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC18F442[] =
{
	0x27, 0x00, 0x0f, 0x0f,
	0x01, 0x00, 0x00, 0x85,
	0xc0, 0x03, 0xe0, 0x03,
	0x40, 0x03, 0x00, 0x00,
	0x27, 0x00, 0x0f, 0x0f,
	0x01, 0x00, 0x00, 0x85,
	0xc0, 0x03, 0xe0, 0x03,
	0x40, 0x03, 0x00, 0x00,
};

const static PIC_DEFINITION PIC18F442 =
{
	"18F442",			// device name
	def_PIC18F442,		// definition
	defx_PIC18F442,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	4,						// Word alignment for writing to this device
	0x300000,			// Configuration memory start address
	0x200000, 0,		// ID Locations addr
	0xF00000,			// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC18F4431[] =
{
	0x20, 0x00,			// size of program space
	0xff, 0xff,			// width of address word
	0xff, 0xff,			// width of data word
	0x0f, 0x0f,			// width of ID
	0x0f, 0x0f,			// ID mask
	0x3f, 0xff,			// width of configuration word
	0x27, 0xff,			// configuration word mask
	0x00, 0xff,			// EEPROM data width
	0x00, 0xff,			// EEPROM data mask
	0x00, 0x00,			// Calibration width
	0x00, 0x00,			// Calibration mask
	0x00, 0x00,			// ??
	0x00, 0x01,			// ??
	0x00, 0x00,			// address of ID locations
	0x04,					// size of ID locations
	0x00, 0x00,			// address of configuration bits
	0x07,					// size of configuration register
	0x00, 0x00,			// address of data space
	0x01, 0x00,			// size of data space
	0x00, 0x00,			// address of internal clock calibration value
	0x00, 0x00,			// size of clock calibration space
	0x03,					// additional programming pulses for C devices
	0x19,					// main programming pulses for C devices
	0x0e, 0x0f,			// ?? ZIF configuration ??
};

const static unsigned char defx_PIC18F4431[] =
{
	0xcf, 0x00, 0x3f, 0x0f,
	0x9d, 0x3c, 0x00, 0x85,
	0xc0, 0x0f, 0xe0, 0x0f,
	0x40, 0x0f, 0x00, 0x00,
	0xcf, 0x00, 0x3f, 0x0f,
	0x9d, 0x3c, 0x00, 0x85,
	0xc0, 0x0f, 0xe0, 0x0f,
	0x40, 0x0f, 0x00, 0x00,
};

const static PIC_DEFINITION PIC18F4431 =
{
	"18F4431",			// device name
	def_PIC18F4431,	// definition
	defx_PIC18F4431,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	4,						// Word alignment for writing to this device
	0x300000,			// Configuration memory start address
	0x200000, 0,		// ID Locations addr
	0xF00000,			// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC18F448[] =
{
	0x20, 0x00,		// size of program space
	0xff, 0xff,		// width of address word
	0xff, 0xff,		// width of data word
	0x0f, 0x0f,		// width of ID
	0x0f, 0x0f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x27, 0x00,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x00, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x00, 0x00,		// address of configuration bits
	0x07,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x01, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x0e, 0x0f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC18F448[] =
{
	0x27, 0x00, 0x0f, 0x0f,
	0x00, 0x00, 0x00, 0x85,
	0xc0, 0x03, 0xe0, 0x03,
	0x40, 0x03, 0x00, 0x00,
	0x27, 0x00, 0x0f, 0x0f,
	0x00, 0x00, 0x00, 0x85,
	0xc0, 0x03, 0xe0, 0x03,
	0x40, 0x03, 0x00, 0x00,
};

const static PIC_DEFINITION PIC18F448 =
{
	"18F448",			// device name
	def_PIC18F448,		// definition
	defx_PIC18F448,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	4,						// Word alignment for writing to this device
	0x300000,			// Configuration memory start address
	0x200000, 0,		// ID Locations addr
	0xF00000,			// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC18C452[] =
{
	0x40, 0x00,		// size of program space
	0xff, 0xff,		// width of address word
	0xff, 0xff,		// width of data word
	0x0f, 0x0f,		// width of ID
	0x0f, 0x0f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x27, 0xff,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x00, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x00, 0x00,		// address of configuration bits
	0x04,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x0b, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC18C452[] =
{
	0x27, 0xff, 0x0f, 0x0f,
	0x01, 0x00, 0x00, 0x01,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x27, 0xff, 0x0f, 0x0f,
	0x01, 0x00, 0x00, 0x01,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC18C452 =
{
	"18C452",			// device name
	def_PIC18C452,		// definition
	defx_PIC18C452,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC18F452[] =
{
	0x40, 0x00,		// size of program space
	0xff, 0xff,		// width of address word
	0xff, 0xff,		// width of data word
	0x0f, 0x0f,		// width of ID
	0x0f, 0x0f,		// ID mask
	0x27, 0x00,		// width of configuration word
	0x27, 0x00,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x40, 0x00,		// ??
	0x00, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x00, 0x00,		// address of configuration bits
	0x07,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x01, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x0e, 0x0f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC18F452[] =
{
	0x27, 0x00, 0x0f, 0x0f,
	0x01, 0x00, 0x00, 0x85,
	0xc0, 0x0f, 0xe0, 0x0f,
	0x40, 0x0f, 0x00, 0x00,
	0x27, 0x00, 0x0f, 0x0f,
	0x01, 0x00, 0x00, 0x85,
	0xc0, 0x0f, 0xe0, 0x0f,
	0x40, 0x0f, 0x00, 0x00,
};

const static PIC_DEFINITION PIC18F452 =
{
	"18F452",			// device name
	def_PIC18F452,		// definition
	defx_PIC18F452,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	4,						// Word alignment for writing to this device
	0x300000,			// Configuration memory start address
	0x200000, 0,		// ID Locations addr
	0xF00000,			// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC18F4550[] =
{
	0x40, 0x00,		// size of program space
	0xff, 0xff,		// width of address word
	0xff, 0xff,		// width of data word
	0x0f, 0x0f,		// width of ID
	0x0f, 0x0f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x27, 0xff,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x00, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x00, 0x00,		// address of configuration bits
	0x07,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x01, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x0e, 0x0f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC18F4550[] =
{
	0xcf, 0x3f, 0x1f, 0x3f,
	0x83, 0x00, 0x00, 0x85,
	0xc0, 0x0f, 0xe0, 0x0f,
	0x40, 0x0f, 0x00, 0x00,

	0xcf, 0x3f, 0x1f, 0x3f,
	0x87, 0x00, 0x00, 0xe5,
	0xc0, 0x0f, 0xe0, 0x0f,
	0x40, 0x0f, 0x00, 0x00,
};

const static PIC_DEFINITION PIC18F4550 =
{
	"18F4550",			// device name
	def_PIC18F4550,	// definition
	defx_PIC18F4550,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	4,						// Word alignment for writing to this device
	0x300000,			// Configuration memory start address
	0x200000, 0,		// ID Locations addr
	0xF00000,			// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_WARP13),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC18F458[] =
{
	0x40, 0x00,		// size of program space
	0xff, 0xff,		// width of address word
	0xff, 0xff,		// width of data word
	0x0f, 0x0f,		// width of ID
	0x0f, 0x0f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x27, 0x00,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x00, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x00, 0x00,		// address of configuration bits
	0x07,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x01, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x0e, 0x0f,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC18F458[] =
{
	0x27, 0x00, 0x0f, 0x0f,
	0x00, 0x00, 0x00, 0x85,
	0xc0, 0x0f, 0xe0, 0x0f,
	0x40, 0x0f, 0x00, 0x00,
	0x27, 0x00, 0x0f, 0x0f,
	0x00, 0x00, 0x00, 0x85,
	0xc0, 0x0f, 0xe0, 0x0f,
	0x40, 0x0f, 0x00, 0x00,
};

const static PIC_DEFINITION PIC18F458 =
{
	"18F458",			// device name
	def_PIC18F458,		// definition
	defx_PIC18F458,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	4,						// Word alignment for writing to this device
	0x300000,			// Configuration memory start address
	0x200000, 0,		// ID Locations addr
	0xF00000,			// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13 | P_JUPIC),	// bit map of supporting programmers
};

//-----------------------------------------------------------
static const unsigned char def_PIC18F6520[] =
{
	0x40, 0x00,		// size of program space
	0xff, 0xff,		// width of address word
	0xff, 0xff,		// width of data word
	0x0f, 0x0f,		// width of ID
	0x0f, 0x0f,		// ID mask
	0x27, 0x00,		// width of configuration word
	0x27, 0x00,		// configuration word mask
	0x00, 0xff,		// EEPROM data width
	0x00, 0xff,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x00, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x00, 0x00,		// address of configuration bits
	0x07,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x01, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x0e, 0x0f,		// ?? ZIF configuration ??
};

static const unsigned char defx_PIC18F6520[] =
{
	0x27, 0x00, 0x0f, 0x0f,
	0x01, 0x00, 0x00, 0x85,
	0xc0, 0x0f, 0xe0, 0x0f,
	0x40, 0x0f, 0x00, 0x00,
	0x27, 0x00, 0x0f, 0x0f,
	0x01, 0x00, 0x00, 0x85,
	0xc0, 0x0f, 0xe0, 0x0f,
	0x40, 0x0f, 0x00, 0x00,
};

static const PIC_DEFINITION PIC18F6520 =
{
	"18F6520",			// device name
	def_PIC18F6520,	// definition
	defx_PIC18F6520,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	4,						// Word alignment for writing to this device
	0x300000,			// Configuration memory start address
	0x200000, 0,		// ID Locations addr & size
	0xF00000,			// EEprom address & size
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART | P_WARP13),	// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC18C658[] =
{
	0x40, 0x00,		// size of program space
	0xff, 0xff,		// width of address word
	0xff, 0xff,		// width of data word
	0x0f, 0x0f,		// width of ID
	0x0f, 0x0f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x27, 0xff,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x00, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x00, 0x00,		// address of configuration bits
	0x04,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x0b, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC18C658[] =
{
	0x27, 0xff, 0x0f, 0x0f,
	0x00, 0x00, 0x00, 0x01,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x27, 0xff, 0x0f, 0x0f,
	0x00, 0x00, 0x00, 0x01,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC18C658 =
{
	"18C658",			// device name
	def_PIC18C658,		// definition
	defx_PIC18C658,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART),		// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PIC18C858[] =
{
	0x40, 0x00,		// size of program space
	0xff, 0xff,		// width of address word
	0xff, 0xff,		// width of data word
	0x0f, 0x0f,		// width of ID
	0x0f, 0x0f,		// ID mask
	0x3f, 0xff,		// width of configuration word
	0x27, 0xff,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x00, 0x00,		// Calibration width
	0x00, 0x00,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x00, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x00, 0x00,		// address of configuration bits
	0x04,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x00, 0x00,		// address of internal clock calibration value
	0x00, 0x00,		// size of clock calibration space
	0x03,				// additional programming pulses for C devices
	0x19,				// main programming pulses for C devices
	0x0b, 0x06,		// ?? ZIF configuration ??
};

const static unsigned char defx_PIC18C858[] =
{
	0x27, 0xff, 0x0f, 0x0f,
	0x00, 0x00, 0x00, 0x01,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x27, 0xff, 0x0f, 0x0f,
	0x00, 0x00, 0x00, 0x01,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PIC18C858 =
{
	"18C858",			// device name
	def_PIC18C858,		// definition
	defx_PIC18C858,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART),		// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PICrf509AF[] =
{
	0x03, 0xff,		// size of program space
	0x0f, 0xff,		// width of address word
	0x0f, 0xff,		// width of data word
	0x00, 0x0f,		// width of ID
	0x00, 0x0f,		// ID mask
	0x00, 0x1f,		// width of configuration word
	0x00, 0x1f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x0f, 0xff,		// Calibration width
	0x0f, 0xff,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x04, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x0f, 0xff,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x03, 0xff,		// address of internal clock calibration value
	0x00, 0x01,		// size of clock calibration space
	0x0b,				// additional programming pulses for C devices
	0x08,				// main programming pulses for C devices
	0x0a, 0x16,		// ?? ZIF configuration ??
};

const static unsigned char defx_PICrf509AF[] =
{
	0x00, 0x1f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x1f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PICrf509AF =
{
	"rf509AF",			// device name
	def_PICrf509AF,	// definition
	defx_PICrf509AF,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART),		// bit map of supporting programmers
};

//-----------------------------------------------------------
const static unsigned char def_PICrf509AG[] =
{
	0x03, 0xff,		// size of program space
	0x0f, 0xff,		// width of address word
	0x0f, 0xff,		// width of data word
	0x00, 0x0f,		// width of ID
	0x00, 0x0f,		// ID mask
	0x00, 0x1f,		// width of configuration word
	0x00, 0x1f,		// configuration word mask
	0x00, 0x00,		// EEPROM data width
	0x00, 0x00,		// EEPROM data mask
	0x0f, 0xff,		// Calibration width
	0x0f, 0xff,		// Calibration mask
	0x00, 0x00,		// ??
	0x00, 0x01,		// ??
	0x04, 0x00,		// address of ID locations
	0x04,				// size of ID locations
	0x0f, 0xff,		// address of configuration bits
	0x01,				// size of configuration register
	0x00, 0x00,		// address of data space
	0x00, 0x00,		// size of data space
	0x03, 0xff,		// address of internal clock calibration value
	0x00, 0x01,		// size of clock calibration space
	0x0b,				// additional programming pulses for C devices
	0x08,				// main programming pulses for C devices
	0x0a, 0x16,		// ?? ZIF configuration ??
};

const static unsigned char defx_PICrf509AG[] =
{
	0x00, 0x1f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x1f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const static PIC_DEFINITION PICrf509AG =
{
	"rf509AG",			// device name
	def_PICrf509AG,	// definition
	defx_PICrf509AG,	// extended definition
	0,						// config word: code protect bit mask
	0,						// config word: watchdog bit mask

	0,						// Word alignment for writing to this device
	0,						// Configuration memory start address
	0, 0,					// ID Locations addr
	0,						// Data eeprom address
	0,						// number of words in cfg bits with factory set bits
	{0, 0, 0, 0, 0, 0, 0, 0},	// fixed bits mask
	(P_PICSTART),		// bit map of supporting programmers
};

//----------------------------------------
// external access through this array only
const PIC_DEFINITION *deviceList[] =
{
	&PIC10F200,
	&PIC10F202,
	&PIC10F204,
	&PIC10F206,
	&PIC12C508,
	&PIC12C508A,
	&PIC12C509,
	&PIC12C509A,
	&PIC12CE518,
	&PIC12CE519,
	&PIC12F508,
	&PIC12F509,
	&PIC12F629,
	&PIC12C671,
	&PIC12C672,
	&PIC12CE673,
	&PIC12CE674,
	&PIC12F675,
	&PIC12F683,
	&PIC14000,
	&PIC16C505,
	&PIC16C52,
	&PIC16C54,
	&PIC16C54A,
	&PIC16C54B,
	&PIC16C54C,
	&PIC16HV540,
	&PIC16C55,
	&PIC16C55A,
	&PIC16C554,
	&PIC16C558,
	&PIC16C56,
	&PIC16C56A,
	&PIC16C57,
	&PIC16C57C,
	&PIC16C58A,
	&PIC16C58B,
	&PIC16C61,
	&PIC16C62,
	&PIC16C62A,
	&PIC16C62B,
	&PIC16C620,
	&PIC16C620A,
	&PIC16C621,
	&PIC16C621A,
	&PIC16C622,
	&PIC16C622A,
	&PIC16CE623,
	&PIC16CE624,
	&PIC16CE625,
	&PIC16F627,
	&PIC16F627A,
	&PIC16F628,
	&PIC16F628A,
	&PIC16C63,
	&PIC16C63A,
	&PIC16C63B,
	&PIC16F630,
	&PIC16C64,
	&PIC16C64A,
	&PIC16C642,
	&PIC16F648,
	&PIC16F648A,
	&PIC16C65,
	&PIC16C65A,
	&PIC16C65B,
	&PIC16C66,
	&PIC16C66A,
	&PIC16C662,
	&PIC16C67,
	&PIC16C67A,
	&PIC16F676,
	&PIC16C71,
	&PIC16C710,
	&PIC16C711,
	&PIC16C712,
	&PIC16C715,
	&PIC16C716,
	&PIC16C717,
	&PIC16C72,
	&PIC16C72A,
	&PIC16F72,
	&PIC16C73,
	&PIC16C73A,
	&PIC16C73B,
	&PIC16F73,
	&PIC16C74,
	&PIC16C74A,
	&PIC16C74B,
	&PIC16F74,
	&PIC16C745,
	&PIC16C76,
	&PIC16F76,
	&PIC16C765,
	&PIC16C77,
	&PIC16F77,
	&PIC16C770,
	&PIC16C771,
	&PIC16C773,
	&PIC16C774,
	&PIC16C781,
	&PIC16C782,
	&PIC16F818,
	&PIC16F819,
	&PIC16F83,
	&PIC16C84,
	&PIC16F84,
	&PIC16F84A,
	&PIC16F87,
	&PIC16F88,
	&PIC16F870,
	&PIC16F871,
	&PIC16F872,
	&PIC16F873,
	&PIC16F873A,
	&PIC16F874,
	&PIC16F874A,
	&PIC16F876,
	&PIC16F876A,
	&PIC16F877,
	&PIC16F877A,
	&PIC16C923,
	&PIC16C924,
	&PIC16C925,
	&PIC16C926,
	&PIC17C42,
	&PIC17C42A,
	&PIC17C43,
	&PIC17C44,
	&PIC17C752,
	&PIC17C756,
	&PIC17C756A,
	&PIC17C762,
	&PIC17C766,
	&PIC18C242,
	&PIC18F242,
	&PIC18F248,
	&PIC18C252,
	&PIC18F252,
	&PIC18F258,
	&PIC18C442,
	&PIC18F442,
	&PIC18F4431,
	&PIC18F448,
	&PIC18C452,
	&PIC18F452,
	&PIC18F4550,
	&PIC18F458,
	&PIC18F6520,
	&PIC18C658,
	&PIC18C858,
	&PICrf509AF,
	&PICrf509AG,
	NULL			// end of list
};
