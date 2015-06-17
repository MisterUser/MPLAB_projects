/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC33E__)
    	#include <p33Exxxx.h>
    #elif defined(__dsPIC33F__)
    	#include <p33Fxxxx.h>
    #endif
#endif

#include <p33Fxxxx.h>
#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */
#include <dsp.h>
#include "fft.h"
#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp              */
#include "uart.h"
#include "timers.h"
#include "ADC.h"

/******************************************************************************/
/* Clock Setup                                                                */
/******************************************************************************/

_FOSCSEL(FNOSC_FRCPLL);								
_FOSC(FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMD_NONE);
_FWDT(FWDTEN_OFF);

/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/
#define FLASH_RATE 50						// smaller value yields faster rate (1-100)
#define FOREVER 1

//fractional inputSignal[NUMSAMP];
fractional* iPtr;
unsigned int doFilterFlag;

/* Extern definitions */
extern fractcomplex sigCmpx[FFT_BLOCK_LENGTH] 		/* Typically, the input signal to an FFT  */
__attribute__ ((section (".ydata, data, ymemory"), 	/* routine is a complex array containing samples */
aligned (FFT_BLOCK_LENGTH * 2 *2)));      		/* of an input signal. For this example, */
							/* we will provide the input signal in an */
							/* array declared in Y-data space. */
extern unsigned int Counter;
extern volatile unsigned int timer_expired;
unsigned long peakFrequency = 0;			/* frequency of the largest spectral component */

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

int16_t main(void)
{
    char c = 'g';
    iPtr = &sigCmpx[0];//&inputSignal[0];
    doFilterFlag = 0;
     
    setup_FFT_input(&sigCmpx[0]);
	peakFrequency = freq_detect_FFT(&sigCmpx[0]);
    //unsigned int counterNum1;
    /* Configure the oscillator for the device */
    ConfigureOscillator();
    
    /* Initialize IO ports and peripherals */
    InitApp();

    

    UART1TX(' ');//first character is discarded
    UART1TX('h');
    UART1TX('e');
    UART1TX('l');
    UART1TX('l');
    UART1TX('o');

    while(FOREVER)
    {
        if (timer_expired && Counter == FLASH_RATE )
		{
			//LATA = ~LATA;
			//LATB = ~LATB;
            LD1_O = ~LD1_O;
			Counter = 0;
			timer_expired = 0;
		}

        if (doFilterFlag)
        {
            setup_FFT_input(&sigCmpx[0]);
            peakFrequency = freq_detect_FFT(&sigCmpx[0]);
            doFilterFlag = 0;
            iPtr = &sigCmpx[0];//&inputSignal[0];
        }
		
        
        //-----------end Microchip code-------------//
        
//        for(counterNum1 = 0;counterNum1<10000;counterNum1++)
//        {
//            counterNum1=counterNum1;
//        } //random delay)
//        LD1_O = 1 - LD1_O;
		if(UART1RXRdy()==1){//check for data waiting
			c=UART1RX();//get the character
			if(c=='0'){
				//LATA &=(~0b11100000000);
//				LD1_O = 0;
//				IO1_O=0;
//				SD_O = 0;
                T1CONbits.TON = ~T1CONbits.TON;          /* enable Timer 1 and start the count */
                if(!T1CONbits.TON) //if timer is off, turn off LED
                {
                    LD1_O = 0;
                }
			}
			UART1TX(c);//echo the character back
  		}
    }
}
