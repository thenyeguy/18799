#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../record/portaudio_utils.h"
#include "../dtw/feature.h"
#include "../grammar/grammar.h"
#include "../grammar/viterbi.h"

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        printf("Usage: ./scoreaurora [recording.out]\n");
        printf("    Performs a viterbi continuous recognition of the");
        printf("test utterance\n");
        printf("    using the specified grammar as its structure.\n\n");
        exit(1);
    }

    //Parse args
    char* grammar_file = "grammar/aurora.txt";
    char* test_file = argv[1];

    //Read in the files
    printf("Building grammar from %s...\n",grammar_file);
    grammar* g = build_grammar(grammar_file);


    //Get the file cepstrum
    //Strip first and last char to get rid of parens
    char filename[52];
    sprintf(filename, "../hwdata/test/%s.wav",test_file);
    printf("    Reading file: %s\n",filename);

    // Set up the wav file for reading
    FILE* data = fopen(filename,"rb");
    if(data == NULL)
    {
        printf("    Could not open file... aborting.\n");
        exit(1);
    }

    // Determine length, minus wav header of 44 bytes
    fseek(data,0,SEEK_END);
    size_t size = (ftell(data) - 44)/sizeof(SAMPLE);

    // Reset to start of data after header, then read into buffer
    fseek(data,44,SEEK_SET);
    SAMPLE* samples = calloc(size, sizeof(SAMPLE));
    fread(samples, sizeof(SAMPLE), size, data);
    fclose(data);

    // Get cepstrum data
    double* samples_d = calloc(size, sizeof(double));
    for(int i = 0; i < size; i++)
        samples_d[i] = (double) samples[i];
    cepstrum_vectors* cepstra = 
        get_cepstrum_vectors(samples_d, size, 40, 13, NULL, 8000);

    // Convert to features
    feature_vectors* features = malloc(sizeof(feature_vectors));
    features->num_vectors = cepstra->num_vectors;
    features->features = calloc(cepstra->num_vectors, sizeof(feature));
    strcpy(features->word_id, "empty");
    raw_cepstrum_to_features(cepstra,features);


    //Run the test and display result
    printf("Testing against %s...\n",filename);
    char* result = viterbi_search(g, features, 0.5);
    printf("\nResult: %s\n", result);

    return 0;
}
