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
#include "user.h"            /* variables/params used by user.c  */
#include "fft.h"             

/******************************************************************************/
/* Local Vars                                                             */
/******************************************************************************/
//notes on guitar ranging from drop C to A# on high e's highest fret
const uint16_t note_frequencies[47] __attribute__ ((aligned(sizeof(uint16_t)*64))) =
{
    65 ,69 ,73 ,78 ,82 ,87 ,92 ,98 ,
    104,110,117,123,131,139,147,156,
    165,175,185,196,208,220,233,247,
    262,277,294,311,329,349,370,392,
    415,440,466,494,523,554,587,622,
    659,698,740,784,831,880,932
};

const char note_letters[47] __attribute ((aligned(sizeof(char)*64)))=
{
    'C','c','D','d','E','F','f','G',
    'g','A','a','B','C','c','D','d',
    'E','F','f','G','g','A','a','B',
    'C','c','D','d','E','F','f','G',
    'g','A','a','B','C','c','D','d',
    'E','F','f','G','g','A','a'
};
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
    UART1TX(' ');//first character is discarded
    UART1TX('h');
    UART1TX('e');
    UART1TX('l');
    UART1TX('l');
    UART1TX('o');
    InitTimer1();
    ADC_Init();
}

void Isolate_Fund_Freqs(fractional* sigCmpx, uint16_t* freq_array)
{
    //E -> mag > 150 @82  (bin 21) -> get rid of bin 42 & 63 (reads as B)
    //A -> mag > 140 @110 (bin 28) -> X57
    //D -> mag > 80  @148 (bin 38) -> X76
    //G -> mag > 100 @196 (bin 50/51) -> X 101 * 114
    //B        > 80  @247 (bin 64)
    //E        > 11  @329 (bin 85)
    //A        > 5   @880 (bin 228)
    int i = 0;
    int freq_array_iter = 0;
   for(i=1; i< FFT_BLOCK_LENGTH/2;i++)
   {
       if(  (i<=70 && sigCmpx[i]>10)
          ||(i>70 && sigCmpx[i]>5))
       {
           freq_array[freq_array_iter++]=i;
       }
   }
   
}