/* MM_NES_Extract - Extract data files from NES version of Maniac Mansion
 * Copyright (C) 2003  The ScummVM Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Header$
 *
 */

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

typedef int BOOL;
#define TRUE 1
#define FALSE 0

/* if defined, generates a set of .LFL files */
/* if not defined, dumps all resources to separate files */
#define	MAKE_LFLS

#ifdef WIN32
	#define CDECL __cdecl
#else
	#define CDECL 
#endif

void CDECL debug (const char *Text, ...)
{
	va_list marker;
	va_start(marker,Text);
	vfprintf(stdout,Text,marker);
	fprintf(stdout,"\n");
	va_end(marker);
}

void CDECL error (const char *Text, ...)
{
	va_list marker;
	va_start(marker,Text);
	vfprintf(stderr,Text,marker);
	fprintf(stderr,"\n");
	va_end(marker);
	exit(1);
}

void CDECL _assert (BOOL condition, const char *Text, ...)
{
	va_list marker;
	if (condition)
		return;
	va_start(marker,Text);
	vfprintf(stderr,Text,marker);
	fprintf(stderr,"\n");
	va_end(marker);
	exit(1);
}

unsigned char	read_byte (FILE *input)
{
	unsigned char val;
	_assert(fread(&val,1,1,input) == 1,"read_byte - unexpected EOF");
	return val;
}
unsigned short	read_word (FILE *input)
{
	unsigned short val;
	_assert(fread(&val,2,1,input) == 1,"read_word - unexpected EOF");
	return val;
}
void	write_byte (FILE *output, unsigned char val)
{
#ifdef	MAKE_LFLS
	val ^= 0xFF;
#endif
	fwrite(&val,1,1,output);
}
void	write_word (FILE *output, unsigned short val)
{
#ifdef	MAKE_LFLS
	val ^= 0xFFFF;
#endif
	fwrite(&val,2,1,output);
}

typedef enum _res_type { RES_UNKNOWN, RES_GLOBDATA, RES_ROOM, RES_SCRIPT, RES_SOUND, RES_COSTUME, RES_ROOMGFX, RES_COSTUMEGFX, RES_PALETTE, RES_UNK_DD75, RES_UNK_DD7D, RES_UNK_DD95, RES_UNK_DD9D } res_type;

typedef	enum _romset { ROMSET_USA, ROMSET_EUROPE, ROMSET_SWEDEN, NUM_ROMSETS } t_romset;

t_romset ROMset = NUM_ROMSETS;

typedef	struct	_resource
{
	unsigned long offset[NUM_ROMSETS];
	unsigned short length[NUM_ROMSETS];
	res_type type;
}	t_resource, *p_resource;
t_resource res_roomgfx[40] = {
	{ {0x04001,0x04001,0x04001}, {0x03C9,0x03B9,0x03F0}, RES_ROOMGFX },
	{ {0x043CA,0x043BA,0x043F1}, {0x069E,0x069E,0x069E}, RES_ROOMGFX },
	{ {0x04A68,0x04A58,0x04A8F}, {0x0327,0x0327,0x0327}, RES_ROOMGFX },
	{ {0x04D8F,0x04D7F,0x04DB6}, {0x053B,0x053B,0x053B}, RES_ROOMGFX },
	{ {0x052CA,0x052BA,0x052F1}, {0x06BE,0x06BE,0x06BE}, RES_ROOMGFX },
	{ {0x05988,0x05978,0x059AF}, {0x0682,0x0682,0x0682}, RES_ROOMGFX },
	{ {0x0600A,0x05FFA,0x06031}, {0x0778,0x0778,0x0778}, RES_ROOMGFX },
	{ {0x06782,0x06772,0x067A9}, {0x0517,0x0517,0x0517}, RES_ROOMGFX },
	{ {0x06C99,0x06C89,0x06CC0}, {0x07FB,0x07FB,0x07FB}, RES_ROOMGFX },
	{ {0x07494,0x07484,0x074BB}, {0x07BE,0x07BE,0x07BE}, RES_ROOMGFX },
	{ {0x08001,0x08001,0x08001}, {0x07A5,0x07A5,0x07A5}, RES_ROOMGFX },
	{ {0x087A6,0x087A6,0x087A6}, {0x06DD,0x06DD,0x06DD}, RES_ROOMGFX },
	{ {0x08E83,0x08E83,0x08E83}, {0x04EA,0x04EA,0x04EA}, RES_ROOMGFX },
	{ {0x0936D,0x0936D,0x0936D}, {0x0846,0x0846,0x07E2}, RES_ROOMGFX },
	{ {0x09BB3,0x09BB3,0x09B4F}, {0x08C8,0x08C8,0x0791}, RES_ROOMGFX },
	{ {0x0A47B,0x0A47B,0x0A2E0}, {0x0844,0x0844,0x07B5}, RES_ROOMGFX },
	{ {0x0ACBF,0x0ACBF,0x0AA95}, {0x0515,0x0515,0x0515}, RES_ROOMGFX },
	{ {0x0B1D4,0x0B1D4,0x0AFAA}, {0x0799,0x0799,0x0799}, RES_ROOMGFX },
	{ {0x0B96D,0x0B96D,0x0B743}, {0x04BB,0x04BB,0x04BF}, RES_ROOMGFX },
	{ {0x07C52,0x07C42,0x0BC02}, {0x0319,0x0319,0x0319}, RES_ROOMGFX },
	{ {0x0C001,0x0C001,0x0C001}, {0x0464,0x0464,0x0464}, RES_ROOMGFX },
	{ {0x0C465,0x0C465,0x0C465}, {0x076D,0x076D,0x072C}, RES_ROOMGFX },
	{ {0x0CBD2,0x0CBD2,0x0CB91}, {0x0827,0x0827,0x0827}, RES_ROOMGFX },
	{ {0x0D3F9,0x0D3F9,0x0D3B8}, {0x0515,0x0515,0x0515}, RES_ROOMGFX },
	{ {0x0D90E,0x0D90E,0x0D8CD}, {0x064E,0x064E,0x064E}, RES_ROOMGFX },
	{ {0x0DF5C,0x0DF5C,0x0DF1B}, {0x0775,0x0775,0x0775}, RES_ROOMGFX },
	{ {0x0E6D1,0x0E6D1,0x0E690}, {0x06DD,0x06DD,0x06DD}, RES_ROOMGFX },
	{ {0x0EDAE,0x0EDAE,0x0ED6D}, {0x0376,0x0376,0x0376}, RES_ROOMGFX },
	{ {0x0F124,0x0F124,0x0F0E3}, {0x05F7,0x05F7,0x05F7}, RES_ROOMGFX },
	{ {0x0F71B,0x0F71B,0x0F6DA}, {0x0787,0x0787,0x0791}, RES_ROOMGFX },
	{ {0x10001,0x10001,0x07C79}, {0x02D6,0x02D6,0x02D6}, RES_ROOMGFX },
	{ {0x102D7,0x102D7,0x10001}, {0x06A3,0x06A3,0x06A3}, RES_ROOMGFX },
	{ {0x1097A,0x1097A,0x106A4}, {0x099F,0x099F,0x0921}, RES_ROOMGFX },
	{ {0x11319,0x11319,0x10FC5}, {0x0361,0x0361,0x0361}, RES_ROOMGFX },
	{ {0x1167A,0x1167A,0x11326}, {0x0489,0x0489,0x0489}, RES_ROOMGFX },
	{ {0x11B03,0x11B03,0x117AF}, {0x0437,0x0437,0x0437}, RES_ROOMGFX },
	{ {0x11F3A,0x11F3A,0x11BE6}, {0x084D,0x084D,0x084F}, RES_ROOMGFX },
	{ {0x0BE28,0x12787,0x12435}, {0x0199,0x0199,0x0199}, RES_ROOMGFX },
	{ {0x12787,0x12920,0x125CE}, {0x09A7,0x09A7,0x0947}, RES_ROOMGFX },
	{ {0x1312E,0x132C7,0x12F15}, {0x037A,0x037A,0x037A}, RES_ROOMGFX }
};
t_resource res_costumegfx[2] = {
	{ {0x30001,0x30001,0x2EFE1}, {0x0EC9,0x0EC9,0x0EC9}, RES_COSTUMEGFX },
	{ {0x2F9F1,0x2F9F1,0x30001}, {0x0351,0x0351,0x0351}, RES_COSTUMEGFX }
};
t_resource res_rooms[55] = {
	{ {0x00000,0x00000,0x00000}, {0x0000,0x0000,0x0000}, RES_ROOM },
	{ {0x14001,0x14001,0x14001}, {0x0D0C,0x0D0C,0x0D12}, RES_ROOM },
	{ {0x134A8,0x13641,0x1328F}, {0x04B3,0x04B3,0x04B3}, RES_ROOM },
	{ {0x15397,0x15397,0x15367}, {0x0849,0x0849,0x0859}, RES_ROOM },
	{ {0x15C68,0x15C68,0x13742}, {0x0685,0x0685,0x0694}, RES_ROOM },
	{ {0x16381,0x16381,0x15C45}, {0x0715,0x0715,0x0707}, RES_ROOM },
	{ {0x1395B,0x16CE8,0x1658F}, {0x04E7,0x04E7,0x04E0}, RES_ROOM },
	{ {0x16CE8,0x18001,0x16A6F}, {0x0AC0,0x0ABF,0x0AC8}, RES_ROOM },
	{ {0x18001,0x171CF,0x18001}, {0x06BA,0x06BA,0x06C7}, RES_ROOM },
	{ {0x17AED,0x13AF4,0x1789C}, {0x03CB,0x03D2,0x03EA}, RES_ROOM },
	{ {0x18BE7,0x18E1A,0x18C09}, {0x0663,0x0663,0x0649}, RES_ROOM },
	{ {0x192A6,0x194D9,0x192AE}, {0x0580,0x04A9,0x04AB}, RES_ROOM },
	{ {0x19A44,0x19BA0,0x19982}, {0x0443,0x0443,0x0447}, RES_ROOM },
	{ {0x1A106,0x1A262,0x1A04D}, {0x0563,0x047C,0x047E}, RES_ROOM },
	{ {0x1A669,0x1A6DE,0x1A4CB}, {0x0446,0x0446,0x0444}, RES_ROOM },
	{ {0x1AAAF,0x1AB24,0x1A90F}, {0x03A7,0x03A7,0x03B9}, RES_ROOM },
	{ {0x1AE56,0x1AECB,0x1ACC8}, {0x07E3,0x07E3,0x07E9}, RES_ROOM },
	{ {0x1B699,0x1B70E,0x1B511}, {0x0692,0x0692,0x06A4}, RES_ROOM },
	{ {0x1C001,0x1C001,0x1C001}, {0x0B49,0x0ACA,0x0B1A}, RES_ROOM },
	{ {0x1CD09,0x1CC8A,0x1CCFD}, {0x04C6,0x04C6,0x0486}, RES_ROOM },
	{ {0x1D4C2,0x1D443,0x1D482}, {0x0568,0x0568,0x0579}, RES_ROOM },
	{ {0x1DF6C,0x1DEED,0x1DF61}, {0x0514,0x0514,0x051E}, RES_ROOM },
	{ {0x1E8FA,0x1E87B,0x1E8EC}, {0x05CC,0x05CC,0x05CF}, RES_ROOM },
	{ {0x1EF83,0x1EF04,0x1EF73}, {0x0389,0x0389,0x0398}, RES_ROOM },
	{ {0x1F5E4,0x1F565,0x1F5F0}, {0x0723,0x0723,0x071A}, RES_ROOM },
	{ {0x20001,0x20001,0x20001}, {0x049A,0x049A,0x049C}, RES_ROOM },
	{ {0x20511,0x20511,0x2051E}, {0x04F8,0x04F8,0x051E}, RES_ROOM },
	{ {0x21666,0x21666,0x21725}, {0x05CB,0x05D5,0x05D5}, RES_ROOM },
	{ {0x21DD6,0x21DE0,0x21EA5}, {0x046B,0x046B,0x047F}, RES_ROOM },
	{ {0x222F0,0x222FA,0x223D1}, {0x0460,0x0460,0x0460}, RES_ROOM },
	{ {0x227B6,0x227C0,0x22897}, {0x0909,0x0909,0x090D}, RES_ROOM },
	{ {0x24001,0x24001,0x24001}, {0x0366,0x0366,0x0378}, RES_ROOM },
	{ {0x23BDF,0x247DB,0x247C9}, {0x03CA,0x03CA,0x03CA}, RES_ROOM },
	{ {0x247DB,0x24BA5,0x24B93}, {0x050D,0x050D,0x050D}, RES_ROOM },
	{ {0x25ACF,0x23BE9,0x25267}, {0x0346,0x0346,0x0346}, RES_ROOM },
	{ {0x1BDBD,0x17DB5,0x17CD0}, {0x01CA,0x01CA,0x01CA}, RES_ROOM },
	{ {0x25E15,0x25E99,0x255AD}, {0x0457,0x0457,0x0453}, RES_ROOM },
	{ {0x2626C,0x262F0,0x25A00}, {0x0547,0x0547,0x053E}, RES_ROOM },
	{ {0x267B3,0x26837,0x25F3E}, {0x064A,0x064A,0x0647}, RES_ROOM },
	{ {0x1FD72,0x1FCF3,0x1BC49}, {0x024B,0x024B,0x024B}, RES_ROOM },
	{ {0x2739A,0x2741E,0x26B58}, {0x01FA,0x01FA,0x01FA}, RES_ROOM },
	{ {0x2766D,0x276F1,0x26E27}, {0x0219,0x0219,0x0217}, RES_ROOM },
	{ {0x28001,0x28001,0x27345}, {0x02F4,0x02F4,0x02F4}, RES_ROOM },
	{ {0x284D6,0x284D6,0x27829}, {0x045C,0x045C,0x045C}, RES_ROOM },
	{ {0x289A3,0x289A3,0x28001}, {0x09CF,0x09CF,0x098A}, RES_ROOM },
	{ {0x293C6,0x293C6,0x289DF}, {0x05A0,0x05A0,0x05A1}, RES_ROOM },
	{ {0x27B65,0x27BE9,0x2A442}, {0x0201,0x0201,0x0201}, RES_ROOM },
	{ {0x2ADD1,0x2ADE3,0x2A6E9}, {0x0325,0x0325,0x0325}, RES_ROOM },
	{ {0x2B339,0x2B34B,0x1FD75}, {0x01FC,0x01FC,0x01FC}, RES_ROOM },
	{ {0x2B535,0x2B547,0x2AC64}, {0x02A9,0x02A9,0x02A9}, RES_ROOM },
	{ {0x2B7DE,0x2B7F0,0x2AF0D}, {0x02DE,0x02DE,0x02D1}, RES_ROOM },
	{ {0x2C001,0x2C001,0x2B2E6}, {0x03CE,0x03CE,0x03CC}, RES_ROOM },
	{ {0x2BBC0,0x2BBD2,0x23D61}, {0x0205,0x0205,0x0205}, RES_ROOM },
	{ {0x2C53A,0x2C53A,0x2B818}, {0x0170,0x0170,0x0168}, RES_ROOM },
	{ {0x13E42,0x2BDD7,0x27CF6}, {0x0169,0x0169,0x0169}, RES_ROOM }
};
t_resource res_scripts[179] = {
	{ {0x00000,0x00000,0x00000}, {0x0000,0x0000,0x0000}, RES_SCRIPT },
	{ {0x29966,0x29966,0x28F80}, {0x044D,0x044D,0x043B}, RES_SCRIPT },
	{ {0x29DB3,0x29DB3,0x293BB}, {0x0207,0x0207,0x0209}, RES_SCRIPT },
	{ {0x29FBA,0x29FBA,0x295C4}, {0x009F,0x009F,0x00AB}, RES_SCRIPT },
	{ {0x2A059,0x2A059,0x2966F}, {0x03F4,0x03F4,0x03FD}, RES_SCRIPT },
	{ {0x2A44D,0x2A44D,0x29A6C}, {0x01A1,0x01A1,0x01A1}, RES_SCRIPT },
	{ {0x00000,0x00000,0x00000}, {0x0000,0x0000,0x0000}, RES_SCRIPT },
	{ {0x2A5EE,0x2A5EE,0x29C0D}, {0x004A,0x005C,0x005C}, RES_SCRIPT },
	{ {0x00000,0x00000,0x00000}, {0x0000,0x0000,0x0000}, RES_SCRIPT },
	{ {0x2A638,0x2A64A,0x29C69}, {0x0005,0x0005,0x0005}, RES_SCRIPT },
	{ {0x2C6AA,0x2C6AA,0x2B980}, {0x000D,0x000D,0x000D}, RES_SCRIPT },
	{ {0x2C6B7,0x2C6B7,0x2B98D}, {0x000D,0x000D,0x000D}, RES_SCRIPT },
	{ {0x186BB,0x17889,0x186C8}, {0x0040,0x0040,0x0040}, RES_SCRIPT },
	{ {0x186FB,0x178C9,0x18708}, {0x0016,0x0016,0x0016}, RES_SCRIPT },
	{ {0x1B639,0x1B6AE,0x1B4B1}, {0x0046,0x0046,0x0046}, RES_SCRIPT },
	{ {0x1EEC6,0x1EE47,0x1EEBB}, {0x00BD,0x00BD,0x00B8}, RES_SCRIPT },
	{ {0x21C31,0x21C3B,0x21CFA}, {0x0055,0x0055,0x005C}, RES_SCRIPT },
	{ {0x177A8,0x18AC0,0x17537}, {0x0027,0x0027,0x0027}, RES_SCRIPT },
	{ {0x1FD07,0x1FC88,0x1FD0A}, {0x0027,0x0027,0x0027}, RES_SCRIPT },
	{ {0x1FD2E,0x1FCAF,0x1FD31}, {0x0027,0x0027,0x0027}, RES_SCRIPT },
	{ {0x1BD2B,0x1BDA0,0x1BBB5}, {0x0022,0x0022,0x0022}, RES_SCRIPT },
	{ {0x15BE0,0x15BE0,0x15BC0}, {0x0088,0x0088,0x0085}, RES_SCRIPT },
	{ {0x22241,0x2224B,0x22324}, {0x0020,0x0020,0x001E}, RES_SCRIPT },
	{ {0x22261,0x2226B,0x22342}, {0x008F,0x008F,0x008F}, RES_SCRIPT },
	{ {0x1924A,0x1947D,0x19252}, {0x002B,0x002B,0x002B}, RES_SCRIPT },
	{ {0x1CB4A,0x1CACB,0x1CB1B}, {0x0061,0x0061,0x006D}, RES_SCRIPT },
	{ {0x1CBAB,0x1CB2C,0x1CB88}, {0x003C,0x003C,0x004C}, RES_SCRIPT },
	{ {0x1CBE7,0x1CB68,0x1CBD4}, {0x0042,0x0042,0x0044}, RES_SCRIPT },
	{ {0x1CC29,0x1CBAA,0x1CC18}, {0x004F,0x004F,0x0053}, RES_SCRIPT },
	{ {0x2049B,0x2049B,0x2049D}, {0x0076,0x0076,0x0081}, RES_SCRIPT },
	{ {0x16A96,0x16A96,0x1634C}, {0x0035,0x0035,0x0035}, RES_SCRIPT },
	{ {0x16ACB,0x16ACB,0x16381}, {0x001C,0x001C,0x001C}, RES_SCRIPT },
	{ {0x16AE7,0x16AE7,0x1639D}, {0x0014,0x0014,0x0014}, RES_SCRIPT },
	{ {0x16AFB,0x16AFB,0x163B1}, {0x001C,0x001C,0x001C}, RES_SCRIPT },
	{ {0x16B17,0x16B17,0x163CD}, {0x0027,0x0027,0x0027}, RES_SCRIPT },
	{ {0x16B3E,0x16B3E,0x163F4}, {0x01AA,0x01AA,0x019B}, RES_SCRIPT },
	{ {0x1D1CF,0x1D150,0x1D183}, {0x0096,0x0096,0x0094}, RES_SCRIPT },
	{ {0x1D265,0x1D1E6,0x1D217}, {0x010E,0x010E,0x0117}, RES_SCRIPT },
	{ {0x1D373,0x1D2F4,0x1D32E}, {0x001C,0x001C,0x001C}, RES_SCRIPT },
	{ {0x1D38F,0x1D310,0x1D34A}, {0x0056,0x0056,0x0056}, RES_SCRIPT },
	{ {0x1D3E5,0x1D366,0x1D3A0}, {0x0072,0x0072,0x0072}, RES_SCRIPT },
	{ {0x1E480,0x1E401,0x1E47F}, {0x0028,0x0028,0x0028}, RES_SCRIPT },
	{ {0x1E4A8,0x1E429,0x1E4A7}, {0x017D,0x017D,0x0175}, RES_SCRIPT },
	{ {0x1E625,0x1E5A6,0x1E61C}, {0x0229,0x0229,0x022B}, RES_SCRIPT },
	{ {0x28932,0x28932,0x27C85}, {0x0071,0x0071,0x0071}, RES_SCRIPT },
	{ {0x17EB8,0x13EC6,0x17C86}, {0x004D,0x004D,0x004A}, RES_SCRIPT },
	{ {0x162ED,0x162ED,0x13DD6}, {0x0039,0x0039,0x0039}, RES_SCRIPT },
	{ {0x18711,0x178DF,0x1871E}, {0x028B,0x028B,0x0270}, RES_SCRIPT },
	{ {0x1899C,0x17B6A,0x1898E}, {0x00BB,0x00BB,0x00C0}, RES_SCRIPT },
	{ {0x18A57,0x17C25,0x18A4E}, {0x018B,0x018B,0x01B6}, RES_SCRIPT },
	{ {0x00000,0x00000,0x00000}, {0x0000,0x0000,0x0000}, RES_SCRIPT },
	{ {0x19E87,0x19FE3,0x19DC9}, {0x00ED,0x00ED,0x00EE}, RES_SCRIPT },
	{ {0x21C86,0x21C90,0x21D56}, {0x00F6,0x00F6,0x00F5}, RES_SCRIPT },
	{ {0x1E84E,0x1E7CF,0x1E847}, {0x009B,0x009B,0x0094}, RES_SCRIPT },
	{ {0x21D7C,0x21D86,0x21E4B}, {0x0047,0x0047,0x0047}, RES_SCRIPT },
	{ {0x2C6C4,0x2C6C4,0x2B99A}, {0x004D,0x004D,0x004D}, RES_SCRIPT },
	{ {0x16326,0x16326,0x13E0F}, {0x0024,0x0024,0x0024}, RES_SCRIPT },
	{ {0x14D0D,0x14D0D,0x14D13}, {0x0014,0x0014,0x0014}, RES_SCRIPT },
	{ {0x177CF,0x18AE7,0x1755E}, {0x0059,0x0059,0x0054}, RES_SCRIPT },
	{ {0x17828,0x18B40,0x175B2}, {0x0109,0x011E,0x011A}, RES_SCRIPT },
	{ {0x17931,0x18C5E,0x176CC}, {0x0009,0x0009,0x0009}, RES_SCRIPT },
	{ {0x14D21,0x14D21,0x14D27}, {0x01B6,0x01B6,0x01B9}, RES_SCRIPT },
	{ {0x2B0F6,0x2B108,0x2AA0E}, {0x0243,0x0243,0x0256}, RES_SCRIPT },
	{ {0x230BF,0x230C9,0x231A4}, {0x067F,0x067F,0x06D2}, RES_SCRIPT },
	{ {0x2C711,0x2C711,0x2B9E7}, {0x001C,0x001C,0x001D}, RES_SCRIPT },
	{ {0x2C72D,0x2C72D,0x2BA04}, {0x001A,0x001A,0x0016}, RES_SCRIPT },
	{ {0x2C747,0x2C747,0x2BA1A}, {0x0021,0x0021,0x002D}, RES_SCRIPT },
	{ {0x2C768,0x2C768,0x2BA47}, {0x0024,0x0024,0x0027}, RES_SCRIPT },
	{ {0x2C78C,0x2C78C,0x2BA6E}, {0x0017,0x0017,0x0016}, RES_SCRIPT },
	{ {0x2C7A3,0x2C7A3,0x2BA84}, {0x0017,0x0017,0x0014}, RES_SCRIPT },
	{ {0x2C7BA,0x2C7BA,0x2BA98}, {0x0014,0x0014,0x0015}, RES_SCRIPT },
	{ {0x2C7CE,0x2C7CE,0x2BAAD}, {0x0024,0x0024,0x0029}, RES_SCRIPT },
	{ {0x00000,0x00000,0x00000}, {0x0000,0x0000,0x0000}, RES_SCRIPT },
	{ {0x2C7F2,0x2C7F2,0x2BAD6}, {0x0011,0x0011,0x0010}, RES_SCRIPT },
	{ {0x1793A,0x18C67,0x176D5}, {0x009D,0x009D,0x00A2}, RES_SCRIPT },
	{ {0x22750,0x2275A,0x22831}, {0x0066,0x0066,0x0066}, RES_SCRIPT },
	{ {0x14ED7,0x14ED7,0x14EE0}, {0x0075,0x0075,0x0077}, RES_SCRIPT },
	{ {0x1F30C,0x1F28D,0x1F30B}, {0x0120,0x0120,0x011A}, RES_SCRIPT },
	{ {0x1FD55,0x1FCD6,0x1FD58}, {0x001D,0x001D,0x001D}, RES_SCRIPT },
	{ {0x1F42C,0x1F3AD,0x1F425}, {0x008F,0x008F,0x0095}, RES_SCRIPT },
	{ {0x1F4BB,0x1F43C,0x1F4BA}, {0x0097,0x0097,0x009E}, RES_SCRIPT },
	{ {0x179D7,0x18D04,0x17777}, {0x006A,0x006A,0x006F}, RES_SCRIPT },
	{ {0x17A41,0x18D6E,0x177E6}, {0x0030,0x0030,0x002F}, RES_SCRIPT },
	{ {0x1F552,0x1F4D3,0x1F558}, {0x0092,0x0092,0x0098}, RES_SCRIPT },
	{ {0x2C803,0x2C803,0x2BAE6}, {0x00CC,0x00CC,0x00C4}, RES_SCRIPT },
	{ {0x2C8CF,0x2C8CF,0x2BBAA}, {0x00BA,0x00BA,0x00AE}, RES_SCRIPT },
	{ {0x2C989,0x2C989,0x2BC58}, {0x0088,0x0088,0x0088}, RES_SCRIPT },
	{ {0x20A09,0x20A09,0x20A3C}, {0x01B0,0x01B0,0x01BB}, RES_SCRIPT },
	{ {0x20BB9,0x20BB9,0x20BF7}, {0x0168,0x0168,0x0197}, RES_SCRIPT },
	{ {0x20D21,0x20D21,0x20D8E}, {0x006C,0x006C,0x006E}, RES_SCRIPT },
	{ {0x20D8D,0x20D8D,0x20DFC}, {0x0037,0x0037,0x0028}, RES_SCRIPT },
	{ {0x20DC4,0x20DC4,0x20E24}, {0x00E4,0x00E4,0x00EA}, RES_SCRIPT },
	{ {0x20EA8,0x20EA8,0x20F0E}, {0x0045,0x0045,0x0049}, RES_SCRIPT },
	{ {0x20EED,0x20EED,0x20F57}, {0x00E1,0x00E1,0x00E7}, RES_SCRIPT },
	{ {0x20FCE,0x20FCE,0x2103E}, {0x00F6,0x00F6,0x010C}, RES_SCRIPT },
	{ {0x210C4,0x210C4,0x2114A}, {0x0141,0x0141,0x0151}, RES_SCRIPT },
	{ {0x21205,0x21205,0x2129B}, {0x0183,0x0183,0x01B0}, RES_SCRIPT },
	{ {0x21388,0x21388,0x2144B}, {0x0034,0x0034,0x0034}, RES_SCRIPT },
	{ {0x213BC,0x213BC,0x2147F}, {0x00A9,0x00A9,0x00A9}, RES_SCRIPT },
	{ {0x24367,0x24367,0x24379}, {0x011B,0x011B,0x010E}, RES_SCRIPT },
	{ {0x1BD4D,0x1BDC2,0x1BBD7}, {0x0070,0x0070,0x0072}, RES_SCRIPT },
	{ {0x1CC78,0x1CBF9,0x1CC6B}, {0x0091,0x0091,0x0092}, RES_SCRIPT },
	{ {0x29372,0x29372,0x2898B}, {0x0054,0x0054,0x0054}, RES_SCRIPT },
	{ {0x19F74,0x1A0D0,0x19EB7}, {0x00CE,0x00CE,0x00D3}, RES_SCRIPT },
	{ {0x1A042,0x1A19E,0x19F8A}, {0x0077,0x0077,0x0077}, RES_SCRIPT },
	{ {0x14F4C,0x14F4C,0x14F57}, {0x0057,0x0057,0x0057}, RES_SCRIPT },
	{ {0x27886,0x2790A,0x2703E}, {0x02DF,0x02DF,0x0307}, RES_SCRIPT },
	{ {0x1DA2A,0x1D9AB,0x1D9FB}, {0x0219,0x0219,0x024F}, RES_SCRIPT },
	{ {0x1DC43,0x1DBC4,0x1DC4A}, {0x00F9,0x00F9,0x00E4}, RES_SCRIPT },
	{ {0x1DD3C,0x1DCBD,0x1DD2E}, {0x0056,0x0056,0x0059}, RES_SCRIPT },
	{ {0x1DD92,0x1DD13,0x1DD87}, {0x01C2,0x01C2,0x01C2}, RES_SCRIPT },
	{ {0x14FA3,0x14FA3,0x14FAE}, {0x004D,0x004D,0x004D}, RES_SCRIPT },
	{ {0x27594,0x27618,0x26D52}, {0x00D9,0x00D9,0x00D5}, RES_SCRIPT },
	{ {0x21DC3,0x21DCD,0x21E92}, {0x0013,0x0013,0x0013}, RES_SCRIPT },
	{ {0x2A63D,0x2A64F,0x29C6E}, {0x00F0,0x00F0,0x00F0}, RES_SCRIPT },
	{ {0x24482,0x24482,0x24487}, {0x00E7,0x00E7,0x00E0}, RES_SCRIPT },
	{ {0x21465,0x21465,0x21528}, {0x00F2,0x00F2,0x00F2}, RES_SCRIPT },
	{ {0x24569,0x24569,0x24567}, {0x002B,0x002B,0x0023}, RES_SCRIPT },
	{ {0x2C3CF,0x2C3CF,0x2B6B2}, {0x010F,0x010F,0x010B}, RES_SCRIPT },
	{ {0x24594,0x24594,0x2458A}, {0x00AA,0x00AA,0x00A1}, RES_SCRIPT },
	{ {0x24CE8,0x250B2,0x250A0}, {0x0DAB,0x0DAB,0x018B}, RES_SCRIPT },
	{ {0x1B67F,0x1B6F4,0x1B4F7}, {0x000D,0x000D,0x000D}, RES_SCRIPT },
	{ {0x1B68C,0x1B701,0x1B504}, {0x000D,0x000D,0x000D}, RES_SCRIPT },
	{ {0x2373E,0x23748,0x23876}, {0x017C,0x017C,0x018E}, RES_SCRIPT },
	{ {0x282F5,0x282F5,0x27639}, {0x01E1,0x01E1,0x01F0}, RES_SCRIPT },
	{ {0x238BA,0x238C4,0x23A04}, {0x0153,0x0153,0x017B}, RES_SCRIPT },
	{ {0x23A0D,0x23A17,0x23B7F}, {0x019C,0x019C,0x01AC}, RES_SCRIPT },
	{ {0x23BA9,0x23BB3,0x23D2B}, {0x0016,0x0016,0x0016}, RES_SCRIPT },
	{ {0x2C4DE,0x2C4DE,0x2B7BD}, {0x005C,0x005C,0x005B}, RES_SCRIPT },
	{ {0x23BBF,0x23BC9,0x23D41}, {0x0020,0x0020,0x0020}, RES_SCRIPT },
	{ {0x27D66,0x27DEA,0x2A643}, {0x00A5,0x00A5,0x00A6}, RES_SCRIPT },
	{ {0x2A72D,0x2A73F,0x29D5E}, {0x034D,0x034D,0x0399}, RES_SCRIPT },
	{ {0x14FF0,0x14FF0,0x14FFB}, {0x00E3,0x00E3,0x00D2}, RES_SCRIPT },
	{ {0x2BABC,0x2BACE,0x2B1DE}, {0x005F,0x005F,0x0063}, RES_SCRIPT },
	{ {0x00000,0x00000,0x00000}, {0x0000,0x0000,0x0000}, RES_SCRIPT },
	{ {0x25A93,0x25E5D,0x2522B}, {0x003C,0x003C,0x003C}, RES_SCRIPT },
	{ {0x1E8E9,0x1E86A,0x1E8DB}, {0x0011,0x0011,0x0011}, RES_SCRIPT },
	{ {0x1634A,0x1634A,0x13E33}, {0x0018,0x0018,0x0018}, RES_SCRIPT },
	{ {0x26DFD,0x26E81,0x26585}, {0x001F,0x001F,0x001F}, RES_SCRIPT },
	{ {0x26E1C,0x26EA0,0x265A4}, {0x0054,0x0054,0x0054}, RES_SCRIPT },
	{ {0x26E70,0x26EF4,0x265F8}, {0x0149,0x0149,0x017D}, RES_SCRIPT },
	{ {0x26FB9,0x2703D,0x26775}, {0x004B,0x004B,0x004B}, RES_SCRIPT },
	{ {0x27004,0x27088,0x267C0}, {0x017D,0x017D,0x0165}, RES_SCRIPT },
	{ {0x27181,0x27205,0x26925}, {0x0027,0x0027,0x0027}, RES_SCRIPT },
	{ {0x271A8,0x2722C,0x2694C}, {0x0041,0x0041,0x0041}, RES_SCRIPT },
	{ {0x271E9,0x2726D,0x2698D}, {0x01B1,0x01B1,0x01CB}, RES_SCRIPT },
	{ {0x16362,0x16362,0x13E4B}, {0x001F,0x001F,0x001F}, RES_SCRIPT },
	{ {0x2463E,0x2463E,0x2462B}, {0x002A,0x002A,0x002A}, RES_SCRIPT },
	{ {0x150D3,0x150D3,0x150CD}, {0x019E,0x019E,0x0187}, RES_SCRIPT },
	{ {0x19275,0x194A8,0x1927D}, {0x0031,0x0031,0x0031}, RES_SCRIPT },
	{ {0x17A71,0x18D9E,0x17815}, {0x007C,0x007C,0x0087}, RES_SCRIPT },
	{ {0x21557,0x21557,0x2161A}, {0x00DC,0x00DC,0x00D8}, RES_SCRIPT },
	{ {0x1D457,0x1D3D8,0x1D412}, {0x0018,0x0018,0x0018}, RES_SCRIPT },
	{ {0x1D46F,0x1D3F0,0x1D42A}, {0x0053,0x0053,0x0058}, RES_SCRIPT },
	{ {0x18BE2,0x17DB0,0x18C04}, {0x0005,0x0005,0x0005}, RES_SCRIPT },
	{ {0x15271,0x15271,0x15254}, {0x011B,0x011B,0x0108}, RES_SCRIPT },
	{ {0x1538C,0x1538C,0x1535C}, {0x000B,0x000B,0x000B}, RES_SCRIPT },
	{ {0x24668,0x24668,0x24655}, {0x0138,0x0138,0x0139}, RES_SCRIPT },
	{ {0x247A0,0x247A0,0x2478E}, {0x0014,0x0014,0x0014}, RES_SCRIPT },
	{ {0x1DF54,0x1DED5,0x1DF49}, {0x0018,0x0018,0x0018}, RES_SCRIPT },
	{ {0x247B4,0x247B4,0x247A2}, {0x0027,0x0027,0x0027}, RES_SCRIPT },
	{ {0x1A0B9,0x1A215,0x1A001}, {0x004D,0x004D,0x004C}, RES_SCRIPT },
	{ {0x00000,0x00000,0x00000}, {0x0000,0x0000,0x0000}, RES_SCRIPT },
	{ {0x2BB1B,0x2BB2D,0x2B241}, {0x00A5,0x00A5,0x00A5}, RES_SCRIPT },
	{ {0x2AA7A,0x2AA8C,0x2A0F7}, {0x00C1,0x00C1,0x00B5}, RES_SCRIPT },
	{ {0x2AB3B,0x2AB4D,0x2A1AC}, {0x0140,0x0140,0x0140}, RES_SCRIPT },
	{ {0x19826,0x19982,0x19759}, {0x00BF,0x00BF,0x00CA}, RES_SCRIPT },
	{ {0x198E5,0x19A41,0x19823}, {0x014D,0x014D,0x014D}, RES_SCRIPT },
	{ {0x19A32,0x19B8E,0x19970}, {0x0012,0x0012,0x0012}, RES_SCRIPT },
	{ {0x2AC7B,0x2AC8D,0x2A2EC}, {0x0005,0x0005,0x0005}, RES_SCRIPT },
	{ {0x2AC80,0x2AC92,0x2A2F1}, {0x0005,0x0005,0x0005}, RES_SCRIPT },
	{ {0x2AC85,0x2AC97,0x2A2F6}, {0x0005,0x0005,0x0005}, RES_SCRIPT },
	{ {0x2AC8A,0x2AC9C,0x2A2FB}, {0x0005,0x0005,0x0005}, RES_SCRIPT },
	{ {0x2AC8F,0x2ACA1,0x2A300}, {0x0005,0x0005,0x0005}, RES_SCRIPT },
	{ {0x21633,0x21633,0x216F2}, {0x0033,0x0033,0x0033}, RES_SCRIPT },
	{ {0x2AC94,0x2ACA6,0x2A305}, {0x0005,0x0005,0x0005}, RES_SCRIPT },
	{ {0x00000,0x00000,0x00000}, {0x0000,0x0000,0x0000}, RES_SCRIPT },
	{ {0x2AC99,0x2ACAB,0x2A30A}, {0x009C,0x009C,0x009C}, RES_SCRIPT },
	{ {0x2AD35,0x2AD47,0x2A3A6}, {0x009C,0x009C,0x009C}, RES_SCRIPT }
};
t_resource res_sounds[82] = {
	{ {0x0FFE8,0x0BF54,0x0BF58}, {0x000A,0x000A,0x000A}, RES_SOUND },
	{ {0x30ECA,0x30ECA,0x30352}, {0x0832,0x0832,0x0832}, RES_SOUND },
	{ {0x30ECA,0x30ECA,0x30352}, {0x0832,0x0832,0x0832}, RES_SOUND },
	{ {0x30ECA,0x30ECA,0x30352}, {0x0832,0x0832,0x0832}, RES_SOUND },
	{ {0x30ECA,0x30ECA,0x30352}, {0x0832,0x0832,0x0832}, RES_SOUND },
	{ {0x30ECA,0x30ECA,0x30352}, {0x0832,0x0832,0x0832}, RES_SOUND },
	{ {0x17FCA,0x0BF5E,0x0BF62}, {0x0011,0x0011,0x0011}, RES_SOUND },
	{ {0x27E0B,0x27ECB,0x27E5F}, {0x0073,0x0073,0x0073}, RES_SOUND },
	{ {0x17FDB,0x0BF6F,0x17F5A}, {0x0011,0x0011,0x0011}, RES_SOUND },
	{ {0x17FEC,0x0FF5D,0x17F6B}, {0x0011,0x0011,0x0011}, RES_SOUND },
	{ {0x27E7E,0x316FC,0x27ED2}, {0x0056,0x0056,0x0056}, RES_SOUND },
	{ {0x27ED4,0x13F4E,0x1BF55}, {0x001F,0x001F,0x001F}, RES_SOUND },
	{ {0x23FEE,0x0FF6E,0x23F66}, {0x0011,0x0011,0x0011}, RES_SOUND },
	{ {0x0FFF2,0x13F6D,0x0BF73}, {0x000A,0x000A,0x000A}, RES_SOUND },
	{ {0x27EF3,0x1BF47,0x1BF74}, {0x000A,0x000A,0x000A}, RES_SOUND },
	{ {0x27EFD,0x1BF51,0x27F28}, {0x0019,0x0019,0x0019}, RES_SOUND },
	{ {0x27F16,0x31752,0x2BF0A}, {0x004B,0x004B,0x004B}, RES_SOUND },
	{ {0x27F61,0x1BF6A,0x1FF71}, {0x000A,0x000A,0x000A}, RES_SOUND },
	{ {0x27F6B,0x27F3E,0x27F41}, {0x000F,0x000F,0x000F}, RES_SOUND },
	{ {0x27F7A,0x27F4D,0x27F50}, {0x001D,0x001D,0x001D}, RES_SOUND },
	{ {0x27F97,0x3179D,0x2FEAA}, {0x0045,0x0045,0x0045}, RES_SOUND },
	{ {0x27FDC,0x27F6A,0x27F6D}, {0x000F,0x000F,0x000F}, RES_SOUND },
	{ {0x2FD42,0x2BF40,0x2BF55}, {0x001B,0x001B,0x001B}, RES_SOUND },
	{ {0x2FD5D,0x317E2,0x2FEEF}, {0x0033,0x0033,0x0033}, RES_SOUND },
	{ {0x27FEB,0x2BF5B,0x2FF22}, {0x0011,0x0011,0x0011}, RES_SOUND },
	{ {0x2BFEF,0x2BF6C,0x2BF70}, {0x000F,0x000F,0x000F}, RES_SOUND },
	{ {0x2FD90,0x31815,0x30B84}, {0x0075,0x0075,0x0075}, RES_SOUND },
	{ {0x2FE05,0x2FF6C,0x2FF33}, {0x0014,0x0014,0x0014}, RES_SOUND },
	{ {0x0FFE8,0x0BF54,0x0BF58}, {0x000A,0x000A,0x000A}, RES_SOUND },
	{ {0x2FE19,0x3188A,0x30BF9}, {0x00FF,0x00FF,0x00FF}, RES_SOUND },
	{ {0x2FF18,0x31989,0x2FF47}, {0x000F,0x000F,0x000F}, RES_SOUND },
	{ {0x2FF27,0x31998,0x2FF56}, {0x000F,0x000F,0x000F}, RES_SOUND },
	{ {0x2FF36,0x319A7,0x30CF8}, {0x0092,0x0092,0x0092}, RES_SOUND },
	{ {0x2FF36,0x319A7,0x30CF8}, {0x0092,0x0092,0x0092}, RES_SOUND },
	{ {0x2FFC8,0x31A39,0x30D8A}, {0x002D,0x002D,0x002D}, RES_SOUND },
	{ {0x316FC,0x31A66,0x30DB7}, {0x00F8,0x00F8,0x00F8}, RES_SOUND },
	{ {0x317F4,0x31B5E,0x2FF65}, {0x0016,0x0016,0x0016}, RES_SOUND },
	{ {0x3180A,0x31B74,0x30EAF}, {0x0011,0x0011,0x0011}, RES_SOUND },
	{ {0x3181B,0x31B85,0x30EC0}, {0x004B,0x004B,0x004B}, RES_SOUND },
	{ {0x31866,0x31BD0,0x30F0B}, {0x0011,0x0011,0x0011}, RES_SOUND },
	{ {0x31877,0x31BE1,0x30F1C}, {0x003B,0x003B,0x003B}, RES_SOUND },
	{ {0x318B2,0x31C1C,0x30F57}, {0x008A,0x008A,0x008A}, RES_SOUND },
	{ {0x3193C,0x31CA6,0x30FE1}, {0x0011,0x0011,0x0011}, RES_SOUND },
	{ {0x3194D,0x31CB7,0x30FF2}, {0x000F,0x000F,0x000F}, RES_SOUND },
	{ {0x3195C,0x31CC6,0x31001}, {0x00A2,0x00A2,0x00A2}, RES_SOUND },
	{ {0x319FE,0x31D68,0x310A3}, {0x00D3,0x00D3,0x00D3}, RES_SOUND },
	{ {0x31AD1,0x31E3B,0x31176}, {0x0097,0x0097,0x0097}, RES_SOUND },
	{ {0x2BFEF,0x2BF6C,0x2BF70}, {0x000F,0x000F,0x000F}, RES_SOUND },
	{ {0x3195C,0x31CC6,0x31001}, {0x00A2,0x00A2,0x00A2}, RES_SOUND },
	{ {0x31B68,0x31ED2,0x3120D}, {0x05D1,0x05D1,0x05D1}, RES_SOUND },
	{ {0x31B68,0x31ED2,0x3120D}, {0x05D1,0x05D1,0x05D1}, RES_SOUND },
	{ {0x32139,0x324A3,0x317DE}, {0x0011,0x0011,0x0011}, RES_SOUND },
	{ {0x0FFE8,0x0BF54,0x0BF58}, {0x000A,0x000A,0x000A}, RES_SOUND },
	{ {0x2FD90,0x31815,0x30B84}, {0x0075,0x0075,0x0075}, RES_SOUND },
	{ {0x27ED4,0x13F4E,0x1BF55}, {0x001F,0x001F,0x001F}, RES_SOUND },
	{ {0x3214A,0x324B4,0x317EF}, {0x098E,0x098E,0x098E}, RES_SOUND },
	{ {0x3181B,0x31B85,0x30EC0}, {0x004B,0x004B,0x004B}, RES_SOUND },
	{ {0x32AD8,0x32E42,0x3217D}, {0x0011,0x0011,0x0011}, RES_SOUND },
	{ {0x30ECA,0x30ECA,0x30352}, {0x0832,0x0832,0x0832}, RES_SOUND },
	{ {0x32AE9,0x32E53,0x3218E}, {0x000F,0x000F,0x000F}, RES_SOUND },
	{ {0x32AF8,0x32E62,0x3219D}, {0x002F,0x002F,0x002F}, RES_SOUND },
	{ {0x32B27,0x32E91,0x321CC}, {0x001D,0x001D,0x001D}, RES_SOUND },
	{ {0x32B44,0x32EAE,0x321E9}, {0x0018,0x0018,0x0018}, RES_SOUND },
	{ {0x32B5C,0x32EC6,0x32201}, {0x0016,0x0016,0x0016}, RES_SOUND },
	{ {0x32B72,0x32EDC,0x32217}, {0x001B,0x001B,0x001B}, RES_SOUND },
	{ {0x32B8D,0x32EF7,0x32232}, {0x0088,0x0088,0x0088}, RES_SOUND },
	{ {0x32C15,0x32F7F,0x322BA}, {0x0065,0x0065,0x0065}, RES_SOUND },
	{ {0x32C7A,0x32FE4,0x3231F}, {0x0065,0x0065,0x0065}, RES_SOUND },
	{ {0x32CDF,0x33049,0x32384}, {0x0073,0x0073,0x0073}, RES_SOUND },
	{ {0x32D52,0x330BC,0x323F7}, {0x00F9,0x00F9,0x00F9}, RES_SOUND },
	{ {0x32E4B,0x331B5,0x324F0}, {0x049E,0x049E,0x049E}, RES_SOUND },
	{ {0x34001,0x34001,0x3298E}, {0x0EA8,0x0EA8,0x0EA8}, RES_SOUND },
	{ {0x332E9,0x34EA9,0x34001}, {0x0B18,0x0B18,0x0B18}, RES_SOUND },
	{ {0x34EA9,0x359C1,0x34B19}, {0x0B9C,0x0B9C,0x0B9C}, RES_SOUND },
	{ {0x35A45,0x3655D,0x356B5}, {0x0C6B,0x0C6B,0x0C6B}, RES_SOUND },
	{ {0x366B0,0x38001,0x36320}, {0x0E56,0x0E56,0x0E56}, RES_SOUND },
	{ {0x38001,0x371C8,0x37176}, {0x0C70,0x0C70,0x0C70}, RES_SOUND },
	{ {0x38C71,0x38E57,0x38001}, {0x0DEC,0x0DEC,0x0DEC}, RES_SOUND },
	{ {0x39A5D,0x39C43,0x38DED}, {0x0B77,0x0B77,0x0B77}, RES_SOUND },
	{ {0x37506,0x33653,0x33836}, {0x042F,0x042F,0x042F}, RES_SOUND },
	{ {0x3A5D4,0x3A7BA,0x39964}, {0x0AC5,0x0AC5,0x0AC5}, RES_SOUND },
	{ {0x3B099,0x3B27F,0x3A429}, {0x0BE4,0x0BE4,0x0BE4}, RES_SOUND }
};
t_resource res_costumes[25] = {
	{ {0x17F05,0x0BEFF,0x0FEF5}, {0x0055,0x0055,0x0055}, RES_COSTUME },
	{ {0x17F05,0x0BEFF,0x0FEF5}, {0x0055,0x0055,0x0055}, RES_COSTUME },
	{ {0x17F05,0x0BEFF,0x0FEF5}, {0x0055,0x0055,0x0055}, RES_COSTUME },
	{ {0x17F05,0x0BEFF,0x0FEF5}, {0x0055,0x0055,0x0055}, RES_COSTUME },
	{ {0x17F05,0x0BEFF,0x0FEF5}, {0x0055,0x0055,0x0055}, RES_COSTUME },
	{ {0x17F05,0x0BEFF,0x0FEF5}, {0x0055,0x0055,0x0055}, RES_COSTUME },
	{ {0x17F05,0x0BEFF,0x0FEF5}, {0x0055,0x0055,0x0055}, RES_COSTUME },
	{ {0x17F05,0x0BEFF,0x0FEF5}, {0x0055,0x0055,0x0055}, RES_COSTUME },
	{ {0x13FAB,0x0FEA2,0x17E9A}, {0x004B,0x004B,0x004B}, RES_COSTUME },
	{ {0x17F05,0x0BEFF,0x0FEF5}, {0x0055,0x0055,0x0055}, RES_COSTUME },
	{ {0x17F05,0x0BEFF,0x0FEF5}, {0x0055,0x0055,0x0055}, RES_COSTUME },
	{ {0x17F05,0x0BEFF,0x0FEF5}, {0x0055,0x0055,0x0055}, RES_COSTUME },
	{ {0x17F5A,0x0FEED,0x0FF4A}, {0x0036,0x0036,0x0036}, RES_COSTUME },
	{ {0x17F90,0x0FF23,0x17EE5}, {0x003A,0x003A,0x004A}, RES_COSTUME },
	{ {0x17F90,0x0FF23,0x17EE5}, {0x003A,0x003A,0x004A}, RES_COSTUME },
	{ {0x17F05,0x0BEFF,0x0FEF5}, {0x0055,0x0055,0x0055}, RES_COSTUME },
	{ {0x1BF87,0x13F13,0x17F1F}, {0x003B,0x003B,0x003B}, RES_COSTUME },
	{ {0x17F05,0x0BEFF,0x0FEF5}, {0x0055,0x0055,0x0055}, RES_COSTUME },
	{ {0x23FA9,0x23F2F,0x1BE94}, {0x0045,0x0045,0x0045}, RES_COSTUME },
	{ {0x1FFBD,0x1FF3E,0x1BED9}, {0x0040,0x0040,0x0040}, RES_COSTUME },
	{ {0x1BFC2,0x27E8F,0x1BF19}, {0x003C,0x003C,0x003C}, RES_COSTUME },
	{ {0x17F90,0x0FF23,0x17EE5}, {0x003A,0x003A,0x004A}, RES_COSTUME },
	{ {0x17F90,0x0FF23,0x17EE5}, {0x003A,0x003A,0x004A}, RES_COSTUME },
	{ {0x17F05,0x0BEFF,0x0FEF5}, {0x0055,0x0055,0x0055}, RES_COSTUME },
	{ {0x13FAB,0x0FEA2,0x17E9A}, {0x004B,0x004B,0x004B}, RES_COSTUME }
};
t_resource res_globdata =
	{ {0x2CA11,0x2CA11,0x2C001}, {0x0307,0x0307,0x0307}, RES_GLOBDATA };
t_resource res_palettes[2] = {
	{ {0x0BFC1,0x07F61,0x07F55}, {0x0010,0x0010,0x0010}, RES_PALETTE },
	{ {0x0BFD1,0x0BEB2,0x07F65}, {0x0010,0x0010,0x0010}, RES_PALETTE }
};
t_resource res_unk_DD75[2] = {
	{ {0x2CE11,0x2CE11,0x2C401}, {0x2BE0,0x2BE0,0x2BE0}, RES_UNK_DD75 },
	{ {0x07F6B,0x0BE28,0x0FE6B}, {0x008A,0x008A,0x008A}, RES_UNK_DD75 }
};
t_resource res_unk_DD7D[2] = {
	{ {0x0FEA2,0x1BE32,0x13E6A}, {0x0115,0x0115,0x0115}, RES_UNK_DD7D },
	{ {0x07FF5,0x07F5B,0x07F4F}, {0x0006,0x0006,0x0006}, RES_UNK_DD7D }
};
t_resource res_unk_DD95[2] = {
	{ {0x0FFB7,0x0BEC2,0x0BF1B}, {0x0031,0x0031,0x0031}, RES_UNK_DD95 },
	{ {0x0BFE1,0x07F71,0x07F75}, {0x0009,0x0009,0x0009}, RES_UNK_DD95 }
};
t_resource res_unk_DD9D[2] = {
	{ {0x2BDC5,0x2FD42,0x2BCE0}, {0x022A,0x022A,0x022A}, RES_UNK_DD9D },
	{ {0x0BFEA,0x0BEF3,0x0BF4C}, {0x000C,0x000C,0x000C}, RES_UNK_DD9D }
};

unsigned long r_offset (p_resource res)
{
	return res->offset[ROMset];
}
unsigned short r_length (p_resource res)
{
	return res->length[ROMset];
}

void	extract_resource (FILE *input, FILE *output, p_resource res)
{
	unsigned short len, i, j;
	unsigned char val;
	unsigned char cnt;

	_assert(res != NULL,"extract_resource - no resource specified");
	if ((r_offset(res) == 0) && (r_length(res) == 0))
		return;	/* there are 8 scripts that are zero bytes long, so we should skip them */
	fseek(input,16 + r_offset(res),SEEK_SET);

	switch (res->type)
	{
	case RES_GLOBDATA:
		len = r_length(res);
		for (i = 0; i < len; i++)
			write_byte(output,read_byte(input));
		break;
	case RES_ROOMGFX:
	case RES_COSTUMEGFX:
		write_word(output,r_length(res) + 2);
		len = read_byte(input);
		write_byte(output,(unsigned char)len);
		if (!len)
			len = 257;
		len = len << 4;
		for (i = 0; i < len;)
		{
			write_byte(output,cnt = read_byte(input));
			for (j = 0; j < (cnt & 0x7F); j++, i++)
				if ((cnt & 0x80) || (j == 0))
					write_byte(output,read_byte(input));
		}
		_assert(ftell(input) - r_offset(res) - 16 == r_length(res),"extract_resource - length mismatch while extracting graphics resource (was %04X, should be %04X)",ftell(input) - r_offset(res) - 16,r_length(res));
		break;
	case RES_ROOM:
	case RES_SCRIPT:
		len = read_word(input);
		_assert(len == r_length(res),"extract_resource - length mismatch while extracting room/script resource (was %04X, should be %04X)",len,r_length(res));
		fseek(input,-2,SEEK_CUR);
		for (i = 0; i < len; i++)
			write_byte(output,read_byte(input));
		break;
	case RES_SOUND:
		len = r_length(res) + 2;
		val = read_byte(input);
		cnt = read_byte(input);
		if ((val == 2) && (cnt == 100))
		{
			write_word(output,len);
			write_byte(output,val);
			write_byte(output,cnt);
			while (1)
			{
				write_byte(output,val = read_byte(input));
				if (val >= 0xFE)
					break;
			}
		}
		else if (((val == 0) || (val == 1) || (val == 4)) && (cnt == 10))
		{
			write_word(output,len);
			write_byte(output,val);
			write_byte(output,cnt);
			while (1)
			{
				write_byte(output,val = read_byte(input));
				if (val >= 0xFE)
					break;
				if (val >= 0x10)
					write_byte(output,read_byte(input));
				else
				{
					write_byte(output,read_byte(input));
					write_byte(output,read_byte(input));
					write_byte(output,read_byte(input));
					write_byte(output,read_byte(input));
				}
			}
		}
		else	error("extract_resource - unknown sound type %i/%i detected",val,cnt);
		_assert(ftell(input) - r_offset(res) - 16 == r_length(res),"extract_resource - length mismatch while extracting sound resource (was %04X, should be %04X)",ftell(input) - r_offset(res) - 16,r_length(res));
		break;
	case RES_COSTUME:
	case RES_PALETTE:
	case RES_UNK_DD75:
	case RES_UNK_DD7D:
	case RES_UNK_DD95:
	case RES_UNK_DD9D:
		len = r_length(res);
		write_word(output,len + 2);
		for (i = 0; i < len; i++)
			write_byte(output,read_byte(input));
		break;
	default:
		error("extract_resource - unknown resource type %i specified!",res->type);
	}
}

#ifdef	MAKE_LFLS
/* based on structure of Classic PC Maniac Mansion LFL files (roomgfx resources are arranged in order, one per file, after the room blocks) */
p_resource lfl_01[] = { &res_rooms[1], &res_roomgfx[1], &res_scripts[57], &res_scripts[61], &res_scripts[76], &res_scripts[105], &res_scripts[111], &res_sounds[5], &res_scripts[132], &res_scripts[148], &res_scripts[155], &res_scripts[156], NULL };
p_resource lfl_02[] = { &res_rooms[2], &res_roomgfx[2], NULL };
p_resource lfl_03[] = { &res_rooms[3], &res_roomgfx[3], &res_scripts[21], &res_sounds[26], NULL };
p_resource lfl_04[] = { &res_rooms[4], &res_roomgfx[4], &res_scripts[46], &res_scripts[56], &res_scripts[137], &res_scripts[146], &res_sounds[12], &res_sounds[11], &res_sounds[13], &res_sounds[42], NULL };
p_resource lfl_05[] = { &res_rooms[5], &res_roomgfx[5], &res_scripts[30], &res_scripts[31], &res_scripts[32], &res_scripts[33], &res_scripts[34], &res_scripts[35], &res_sounds[22], &res_sounds[23], &res_sounds[24], &res_sounds[21], &res_sounds[46], NULL };
p_resource lfl_06[] = { &res_rooms[6], &res_roomgfx[6], NULL };
p_resource lfl_07[] = { &res_rooms[7], &res_roomgfx[7], &res_scripts[17], &res_scripts[58], &res_scripts[59], &res_scripts[60], &res_scripts[74], &res_scripts[81], &res_scripts[82], &res_scripts[150], &res_sounds[14], &res_sounds[15], &res_sounds[16], &res_sounds[17], NULL };
p_resource lfl_08[] = { &res_rooms[8], &res_roomgfx[8], &res_scripts[7], &res_scripts[12], &res_scripts[8], &res_scripts[13], &res_scripts[47], &res_scripts[48], &res_scripts[49], &res_scripts[155], &res_sounds[32], &res_sounds[33], &res_sounds[36], NULL };
p_resource lfl_09[] = { &res_rooms[9], &res_roomgfx[9], &res_scripts[10], &res_scripts[11], &res_scripts[45], &res_scripts[55], &res_scripts[84], &res_scripts[85], &res_scripts[86], NULL };
p_resource lfl_10[] = { &res_rooms[10], &res_roomgfx[10], &res_scripts[24], &res_scripts[149], &res_sounds[28], NULL };
p_resource lfl_11[] = { &res_rooms[11], &res_roomgfx[11], &res_scripts[166], &res_scripts[167], &res_scripts[168], NULL };
p_resource lfl_12[] = { &res_rooms[12], &res_roomgfx[12], &res_scripts[51], &res_scripts[103], &res_scripts[104], &res_scripts[161], &res_sounds[63], &res_costumes[14], NULL };
p_resource lfl_13[] = { &res_rooms[13], &res_roomgfx[13], NULL };
p_resource lfl_14[] = { &res_rooms[14], &res_roomgfx[14], NULL };
p_resource lfl_15[] = { &res_rooms[15], &res_roomgfx[15], &res_sounds[27], NULL };
p_resource lfl_16[] = { &res_rooms[16], &res_roomgfx[16], &res_scripts[14], &res_scripts[121], &res_scripts[122], &res_sounds[40], &res_sounds[64], &res_sounds[68], NULL };
p_resource lfl_17[] = { &res_rooms[17], &res_roomgfx[17], &res_scripts[20], &res_scripts[100], &res_sounds[25], &res_sounds[44], &res_sounds[2], &res_sounds[50], &res_sounds[52], NULL };
p_resource lfl_18[] = { &res_rooms[18], &res_roomgfx[18], &res_scripts[25], &res_scripts[26], &res_scripts[27], &res_scripts[28], &res_scripts[64], &res_scripts[65], &res_scripts[66], &res_scripts[67], &res_scripts[68], &res_scripts[69], &res_scripts[70], &res_scripts[71], &res_scripts[72], &res_scripts[73], &res_scripts[101], &res_sounds[35], NULL };
p_resource lfl_19[] = { &res_rooms[19], &res_roomgfx[19], &res_scripts[36], &res_scripts[37], &res_scripts[38], &res_scripts[39], &res_scripts[40], &res_scripts[152], &res_scripts[153], &res_costumes[10], NULL };
p_resource lfl_20[] = { &res_rooms[20], &res_roomgfx[20], &res_scripts[107], &res_scripts[108], &res_scripts[109], &res_scripts[110], &res_scripts[159], NULL };
p_resource lfl_21[] = { &res_rooms[21], &res_roomgfx[21], &res_scripts[41], &res_scripts[42], &res_scripts[43], &res_scripts[53], &res_scripts[136], &res_sounds[29], &res_sounds[20], &res_sounds[37], NULL };
p_resource lfl_22[] = { &res_rooms[22], &res_roomgfx[22], &res_scripts[15], NULL };
p_resource lfl_23[] = { &res_rooms[23], &res_roomgfx[23], &res_scripts[77], &res_scripts[79], &res_scripts[80], &res_scripts[83], &res_sounds[41], NULL };
p_resource lfl_24[] = { &res_rooms[24], &res_roomgfx[24], &res_scripts[18], &res_scripts[19], &res_scripts[78], &res_sounds[7], &res_sounds[3], &res_sounds[18], &res_sounds[34], &res_costumes[12], NULL };
p_resource lfl_25[] = { &res_rooms[25], &res_roomgfx[25], &res_scripts[29], &res_sounds[30], &res_sounds[31], NULL };
p_resource lfl_26[] = { &res_rooms[26], &res_roomgfx[26], &res_scripts[87], &res_scripts[88], &res_scripts[89], &res_scripts[90], &res_scripts[91], &res_scripts[92], &res_scripts[93], &res_scripts[94], &res_scripts[95], &res_scripts[96], &res_scripts[97], &res_scripts[98], &res_scripts[116], &res_scripts[151], &res_scripts[174], &res_costumes[11], NULL };
p_resource lfl_27[] = { &res_rooms[27], &res_roomgfx[27], &res_scripts[16], &res_scripts[52], &res_scripts[54], &res_scripts[50], &res_scripts[113], &res_sounds[45], &res_costumes[19], NULL };
p_resource lfl_28[] = { &res_rooms[28], &res_roomgfx[28], &res_scripts[22], &res_scripts[23], NULL };
p_resource lfl_29[] = { &res_rooms[29], &res_roomgfx[29], &res_scripts[75], &res_sounds[43], NULL };
p_resource lfl_30[] = { &res_rooms[30], &res_roomgfx[30], &res_scripts[63], &res_sounds[0], &res_scripts[123], &res_scripts[125], &res_scripts[126], &res_scripts[127], &res_scripts[129], &res_sounds[55], &res_sounds[59], &res_sounds[60], &res_costumes[8], NULL };
p_resource lfl_31[] = { &res_rooms[31], &res_roomgfx[31], &res_scripts[99], &res_scripts[115], &res_scripts[117], &res_scripts[119], &res_scripts[147], &res_scripts[157], &res_scripts[158], &res_scripts[160], &res_costumes[13], &res_costumes[9], &res_costumes[23], &res_costumes[24], &res_costumes[25], NULL };
p_resource lfl_32[] = { &res_rooms[32], &res_roomgfx[32], &res_costumes[15], NULL };
p_resource lfl_33[] = { &res_rooms[33], &res_roomgfx[33], &res_scripts[120], &res_scripts[135], &res_sounds[56], &res_sounds[57], &res_sounds[58], &res_sounds[1], &res_costumes[22], NULL };
p_resource lfl_34[] = { &res_rooms[34], &res_roomgfx[34], NULL };
p_resource lfl_35[] = { &res_rooms[35], &res_roomgfx[35], NULL };
p_resource lfl_36[] = { &res_rooms[36], &res_roomgfx[36], &res_sounds[10], &res_sounds[4], NULL };
p_resource lfl_37[] = { &res_rooms[37], &res_roomgfx[37], NULL };
p_resource lfl_38[] = { &res_rooms[38], &res_roomgfx[38], &res_scripts[138], &res_scripts[139], &res_scripts[140], &res_scripts[141], &res_scripts[142], &res_scripts[143], &res_scripts[144], &res_scripts[145], NULL };
p_resource lfl_39[] = { &res_rooms[39], &res_roomgfx[39], NULL };
p_resource lfl_40[] = { &res_rooms[40], &res_roomgfx[0], &res_scripts[112], &res_costumes[17], NULL };
p_resource lfl_41[] = { &res_rooms[41], &res_scripts[106], &res_sounds[47], &res_sounds[48], &res_sounds[53], &res_sounds[49], &res_sounds[51], NULL };
p_resource lfl_42[] = { &res_rooms[42], &res_scripts[124], &res_costumes[18],  NULL };
p_resource lfl_43[] = { &res_rooms[43], &res_scripts[44], &res_sounds[19], NULL };
p_resource lfl_44[] = { &res_rooms[44], &res_scripts[102], &res_sounds[6], &res_sounds[38], &res_sounds[8], &res_sounds[9], &res_costumes[1], &res_costumes[2], &res_costumes[5], &res_costumes[6], &res_costumes[3], &res_costumes[4], &res_costumes[7], NULL };
p_resource lfl_45[] = { &res_rooms[45], &res_scripts[0], &res_scripts[1], &res_scripts[2], &res_scripts[3], &res_scripts[4], &res_scripts[6], &res_scripts[5], &res_scripts[9], &res_scripts[114], &res_scripts[131], &res_scripts[164], &res_scripts[165], &res_scripts[169], &res_scripts[170], &res_scripts[171], &res_scripts[172], &res_scripts[173], &res_scripts[175], &res_sounds[54], NULL };
p_resource lfl_46[] = { &res_rooms[46], &res_scripts[130], &res_sounds[65], &res_costumes[0], &res_costumes[21], NULL };
p_resource lfl_47[] = { &res_rooms[47], &res_scripts[62], &res_sounds[69], NULL };
p_resource lfl_48[] = { &res_rooms[48], NULL };
p_resource lfl_49[] = { &res_rooms[49], NULL };
p_resource lfl_50[] = { &res_rooms[50], &res_scripts[133], &res_scripts[162], &res_scripts[163], NULL };
p_resource lfl_51[] = { &res_rooms[51], &res_scripts[118], &res_scripts[128], &res_scripts[134], &res_sounds[61], &res_sounds[62], &res_sounds[67], &res_sounds[66], &res_costumes[16], &res_costumes[20], NULL };
p_resource lfl_52[] = { &res_rooms[52], NULL };
/*	remaining 'standard' resources (not used by any of the original LFL files) */
p_resource lfl_53[] = { &res_rooms[53], &res_scripts[177], &res_scripts[178], &res_sounds[70], &res_sounds[71], &res_sounds[72], &res_sounds[73], &res_sounds[74], &res_sounds[75], &res_sounds[76], &res_sounds[77], &res_sounds[78], &res_sounds[79], &res_sounds[80], &res_sounds[81], NULL };
/*	all 'non-standard' resources (the costume-related stuff) */
p_resource lfl_54[] = { &res_rooms[54], &res_costumegfx[0], &res_costumegfx[1], &res_palettes[0], &res_palettes[1], &res_unk_DD75[0], &res_unk_DD75[1], &res_unk_DD7D[0], &res_unk_DD7D[1], &res_unk_DD95[0], &res_unk_DD95[1], &res_unk_DD9D[0], &res_unk_DD9D[1], NULL };

typedef	struct	_lfl
{
	int num;
	p_resource *entries;
}	t_lfl, *p_lfl;

t_lfl	lfls[] = {
	{  1, lfl_01 },
	{  2, lfl_02 },
	{  3, lfl_03 },
	{  4, lfl_04 },
	{  5, lfl_05 },
	{  6, lfl_06 },
	{  7, lfl_07 },
	{  8, lfl_08 },
	{  9, lfl_09 },
	{ 10, lfl_10 },
	{ 11, lfl_11 },
	{ 12, lfl_12 },
	{ 13, lfl_13 },
	{ 14, lfl_14 },
	{ 15, lfl_15 },
	{ 16, lfl_16 },
	{ 17, lfl_17 },
	{ 18, lfl_18 },
	{ 19, lfl_19 },
	{ 20, lfl_20 },
	{ 21, lfl_21 },
	{ 22, lfl_22 },
	{ 23, lfl_23 },
	{ 24, lfl_24 },
	{ 25, lfl_25 },
	{ 26, lfl_26 },
	{ 27, lfl_27 },
	{ 28, lfl_28 },
	{ 29, lfl_29 },
	{ 30, lfl_30 },
	{ 31, lfl_31 },
	{ 32, lfl_32 },
	{ 33, lfl_33 },
	{ 34, lfl_34 },
	{ 35, lfl_35 },
	{ 36, lfl_36 },
	{ 37, lfl_37 },
	{ 38, lfl_38 },
	{ 39, lfl_39 },
	{ 40, lfl_40 },
	{ 41, lfl_41 },
	{ 42, lfl_42 },
	{ 43, lfl_43 },
	{ 44, lfl_44 },
	{ 45, lfl_45 },
	{ 46, lfl_46 },
	{ 47, lfl_47 },
	{ 48, lfl_48 },
	{ 49, lfl_49 },
	{ 50, lfl_50 },
	{ 51, lfl_51 },
	{ 52, lfl_52 },
	{ 53, lfl_53 },
	{ 54, lfl_54 },
	{ -1, NULL }
};

#define GCC_PACK __attribute__((packed))

#if defined(_MSC_VER)
#pragma	pack(push,1)
#endif
struct	_lfl_index
{
	unsigned char	room_lfl[55];
	unsigned short	room_addr[55];
	unsigned char	costume_lfl[35];
	unsigned short	costume_addr[35];
	unsigned char	script_lfl[200];
	unsigned short	script_addr[200];
	unsigned char	sound_lfl[100];
	unsigned short	sound_addr[100];
  #if defined(__GNUC__)
} __attribute__((packed))	lfl_index;
  #else
}	lfl_index;
  #endif
#if defined(_MSC_VER)
#pragma	pack(pop)
#endif
#else	/* !MAKE_LFLS */
void	dump_resource (FILE *input, char *fn_template, int num, p_resource res)
{
	char fname[256];
	FILE *output;
	sprintf(fname,fn_template,num);
	if (!(output = fopen(fname,"wb")))
		error("Error: unable to create %s!",fname);
	extract_resource(input,output,res);
	fclose(output);
}
#endif	/* MAKE_LFLS */

unsigned long	CRCtable[256];
void	InitCRC (void)
{
	const unsigned long poly = 0xEDB88320;
	int i, j;
	unsigned long n;
	for (i = 0; i < 256; i++)
	{
		n = i;
		for (j = 0; j < 8; j++)
			n = (n & 1) ? ((n >> 1) ^ poly) : (n >> 1);
		CRCtable[i] = n;
	}
}
unsigned long	ROM_CRC (FILE *file)
{
	unsigned long CRC = 0xFFFFFFFF;
	unsigned long i;
	fseek(file,0x10,SEEK_SET);
	for (i = 0; i < 0x40000; i++)
		CRC = (CRC >> 8) ^ CRCtable[(CRC ^ read_byte(file)) & 0xFF];
	return CRC ^ 0xFFFFFFFF;
}

int main (int argc, char **argv)
{
	FILE *input, *output;
	char fname[256];
	int i, j;
	unsigned long CRC;

	if (argc < 2)
	{
		printf("Syntax: %s <infile.NES>\n",argv[0]);
		return 1;
	}
	if (!(input = fopen(argv[1],"rb")))
		error("Error: unable to open file %s for input!",argv[1]);

	InitCRC();
	CRC = ROM_CRC(input);
	switch (CRC)
	{
	case 0x0D9F5BD1:
		ROMset = ROMSET_USA;
		debug("ROM contents verified as Maniac Mansion (USA)");
		break;
	case 0xF59CFC3D:
		ROMset = ROMSET_EUROPE;
		debug("ROM contents verified as Maniac Mansion (Europe)");
		break;
	case 0x3F2BDA65:
		ROMset = ROMSET_SWEDEN;
		debug("ROM contents verified as Maniac Mansion (Sweden)");
		break;
	case 0x3DA2085E:
		error("Maniac Mansion (Japan) is not supported!");
		break;
	case 0x931472D0:
		error("Bad dump of Maniac Mansion (Japan) is not supported!");
		break;
	default:
		error("ROM contents not recognized!");
		break;
	}
#ifdef	MAKE_LFLS
	memset(&lfl_index,0,sizeof(lfl_index));

	for (i = 0; lfls[i].num != -1; i++)
	{
		p_lfl lfl = &lfls[i];
		sprintf(fname,"%02i.LFL",lfl->num);
		if (!(output = fopen(fname,"wb")))
			error("Error: unable to create %s!",fname);
		debug("Creating %s...",fname);
		for (j = 0; lfl->entries[j] != NULL; j++)
		{
			p_resource entry = lfl->entries[j];
			switch (entry->type)
			{
			case RES_ROOM:
				lfl_index.room_lfl[entry - res_rooms] = lfl->num;
				lfl_index.room_addr[entry - res_rooms] = (unsigned short)ftell(output);
				break;
			case RES_COSTUME:
				lfl_index.costume_lfl[entry - res_costumes] = lfl->num;
				lfl_index.costume_addr[entry - res_costumes] = (unsigned short)ftell(output);
				break;
			case RES_SCRIPT:
				lfl_index.script_lfl[entry - res_scripts] = lfl->num;
				lfl_index.script_addr[entry - res_scripts] = (unsigned short)ftell(output);
				break;
			case RES_SOUND:
				lfl_index.sound_lfl[entry - res_sounds] = lfl->num;
				lfl_index.sound_addr[entry - res_sounds] = (unsigned short)ftell(output);
				break;
			default:
				break;
			}
			extract_resource(input,output,entry);
		}
		write_word(output,0xF5D1);
		fclose(output);
	}
	if (!(output = fopen("00.LFL","wb")))
		error("Error: unable to create index file!");
	debug("Creating 00.LFL...");
	write_word(output,0x4643);
	extract_resource(input,output,&res_globdata);
	for (i = res_globdata.length[ROMset]; i < 800; i++)
		write_byte(output,0);
	for (i = 0; i < (int)sizeof(lfl_index); i++)
		write_byte(output,((unsigned char *)&lfl_index)[i]);
	fclose(output);
#else	/* !MAKE_LFLS */
	dump_resource(input,"globdata.dmp",0,&res_globdata);
	for (i = 0; i < 40; i++)
		dump_resource(input,"roomgfx-%i.dmp",i,&res_roomgfx[i]);
	for (i = 0; i < 2; i++)
		dump_resource(input,"costumegfx-%i.dmp",i,&res_costumegfx[i]);
	for (i = 0; i < 55; i++)
		dump_resource(input,"room-%i.dmp",i,&res_rooms[i]);
	for (i = 0; i < 179; i++)
		dump_resource(input,"script-%i.dmp",i,&res_scripts[i]);
	for (i = 0; i < 82; i++)
		dump_resource(input,"sound-%i.dmp",i,&res_sounds[i]);
	for (i = 0; i < 25; i++)
		dump_resource(input,"costume-%i.dmp",i,&res_costumes[i]);
	for (i = 0; i < 2; i++)
		dump_resource(input,"costumepal-%i.dmp",i,&res_palettes[i]);
	for (i = 0; i < 2; i++)
		dump_resource(input,"cost_DD75-%i.dmp",i,&res_unk_DD75[i]);
	for (i = 0; i < 2; i++)
		dump_resource(input,"cost_DD7D-%i.dmp",i,&res_unk_DD7D[i]);
	for (i = 0; i < 2; i++)
		dump_resource(input,"cost_DD95-%i.dmp",i,&res_unk_DD95[i]);
	for (i = 0; i < 2; i++)
		dump_resource(input,"cost_DD9D-%i.dmp",i,&res_unk_DD9D[i]);
#endif	/* MAKE_LFLS */
	debug("All done!");

	return 0;
}
