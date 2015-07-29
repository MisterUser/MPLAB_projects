#include "timers.h"   
#include "user.h"

/******************************************************************************/
/* File-scope Variables                                                       */
/******************************************************************************/

uint8_t fourBitSine[16]={8,11,13,14,15,14,12,10,7,4,2,1,0,1,3,5};
uint8_t O1Ptr=0;
unsigned int Counter;
volatile unsigned int timer_expired;

/*---------------------------------------------------------------------
  Function Name: InitTimer1
  Description:   Initialize Timer1 for 1 second intervals
  Inputs:        None
  Returns:       None
-----------------------------------------------------------------------*/
void InitTimer1( void )
{
	Counter = 0;
    timer_expired = 0;
    
	T1CON = 0;					/* ensure Timer 1 is in reset state */
 	IFS0bits.T1IF = 0;          /* reset Timer 1 interrupt flag */
	IPC0bits.T1IP = 4;          /* set Timer1 interrupt priority level to 4 */
 	IEC0bits.T1IE = 1;          /* enable Timer 1 interrupt */
	PR1 = PERIOD;               /* set Timer 1 period register */
	T1CONbits.TCKPS1 = 0;       /* select Timer1 Input Clock Prescale */
	T1CONbits.TCS = 0;          /* select external timer clock */
	T1CONbits.TON = 1;          /* enable Timer 1 and start the count */ 
	
}


/*---------------------------------------------------------------------
  Function Name: _T1Interrupt
  Description:   Timer1 Interrupt Handler
  Inputs:        None
  Returns:       None
-----------------------------------------------------------------------*/
void __attribute__((interrupt, auto_psv)) _T1Interrupt( void )
{
//	timer_expired = 1;				/* flag */
//	Counter++;						/* keep a running counter */
//    MSLED_O = ~MSLED_O;                 //toggling the LED every TIM1 Period
 	
    uint8_t current_4_bit_value;
    
    //Read value of fourBitSine and concurrently increment the pointer
    current_4_bit_value = fourBitSine[O1Ptr++];
    if(O1Ptr>15){O1Ptr=0;}

    O1_Bit4=!!(current_4_bit_value & 8);
    O1_Bit3=!!(current_4_bit_value & 4);
    O1_Bit2=!!(current_4_bit_value & 2);
    O1_Bit1=!!(current_4_bit_value & 1);
    
    IFS0bits.T1IF = 0;				/* reset timer interrupt flag	*/
}

void __attribute__((interrupt, auto_psv)) _T3Interrupt( void )
{
	//timer_expired = 1;				/* flag */
	//Counter++;						/* keep a running counter */
    //MSLED_O = ~MSLED_O;                 //toggling the LED every TIM3 Period
 	IFS0bits.T3IF = 0;				/* reset timer interrupt flag	*/
}