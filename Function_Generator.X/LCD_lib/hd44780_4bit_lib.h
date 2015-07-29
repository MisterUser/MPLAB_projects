/*
 *	A hd44780 4 bit library for use with the TI MSP430
 *  hd44780_4bit_lib.h

 *  Copyright (C) 2014  Joe Pitz (j_pitz@yahoo.com)

 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.

 *  Special Thanks goes to nikosapi at
 *  http://nikosapi.org/w/index.php/MSP430_HD44780_Controller_Software
 *  I used the code provided at the above site as a model for my code
*/

#ifndef HD44780_4BIT_LIB_H
#define HD44780_4BIT_LIB_H

#include <stdint.h>
#include <string.h>
#include "hd44780_4bit_lib_config.h"

// Command definitions, see page 24 of the datasheet for more info

#define HD44780_CMD_CLEAR_DISPLAY               0x01
#define HD44780_CMD_RETURN_HOME                 0x02
#define HD44780_CMD_DECREMENT					0x04
#define HD44780_CMD_INCREMENT					0x06
#define HD44780_CMD_DISPLAY_SHIFT_ON            0x07
#define HD44780_CMD_DISPLAY_SHIFT_OFF           0x06
#define HD44780_CMD_DISPLAY_ON_CURSOR_BLINK     0x0F
#define HD44780_CMD_DISPLAY_ON_BLINK            0x0D
#define HD44780_CMD_DISPLAY_ON_CURSOR           0x0E
#define HD44780_CMD_DISPLAY_ON                  0x0C
#define HD44780_CMD_DISPLAY_OFF                 0x08
#define HD44780_CMD_DISPLAY_SHIFT_RIGHT         0x1C
#define HD44780_CMD_DISPLAY_SHIFT_LEFT          0x18
#define HD44780_CMD_MOVE_CURSOR_RIGHT           0x14
#define HD44780_CMD_MOVE_CURSOR_LEFT            0x10

#define FUNCTION_SET							0x20	// sets the Function set bit, DB5
//#define DL_8BITS								0x10	// sets data length to 8 bits
#define DL_4BITS								0x0		// sets data length to 4 bits
#define N_2LINE									0x8		// sets the number of lines to 2
//#define N_1LINE									0x0		// sets the number of lines to 1
//#define FONT_10									0x4		// sets the font to 5x10 dots
#define FONT_8									0x0		// sets the font to 5x8 dots

// constants used for __delay_cycles() function to calculate microsecond wait times
// CPUSPEED currently set for 1 MHZ
//#define CPU1MHZ									1
//#define CPU4MHZ									4
//#define CPU8MHZ									8
//#define CPU16MHZ								16
#define CPUSPEED								40

/*
 * HD44780 initialization function must be run before any other hd44780_*
 *	functions.
 *
 *	This function sets up the lcd display for the following hd44780 functions
 *	Data Length, display lines and font size.  The init function also clears the
 *	display in preperation of operation.

 *	The arguments passed to the hd44780_init() function as as follows:
 *	dispLines = either N_2LINE or N_1LINE
 *	fontSize = either FONT_10 or FONT_8

 *	return = function returns a void
*/

void hd44780_init(uint8_t dispLines, uint8_t fontSize);

/*
 * Send the HD44780 a command. Commands are #define'd above (HD44780_CMD_*).
 * Note: There is no need to check the busy flag before sending commands,
 * this is done for you in the function.
 */

void hd44780_send_command(uint8_t);

/*
 * sets DDRAM address for reading and writing to display
 * at a given row, column position
 */

void hd44780_setCursorPosition(uint8_t row, uint8_t col);


/* Write a single character to the display.
   Note: This function waits for the busy flag for you. */

void hd44780_write_char(char);

/*
 * write special characters, take a look at the table 4 (ROM Code: A00)
 * in the hd44780 hardware specification,  used to display any of the Japanese Katakana
 * characters or the lower case Greek characters.  Using this function you
 * pass the hex number that represents the binary value of the characters shown
 * in the above mentioned table
 */

void hd44780_write_special_char(uint8_t c);

/*
 * Write a null-terminated string of characters to the display. This
 * function simply loops over your string writing one character at a
 * time using hd44780_write_char().
*/

void hd44780_write_string(char*);

/*
 * anaimates drawing text to display from the left to right of the display, function parameters:
 * string to print to display
 * visible width of the number of characters your text will be  printed to, used for justitifcation
 * justify - 'L' for left justification, 'C' for center justification
 * 'R' for right justification
 * number of click cycles to wait after drawing each character
 */

//void hd44780_animateRight(char *str, int dispWidth, char justify, int pause);

/*
 * anaimates drawing text to display from the right to the left  of the display, function parameters:
 * string to print to display
 * visible width of the number of characters your text will be  printed to, used for justitifcation
 * justify - 'L' for left justification, 'C' for center justification
 * 'R' for right justification
 * number of click cycles to wait after drawing each character
 */

//void hd44780_animateLeft(char *str, int dispWidth, char justify, int pause);

/*
 * reads a character from DDRAM of the display based on the current cursor position
 */

//uint8_t hd44780_readByte();

/*
 * Returns the busy flag as reported by the HD44780.
 * Returns either 0x80 (busy) or 0x00 (not busy).
*/

//uint8_t hd44780_get_busy_flag();

void __delay_cycles(int cycles);

#endif // HD44780LIB_H
