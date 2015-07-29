#include <msp430.h>
#include "hd44780_4bit_lib.h"
#include <string.h>

/*
 * This code example writes text to the screen using the
 * Cursor Increment, Decrement and Cursor Left and Right
 * commands, along with some functions that anaimate the
 * printing of characters to the display
 * I was going to use timer events for the character
 * wait time, but I wanted the code to be non CPU specific.
 */

void main()
{
    WDTCTL = WDTPW | WDTHOLD; // Disable watchdog timer

    // according to spec give the display 10ms to come up to voltage
    __delay_cycles(10000 * CPUSPEED);

    // set up data length, number of lines and font
    // notice we have setup the display for 2 lines and using the 5 x 8 character size
    hd44780_init(N_2LINE, FONT_8);

    // if needed set up the cursor
    // hd44780_send_command(HD44780_CMD_DISPLAY_ON_CURSOR);
    // hd44780_send_command(HD44780_CMD_DISPLAY_ON_CURSOR_BLINK);

	hd44780_send_command(HD44780_CMD_INCREMENT);
	hd44780_send_command(HD44780_CMD_MOVE_CURSOR_RIGHT);

	hd44780_setCursorPosition(0, 0);

	// The anaimate functions take three parameters:
	// The text string to print
	// The visible width of the display in characters
	// The justification, 'L' for Left justification
	// 'C' for centered text
	// 'R' for right jusificatiopn
	// The number of clock cycles you want to wait after
	// each character is printed

	hd44780_animateRight("This is Right", 20, 'C', 2500);

	hd44780_send_command(HD44780_CMD_DECREMENT);
	hd44780_send_command(HD44780_CMD_MOVE_CURSOR_LEFT);
	hd44780_setCursorPosition(1, 19);

	hd44780_animateLeft("This is Left", 20, 'C', 2500);

	hd44780_send_command(HD44780_CMD_INCREMENT);
	hd44780_send_command(HD44780_CMD_MOVE_CURSOR_RIGHT);
	hd44780_send_command(HD44780_CMD_RETURN_HOME);
}
