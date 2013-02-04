#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>


/* initialize_feature_engine - given a window size for our FFT buffer, this
 *                             function allocates space for our new buffers to
 *                             prepare the FFT engine. DO NOT run other code
 *                             without first running initialize.
 */
void initialize_feature_engine(int init_buffer_length);

/* premphasize - given a windowed signal, preemphasizes the signal to highboost
 *               it slighty for processing. Change the alpha to change the
 *               amount of high boost.
 */
#define PREMPH_ALPHA .95
void premphasize(double* window);

/* hamming_window - takes an windowed array from our signal and performs a
 *                  hamming weight to smooth the edges to 0
 */
void hamming_window(double* window);

/* dtft - a wrapper for the library fft code in this project. It handles the
 *       magic to set up the state for the fft, then copies the magnitude of the
 *       result to the output buffer supplied by the user
 */
void dtft(double* input, double* output);

/* log_mel_filter - takes in the DTFT of a windowed sample, computes the lcation
 *                  of each mel filter, then runs the DTFT through the mel 
 *                  filters, logs it and stores the output in log_spectra.
 */
void log_mel_filter(double* freqs, int num_filters, double* log_spectra);

/* close_feature_engine - clean up after ourselves. Probably not nessecary in
 *                        many cases, only frees memory allocated for buffers.
 */
void close_feature_engine();
