/* 
 * File:   timers.h
 * Author: Mr
 *
 * Created on June 16, 2015, 8:02 PM
 */

#ifndef TIMERS_H
#define	TIMERS_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <p33Fxxxx.h>
#include "HardwareProfile.h"
    
/* globals */
#define PERIOD  0xFFFF						// sets the default interval flash rate

/* function prototypes */
void InitTimer1();	

#ifdef	__cplusplus
}
#endif

#endif	/* TIMERS_H */

