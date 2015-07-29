/*
	A hd44780 4 bit data length library for use with the TI MSP430
    hd44780lib_4bit_config.h

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

#ifndef HD44780LIB_4BIT_CONFIG_H
#define HD44780LIB_4BIT_CONFIG_H

#include <p24Hxxxx.h>      /* Pin definitions */

#define HD44780_RS          LATBbits.LATB9
#define HD44780_DATA4       LATBbits.LATB10
#define HD44780_DATA5       LATBbits.LATB11
#define HD44780_DATA6       LATBbits.LATB12
#define HD44780_DATA7       LATBbits.LATB13
#define HD44780_EN          LATBbits.LATB14

#define HD44780_DATA_OFFSET 10

#endif // HD44780LIB_4BIT_CONFIG_H
