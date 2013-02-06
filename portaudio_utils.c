#include <stdlib.h>
#include <stdio.h>
#include "portaudio.h"
#include "portaudio_utils.h"


//Full function documentation available in portaudio_utils.h


int error_check(char* location, int err)
{
    if(err == paNoError) return err;

    printf("An error occured during %s: %s\n", location, Pa_GetErrorText(err));
    exit(err);
}


int initialize_portaudio(PaStream** stream)
{
    //Initialize portaudio
    error_check("PaInitialize",Pa_Initialize());

    //Use default input and output devices on the computer
    //  with sample type defined in header
    PaStreamParameters inputParams, outputParams;

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
    printf("Sample rate: %d, buffer size: %d\n\n",SAMPLE_RATE,SAMPLES_PER_BUFFER);

    //Open the stream!
    error_check("Pa_OpenStream",
        Pa_OpenStream(stream, &inputParams, &outputParams,
                      SAMPLE_RATE, SAMPLES_PER_BUFFER, paNoFlag,
                      NULL, NULL));
    return 0;
}


int close_portaudio(PaStream ** stream)
{
    error_check("Pa_CloseStream", Pa_CloseStream(stream));
    error_check("Pa_Terminate",Pa_Terminate());
    printf("You are done!\n");
    return 0;
}
