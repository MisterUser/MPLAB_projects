/* 
 * File:   timers.h
 * Author: Mr
 *
 * Created on July 28, 2015, 3:34 PM
 */

#ifndef TIMERS_H
#define	TIMERS_H

#ifdef	__cplusplus
extern "C" {
#endif

    
#include <p24Hxxxx.h>      /* Pin definitions */
#include <stdint.h>        /* Includes uint16_t definition*/

/* globals */
#define PERIOD  0xFFFF						// sets the default interval flash rate


    
    
/* function prototypes */
void InitTimer1();	


#ifdef	__cplusplus
}
#endif

#endif	/* TIMERS_H */

