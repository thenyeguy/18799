#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include "portaudio.h"
#include "portaudio_utils.h"


/* test.c - main function for our working test of portaudio import
 *      TODO: rename and continue refactoring code
 *
 *      Currently performs a basic push-to-begin recording of user input, with
 *      a windowing function to determine when to stop audio. Writes the raw
 *      recorded data to a file recorded.out. */
int main()
{
    //Initialize portaudio stream
    PaStream* stream = NULL;
    initialize_portaudio(&stream);
    
    //Create buffer to read audio into
    SAMPLE* samples = malloc(NUM_CHANNELS * SAMPLES_PER_BUFFER * sizeof(SAMPLE));


    //Wait for the user to begin
    printf("\nPress enter to begin recording...\n");
    getchar();

    //Run until windowing function signals the end, ignoring the
    //  first sample to let the user begin talking
    //Write the recorded data to an output file
    error_check("StartStream",Pa_StartStream(stream));
    int outputfile = open("./recorded.out", O_WRONLY|O_CREAT, 0644);

    bool listening = true;
    int counter = 0;
    while(listening || counter < 2)
    {
        Pa_ReadStream(stream, samples, SAMPLES_PER_BUFFER);
        write(outputfile, samples, SAMPLES_PER_BUFFER);

        //Window - stop recording when the magnitude averaged over a
        //  second drops below an arbitrary constant
        int sum = 0;
        for(int i = 0; i < SAMPLES_PER_BUFFER; i++)
        {
            sum += abs(samples[i]);
        }
        int average = sum/SAMPLES_PER_BUFFER;
        if(average < 250) listening = false;
        counter++;
    }

    //Close portaudio and clean up   
    error_check("StopStream", Pa_StopStream(stream));
    close_portaudio(stream);
    close(outputfile);
    free(samples);

    return 0;
}
