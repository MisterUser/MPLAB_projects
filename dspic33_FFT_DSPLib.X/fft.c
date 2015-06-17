/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC33E__)
    	#include <p33Exxxx.h>
    #elif defined(__dsPIC33F__)
    	#include <p33Fxxxx.h>
    #endif
#endif

#include <stdint.h>          /* For uint16_t definition                       */
#include <stdbool.h>         /* For true/false definition                     */
#include <dsp.h>
#include "fft.h"

int	peakFrequencyBin;				/* Declare post-FFT variables to compute the */

void setup_FFT_input(fractcomplex* sigCmpx)
{
    int i = 0;
	fractional *p_real = &(sigCmpx->real);
	fractcomplex *p_cmpx = sigCmpx;


#ifndef FFTTWIDCOEFFS_IN_PROGMEM					/* Generate TwiddleFactor Coefficients */
	TwidFactorInit (LOG2_BLOCK_LENGTH, &twiddleFactors[0], 0);	/* We need to do this only once at start-up */
#endif

	for ( i = 0; i < FFT_BLOCK_LENGTH; i++ )/* The FFT function requires input data */
	{					/* to be in the fractional fixed-point range [-0.5, +0.5]*/
		*p_real = *p_real >>1 ;		/* So, we shift all data samples by 1 bit to the right. */
		p_real++;			/* Should you desire to optimize this process, perform */
	}					/* data scaling when first obtaining the time samples */
						/* Or within the BitReverseComplex function source code */

	p_real = &((sigCmpx + (FFT_BLOCK_LENGTH/2)-1)->real);	/* Set up pointers to convert real array */
	p_cmpx = sigCmpx + FFT_BLOCK_LENGTH-1; /* to a complex array. The input array initially has all */
						/* the real input samples followed by a series of zeros */


	for ( i = FFT_BLOCK_LENGTH; i > 0; i-- ) /* Convert the Real input sample array */
	{					/* to a Complex input sample array  */
		(*p_cmpx).real = (*p_real--);	/* We will simpy zero out the imaginary  */
		(*p_cmpx--).imag = 0x0000;	/* part of each data sample */
	}
}

unsigned long freq_detect_FFT(fractcomplex* sigCmpx)
{
    peakFrequencyBin = 0;
    /* Perform FFT operation */
#ifndef FFTTWIDCOEFFS_IN_PROGMEM
	FFTComplexIP(LOG2_BLOCK_LENGTH, sigCmpx, &twiddleFactors[0], COEFFS_IN_DATA);
#else
	FFTComplexIP(LOG2_BLOCK_LENGTH, sigCmpx, (fractcomplex *) __builtin_psvoffset(&twiddleFactors[0]), (int) __builtin_psvpage(&twiddleFactors[0]));
#endif

	/* Store output samples in bit-reversed order of their addresses */
	BitReverseComplex (LOG2_BLOCK_LENGTH, sigCmpx);

	/* Compute the square magnitude of the complex FFT output array so we have a Real output vetor */
	SquareMagnitudeCplx(FFT_BLOCK_LENGTH, sigCmpx, &(sigCmpx->real));

	/* Find the frequency Bin ( = index into the SigCmpx[] array) that has the largest energy*/
	/* i.e., the largest spectral component */
	VectorMax(FFT_BLOCK_LENGTH/2, &(sigCmpx->real), &peakFrequencyBin);

	/* Compute the frequency (in Hz) of the largest spectral component */
	return peakFrequencyBin*(SAMPLING_RATE/FFT_BLOCK_LENGTH);
    
}