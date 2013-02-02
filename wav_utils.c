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
	memset(header, RIFF_ID, 4);
	memset(header+4, CHUNK_SIZE, 4);
	memset(header+8, WAVE_ID, 4);
	memset(header+12, FMT_ID, 4);
	memset(header+16, SUBCHUNK_1_SIZE, 4);
	memset(header+20, AUDIO_FORMAT, 2);
	memset(header+22, HEADER_NUM_CHANNELS, 2);
}
