#include <stdlib.h>
#include <stdio.h>

#include "wav_utils.h"


/* 
raw_to_wav: Convert a file of raw audio data to a wav file

*/
int raw_to_wav(char *infile_name, char *outfile_name, int num_channels, int sample_rate, int bits_per_sample) {
	FILE* infile = fopen(infile_name, "r+");
	FILE* outfile = fopen(outfile_name, "w+");
	
	char* header = malloc(WAV_HEADER_SIZE);
	make_header(num_channels, sample_rate, bits_per_sample, header);
	
	fwrite(header, sizeof(char), 

	
}


void make_header(int num_channels, int sample_rate, int bits_per_sample, char* header) {
	
}

