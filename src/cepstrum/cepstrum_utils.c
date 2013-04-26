#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "../libraries/fft4g.h"
#include "../record/portaudio_utils.h"
#include "cepstrum_utils.h"


// Global variables used by the dft/dct calculations

//Buffer to copy input/output through
int dft_buffer_length;
int dct_buffer_length;
double* dft_buffer;
double* dct_buffer;

//Magic used by the fft library to speed up performance
int* dft_scratch;
int* dct_scratch;
double* dft_sintable;
double* dct_sintable;


void initialize_feature_engine(int fft_length, int dct_length)
{
    dft_buffer_length = fft_length;
    dct_buffer_length = dct_length;

    dft_buffer = calloc(fft_length*2,sizeof(double));
    dft_scratch = calloc(2+fft_length,sizeof(int));
    dft_sintable = calloc(fft_length,sizeof(double));

    dct_buffer = calloc(dct_length,sizeof(double));
    dct_scratch = calloc(2+(1<<(int)(log(dct_length/2+0.5)/log(2))/2),
                         sizeof(int));
    dct_sintable = calloc(dct_length*5/4-1,sizeof(double));
}


void premphasize(double* window, int n)
{
    for(int i = n-1; i > 0; i--)
        window[i] -= PREMPH_ALPHA*window[i-1];
}


void hamming_window(double* window, int n)
{
    for(int i = 0; i < n; i++)
    {
        double weight = .54 - 0.46*cos(2*M_PI*i/n);
        window[i] = window[i]*weight;
    }
}


double hz_to_mel(double hz)
{
    return 2595*log10(1.0+hz/700);
}


double mel_to_hz(double mel)
{
    return 700*(pow(10,mel/2595)-1);
}


void dtft(double* input, double* output)
{
    //Copy input into a buffer that weaves real and complex values for
    //each sample point. Imaginary value is always 0 since we are recording real
    //signals
    for(int i = 0; i<dft_buffer_length; i++)
    {
        dft_buffer[2*i] = input[i];
        dft_buffer[2*i+1] = 0.0;
    }

    //Perform the fft
    //Library function takes the lenght of our buffer, a 1 to indicate we are
    //going into the frequency domain, the buffer above, and two scratch tables
    //to store interior data for. Performs the fft in place.
    cdft(dft_buffer_length*2,1,dft_buffer,dft_scratch,dft_sintable);

    //Copy the magnitude of each sample into the output buffer
    for(int i = 0; i<dft_buffer_length; i++)
    {
        double r = (dft_buffer[2*i]);
        double j = (dft_buffer[2*i+1]);
        output[i] = sqrt(r*r+j*j);
    }
}


void dtct(double* input, double* output)
{
    //Copy the input into the buffer
    for(int i = 0; i<dct_buffer_length; i++)
    {
        dct_buffer[i] = input[i];
    }

    //Perform the dct
    //Library function takes the lenght of our buffer, a -1 to indicate we are
    //going doing a normal, not inverse, DCT, the buffer above, and two scratch
    //tables to store interior data for. Performs the dct in place.
    ddct(dct_buffer_length,-1,dct_buffer,dct_scratch,dct_sintable);

    //Copy and scale the output as nessecary - the ddct doesn't handle the
    //scaling
    for(int i = 0; i<dct_buffer_length; i++)
    {
        if(i == 0)
            output[i] = dct_buffer[i] * sqrt(1.0/dct_buffer_length);
        else
            output[i] = dct_buffer[i] * sqrt(2.0/dct_buffer_length);
    }
}


void log_mel_filter(double* freqs, int num_filters, double* log_spectra,
                    int sample_rate)
{
    //Define basic mel parameters
    //
    //We want to linearly sample in the mel space, so we convert into and out of
    //it to find the real frequencies;
    double lowest = hz_to_mel(MEL_LOW_FREQ);
    double highest = hz_to_mel(MEL_HIGH_FREQ);
    double delta = (highest-lowest)/(num_filters-1);

    //Loop one for each mel sample;
    for(int i = 0; i < num_filters; i++)
    {
        //Use the linear distribution in mel space, find the equivalent lower
        //and upper bounds in hz for our filters
        double center_mel = lowest + i*delta;
        double center = mel_to_hz(center_mel);
        double lower = mel_to_hz(center_mel-delta);
        double upper = mel_to_hz(center_mel+delta);

        //Convert the frequency values to indeces into the fft array
        //Rounds down and uses the closest freq
        double hz_to_i = ((double) dft_buffer_length)/((double) sample_rate);
        int lower_i = (int) (lower * hz_to_i);
        int center_i = (int) (center * hz_to_i);
        int upper_i = (int) (upper * hz_to_i);

        //Use a triangle filter for each mel filter, and normalize each filter
        //such that its area is 1
        double filter_area = 0;
        double mel_value = 0;
        //First the left half of the triangle
        for(int j = lower_i; j < center_i; j++)
        {
            //Ignore any negative frequencies
            if(j > 0)
            {
                double weight = ((double) (j-lower_i)) / ((double) (center_i-lower_i));
                filter_area += weight;
                mel_value += weight * freqs[j];
            }
        }
        //Then the right half
        for(int j = center_i; j < upper_i; j++)
        {
            //Ignore any frequencies above fs/2
            if(j <= dft_buffer_length/2)
            {
                double weight = ((double) (j-upper_i)) / ((double) (center_i-upper_i));
                filter_area += weight;
                mel_value += weight * freqs[j];
            }
        }

        //Finally store the mel value in the result
        log_spectra[i] = log(mel_value/filter_area);
    }
}


void mel_cepstrum(double* log_spectra, int num_filters,
                  double* cepstrum, int num_cepstra)
{
    double* in_buffer = calloc(dct_buffer_length,sizeof(double));
    double* out_buffer = calloc(dct_buffer_length,sizeof(double));

    //Copy log_spectra into zero padded buffer
    for(int i  = 0; i < num_filters; i++)
        in_buffer[i] = log_spectra[i];

    //Run the cosine transform
    dtct(in_buffer, out_buffer);

    //Copy the requested number of values into output array
    for(int i = 0; i < num_cepstra; i++)
        cepstrum[i] = out_buffer[i];
}


void close_feature_engine()
{
    free(dft_buffer);
    free(dft_scratch);
    free(dft_sintable);

    free(dct_buffer);
    free(dct_scratch);
    free(dct_sintable);
}
