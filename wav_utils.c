#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
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
	
	printf("Data Length: %d\n",data_length);
	
	//Chunk Descriptor
	(header)[0] = 0x52;
	(header)[1] = 0x49;
	(header)[2] = 0x46;
	(header)[3] = 0x46;
	
	//ChunkSize: 2084		//file_size - 8 or data+36?
	(header)[4] = 0x96;
	(header)[5] = 0x92;
	(header)[6] = 0x00;
	(header)[7] = 0x00;				
	
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
	
	//Subchunk Size: 16
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
	(header)[24] = 0x11;			//(header)[24] = 0x80;
	(header)[25] = 0x2B;				//(header)[25] = 0x3E;
	(header)[26] = 0x00;
	(header)[27] = 0x00;

	//ByteRate: 32,000
	(header)[28] = 0x11;	//(header)[28] = 0x00;
	(header)[29] = 0x2b;	//(header)[29] = 0x7D;
	(header)[30] = 0x00;
	(header)[31] = 0x00;

	//Block Align
	(header)[32] = 0x02;	//4 or 2?
	(header)[33] = 0x00;

	//BitsPerSample: 16
	(header)[34] = 0x10;
	(header)[35] = 0x00;

	//data
	(header)[36] = 0x64;
	(header)[37] = 0x61;
	(header)[38] = 0x74;
	(header)[39] = 0x61;
	
	//SubChunk2Size: NumSamples * NumChannels * BitsPerSample/8
	//int subchunk2size = NUMSAMPLES * NUMCHANNELS * BITSPERSAMPLE / 8 = file_size-44;
	(header)[40] = 0x66;
	(header)[41] = 0x92;
	(header)[42] = 0x00;
	(header)[43] = 0x00;
	
	
	int i;
	for(i=0; i <44; i++){
		if(i%4==0){
			printf("\n");
		}
		printf("%02x ",((unsigned char *)header)[i]);
	}
}

