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

#include <stdint.h>          /* For uint16_t definition                       */
#include <stdbool.h>         /* For true/false definition                     */
#include "user.h"            /* variables/params used by user.c               */

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */

void InitApp(void)
{
    /* Setup analog functionality and port direction */
    /*
     * PIC pins start as analog inputs to protect any sensitive 
     * externally connected components from unwanted pin output. 
     * To use a pin for digital input and output, we need to 
     * disable the analog functions on a pin by writing 1 to 
     * the corresponding bits of the AD1PCFGL register.
     */
	AD1PCFGL = 0xFFFF; //digital pins

    /* 	Initialize ports */
	LATA  = 0x0000; 				// set latch levels
	TRISA = 0x0000; 				// set IO as outputs
	LATB  = 0x0000; 				// set latch levels
	TRISB = 0x0000; 				// set IO as outputs
	TRISBbits.TRISB6 = 1; //UART RX INPUT
    TRISBbits.TRISB3 = 1; //AN5 on pin 7 as input for ADC pos
    TRISAbits.TRISA1 = 1; //AN1 on pin 3 as input for ADC neg
    
    
    /* Setup LEDs */
    LD1_TRIS = 0;
    LD1_O = 0;
    
    /* Initialize peripherals */
    InitializeUART1();
    InitTimer1();
    ADC_Init();
}
