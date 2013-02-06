#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include "portaudio_utils.h"
#include <time.h>
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

	void * buf = malloc(BUFFER_SIZE);

	while (fread(buf, 1, BUFFER_SIZE, infile)) {
		fwrite(buf, 1, BUFFER_SIZE, outfile);
	}
	
	//printf("Data transfered: %d\n",data_transfered);
	
	fclose(infile);
	fclose(outfile);
	return 0;
}


void make_header(char* header, int data_length) {
	
	int subchunk1size = 16; //always
	int subchunk2size = data_length;
	int chunksize = 4 + (8 + subchunk1size) + (8 + subchunk2size);
	
	//Chunk Descriptor
	(header)[0] = 0x52;
	(header)[1] = 0x49;
	(header)[2] = 0x46;
	(header)[3] = 0x46;
	
	//ChunkSize: 2084	
	int * int_header = (int*)header;
	int_header[1] = chunksize;
	
	//WAVE
	(header)[8] = 0x57;
	(header)[9] = 0x41;
	(header)[10] = 0x56;
	(header)[11] = 0x45;
	
	//fmt subchunk
	(header)[12] = 0x66;
	(header)[13] = 0x6d;
	(header)[14] = 0x74;
	(header)[15] = 0x20;
	
	//Subchunk 1 Size: 16
	(header)[16] = 0x10;
	(header)[17] = 0x00;
	(header)[18] = 0x00;
	(header)[19] = 0x00;
	
	//AudioFormat
	(header)[20] = 0x01;
	(header)[21] = 0x00;
	
	//Num Channels: 1
	(header)[22] = 0x01;
	(header)[23] = 0x00;

	
	//SampleRate: 16,000
	int_header[6] = SAMPLE_RATE;

	//ByteRate: 32,000
	int_header[7] = SAMPLE_RATE * sizeof(SAMPLE);

	//Block Align
	(header)[32] = 0x02;	
	(header)[33] = 0x00;

	//BitsPerSample: 16
	(header)[34] = 0x10;
	(header)[35] = 0x00;

	//data
	(header)[36] = 0x64;
	(header)[37] = 0x61;
	(header)[38] = 0x74;
	(header)[39] = 0x61;
	
	//SubChunk2Size:	
	int_header[10] = subchunk2size;
}


char * get_time_stamped_filename(char * prefix,char * suffix){
	time_t sys_time = time(NULL);
    	//char * prefix = "./recordings/recorded-";
    	char timestamp[32];
	sprintf(timestamp,"%lu",(long unsigned) sys_time);
        int prefix_length = strlen(prefix);
    	int suffix_length = strlen(suffix);
	int time_length = strlen(timestamp);
    	int total_length = prefix_length + suffix_length + time_length;
	char temp_filename[256];
	snprintf(temp_filename, total_length+1, "%s%s%s", prefix, timestamp, suffix);
		
	//char wavFileName[256];
	//char * wavFileName = (char * ) malloc(256);
	//char *wav_suffix = ".wav";
	int filename_length = strlen(temp_filename);
	char * filename = (char *) malloc(filename_length);
	snprintf(filename, filename_length+1, "%s%s%s", prefix, timestamp, suffix);

	return filename;
}



