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
int	peakFrequencyBinMain = 0;				/* Declare post-FFT variables to compute the */
volatile unsigned long peakFrequency = 0;			/* frequency of the largest spectral component */
volatile unsigned long peakViewer = 0;
/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

int16_t main(void)
{
    char c = 'g';
    iPtr = &sigCmpx[0];//&inputSignal[0];
    doFilterFlag = 0;
     
//    setup_FFT_input(&sigCmpx[0]);
//	peakFrequency = freq_detect_FFT(&sigCmpx[0]);
//    peakViewer = peakFrequency;
    int i = 0;
	fractional *p_real = &sigCmpx[0].real ;
	fractcomplex *p_cmpx = &sigCmpx[0] ;


#ifndef FFTTWIDCOEFFS_IN_PROGMEM					/* Generate TwiddleFactor Coefficients */
	TwidFactorInit (LOG2_BLOCK_LENGTH, &twiddleFactors[0], 0);	/* We need to do this only once at start-up */
#endif

	for ( i = 0; i < FFT_BLOCK_LENGTH; i++ )/* The FFT function requires input data */
	{					/* to be in the fractional fixed-point range [-0.5, +0.5]*/
		*p_real = *p_real >>1 ;		/* So, we shift all data samples by 1 bit to the right. */
		*p_real++;			/* Should you desire to optimize this process, perform */
	}					/* data scaling when first obtaining the time samples */
						/* Or within the BitReverseComplex function source code */

	p_real = &sigCmpx[(FFT_BLOCK_LENGTH/2)-1].real ;	/* Set up pointers to convert real array */
	p_cmpx = &sigCmpx[FFT_BLOCK_LENGTH-1] ; /* to a complex array. The input array initially has all */
						/* the real input samples followed by a series of zeros */


	for ( i = FFT_BLOCK_LENGTH-1; i > 0; i-- ) /* Convert the Real input sample array */
	{					/* to a Complex input sample array  */
		(*p_cmpx).real = (*p_real--);	/* We will simpy zero out the imaginary  */
		(*p_cmpx--).imag = 0x0000;	/* part of each data sample */
	}
    (*p_cmpx).imag = 0x0000; //set first element's imag to 0
	/* Perform FFT operation */
#ifndef FFTTWIDCOEFFS_IN_PROGMEM
	FFTComplexIP (LOG2_BLOCK_LENGTH, &sigCmpx[0], &twiddleFactors[0], COEFFS_IN_DATA);
#else
	FFTComplexIP (LOG2_BLOCK_LENGTH, &sigCmpx[0], (fractcomplex *) __builtin_psvoffset(&twiddleFactors[0]), (int) __builtin_psvpage(&twiddleFactors[0]));
#endif

	/* Store output samples in bit-reversed order of their addresses */
	BitReverseComplex (LOG2_BLOCK_LENGTH, &sigCmpx[0]);

	/* Compute the square magnitude of the complex FFT output array so we have a Real output vetor */
	SquareMagnitudeCplx(FFT_BLOCK_LENGTH, &sigCmpx[0], &sigCmpx[0].real);

	/* Find the frequency Bin ( = index into the SigCmpx[] array) that has the largest energy*/
	/* i.e., the largest spectral component */
	VectorMax(FFT_BLOCK_LENGTH/2, &sigCmpx[0].real, &peakFrequencyBinMain);

	/* Compute the frequency (in Hz) of the largest spectral component */
	peakFrequency = peakFrequencyBinMain*(SAMPLING_RATE/FFT_BLOCK_LENGTH);
    peakViewer = peakFrequency;
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
//        if (timer_expired && Counter == FLASH_RATE )
//		{
//			//LATA = ~LATA;
//			//LATB = ~LATB;
//            LD1_O = ~LD1_O;
//			Counter = 0;
//			timer_expired = 0;
//		}

        if (doFilterFlag)
        {
            setup_FFT_input(&sigCmpx[0]);
            peakFrequency = freq_detect_FFT(&sigCmpx[0]);
            doFilterFlag = 0;
            peakViewer = peakFrequency;
            iPtr = &sigCmpx[0];//&inputSignal[0];
            AD1CON1bits.ADON = 1;
            T3CONbits.TON = 1;
            //UART1TX('\r');
            UART1TX('F');
            UART1TX('r');
            UART1TX('e');
            UART1TX('q');
            UART1TX(':');
            
            usart_print_decimal(peakFrequency);
            //UART1TX('\r');
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
