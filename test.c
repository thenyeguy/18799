#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "portaudio.h"
#include "wav_utils.h"

/* #define SAMPLE_RATE  (17932) // Test failure to open with this value. */
#define SAMPLE_RATE  (44100)
#define FRAMES_PER_BUFFER (1024)
#define NUM_CHANNELS    (1)
#define NUM_SECONDS     (15)
/* #define DITHER_FLAG     (paDitherOff)  */
#define DITHER_FLAG     (0) /**/

/* @todo Underflow and overflow is disabled until we fix priming of blocking write. */
#define CHECK_OVERFLOW  (0)
#define CHECK_UNDERFLOW  (0)


/* Select sample format. */
#if 0
#define PA_SAMPLE_TYPE  paFloat32
#define SAMPLE_SIZE (4)
#define SAMPLE_SILENCE  (0.0f)
#define CLEAR(a) memset( (a), 0, FRAMES_PER_BUFFER * NUM_CHANNELS * SAMPLE_SIZE )
#define PRINTF_S_FORMAT "%.8f"
#elif 1
#define PA_SAMPLE_TYPE  paInt16
#define SAMPLE_SIZE (2)
#define SAMPLE_SILENCE  (0)
#define CLEAR(a) memset( (a), 0,  FRAMES_PER_BUFFER * NUM_CHANNELS * SAMPLE_SIZE )
#define PRINTF_S_FORMAT "%d"
#elif 0
#define PA_SAMPLE_TYPE  paInt24
#define SAMPLE_SIZE (3)
#define SAMPLE_SILENCE  (0)
#define CLEAR(a) memset( (a), 0,  FRAMES_PER_BUFFER * NUM_CHANNELS * SAMPLE_SIZE )
#define PRINTF_S_FORMAT "%d"
#elif 0
#define PA_SAMPLE_TYPE  paInt8
#define SAMPLE_SIZE (1)
#define SAMPLE_SILENCE  (0)
#define CLEAR(a) memset( (a), 0,  FRAMES_PER_BUFFER * NUM_CHANNELS * SAMPLE_SIZE )
#define PRINTF_S_FORMAT "%d"
#else
#define PA_SAMPLE_TYPE  paUInt8
#define SAMPLE_SIZE (1)
#define SAMPLE_SILENCE  (128)
#define CLEAR( a ) { \
    int i; \
    for( i=0; i<FRAMES_PER_BUFFER*NUM_CHANNELS; i++ ) \
        ((unsigned char *)a)[i] = (SAMPLE_SILENCE); \
}
#define PRINTF_S_FORMAT "%d"
#endif


/*******************************************************************/
int main(void);
int main(void)
{
    PaStreamParameters inputParameters, outputParameters;
    PaStream *stream = NULL;
    PaError err;
    char *sampleBlock;
    
    int numBytes;
    
    
    printf("patest_read_write_wire.c\n"); fflush(stdout);

    numBytes = FRAMES_PER_BUFFER * NUM_CHANNELS * SAMPLE_SIZE ;
    sampleBlock = (char *) malloc( numBytes );
    if( sampleBlock == NULL )
    {
        printf("Could not allocate record array.\n");
        exit(1);
    }
    CLEAR( sampleBlock );

    err = Pa_Initialize();
    if( err != paNoError ) goto error;

    inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
    printf( "Input device # %d.\n", inputParameters.device );
    printf( "Input LL: %g s\n", Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency );
    printf( "Input HL: %g s\n", Pa_GetDeviceInfo( inputParameters.device )->defaultHighInputLatency );
    inputParameters.channelCount = NUM_CHANNELS;
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultHighInputLatency ;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    printf( "Output device # %d.\n", outputParameters.device );
    printf( "Output LL: %g s\n", Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency );
    printf( "Output HL: %g s\n", Pa_GetDeviceInfo( outputParameters.device )->defaultHighOutputLatency );
    outputParameters.channelCount = NUM_CHANNELS;
    outputParameters.sampleFormat = PA_SAMPLE_TYPE;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultHighOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    /* -- setup -- */

   err = Pa_OpenStream(
              &stream,
              &inputParameters,
              &outputParameters,
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              paClipOff,      /* we won't output out of range samples so don't bother clipping them */
              NULL, /* no callback, use blocking API */
              NULL ); /* no callback, so no callback userData */
    if( err != paNoError ) goto error;

    err = Pa_StartStream( stream );
    if( err != paNoError ) goto error;
    printf("Wire on. Will run %d seconds.\n", NUM_SECONDS); fflush(stdout);
    /* 
    FILE * outputfile = fopen("wavefiletest.out","w");
    int bytes_written = 0;
    for( i=0; i<(NUM_SECONDS*SAMPLE_RATE)/FRAMES_PER_BUFFER; ++i )
    {
       //printf("%d ",i);
       //err = Pa_WriteStream( stream, sampleBlock, FRAMES_PER_BUFFER );
       //if( err && CHECK_UNDERFLOW ) goto xrun;
       err = Pa_ReadStream( stream, sampleBlock, FRAMES_PER_BUFFER );
       if( err && CHECK_OVERFLOW ) goto xrun;
       fwrite(sampleBlock,1,sizeof(sampleBlock),outputfile);
       bytes_written +=  sizeof(sampleBlock);	
    }
    */


    char * output_file_name = get_time_stamped_filename("./recordings/recorded-",".out");
    char * output_wave_file_name = get_time_stamped_filename("./recordings/recorded-",".wav");
	
    printf(".out: %s\n",output_file_name);
    printf(".wav: %s\n",output_wave_file_name);

//    char * output_file_name = "./recordings/recorded-9.out";
//    char * output_wave_file_name = "./recordings/recorded-9.wav";

    FILE * outputfile = fopen(output_file_name,"w");
    if(!outputfile){
	printf("could not open file: %s\n",output_file_name);
	exit(1);
    }
    int bytes_written = 0;
    	

    //int i;
    //for( i=0; i<(NUM_SECONDS*SAMPLE_RATE)/FRAMES_PER_BUFFER; ++i )
    while(1)
    {
       //printf("%d ",i);
       err = Pa_WriteStream( stream, sampleBlock, FRAMES_PER_BUFFER );
       if( err && CHECK_UNDERFLOW ) goto xrun;
       err = Pa_ReadStream( stream, sampleBlock, FRAMES_PER_BUFFER );
       if( err && CHECK_OVERFLOW ) goto xrun;
       fwrite(sampleBlock,1,numBytes,outputfile);
       bytes_written += numBytes;

	//Calculate average level
	unsigned short * short_ptr = (unsigned short *) sampleBlock;	
	int j;
	int sum=0;
	for(j=0; j<(numBytes/sizeof(short)); j++){
		//Prints out each short
		//printf("%04x ",short_ptr[j]);
		sum+=short_ptr[j];	
	}
	int average_level = sum/(numBytes/sizeof(short));
	printf("average_level: %d\n",average_level);
	if(average_level<25000 && average_level>10000) break;	
    }
    
    
    raw_to_wav(output_file_name, output_wave_file_name, bytes_written);
    printf("Channels: %d\n",NUM_CHANNELS);    
    printf("Bytes Written: %d\n",bytes_written);
    printf("FRAMES_PER_BUFFER: %d\n",FRAMES_PER_BUFFER);
    printf("numBytes: %d\n",numBytes);

    err = Pa_StopStream( stream );
    if( err != paNoError ) goto error;

    CLEAR( sampleBlock );
/*
    err = Pa_StartStream( stream );
    if( err != paNoError ) goto error;
    printf("Wire on. Interrupt to stop.\n"); fflush(stdout);

    while( 1 )
    {
       err = Pa_WriteStream( stream, sampleBlock, FRAMES_PER_BUFFER );
       if( err ) goto xrun;
       err = Pa_ReadStream( stream, sampleBlock, FRAMES_PER_BUFFER );
       if( err ) goto xrun;
    }
    err = Pa_StopStream( stream );
    if( err != paNoError ) goto error;

    Pa_CloseStream( stream );
*/
    free( sampleBlock );

    Pa_Terminate();
    return 0;

xrun:
    if( stream ) {
       Pa_AbortStream( stream );
       Pa_CloseStream( stream );
    }
    free( sampleBlock );
    Pa_Terminate();
    if( err & paInputOverflow )
       fprintf( stderr, "Input Overflow.\n" );
    if( err & paOutputUnderflow )
       fprintf( stderr, "Output Underflow.\n" );
    return -2;

error:
    if( stream ) {
       Pa_AbortStream( stream );
       Pa_CloseStream( stream );
    }
    free( sampleBlock );
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return -1;
}

