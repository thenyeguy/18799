#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "../record/portaudio_utils.h"
#include "cepstrum_utils.h"
#include "cepstrum_vectors.h"


int main(int argc, char **argv)
{
    if(argc < 2)
    {
        printf("Usage: ./cepstrum [file to read] [optional number of mel filters]\n");
        printf("    Runs cepstrum vector analysis and outputs to cepstrum\n\n");
        printf("E.G. \"./cepstrum mytest\" reads in:\n");
        printf("         recordings/mytest.out\n");
        printf("     and saves files such as:\n");
        printf("         analysis/mytest-cepstrum.txt\n");
        exit(1);
    }

    //Determine the specified recording out file
    char data_path[256];

	//Determine number of filters
    int filter_num = 40;
    if(argc > 2)
        filter_num = atoi(argv[2]);
	
	sprintf(data_path, "recordings/%s.out", argv[1]);
    printf("Reading recorded data from:\n    %s\n\n", data_path);
	
	char name[256];
	sprintf(name, "%s-%d", argv[1], filter_num);

    //Open the file and determine its length to read in
    //Then close it when done
    FILE* data = fopen(data_path, "rb"); 
    if(data == NULL)
    {
        printf("Could not open specified file.\n");
        exit(1);
    }

    fseek(data,0,SEEK_END);
    size_t size = ftell(data)/sizeof(SAMPLE);
    fseek(data,0,SEEK_SET);

    SAMPLE* buffer = calloc(sizeof(SAMPLE), size);
    fread(buffer, sizeof(SAMPLE), size, data);

    fclose(data);
    printf("Data read. Samples: %u\n\n", (unsigned)size);

    
    //Processing needs doubles, so copy over intoa  double buffer
    double* sample = calloc(sizeof(double), size);
    for(int i = 0; i < size; i++)
        sample[i] = ((double) buffer[i]);// / 32768.0;

    get_cepstrum_vectors(sample, size, filter_num, 13, name, SAMPLE_RATE);

    //Clean up
    printf("Completed anaylsis. Output files in analysis directory.\n");
    free(buffer);
    free(sample);

    return 0;
}
