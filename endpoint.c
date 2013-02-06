#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "portaudio.h"
#include "portaudio_utils.h"
#include "endpoint.h"

// Abstract documentation in header file

#define sampleToDouble(X) (((double) (X)) / 32767)
#define dB(X)             (10*log10((X)*(X) * 25000))


/* Simple, naive algorithm - averages the samples in the entire frame,
 * and if the average sample value is too low then you are done */
bool done_speaking(SAMPLE* buffer, int n)
{
    static int counter = 0;

    //Ignore the first sample set, to let the user start speaking
    counter++;
    if(counter < 4) return false;

    double current;
    double level = 0;
    int count = 0;
    for(int i = 0; i < n; i++)
    {
        //dB doesn't like zero values, safe to just skip over them
        if(buffer[i] == 0) continue;
        
        //Average of the levels
        current = fabs(sampleToDouble(buffer[i]));
        level += current;
        count++;
    }
    level /= count;

    printf("level: %f %d %d\n", level, NAIVEFLOOR < level, level <
    NAIVETHRESHOLD);
    if(NAIVEFLOOR < level && level < NAIVETHRESHOLD)
        return true;
    else
        return false;
}


/* Keeps a moving average of the background noise level over all time
 * and a moving average of the energy intensity for each buffer we read.
 * If the energy of the buffer minus the background noise drops below the
 * threshold, then the speaker is most likely done.
 */
/*
bool done_speaking(SAMPLE* buffer, int n)
{
    static int counter = 0;
    static double background = NAN;

    //Ignore the first sample set, to collect loudness data and let the user
    //  start speaking
    if(isnan(background))
    {
        background = 0;
        for(int i = 0; i < 10; i++)
            background += dB(sampleToDouble(buffer[i]))/10;
    }

    counter++;
    if(counter < 2) return false;

    //Collect data for the buffer
    double level = dB(sampleToDouble(buffer[0]));
    double current;
    int nonspeech_count = 0;
    for(int i = 0; i < n; i++)
    {
        //dB doesn't like zero values, safe to just skip over them
        if(buffer[i] == 0) continue;
        
        //Moving average of the dB
        current = dB(sampleToDouble(buffer[i]));
        level = ((level*SMOOTHINGFACTOR) + current) / (1+SMOOTHINGFACTOR);

        //Moving average of background
        if(level < background) background = level;
        background += (current-background)*BACKGROUNDFACTOR;

        //Determine if this sample is speech
        if(level-background < THRESHOLD)
          nonspeech_count++;
        //printf("(%d %f %f %f)\n ",buffer[i],current,level,background);
    }
    printf("\n\nLevel average: %f \nBackground average: %f \n",level,background);
    printf("Nonspeech samples: %d/%d\n", nonspeech_count,n);

    //Done speaking if we no longer exceed the threshold
    //if(level-background < THRESHOLD)
    if(nonspeech_count > n/2)
        return true;
    else
        return false;
}
*/
