#include <msp430.h>
#include "hd44780_4bit_lib.h"

/*
 * This code example uses 5 x 10 character and one line
 * to display the special characters show in table 4, ROM Code A00
 * in the hd44780 specification.
 * Note using the 5 x 10 characters you can only write to one line on
 * the display.
 *
 */

void main()
{
    WDTCTL = WDTPW | WDTHOLD; // Disable watchdog timer

    // according to spec give the display 10ms to come up to voltage
    __delay_cycles(10000 * CPUSPEED);

    // set up data length, number of lines and font
    // Note 1 line and 5 x 10 characters
    hd44780_init(N_1LINE, FONT_10);

    // if needed set up the cursor and blink
    // hd44780_send_command(HD44780_CMD_DISPLAY_ON_CURSOR);
    // hd44780_send_command(HD44780_CMD_DISPLAY_ON_CURSOR_BLINK);

    // set Increment direction and cursor direction
    hd44780_send_command(HD44780_CMD_INCREMENT);
    hd44780_send_command(HD44780_CMD_MOVE_CURSOR_RIGHT);

    // refer to table 4 ROM Code AOO
    // on how to convert upper 4 bits and
    // lower 4 bits to formulate the below characters

    hd44780_write_special_char(0xEB);
    hd44780_write_special_char(0xEB);
    hd44780_write_special_char(0xEB);

    hd44780_write_special_char(0xEF);
    hd44780_write_special_char(0xEF);
    hd44780_write_special_char(0xEF);

    hd44780_write_special_char(0xFF);
    hd44780_write_special_char(0xFF);
    hd44780_write_special_char(0xFF);

    // this character and the next
    // character takes up the full
    // 10 dots in height to print the
    // decending j and g

    hd44780_write_special_char(0xE7);
    hd44780_write_special_char(0xE7);
    hd44780_write_special_char(0xE7);

    hd44780_write_special_char(0xEA);
    hd44780_write_special_char(0xEA);
    hd44780_write_special_char(0xEA);

    hd44780_write_special_char(0xEB);
    hd44780_write_special_char(0xEB);
    hd44780_write_special_char(0xEB);

}
