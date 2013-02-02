#include <stdlib.h>
#include <stdio.h>

#include "wav_utils.h"

/* 
raw_to_wav: Convert a file of raw audio data to a wav file

*/
int raw_to_wav(char *infile_name, char *outfile_name, int data_length) {
	
	FILE* infile = fopen(infile_name, "r+");
	FILE* outfile = fopen(outfile_name, "w+");

	char* header = malloc(WAV_HEADER_SIZE);
	make_header(header, data_length);
	
	fwrite(header, 1, WAV_HEADER_SIZE, outfile);

	void* buf = malloc(BUFFER_SIZE);

	while (fread(buf, 1, BUFFER_SIZE, infile)) {
		fwrite(buf, 1, BUFFER_SIZE, outfile);
	}
	
	fclose(infile);
	fclose(outfile);
	
	return 0;
}


void make_header(char* header, int data_length) {
	/* make a wav header based on the information passed into the function */
<<<<<<< HEAD
	memset(header, RIFF_ID, 4);
	memset(header+4, CHUNK_SIZE, 4);
	memset(header+8, WAVE_ID, 4);
	memset(header+12, FMT_ID, 4);
	memset(header+16, SUBCHUNK_1_SIZE, 4);
	memset(header+20, AUDIO_FORMAT, 2);
	memset(header+22, HEADER_NUM_CHANNELS, 2);
=======
	sprintf(header, "Hello, I am a wav header\n\n\n");

	

	//AudioFormat
	(header)[20] = 0x01;
	(header)[21] = 0x00;
	(header)[22] = 0x02;
	(header)[23] = 0x00;

	//SampleRate
	(header)[24] = 0x22;
	(header)[25] = 0x56;
	(header)[26] = 0x00;
	(header)[27] = 0x00;

	//ByteRate
	(header)[28] = 0x88;
	(header)[29] = 0x58;
	(header)[30] = 0x01;
	(header)[31] = 0x00;

	//Block Align
	(header)[32] = 0x04;
	(header)[33] = 0x00;

	//BitsPerSample
	(header)[34] = 0x10;
	(header)[35] = 0x00;

	//data
	(header)[36] = 0x64;
	(header)[37] = 0x61;
	(header)[38] = 0x74;
	(header)[39] = 0x61;
	
	
	
>>>>>>> Second half of Header Buffer
}

