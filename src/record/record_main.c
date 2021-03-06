#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "endpoint.h"
#include "portaudio.h"
#include "portaudio_utils.h"
#include "wav_utils.h"


/* record.c - main function for our working test of portaudio import
 *            Currently performs a basic push-to-begin recording of user input,
 *            with a windowing function to determine when to stop audio. Writes
 *            the raw recorded data to a file recorded.out. */
int main(int argc, char* argv[])
{
    //Initialize portaudio stream
    PaStream* stream = NULL;
    initialize_portaudio(&stream);
    
    //Create buffer to read audio into
    SAMPLE* samples = malloc(NUM_CHANNELS * SAMPLES_PER_BUFFER * sizeof(SAMPLE));


    //Sets up time stamped file name. There's gotta be a better way to do this...
    //Allows user to name it themselves
    char output_file_name[256];
    char output_wave_file_name[256];
    if(argc > 1)
    {
        char* name = argv[1];
        sprintf(output_file_name, "recordings/%s.out", name);
        sprintf(output_wave_file_name, "recordings/%s.wav", name);
    }
    else
    {
        time_t timestamp = time(NULL);
        sprintf(output_file_name, "recordings/recorded-%lu.out", timestamp);
        sprintf(output_wave_file_name, "recordings/recorded-%lu.wav", timestamp);
    }
    
    printf(".out: %s\n",output_file_name);
    printf(".wav: %s\n",output_wave_file_name);
    int outputfile = open(output_file_name, O_WRONLY|O_CREAT, 0644);
    
    //Run until windowing function signals the end
    error_check("StartStream",Pa_StartStream(stream));
    
    //Wait for the user to begin
    printf("\nPress enter to begin recording...\n");
    getchar();

    bool listening = true;
    bool first = true;
    int dataCaptured = 0;
    while(listening)
    {
        Pa_ReadStream(stream, samples, SAMPLES_PER_BUFFER);
        //Pa_WriteStream(stream, samples, SAMPLES_PER_BUFFER);

        //Filter out portaudio zeros
        //Not sure why, but its messing up our data
        //Also ignore the first sample to avoid the spacebar hit
        if(samples[0] != 0 && !first)
        {
            write(outputfile, samples, SAMPLES_PER_BUFFER*sizeof(SAMPLE));
            dataCaptured += SAMPLES_PER_BUFFER*sizeof(SAMPLE);
        }
        else
        {
            first = false;
        }

        if(done_speaking(samples, SAMPLES_PER_BUFFER))
            listening = false;
    }

    //Write out to wav
    raw_to_wav(output_file_name, output_wave_file_name, dataCaptured);

    //Close portaudio and clean up   
    printf("size: %d\n", dataCaptured);
    error_check("StopStream", Pa_StopStream(stream));
    close_portaudio(stream);
    close(outputfile);
    free(samples);

    return 0;
}
