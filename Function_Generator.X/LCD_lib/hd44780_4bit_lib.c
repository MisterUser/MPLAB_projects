/*
	A hd44780 library for use with the TI MSP430
    hd44780_4bit_lib_.c

    Copyright (C) 2014  Joe Pitz (j_pitz@yahoo.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Special Thanks goes to nikosapi at
    http://nikosapi.org/w/index.php/MSP430_HD44780_Controller_Software
    I used the code provided at the above site as a model for my code
*/

/*
 * The port is configured as follows, if you are using different port pins
 * just change the constants to use the proper pre-processor macros, i.e. BIT0 etc.
 *
 * HD44780_RS = P1.0
 * HD44780_RW = P1.1
 * HD44780_EN = P1.2
 * HD44780_DATA (DB7 (BIT7) = P1.7, DB6 (BIT6) = P1.6, DB5 (BIT5) = P1.5, DB4 (BIT4) = P1.4)
 *
 * For details on you LCD check the specification for your display
 * for example I have the ERM2004-1 display so I searched for this specification data sheet
 *
 * For more details on the HD44780 Control/Driver please refer to:
 * https://www.sparkfun.com/datasheets/LCD/HD44780.pdf
 *
 * If for any reason the above link is bad, just search for HD44780 datasheet on the web
 *
 */




#include "hd44780_4bit_lib.h"

// Internal functions
//static uint8_t _read_4bit();
//static uint8_t _read_8bit();
static void _write_4bit(uint8_t);
static void _write_8bit(uint8_t);
//static void _hd44780_writeStrPause(char* str, long waitCnt);
//static void _hd44780_writeStrPauseReverse(char *str, long waitCnt);


void hd44780_init(uint8_t dispLines, uint8_t fontSize)
{
	uint8_t fsByte;

//    HD44780_PORTDIR = HD44780_RS | HD44780_RW | BIT3 | HD44780_EN | HD44780_8DATA;
//    HD44780_PORT &= ~(HD44780_RS | HD44780_RW | BIT3 | HD44780_EN | HD44780_8DATA);

    fsByte = (FUNCTION_SET | DL_4BITS | dispLines | fontSize);

    // _write_4bit(0x02);
    _write_8bit(fsByte);

    // We can only set the number of lines upon initialization (see
    // datasheet, page 27). The status bit cannot be checked otherwise the
    // controller will ignore this command.
    // _write_8bit(line_mode)
    ;
    hd44780_send_command(HD44780_CMD_DISPLAY_ON);
    //hd44780_send_command(HD44780_CMD_MOVE_CURSOR_RIGHT);
    hd44780_send_command(HD44780_CMD_DISPLAY_ON_CURSOR_BLINK);
    hd44780_send_command(HD44780_CMD_RETURN_HOME);
    hd44780_send_command(HD44780_CMD_CLEAR_DISPLAY);
}

/*
 * Sends the function set byte to setup data length, number of
 * lines and font size
 */

void hd44780_send_command(uint8_t c)
{
    HD44780_RS=0;
    _write_8bit(c);
}

void hd44780_write_special_char(uint8_t c)
{
    
    HD44780_RS = 1;
    _write_8bit(c);
    HD44780_RS = 0;
}

void hd44780_write_char(char c)
{
    HD44780_RS = 1;
    _write_8bit(c);
    HD44780_RS = 0;
}

void hd44780_write_string(char *str)
{
    while (*str != '\0')
    {
        hd44780_write_char(*str++);
    }
}

/*
// used by anaimate functions to print character at a time
// waiting number of cycles between each character
void _hd44780_writeStrPause(char *str, long waitCnt)
{
	long cnt;

    while (*str != '\0')
    {
        hd44780_write_char(*str++);
        for(cnt = 0; cnt < waitCnt; cnt++)
        {
        	__delay_cycles(1 * CPUSPEED);
        }
    }
}
*/

/*
// used by animate functions to print character at a time
// waiting number of cycles between each character
// reverses string so printing can be performed right to left
//void _hd44780_writeStrPauseReverse(char *str, long waitCnt)
//{
//
//	long cnt;
//	int end;
//	int pos;
//
//	end = strlen(str) -1;
//
//	for (pos = end; pos >= 0; --pos)
//	{
//		hd44780_write_char(str[pos]);
//
//		for(cnt = 0; cnt < waitCnt; cnt++)
//		{
//			__delay_cycles(1 * CPUSPEED);
//		}
//	}
//}
*/

/*
// prints characters from left to right of display
void hd44780_animateRight(char *str, int dispWidth, char justify, int pause)
{
	int len = strlen(str);
	int pad = (dispWidth - len);
	int cnt;

	if (pad < 0)
	{
		pad = 0;
	}

	if (justify == 'C')
	{
		for(cnt = 0; cnt < (pad/2); cnt++)
		{
			hd44780_write_char(' ');
		}
	}

	if (justify == 'R')
	{
		for(cnt = 0; cnt < pad; cnt++)
		{
			hd44780_write_char(' ');
		}
	}

	_hd44780_writeStrPause(str, pause);
}
*/

/*
// prints characters from right to left on the display
void hd44780_animateLeft(char *str, int dispWidth, char justify, int pause)
{
	int len = strlen(str);
	int pad = (dispWidth - len);
	int cnt;

	if (pad < 0)
	{
		pad = 0;
	}

	if (justify == 'C')
	{
		for(cnt = 0; cnt < (pad/2); cnt++)
		{
			hd44780_write_char(' ');
		}
	}

	if (justify == 'L')
	{
		for(cnt = 0; cnt < pad; cnt++)
		{
			hd44780_write_char(' ');
		}
	}

	_hd44780_writeStrPauseReverse(str, pause);
}
*/

/*
uint8_t hd44780_get_busy_flag()
{
    HD44780_PORT &= ~(HD44780_RS | HD44780_4DATA);
    return _read_8bit() & 0x80;
}
*/

/*
// Read an 8bit value using two calls to _read_4bit()
static uint8_t _read_8bit()
{
    uint8_t data;
    HD44780_PORTDIR &= ~HD44780_4DATA;
    HD44780_PORT |= HD44780_RW;
    data = _read_4bit() << 4;
    data |= _read_4bit();
    return data;
}
*/

// sets cursor position for characters can be read or written
void hd44780_setCursorPosition(uint8_t row, uint8_t col)
{
	 uint8_t address;

	 //
	 // construct address from (row, col) pair
	 //
	 if (row == 0)
	 {
		 address = 0;
	 }
	 else
	 {
		 address = 0x40;
	 }

	 address |= (0x80 | col);
	 _write_8bit(address);
}

/*
// reads characters from display, based on current cursor position
uint8_t hd44780_readByte()
{
	uint8_t data;

	 while (hd44780_get_busy_flag());

	HD44780_PORT |= (HD44780_RW | HD44780_RS);

	__delay_cycles(1 * CPUSPEED);

	data = _read_4bit() << 4;
	data |= _read_4bit();
	HD44780_PORT &= ~HD44780_RS;

	return data;
}
*/

/*
// Read a 4bit data value according to the timing characteristics and
// timing diagrams listed on page 49 and page 58 of the datasheet.
// We assume the MCLK frequency is 16MHz that way this code will also
// work on slower chips without any problems.
static uint8_t _read_4bit()
{
    // no need to wait 60ns for t_AS, the function call provides the delay
    HD44780_PORT |= HD44780_EN;

    // Need to wait 360ns for data lines to stabilize the data lines (t_DDR)
    __delay_cycles(1 * CPUSPEED);

    uint8_t data = HD44780_PORTIN >> HD44780_DATA_OFFSET; // Data is valid, read 4bits
    HD44780_PORT &= ~HD44780_EN;

    // Satisfy T_cycE requirement (this is probably overkill)
    __delay_cycles(1 * CPUSPEED);

    return data;
}
*/

// Writes an 8bit value on the bus using _write_4bit()
static void _write_8bit(uint8_t data)
{
    //Don't need the following two lines because never read, so exit only!
    //HD44780_PORTDIR |= HD44780_4DATA;
    //HD44780_PORT &= ~HD44780_RW;
    _write_4bit(data>>4);
    _write_4bit(data & 0x0f);
}

// Does a 4bit write (the 4 MSB are ignored)
// Assuming we're running at 16MHz (~62ns per clock)
// Timing characteristics and timing diagrams are available on page 49 and
// page 58 of the datasheet

static void _write_4bit(uint8_t data)
{
    // no need to wait 60ns for t_AS, the function call provides the delay
    HD44780_EN =1;
    LATB |= data << HD44780_DATA_OFFSET;

    // Satisfy the PW_EH (hold EN for 450ns)
    // and t_DSW requirement (data set-up 195ns)
    __delay_cycles(1 * CPUSPEED);

    HD44780_EN=0;

    // The data hold t_H is 10ns, no extra delay is needed
    LATB &= 0b1100001111111111; //clear data lines

    // Delay required to satisfy t_cycE parameter (min 1000ns for 1 EN cycle)
    __delay_cycles(1 * CPUSPEED);
}

void __delay_cycles(int cycles)
{
    int i = 0,j;
    for(i;i<cycles;i++)
    {
        j=i;
    }
}