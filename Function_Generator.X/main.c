/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

/* Device header file */
#include <xc.h>
#include <p24Hxxxx.h>      /* Pin definitions */
#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp              */
#include "uart.h"
#include "LCD_lib/hd44780_4bit_lib.h"


/******************************************************************************/
/* Clock Setup                                                                */
/******************************************************************************/

_FOSCSEL(FNOSC_FRCPLL);								
_FOSC(FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMD_NONE);
_FWDT(FWDTEN_OFF);

/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/

/* i.e. uint16_t <variable_name>; */

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

int16_t main(void)
{
    char c = 'g';
    
    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* Initialize IO ports and peripherals */
    InitApp();

    /* TODO <INSERT USER APPLICATION CODE HERE> */

    while(1)
    {
        if(UART1RXRdy()==1){//check for data waiting
			c=UART1RX();//get the character
			if(c=='0')
            {
                T1CONbits.TON = ~T1CONbits.TON;  /* enable Timer 1 and start the count */
                if(!T1CONbits.TON) //if timer is off, turn off LED
                {
                    UART1TX('T');
                    UART1TX('1');
                    UART1TX('-');
                    UART1TX('O');
                    UART1TX('F');
                    UART1TX('F');
                    UART1TX('\n');
                    UART1TX('\r');
                    MSLED_O = 0;
                }
                else
                {
                    UART1TX('T');
                    UART1TX('1');
                    UART1TX('-');
                    UART1TX('O');
                    UART1TX('N');
                    UART1TX('\n');
                    UART1TX('\r');
                    MSLED_O=1;
                }
			}
            else UART1TX(c);//echo the character back
  		}
    }
}
