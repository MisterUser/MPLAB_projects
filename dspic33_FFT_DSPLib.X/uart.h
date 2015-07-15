/* 
 * File:   uart.h
 * Author: Mr
 *
 * Created on June 16, 2015, 7:04 PM
 */

#ifndef UART_H
#define	UART_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#include "HardwareProfile.h"
#include <math.h>

unsigned char UART1RXRdy(void);
unsigned char UART1RX(void);
void UART1TX(char c);
void InitializeUART1(void);
void usart_print_decimal(unsigned long value);

#endif	/* UART_H */

