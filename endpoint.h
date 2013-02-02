#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


/* Constants to tweak the performance of the endpointing
 * Since we try to account for the background level of a signal dynamically,
 * adjusting these constants will adjust the performance of the endpointing.
 */
#define SMOOTHINGFACTOR   2.2
#define BACKGROUNDFACTOR  0.05
#define THRESHOLD         1.0

#define NAIVETHRESHOLD    0.005

/* window - takes in a buffer of audio samples its length, and returns true if it
 *          the speaker has stopped speaking */
bool done_speaking(SAMPLE* buffer, int n);
