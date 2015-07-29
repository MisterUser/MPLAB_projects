#include <msp430.h>
#include "hd44780_4bit_lib.h"

/*
 * This code example uses the hd44780_setCursorPosition() functioh
 * to set the starting cursor position using row, col information
 * This cursor info is then written to the DDRAM address.
 * Using this method it is very easy to postion text on the dissplay
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

    // set Increment direction and cursor direction
    hd44780_send_command(HD44780_CMD_INCREMENT);
    hd44780_send_command(HD44780_CMD_MOVE_CURSOR_RIGHT);

    hd44780_send_command(HD44780_CMD_RETURN_HOME);
    hd44780_send_command(HD44780_CMD_CLEAR_DISPLAY);

    // The display I am testing with is a two line 40 character display
    // The display shows 20 characters per line so each line wraps twice
    // for what looks like a 4 line display.
    // On line one if you write past the 20th character the text shows up on what appears
    // to be the third line.
    // Same goes for the second line, any character past the 20th character is written
    // to what appears to be the fourth line.
    //

    hd44780_setCursorPosition(0, 0);
    hd44780_write_string("0123456789");

    hd44780_setCursorPosition(0, 10);
    hd44780_write_string("0123456789");

    hd44780_setCursorPosition(0, 20);
    hd44780_write_string("11234567890123456789");

    hd44780_setCursorPosition(1, 0);
    hd44780_write_string("21234567890123456789");

    hd44780_setCursorPosition(1, 20);
    hd44780_write_string("31234567890123456789");

    hd44780_send_command(HD44780_CMD_RETURN_HOME);

}
