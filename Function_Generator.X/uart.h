/* 
 * File:   uart.h
 * Author: Mr
 *
 */

#ifndef UART_H
#define	UART_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#include <p24Hxxxx.h>      /* Pin definitions */
#include <math.h>

#define U1RXR_I RPINR18bits.U1RXR
#define RP7_O RPOR3bits.RP7R
#define U1TX_O		3

unsigned char UART1RXRdy(void);
unsigned char UART1RX(void);
void UART1TX(char c);
void InitializeUART1(void);
void usart_print_decimal(unsigned long value);

#endif	/* UART_H */

