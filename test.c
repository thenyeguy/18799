#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "portaudio.h"
#include <time.h>

// Need this?
#define CHECK_OVERFLOW  (0)
#define CHECK_UNDERFLOW  (0)

// Define sample format
typedef short SAMPLE;
#define PA_SAMPLE_TYPE  paInt16
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"

// Define recording constants
#define SAMPLE_RATE        (16000)
#define SAMPLES_PER_BUFFER (SAMPLE_RATE)
#define NUM_CHANNELS       (1)
#define NUM_SECONDS        (10)


int main()
{
    //Define portaudio magic
    PaStreamParameters inputParams, outputParams;
    PaStream* stream = NULL;
    PaError err;
    SAMPLE* samples = malloc(NUM_CHANNELS * SAMPLE_RATE * NUM_SECONDS * sizeof(SAMPLE));

    //Initialize portaudio
    err = Pa_Initialize();

    //Set up stream params
    inputParams.device = Pa_GetDefaultInputDevice();
    inputParams.channelCount = 1;
    inputParams.sampleFormat = PA_SAMPLE_TYPE;
    inputParams.suggestedLatency =
        Pa_GetDeviceInfo(inputParams.device)->defaultHighInputLatency;
    inputParams.hostApiSpecificStreamInfo = NULL;

    outputParams.device = Pa_GetDefaultOutputDevice();
    outputParams.channelCount = 1;
    outputParams.sampleFormat = PA_SAMPLE_TYPE;
    outputParams.suggestedLatency =
        Pa_GetDeviceInfo(outputParams.device)->defaultHighOutputLatency;
    outputParams.hostApiSpecificStreamInfo = NULL;

    printf("Using input device #%d and output device #%d.\n",
           inputParams.device, outputParams.device);

    //Open the stream!
    err = Pa_OpenStream(&stream, &inputParams, &outputParams,
                        SAMPLE_RATE, SAMPLES_PER_BUFFER, paClipOff,
                        NULL, NULL);
    
    //Waits for push to talk...
    printf("\nPress enter to begin recording...\n");
    getchar();
    
    //Sets up time stamped file name. There's gotta be a better way to do this...
    time_t sys_time = time(NULL);
    char * prefix = "./recordings/recorded-";
    char timestamp[32];
    sprintf(timestamp, "%lu", (long unsigned)sys_time);
    char * suffix = ".out";
    char dataFileName[256];
	snprintf(dataFileName, sizeof(dataFileName), "%s%s%s", prefix,timestamp, suffix);
	printf("Naming file:\t%s\n",dataFileName);
    int outputfile = open(dataFileName, O_WRONLY|O_CREAT, 0644);
    
    
    int listening = 1;
    int counter = 0;

	int dataCaptured = 0;

    //Run until user ends it!
    err = Pa_StartStream(stream);
    while(listening || counter < 2)
    {
        err = Pa_ReadStream(stream, samples, SAMPLES_PER_BUFFER);
        //err = Pa_WriteStream(stream, samples, SAMPLES_PER_BUFFER);
        write(outputfile, samples, SAMPLES_PER_BUFFER);
		dataCaptured+=SAMPLES_PER_BUFFER;

        //Window - stop recording when the magnitude averaged over a
        //  second drops below an arbitrary constant
        int sum = 0;
        for(int i = 0; i < SAMPLES_PER_BUFFER; i++)
        {
            sum += abs(samples[i]);
        }
        int average = sum/SAMPLES_PER_BUFFER;
        if(average < 250) listening = 0;
        counter++;
    }
    //No escape yet

    printf("You are done!\n");
    err = Pa_StopStream(stream);
    err = Pa_CloseStream(stream);
    err = Pa_Terminate();

    close(outputfile);
    free(samples);

    return 0;
}
