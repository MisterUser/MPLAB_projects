#include <msp430.h>
#include "hd44780_4bit_lib.h"
#include <string.h>

/*
 * This code example show how to read text from DDRAM, in this example we will write some text to the display
 * then read the text and then write the read text back to the display.
 */

void main()
{
    WDTCTL = WDTPW | WDTHOLD; // Disable watchdog timer

    uint8_t rtn = 0;
    uint8_t readStr[80];
    uint8_t *pint;
    unsigned int cnt = 0;
    unsigned int charCnt = 0;

    // according to spec give the display 10ms to come up to voltage
    __delay_cycles(10000 * CPUSPEED);

    // set up data length, number of lines and font
    // notice we have setup the display for 2 lines and using the 5 x 8 character size
    hd44780_init(N_2LINE, FONT_8);

   // turn display on and clear the display
   // and set cursor to home position 0x00
   // hd44780_send_command(HD44780_CMD_DISPLAY_ON);
   // hd44780_send_command(HD44780_CMD_RETURN_HOME);
   // hd44780_send_command(HD44780_CMD_CLEAR_DISPLAY);

   hd44780_setCursorPosition(0, 0);
   hd44780_write_string("this");

   hd44780_setCursorPosition(0, 0);

   pint = readStr;

   // reads characters from display
   while(1)
   {
	   rtn = hd44780_readByte();
	   if (rtn == ' ')
	   {
		   break;
	   }

	   *pint = rtn;
	   pint++;
	   charCnt++;
   }


   // writes read characters back to display
   for(cnt = 0; cnt < charCnt; cnt++)
   {
	   hd44780_write_special_char(readStr[cnt]);
   }

   hd44780_send_command(HD44780_CMD_RETURN_HOME);
}
