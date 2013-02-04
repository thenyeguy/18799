#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "fft4g.c"
#include "portaudio_utils.h"
#include "feature_utils.h"


// Global variables used by the dft calculations
//Buffer to copy input/output through
int buffer_length;
double* fft_buffer;
//Magic used by the fft library to speed up performance
int* fft_scratch;
double* fft_sintable;


void initialize_feature_engine(int init_buffer_length)
{
    buffer_length = init_buffer_length;
    fft_buffer = calloc(init_buffer_length*2,sizeof(double));
    fft_scratch = calloc(2+init_buffer_length,sizeof(int));
    fft_sintable = calloc(init_buffer_length,sizeof(double));
}


void premphasize(double* window)
{
    for(int i = buffer_length-1; i > 0; i--)
        window[i] -= PREMPH_ALPHA*window[i-1];
}


void hamming_window(double* window)
{
    for(int i = 0; i < buffer_length; i++)
    {
        double weight = .054 - 0.46*cos(2*M_PI*i/buffer_length);
        window[i] = window[i]*weight;
    }
}


void dtft(double* input, double* output)
{
    //Copy input into a buffer that weaves real and complex values for
    //each sample point. Imaginary value is always 0 since we are recording real
    //signals
    for(int i = 0; i<buffer_length; i++)
    {
        fft_buffer[2*i] = input[i];
        fft_buffer[2*i+1] = 0.0;
    }

    //Perform the FFT
    //Library function takes the lenght of our buffer, a 1 to indicate we are
    //going into the frequency domain, the buffer above, and two scratch tables
    //to store interior data for. Performs the FFT in place.
    cdft(buffer_length*2,1,fft_buffer,fft_scratch,fft_sintable);

    //Copy the magnitude of each sample into the output buffer
    for(int i = 0; i<buffer_length; i++)
    {
        double r = (fft_buffer[2*i]);
        double j = (fft_buffer[2*i+1]);
        output[i] = sqrt(r*r+j*j);
    }
}


void close_feature_engine()
{
    free(fft_buffer);
    free(fft_scratch);
    free(fft_sintable);
}
