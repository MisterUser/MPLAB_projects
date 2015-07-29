/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__PIC24E__)
    	#include <p24Exxxx.h>
    #elif defined (__PIC24F__)||defined (__PIC24FK__)
	#include <p24Fxxxx.h>
    #elif defined(__PIC24H__)
	#include <p24Hxxxx.h>
    #endif
#endif

#include <stdint.h>          /* For uint32_t definition */
#include <stdbool.h>         /* For true/false definition */

#include "user.h"            /* variables/params used by user.c */
#include "timers.h"
#include "uart.h"



/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */

/* TODO Initialize User Ports/Peripherals/Project here */

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
    //TRISBbits.TRISB3 = 1; //AN5 on pin 7 as input for ADC pos
    //TRISAbits.TRISA1 = 1; //AN1 on pin 3 as input for ADC neg
    
    //For LCD
    TRISBbits.TRISB9=0;
    TRISBbits.TRISB10=0;
    TRISBbits.TRISB11=0;
    TRISBbits.TRISB12=0;
    TRISBbits.TRISB13=0;
    TRISBbits.TRISB14=0;
    
    //Function Pins
    TRISBbits.TRISB0=0;
    TRISBbits.TRISB1=0;

    /* Setup LEDs */
    MSLED_TRIS = 0;
    MSLED_O = 0;
    
    /* Initialize peripherals */
    InitializeUART1();
    UART1TX(' ');//first character is discarded
    UART1TX('h');
    UART1TX('e');
    UART1TX('l');
    UART1TX('l');
    UART1TX('o');
    UART1TX('\n');
    UART1TX('\r');
    InitTimer1();
    //ADC_Init();
}

void update_O1()
{
    
}