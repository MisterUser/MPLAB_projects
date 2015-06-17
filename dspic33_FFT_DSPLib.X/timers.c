#include "timers.h"   

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
	timer_expired = 1;				/* flag */
	Counter++;						/* keep a running counter */
 	IFS0bits.T1IF = 0;				/* reset timer interrupt flag	*/
}