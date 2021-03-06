#ifndef FFT_GK_H
#define FFT_GK_H

/* Constant Definitions */
#define FFT_BLOCK_LENGTH	512     /* = Number of frequency points in the FFT */
#define LOG2_BLOCK_LENGTH 	9	/* = Number of "Butterfly" Stages in FFT processing */
#define SAMPLING_RATE		2000	/* = Rate at which input signal was sampled */
                                        /* SAMPLING_RATE is used to calculate the frequency*/
                                        /* of the largest element in the FFT output vector*/

#define FFTTWIDCOEFFS_IN_PROGMEM 	/*<---Comment out this line of the code if twiddle factors (coefficients) */
                                	/*reside in data memory (RAM) as opposed to Program Memory */
                                	/*Then remove the call to "TwidFactorInit()" and add the twiddle factor*/
                                	/*coefficient file into your Project. An example file for a 256-pt FFT*/
                                	/*is provided in this Code example */


/* Global Definitions */
#ifndef FFTTWIDCOEFFS_IN_PROGMEM
fractcomplex twiddleFactors[FFT_BLOCK_LENGTH/2] 	/* Declare Twiddle Factor array in X-space*/
__attribute__ ((section (".xbss, bss, xmemory"), aligned (FFT_BLOCK_LENGTH*2)));
#else
extern const fractcomplex twiddleFactors[FFT_BLOCK_LENGTH/2]	/* Twiddle Factor array in Program memory */
__attribute__ ((space(auto_psv), aligned (FFT_BLOCK_LENGTH*2)));
#endif




void setup_FFT_input(fractcomplex* sigCmpx);
unsigned long freq_detect_FFT(fractcomplex* sigCmpx);

#endif //FFT_GK_H