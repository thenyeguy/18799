#include <stdlib.h>
#include <stdio.h>
#include "portaudio.h"

#ifndef AUDIOCONSTS
#define AUDIOCONSTS

/* Define the nessecary constants for portaudio sampling.
 * Per assignment specs, uses 16-bit integers for each sample. */
typedef short SAMPLE;
#define PA_SAMPLE_TYPE  paInt16
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"

/* Define the recording constants for our streams. Try to use these constants
 * everywhere when possible. */
#define SAMPLE_RATE        (16000)		//Mac: 44100	PC: 16000
#define NUM_CHANNELS       (1)
#define SAMPLES_PER_BUFFER (SAMPLE_RATE)
#define FRAME_RATE		(1024)

#endif


/* error_check - takes a string describing the function called, and an
 *               error code denoting that function. If the function
 *               worked successfully, it just returns. Otherwise, it prints out
 *               the location and the error description, then exits().
 *
 *               Currently returns 0.
 *
 * Sample usage: error_check("Pa_MyFunc", Pa_MyFunc(args));
 */
int error_check(char* location, int err);


/* initialize_portaudio - given a pointer to store the stream data to, performs
 *                        all nessecary initialization to use portaudio streams.
 *                        
 *                        Currently returns 0
 */
int initialize_portaudio(PaStream** stream);


/* close_portaudio - given an initialized stream pointer, performs the nessecary
 *                   procedures to shut down portaudio.
 *
 *                   Currently returns 0;
 */
int close_portaudio(PaStream** stream);
