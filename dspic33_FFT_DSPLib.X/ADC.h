/* 
 * File:   ADC.h
 * Author: Mr
 *
 * Created on June 16, 2015, 9:13 PM
 */

#ifndef ADC_H
#define	ADC_H
//#include "fft.h" //for SAMPLING_RATE ->should move to common.h
#ifdef	__cplusplus
extern "C" {
#endif

#define SAMPLING_RATE 8000
#define FCY_W_PLL 80000000 //80MHz
#define NUMSAMP         256
//NOTE: The actual sampling rate realized may be 7998.698 Hz
//      due to a small round off error. Ensure you provide the
//      true sampling rate to dsPICworks if you are trying to plot
//      the sampled or filtered signal.
//#define SAMPLINGRATE    8000
#define SAMPCOUNT       (FCY_W_PLL/SAMPLING_RATE)+1


//Functions and Variables with Global Scope:
void ADC_Init(void);
void __attribute__((__interrupt__)) _ADC1Interrupt(void);

#ifdef	__cplusplus
}
#endif

#endif	/* ADC_H */

