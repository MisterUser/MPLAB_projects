//The compiled firmware can be used 'as-is' with the ds30 Loader bootloader
//you may need to locate your own p33FJ128gp204.gld file.
//   It's in the C30 compiler directory:
/* \C30\support\dsPIC33F\gld\ */

#include "HardwareProfile.h"

/*  From web example
//it's important to keep configuration bits that are compatibale with the bootloader
//if you change it from the internall/PLL clock, the bootloader won't run correctly
_FOSCSEL(FNOSC_FRCPLL)		//INT OSC with PLL (always keep this setting)
_FOSC(OSCIOFNC_OFF & POSCMD_NONE)	//disable external OSC (always keep this setting)
_FWDT(FWDTEN_OFF)				//watchdog timer off
_FICD(JTAGEN_OFF & 0b11);//JTAG debugging off, debugging on PG1 pins enabled
*/

/* From Microchip example*/
_FOSCSEL(FNOSC_FRCPLL);								
_FOSC(FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMD_NONE);
_FWDT(FWDTEN_OFF);


#define PERIOD  0xFFFF						// sets the default interval flash rate
#define FLASH_RATE 50						// smaller value yields faster rate (1-100)
#define FOREVER 1

/* function prototypes */
void InitTimer1();							 

/* globals */
unsigned int Counter = 0;
volatile unsigned int timer_expired;
//-----------end Microchip code-------------//



unsigned char UART1RXRdy(void);
unsigned char UART1RX(void);
void UART1TX(char c);
void InitializeUART1(void);
unsigned int counterNum1;

int main(void){ //main function, execution starts here
	char c = 'g';

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
	TRISBbits.TRISB6 = 1;
	
    
	//setup internal clock for 80MHz/40MIPS
	//7.37/2=3.685*43=158.455/2=79.2275
	CLKDIVbits.PLLPRE=0; // PLLPRE (N2) 0=/2 
	PLLFBD=41; //pll multiplier (M) = +2
	CLKDIVbits.PLLPOST=0;// PLLPOST (N1) 0=/2
    while(!OSCCONbits.LOCK);//wait for PLL ready
    
    InitTimer1();

	//uart
	//UART can be placed on any RPx pin
	//we need to configure it for RP14/RP15 to use the FTDI usb->serial converter
	//you could also output one (or both) of the two available UARTS to the I/O header
	//assign pin 14 to the UART1 RX input register
	//RX PR14 (input)
	U1RXR_I = 6;
	//assign UART1 TX function to the pin 15 output register
	//TX RP15 (output)
	RP7_O=U1TX_O;

	//InitializeUART1();	
	//setup UART
    U1BRG = 85;//86@80mhz, 85@79.xxx=115200
    U1MODE = 0; //clear mode register
    U1MODEbits.BRGH = 1; //use high percison baud generator
    U1STA = 0;	//clear status register
    U1MODEbits.UARTEN = 1; //enable the UART RX
    IFS0bits.U1RXIF = 0;  //clear the receive flag

    /*
	//setup LEDs
//	SD_TRIS = 0; //set pin direction to output
//	IO1_TRIS = 0;
	LD1_TRIS = 0;
//	SD_O = 1;	//set all pins high (LED on)
	LD1_O = 0;
//	IO1_O=1;
 */
    UART1TX(' ');//first character is discarded
    UART1TX('h');
    UART1TX('e');
    UART1TX('l');
    UART1TX('l');
    UART1TX('o');
    
    
    while(FOREVER){//never ending loop

        
        /*Microchip code*/
        if (timer_expired && Counter == FLASH_RATE )
		{
			LATA = ~LATA;
			LATB = ~LATB;
			Counter = 0;
			timer_expired = 0;
		}

		// or do something else
		Nop();
		Nop();
		Nop();
        //-----------end Microchip code-------------//
        
//        for(counterNum1 = 0;counterNum1<10000;counterNum1++)
//        {
//            counterNum1=counterNum1;
//        } //random delay)
//        LD1_O = 1 - LD1_O;
		if(UART1RXRdy()==1){//check for data waiting
			c=UART1RX();//get the character
			/*if(c=='0'){
				//LATA &=(~0b11100000000);
				LD1_O = 0;
				IO1_O=0;
				SD_O = 0;
			}else if(c=='1'){
				LD1_O = 1;
			}else if(c=='2'){
				IO1_O=1;
			}else if(c=='3'){
				SD_O = 1;
			}*/
			UART1TX(c);//echo the character back
  		}	
          
	}
}

//is data available in RX buffer?
unsigned char UART1RXRdy(void){
    return U1STAbits.URXDA;
}

//get a byte from UART
unsigned char UART1RX(void){

    while(U1STAbits.URXDA == 0);
	return U1RXREG;
}

//add byte to buffer, pause if full
//uses PIC 4 byte UART FIFO buffer
void UART1TX(char c){
	static unsigned char discard=1;//discard the first TX byte before enabling UART TX

	if(discard){//discard the first few bytes to avoid back powering the FTDI chip
		discard--;
		return;
	}
	if(U1STAbits.UTXEN ==0)U1STAbits.UTXEN = 1; //enable UART TX
	while(U1STAbits.UTXBF == 1); //if buffer is full, wait
    U1TXREG = c;
}

//Initialize the terminal UART
void InitializeUART1(void){
	//setup UART
    U1BRG = 85;//86@80mhz, 85@79.xxx=115200
    U1MODE = 0;
    U1MODEbits.BRGH = 1;
    U1STA = 0;
    U1MODEbits.UARTEN = 1;
    IFS0bits.U1RXIF = 0;
}



/*---------------------------------------------------------------------
  Function Name: InitTimer1
  Description:   Initialize Timer1 for 1 second intervals
  Inputs:        None
  Returns:       None
-----------------------------------------------------------------------*/
void InitTimer1( void )
{
	

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

