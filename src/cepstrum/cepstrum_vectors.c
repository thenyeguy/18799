#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <fcntl.h>
#include "../record/portaudio_utils.h"
#include "cepstrum_utils.h"
#include "cepstrum_vectors.h"


void log_window(char* file_prefix, double* window, int n)
{
    char filename[256];
    sprintf(filename, "./analysis/%s-window.out", file_prefix);
    FILE* out = fopen(filename, "a");

    for(int i = 0; i < n; i++)
        fprintf(out, "%1.4f ", window[i]);
    fprintf(out,"\n");

    fclose(out);
}


void log_dtft(char* file_prefix, double* dtft, int n)
{

    char filename[256];
    sprintf(filename, "./analysis/%s-dtft.out", file_prefix);
    FILE* out = fopen(filename, "a");

    for(int i = 0; i < n; i++)
        fprintf(out, "%1.4f ", dtft[i]);
    fprintf(out,"\n");

    fclose(out);
}


void log_logmel(char* file_prefix, double* logmel, int n)
{

    char filename[256];
    sprintf(filename, "./analysis/%s-log-mel.out", file_prefix);
    FILE* out = fopen(filename, "a");

    for(int i = 0; i < n; i++)
        fprintf(out, "%1.4f ", logmel[i]);
    fprintf(out,"\n");

    fclose(out);
}


void log_cepstrum(char* file_prefix, double* cepstrum, int n)
{

    char filename[256];
    sprintf(filename, "./analysis/%s.out", file_prefix);
    FILE* out = fopen(filename, "a");

    for(int i = 0; i < n; i++)
        fprintf(out, "%1.4f ", cepstrum[i]);
    fprintf(out,"\n");

    fclose(out);
}


cepstrum_vectors* get_cepstrum_vectors(double* signal, int num_samples,
                                       int num_mel_filters, int num_cepstra,
                                       char* file_prefix, int sample_rate)
{
    int samples_per_window = sample_rate*WINDOW_WIDTH/1000;
    int num_vectors = num_samples * 1000/(sample_rate*WINDOW_SLIDE);

    //Delete old output files
    char filename[256];
    sprintf(filename, "./analysis/%s-window.out", file_prefix);
    remove(filename);
    sprintf(filename, "./analysis/%s-dtft.out", file_prefix);
    remove(filename);
    sprintf(filename, "./analysis/%s-log-mel.out", file_prefix);
    remove(filename);
    sprintf(filename, "./analysis/%s.out", file_prefix);
    remove(filename);

    //Create output struct and allocate nessecary matrix
    cepstrum_vectors* result = malloc(sizeof(struct cepstrum_vectors));
    result->num_vectors = num_vectors;
    result->num_points = num_cepstra;
    result->cepstra = calloc(num_vectors, sizeof(double*));
    for(int i = 0; i < num_vectors; i++)
        result->cepstra[i] = calloc(num_cepstra, sizeof(double));


    //Initialize DTFT/DCT engine
    //Buffer sizes must be a power of 2, so find the next power of 2 above the
    //requeted sizes. We will then just 0 pad our buffers.
    //This shouldn't impact result
    int fft_length = pow(2, ceil(log(samples_per_window)/log(2)));
    int dct_length = pow(2, ceil(log(num_mel_filters)/log(2)));
    initialize_feature_engine(fft_length, dct_length);

    double* window = calloc(fft_length,sizeof(double));
    double* window_dft = calloc(fft_length,sizeof(double));
    double* window_log_spectra = calloc(num_mel_filters, sizeof(double));


    //Generate each window using cepstrum utils
    //If we want a log file, then output each intermediary step
    for(int i = 0; i < num_vectors; i++)
    {
        //Generate the window, then premph and weight it
        int offset = i*sample_rate*WINDOW_SLIDE/1000;
        double* start = signal+offset;
        for(int j = 0; j < samples_per_window; j++)
        {
            if(offset+j < num_samples)
                window[j] = start[j];
            else
                window[j] = 0;
        }
        premphasize(window, samples_per_window);
        hamming_window(window, samples_per_window);
        
        if(CEPSTRUM_VERBOSE && file_prefix != NULL)
            log_window(file_prefix, window, fft_length);

        // Get the log spectra for the window
        dtft(window, window_dft);
        log_mel_filter(window_dft, num_mel_filters, window_log_spectra,
                       sample_rate);

        if(CEPSTRUM_VERBOSE && file_prefix != NULL)
            log_dtft(file_prefix, window_dft, fft_length);
        if(CEPSTRUM_VERBOSE && file_prefix != NULL)
            log_logmel(file_prefix, window_log_spectra, num_mel_filters);

        // Get the cepstrum vector and store it in output
        mel_cepstrum(window_log_spectra, num_mel_filters,
                     result->cepstra[i], num_cepstra);

        if(file_prefix != NULL)
            log_cepstrum(file_prefix, result->cepstra[i], num_cepstra);
    }


    //Clean up after ourselves and exit
    close_feature_engine();
    return result;
}
