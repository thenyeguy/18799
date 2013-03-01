#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "feature.h"


feature_vectors* features_from_file(char* filename)
{
    //First open file
    FILE* file = fopen(filename, "r");
    if(file == NULL)
    {
        printf("Could not open file: %s\n\n",filename);
        exit(1);
    }

    //Figure out the number of vectors we have by counting lines
    //Then reset to beginning to read
    char c;
    int lines = 0;
    while((c = fgetc(file)) != EOF)
    {
        if(c == '\n')
            lines++;
    }
    fseek(file, 0, SEEK_SET);

    //Allocate space for features
    feature_vectors* fs = malloc(sizeof(feature_vectors));
    fs->num_vectors = lines;
    fs->features = malloc(lines*sizeof(feature));

    //For each line, read in the raw cepstrum
    //char line[512];
    double** raws = malloc(fs->num_vectors*sizeof(double*));
    for(int i = 0; i < fs->num_vectors; i++)
    {
        raws[i] = malloc(CEPSTRUM_DIMENSION*sizeof(double));
        //fgets(line, 512, file);
        for(int j = 0; j < CEPSTRUM_DIMENSION; j++)
            fscanf(file,"%lf",&raws[i][j]);

        //Ignore infinite/nan lines
        if(!isfinite(raws[i][0]))
        {
            i--;
            fs->num_vectors--;
        }
    }

    //Process the raws
    cepstrum_vectors* raw_cepstra = malloc(sizeof(cepstrum_vectors));
    raw_cepstra->num_vectors = lines;
    raw_cepstra->num_points = CEPSTRUM_DIMENSION;
    raw_cepstra->cepstra = raws;
    raw_cepstrum_to_features(raw_cepstra, fs);

    //Free memory
    for(int i = 0; i < fs->num_vectors; i++)
        free(raws[i]); 
    free(raws);
    free(raw_cepstra);

    return fs;
}


void raw_cepstrum_to_features(cepstrum_vectors* raws, feature_vectors* output)
{
    //First copy in the raw cepstrum
    for(int i = 0; i < output->num_vectors; i++)
        memcpy(&(output->features[i].values), raws->cepstra[i],
               CEPSTRUM_DIMENSION*sizeof(double));

    //Then mean normalize
    double mean[CEPSTRUM_DIMENSION];
    for(int i = 0; i < CEPSTRUM_DIMENSION; i++)
        mean[i] = 0.0;

    for(int i = 0; i < output->num_vectors; i++)
        for(int j = 0; j < CEPSTRUM_DIMENSION; j++)
            mean[j] += output->features[i].values[j];

    for(int i = 0; i < CEPSTRUM_DIMENSION; i++)
        mean[i] /= output->num_vectors;

    for(int i = 0; i < output->num_vectors; i++)
        for(int j = 0; j < CEPSTRUM_DIMENSION; j++)
            output->features[i].values[j] -= mean[j];

    //Then variance normalize
    double var[CEPSTRUM_DIMENSION];
    for(int i = 0; i < CEPSTRUM_DIMENSION; i++)
        var[i] = 0.0;

    for(int i = 0; i < output->num_vectors; i++)
        for(int j = 0; j < CEPSTRUM_DIMENSION; j++)
            var[j] += pow(output->features[i].values[j], 2);

    for(int i = 0; i < CEPSTRUM_DIMENSION; i++)
        var[i] = sqrt(var[i]/output->num_vectors);

    for(int i = 0; i < output->num_vectors; i++)
        for(int j = 0; j < CEPSTRUM_DIMENSION; j++)
            output->features[i].values[j] /= var[j];

    //Then compute single deltas - copy second single delta into first
    for(int i = 1; i < output->num_vectors; i++)
        for(int j = 0; j < CEPSTRUM_DIMENSION; j++)
            output->features[i].deltas[j] =
                output->features[i].values[j] - 
                output->features[i-1].values[j];

    for(int i = 0; i < CEPSTRUM_DIMENSION; i++)
        output->features[0].deltas[i] = 
            output->features[1].deltas[i];

    //Then compute double deltas - copy second double delta into first
    for(int i = 1; i < output->num_vectors; i++)
        for(int j = 0; j < CEPSTRUM_DIMENSION; j++)
            output->features[i].doubles[j] =
                output->features[i].deltas[j] - 
                output->features[i-1].deltas[j];

    for(int i = 0; i < CEPSTRUM_DIMENSION; i++)
        output->features[0].doubles[i] = 
            output->features[1].doubles[i];
}


void print_feature(feature* f)
{
    printf("FEATURE: Cepstra\t| Deltas\t| Double deltas\n");
    for(int i = 0; i < CEPSTRUM_DIMENSION; i++)
    {
        printf("\t %5.5f\t| %5.5f\t| %5.5f\n",
               f->values[i], f->deltas[i], f->doubles[i]);
    }
}


void print_feature_vectors(feature_vectors* fs)
{
    printf("FEATURE VECTORS PACKET {{{{{{{{{{{{\n");
    for(int i = 0; i < fs->num_vectors; i++)
        print_feature(&fs->features[i]);
    printf("}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}\n\n");
}


void free_feature_vectors(feature_vectors* fs)
{
    free(fs->features);
    free(fs);
}
