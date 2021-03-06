#include "uart.h"

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
    
    //UART can be placed on any RPx pin
	//we need to configure it for RP14/RP15 to use the FTDI usb->serial converter
	//you could also output one (or both) of the two available UARTS to the I/O header
	//assign pin 14 to the UART1 RX input register
	//RX RP6 (input) -> pin 15
	U1RXR_I = 6;
	//assign UART1 TX function to the pin 15 output register
	//TX RP7 (output) -> pin 16
	RP7_O=U1TX_O;
    
	//setup UART
    U1BRG = 85;//86@80mhz, 85@79.xxx=115200
    U1MODE = 0; //clear mode register
    U1MODEbits.BRGH = 1; //use high percison baud generator
    U1STA = 0;	//clear status register
    U1MODEbits.UARTEN = 1; //enable the UART RX
    IFS0bits.U1RXIF = 0;  //clear the receive flag
}

void usart_print_decimal(unsigned long value)
{
  int powOfTen = 5;// numDigits-1;
/*
  if(value>=10^(numDigits-1))
  {
     powOfTen = numDigits -1;
  }
  else if(value>=10^(numDigits-2))
  {
     powOfTen = numDigits -2;
  }
  else if(value>=10^(numDigits-3))
  {
     powOfTen = numDigits -3;
  }
  else powOfTen = 0;
*/

  long denom;
  int valreached = 0;
  while(powOfTen >=0)
  {
    denom=pow(10,powOfTen);
    if(value >=denom || valreached)
	{
	   UART1TX((char)((value/denom)+'0'));
	   valreached = 1;
	}
	value = value%denom;
	powOfTen--;
  } 
   
}



