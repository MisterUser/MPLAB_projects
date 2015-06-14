
#if defined(__dsPIC33F__)
#include "p33Fxxxx.h"
#elif defined(__PIC24H__)
#include "p24Hxxxx.h"
#endif

#include<adc.h>	//utilize adc peripheral library

//Macros for Configuration Fuse Registers:
//Invoke macros to set up  device configuration fuse registers.
//The fuses will select the oscillator source, power-up timers, watch-dog
//timers etc. The macros are defined within the device
//header files. The configuration fuse registers reside in Flash memory.


// Internal FRC Oscillator
_FOSCSEL(FNOSC_FRC);			// FRC Oscillator 
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF  & POSCMD_NONE); 
								// Clock Switching is disabled and Fail Safe Clock Monitor is disabled
								// OSC2 Pin Function: OSC2 is Clock Output
								// Primary Oscillator Mode: Disabled


_FWDT(FWDTEN_OFF);              // Watchdog Timer Enabled/disabled by user software
								// (LPRC can be disabled by clearing SWDTEN bit in RCON register

//Global Variables

float Vout;
float temp;


/***********************************************************************
Function: 	initAdc1
Parameters:	none
returns:	none
overview: 	Initializes ADC1 
************************************************************************/

void initAdc1(void)
{

	//Using the adc peripheral library, configure ADC1 as follows:
		// 12-bit
		// sample off of AN0
		// Use dedicated ADC RC oscillator
		// Automatically start new conversion after previous
		// Use Avdd and Avss as reference levels

	OpenADC1(
				ADC_MODULE_OFF & ADC_AD12B_12BIT & ADC_FORMAT_INTG & ADC_CLK_AUTO & ADC_AUTO_SAMPLING_ON,
				ADC_VREF_AVDD_AVSS & ADC_SCAN_OFF,
				ADC_SAMPLE_TIME_31 & ADC_CONV_CLK_INTERNAL_RC,
				ADC_DMA_BUF_LOC_1,
				ENABLE_AN0_ANA,
				ENABLE_ALL_DIG_16_31,
				0,
				0);

	AD1CON1bits.ADON = 1;	//Turn on ADC
}


/***********************************************************************
Function: 	calc_Vout
Parameters:	none
returns:	none
overview: 	Calculates a voltage level based off of the ADC result
************************************************************************/
void calc_Vout(void)
{

	//Using a 5Vdd and the ADC in 12-bit mode, gives us a 3.3/4096 = around 806uV/step
	Vout = ReadADC1(0)*0.0008056640625;

}

/***********************************************************************
Function: 	calc_temp
Parameters:	none
returns:	none
overview: 	Calculates temperature value based off of ADC voltage result
************************************************************************/
void calc_temp(void)
{

	//MCP9700 transfer function Ambient Temp (degrees C) = (Vout - 500mV)/10mV
	temp = (Vout - 0.5)/0.01;
	
}



int main(void)
{	

	initAdc1();	//initialize ADC
	while (1)
	{
	
		calc_Vout();	//Calculate ADC result as a voltage
		calc_temp();	//Calculate Temperature

	}
} //End of main

				 	

	



	
 


