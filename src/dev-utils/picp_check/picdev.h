//-----------------------------------------------------------------------------
//
//	PICSTART Plus programming interface
//
//-----------------------------------------------------------------------------
//
//	Cosmodog, Ltd.
//	415 West Huron Street
//	Chicago, IL   60610
//	http://www.cosmodog.com
//
// Maintained at
// http://home.pacbell.net/theposts/picmicro
//
//-----------------------------------------------------------------------------

#ifndef __PICDEV_H_
#define __PICDEV_H_

// Chip support for various programmers.
// These bits specify if a given chip is supported by a given programmer.

#define	P_PICSTART		1
#define	P_WARP13			2
#define	P_JUPIC			4
#define	P_OLIMEX			8	// non yet supported by picp - no way to test

// indices into processor definition arrays
//
#define	PD_PGM_SIZEH	0		// size of the program space, in words
#define	PD_PGM_SIZEL	1
#define	PD_PGM_WIDTHH	2		// width of address word
#define	PD_PGM_WIDTHL	3
#define	PD_DATA_WIDTHH	4		// width of data word
#define	PD_DATA_WIDTHL	5

// PD_ID_ info provided by Adam Jurkiewicz

#define	PD_ID_WIDTHH	6
#define	PD_ID_WIDTHL	7
#define	PD_ID_MASKH		8
#define	PD_ID_MASKL		9

// One of these words is a bit mask for the configuration bits.
// It is not known at this time which is correct--the two words
// always have the same value.
// Until I can determine which is which, the first word will be
// deemed to define the width of the config word, and the
// second will be deemed the mask.
// 15 Apr 2004 - JLP
#define	PD_CFG_WIDTHH	10
#define	PD_CFG_WIDTHL	11
#define	PD_CFG_MASKH	12
#define	PD_CFG_MASKL	13

// PD_EE_ info provided by Adam Jurkiewicz

#define	PD_EE_WIDTHH	14
#define	PD_EE_WIDTHL	15
#define	PD_EE_MASKH		16
#define	PD_EE_MASKL		17

// PD_CAL_ info provided by Adam Jurkiewicz

#define	PD_CAL_WIDTHH	18
#define	PD_CAL_WIDTHL	19
#define	PD_CAL_MASKH	20
#define	PD_CAL_MASKL	21

#define	PD_ID_ADDRH		26		// Address of ID Locations Area
#define	PD_ID_ADDRL		27
#define	PD_ID_SIZE		28		// Size of ID Locations Area

#define	PD_CFG_ADDRH	29		// address of the configuration bits
#define	PD_CFG_ADDRL	30
#define	PD_CFG_SIZE		31		// size of config register
#define	PD_DATA_ADDRH	32		// address of data space
#define	PD_DATA_ADDRL	33
#define	PD_DATA_SIZEH	34		// EEPROM data space size, in bytes
#define	PD_DATA_SIZEL	35
#define	PD_CLK_ADDRH	36		// address of clock calibration space
#define	PD_CLK_ADDRL	37
#define	PD_CLK_SIZEH	38		// size of clock calibration space, in words
#define	PD_CLK_SIZEL	39

// PD_xx_PGM_PLUSES info provided by Adam Jurkiewicz

#define	PD_ADD_PGM_PULSES		40	// additional programming pulses for C devices
#define	PD_MAIN_PGM_PULSES	41	// main programming pulses for C devices

// 42-43: may be configuration for ZIF socket pins
// per Adam Jurkiewicz.

#define	PD_ZIF_CFG_HI		42
#define	PD_ZIF_CFG_LO		43

#define	PICDEV_DEFSIZE		44		// profile is 44 bytes long
#define	PICDEV_DEFXSIZE	32		// extended profile is 32 bytes long

typedef struct
{
	const char *name;						// name of the device
	const unsigned char *def;			// definition (initialization) (always 44 bytes long)
	const unsigned char *defx;			// extended definition (initialization) (always 16 bytes long)
	const unsigned short int cpbits;	// set of code protection bits in configuration word (0 = read protected)
	const unsigned short int wdbit;	// watchdog enable bit in configuration word (1 = enabled)

	// Additions below by Alexandre Nunes
	// If set, these values override those guessed from the header.
	const unsigned short int wordalign;	// Word alignment for writing to this device.
	const unsigned int cfgmem;		// Configuration Memory Start addr
	const unsigned int idaddr;		// Address of ID Locations area
	const unsigned short int idsize;
	const unsigned int eeaddr;		// Data EEPROM address
	// Added by J. Post to handle factory set bits in config register (eg: band gap info)
	const unsigned char fixedCfgBitsSize;		// number of words (1-8, 0 = no fixed bits)
	const unsigned short int fixedCfgBits[8];	// up to 8 words of config bits
	const unsigned short int pgm_support;	// bit map of supporting programmers
	// More to come.
} PIC_DEFINITION;

// return codes from blank check
//
#define BLANK_PGM		0x01		// 0 = program memory blank
#define BLANK_CFG		0x02		// 0 = configuration bits blank
#define BLANK_ID		0x04		// 0 = ID locations blank
#define BLANK_DATA	0x08		// 0 = EEPROM data blank

extern const PIC_DEFINITION *deviceList[];

#endif // defined __PICDEV_H_
