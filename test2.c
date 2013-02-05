#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>

#include "portaudio.h"
#include "portaudio_utils.h"
#include "wav_utils.h"

/* test.c - main function for our working test of portaudio import
 *      TODO: rename and continue refactoring code
 *		TODO: convert output file from raw data to wav
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
    int buffer_size = NUM_CHANNELS * SAMPLES_PER_BUFFER * sizeof(SAMPLE);
    SAMPLE* samples = (SAMPLE *)malloc(buffer_size);


    //Sets up time stamped file name. There's gotta be a better way to do this...
    time_t sys_time = time(NULL);
    char * prefix = "./recordings/recorded-";
    char timestamp[32];
    sprintf(timestamp, "%lu", (long unsigned)sys_time);
    char * suffix = ".out";
    char dataFileName[256];
	snprintf(dataFileName, sizeof(dataFileName), "%s%s%s", prefix,timestamp, suffix);
	
	char wavFileName[256];
	char *wav_suffix = ".wav";
	snprintf(wavFileName, sizeof(wavFileName), "%s%s%s", prefix, timestamp, wav_suffix);
	

	printf("Output will be stored in %s\n",dataFileName);
    int outputfile = open(dataFileName, O_WRONLY|O_CREAT, 0644);
   
    //Wait for the user to begin
    printf("\nPress enter to begin recording... \n");
	getchar();
    //outputfile = open("./recorded.out", O_WRONLY|O_CREAT, 0644);
	bool listening = true;
    int counter = 0;
	int err = 0;
	
    //Run until windowing function signals the end, ignoring the
    //  first sample to let the user begin talking
    //Write the recorded data to an output file
    error_check("StartStream",Pa_StartStream(stream));

    listening = true;
    counter = 0;
    int dataCaptured = 0;
    printf("Size of buffer: %d\n",buffer_size);
    printf("Size of SAMPLES_PER_BUFFER: %d\n",SAMPLES_PER_BUFFER);

    while( counter < 4)
    {
	printf("Counter: %d\n",counter);

        err = Pa_ReadStream(stream, samples, SAMPLES_PER_BUFFER);
//        error_check("read stream",err);		//		<------- THAT LINE
        printf("error = %d\n", err);
		write(outputfile, samples, SAMPLES_PER_BUFFER);
		dataCaptured+=SAMPLES_PER_BUFFER;

        // Window - stop recording when the magnitude averaged over a
        //  second drops below an arbitrary constant
        /*
        int sum = 0;
        for(int i = 0; i < SAMPLES_PER_BUFFER; i++)
        {
            sum += abs(samples[i]);
        }
        int average = sum/SAMPLES_PER_BUFFER;
        if(average < 250) listening = false;
        */
        counter++;
    }

	//Should we be sending dataCaptured?
	raw_to_wav(dataFileName, wavFileName, dataCaptured);


    //Close portaudio and clean up   
    error_check("StopStream", Pa_StopStream(stream));
    close_portaudio(stream);
    close(outputfile);
    free(samples);

	return 0;
}
