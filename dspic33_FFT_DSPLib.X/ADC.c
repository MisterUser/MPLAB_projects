/**********************************************************************
* © 2005 Microchip Technology Inc.
*
* FileName:        ADC.c
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       dsPIC30Fxxxx
* Compiler:        MPLAB® C30 v1.33.00 or higher
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Inc. (“Microchip”) licenses this software to you
* solely for use with Microchip dsPIC® digital signal controller
* products. The software is owned by Microchip and is protected under
* applicable copyright laws.  All rights reserved.
*
* SOFTWARE IS PROVIDED “AS IS.”  MICROCHIP EXPRESSLY DISCLAIMS ANY
* WARRANTY OF ANY KIND, WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
* PARTICULAR PURPOSE, OR NON-INFRINGEMENT. IN NO EVENT SHALL MICROCHIP
* BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL
* DAMAGES, LOST PROFITS OR LOST DATA, HARM TO YOUR EQUIPMENT, COST OF
* PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
* BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
* ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER SIMILAR COSTS.
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author            Date      Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Hrushikesh Vasuki 07/29/05  First release of source file
* Hrushikesh Vasuki 10/07/05  Mods to increase filter block size to 256
*                             samples and use 30F6014A device
*                             instead of 30F6014
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
* ADDITIONAL NOTES:
* This file contains two functions - ADC_Init() and _ADCInterrupt().
*
**********************************************************************/

#include <p33Fxxxx.h>
#include "ADC.h"
#include <dsp.h>
#include <p33FJ64MC802.h>
#include "fft.h"

//vars from main.c
extern fractional * iPtr;
extern int doFilterFlag;

//from inputsignal_square1khz.c
/* Typically, the input signal to an FFT  
 * routine is a complex array containing samples 
 * of an input signal. For this example, */
extern fractcomplex sigCmpx[FFT_BLOCK_LENGTH] __attribute__ ((section (".ydata, data, ymemory"), aligned (FFT_BLOCK_LENGTH * 2 *2)));	



volatile unsigned int * adcPtr;

//Functions:
//ADC_Init() is used to configure A/D to convert 16 samples of 1 input
//channel per interrupt. The A/D is set up for a sampling rate of 8KHz
//Timer3 is used to provide sampling time delay.
//The input pin being acquired and converted is AN7.
void ADC_Init(void)
{
        //AD1CON1 Register
        //Set up A/D for Automatic Sampling
        //Use Timer3 to provide sampling time
        //Set up A/D conversrion results to be read in 1.15 fractional
        //number format.
        //All other bits to their default state
        AD1CON1bits.AD12B = 1;// 12-bit, 1-channel
        AD1CON1bits.FORM = 3; // Output Format: 3 = 11 = for 12-bit Signed Fractional
                              // (DOUT = sddd dddd dd00 0000, where s = sign, d = data)
        AD1CON1bits.SSRC = 2; // Sample CLK Source Select: 2 = 010 = GP timer (TIM3 for ADC1)
        AD1CON1bits.ASAM = 1; // ADC Sample Auto-Start: 1 = begin after last conversion

        //AD1CON2 Register
        //Set up A/D for interrupting after 16 samples get filled in the buffer
        //All other bits to their default state
        AD1CON2bits.SMPI = 0; // Sample and Conversion Operation: 15 = 1111 
                               // = ADC Int generated after 16th sample/conv.
        AD1CON2bits.VCFG = 0; //0b000 = Vrefh -> AVdd and Vrefl -> AVss
        //_VCFG = 0; //0b000 = Vrefh -> AVdd and Vrefl -> AVss
                
        //AD1CON3 Register
        //We would like to set up a sampling rate of 8KHz
        //Total Conversion Time= 1/Sampling Rate = 125 microseconds
        //At 29.4 MIPS, Tcy = 33.9 ns = Instruction Cycle Time
        //Tad > 667ns (for -40C to 125C temperature range)
        //We will set up Sampling Time using Timer3 & Tad using ADCS<5:0> bits
        //All other bits to their default state
        //Let's set up ADCS arbitrarily to the maximum possible amount = 63
        //So Tad = Tcy*(ADCS+1)/2 = 1.085 microseconds
        //So, the A/D converter will take 14*Tad periods to convert each sample
        
        //TCY = 1/80Mhz = 12.5ns
        //TAD for FAD of 40kHz = 25us
        //ADCS = 2*TAD/TCY = 2*25000ns/12.5ns = 4000 (Only goes to 63)
        //TAD = 12.5*(64)/2 = 400ns (FAD = 2.5MHz)
        //TCY = 1/20MHz = 50ns
        //
        AD1CON3bits.ADCS = 10; // Conversion Clock Select: 63 * Tcy = TAD

        //Next, we will to set up Timer 3 to time-out every 125 microseconds
        //As a result, the module will stop sampling and trigger a conversion
        //on every Timer3 time-out, i.e., 125 microseconds. At that time,
        //the conversion process starts and completes 14*Tad periods later.
        //When the conversion completes, the module starts sampling again
        //However, since Timer3 is already on and counting, about 110
        //microseconds later (=125 microseconds - 14*Tad), Timer3 will expire
        //again. Effectively, the module samples for 110 microseconds and
        //converts for 15 microseconds
        //NOTE: The actual sampling rate realized may be 7998.698 Hz
        //      due to a small round off error. Ensure you provide the
        //      true sampling rate to dsPICworks if you are trying to plot
        //      the sampled or filtered signal.
        TMR3 = 0x0000;
        PR3 = SAMPCOUNT;
        IFS0bits.T3IF = 0;
        
        //ENABLE interrupt to check sample rate
        IPC2bits.T3IP = 4; //TEMP!!!/* set Tim3 interrupt priority level to 4 */
        IEC0bits.T3IE = 1; //TEMP!!!

        //ADCHS Register
        //Set up A/D Channel Select Register to convert AN5 on Mux A input
        AD1CHS0bits.CH0SA = 5; //0b0101 = Channel 0 positive input is AN5
        AD1CHS0bits.CH0NA = 0; //0 = Channel 0 neg input = Vref-

        //AD1CSSL Register
        //Channel Scanning is disabled. All bits left to their default state
        AD1CSSL = 0x0000;

        //ADPCFG Register
        //Set up channels AN5 as analog input and configure rest as digital
        //Recall that we configured all A/D pins as digital when code execution
        //entered main() out of reset
        AD1PCFGL = 0xFFFF;
        AD1PCFGLbits.PCFG5 = 0;
        AD1PCFGLbits.PCFG1 = 0;

        //Clear the A/D interrupt flag bit
        IFS0bits.AD1IF = 0;
        IPC3bits.AD1IP = 6; //6/7 priority

        //Set the A/D interrupt enable bit
        IEC0bits.AD1IE = 1;
        

        //Turn on the A/D converter
        //This is typically done after configuring other registers
        AD1CON1bits.ADON = 1;

        //Start Timer 3
        T3CONbits.TON = 1;
}

//_ADCInterrupt() is the A/D interrupt service routine (ISR).
//The routine must have global scope in order to be an ISR.
//The ISR name is chosen from the device linker script.
void __attribute__((__interrupt__)) _ADC1Interrupt(void)
{
        //Clear the Timer3 Interrupt Flag
        IFS0bits.T3IF = 0;

        //int i = 0;

        //Clear the A/D Interrupt flag bit or else the CPU will
        //keep vectoring back to the ISR
        IFS0bits.AD1IF = 0;

        //Copy the A/D conversion results to variable "inputSignal"
        adcPtr = &ADCBUF0 ;
        *iPtr = *adcPtr;
        iPtr++;
//        for (i=0;i<16;i++)
//        {
//                *iPtr++= *adcPtr++;
//        }
        
        //sigCmpx is fractcomplex and iPtr is only
        //fractional, meaning that checking that iPtr
        //is > sigCmpx[127] will not allow the 256th
        //fractional element to be written because after
        //the 255th, it will be > 127th fractcomplex address
        if(iPtr >= &(sigCmpx[FFT_BLOCK_LENGTH/2])) 
        {
            doFilterFlag = 1;
            //Start Timer 3
            T3CONbits.TON = 0;
            AD1CON1bits.ADON = 0;
        }

}
