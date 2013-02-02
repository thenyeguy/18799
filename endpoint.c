#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "portaudio.h"
#include "portaudio_utils.h"
#include "endpoint.h"

// Abstract documentation in header file


#define sampleToDouble(X) (((double) (X)) / 32767)
#define dB(X)             (10*log10((X)*(X) * 100000))

/* Keeps a moving average of the background noise level over all time
 * and a moving average of the energy intensity for each buffer we read.
 * If the energy of the buffer minus the background noise drops below the
 * threshold, then the speaker is most likely done.
 */
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
        {
            background += dB(sampleToDouble(buffer[i]))/10;
        }
    }

    counter++;
    if(counter < 2) return false;

    //Collect data for the buffer
    double level = dB(sampleToDouble(buffer[0]));
    double current;
    for(int i = 0; i < n; i++)
    {
        //dB doesn't like zero values, safe to just skip over them
        if(buffer[i] == 0) continue;
        
        //Moving average of the dB
        current = dB(sampleToDouble(buffer[i]));
        level = ((level*SMOOTHINGFACTOR) + current) / (1+SMOOTHINGFACTOR);
    }
    //Update the background
    if(level < background) background = level;
    background += (level-background)*BACKGROUNDFACTOR;

    printf("\n\nLevel average: %f \nBackground average: %f \n",level,background);

    //Done speaking if we no longer exceed the threshold
    if(level-background < THRESHOLD)
        return true;
    else
        return false;
}
