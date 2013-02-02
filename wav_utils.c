#include <stdlib.h>
#include <stdio.h>

#include "wav_utils.h"

/* 
raw_to_wav: Convert a file of raw audio data to a wav file

*/
int raw_to_wav(char *infile_name, char *outfile_name, int num_channels, int sample_rate, int bits_per_sample, int data_length) {
	printf("I was called!!!\n");
	
	FILE* infile = fopen(infile_name, "r+");
	FILE* outfile = fopen(outfile_name, "w+");

	printf("infile=%s, outfile=%s\n", infile_name, outfile_name);

	char* header = malloc(WAV_HEADER_SIZE);
	make_header(num_channels, sample_rate, bits_per_sample, header, data_length);
	
	fwrite(header, 1, WAV_HEADER_SIZE, outfile);

	void* buf = malloc(BUFFER_SIZE);

	while (fread(buf, 1, BUFFER_SIZE, infile)) {
		fwrite(buf, 1, BUFFER_SIZE, outfile);
	}
	
	fclose(infile);
	fclose(outfile);
	
	return 0;
}


void make_header(int num_channels, int sample_rate, int bits_per_sample, char* header, int data_length) {
	/* make a wav header based on the information passed into the function */
	sprintf(header, "Hello, I am a wav header\n\n\n");

}

