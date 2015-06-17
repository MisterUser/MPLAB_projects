#ifndef USER_H
#define	USER_H


#include "uart.h"
#include "timers.h"
#include "ADC.h"

/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

/* TODO Application specific user parameters used in user.c may go here */

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

void InitApp(void);         /* I/O and Peripheral Initialization */

#endif	/* USER_H */