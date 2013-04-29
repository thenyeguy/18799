#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../record/portaudio_utils.h"
#include "../cepstrum/cepstrum_vectors.h"
#include "training_utils.h"

/* 
 * Reads in data from the aurora data set and runs it through the continuous
 * trainer. Saves its hmm results as template-aurora.hmm.
 */


/* Used to convert a string like "0" to the index into initial_models
 */
int wordToIndex(char* word)
{
    if(strcmp(word,"zero") == 0)
        return 0;
    else if(strcmp(word,"one") == 0)
        return 1;
    else if(strcmp(word,"two") == 0)
        return 2;
    else if(strcmp(word,"three") == 0)
        return 3;
    else if(strcmp(word,"four") == 0)
        return 4;
    else if(strcmp(word,"five") == 0)
        return 5;
    else if(strcmp(word,"six") == 0)
        return 6;
    else if(strcmp(word,"seven") == 0)
        return 7;
    else if(strcmp(word,"eight") == 0)
        return 8;
    else if(strcmp(word,"nine") == 0)
        return 9;
    else if(strcmp(word,"sil") == 0)
        return 10;
    else if(strcmp(word,"oh") == 0)
        return 11;

    return -1;
}


int main(int argc, char** argv)
{
    if(argc < 2)
    {
        printf("USAGE: bin/trainaurora [initial models]\n");
        printf("       Initial model options:\n");
        printf("          init - naive isolated word models\n");
        printf("          new  - our voices, continuously trained\n");
        printf("          aurora - the last trained aurora dataset\n");
        printf("\n");
        exit(1);
    }

    // Count number of training instances
    unsigned buffer_size = 512;
    char buffer[buffer_size];
    FILE* transcript = fopen("../hwdata/TRAIN.transcripts","r");
    if(transcript == NULL)
    {
        printf("Could not open transcript file\n");
        exit(1);
    }

    int num_templates = 0;
    while(fgets(buffer, buffer_size, transcript) != NULL)
        num_templates++;


    // Initialize containers
    feature_vectors** templates =
        calloc(num_templates, sizeof(feature_vectors*));

    int* num_models = calloc(num_templates, sizeof(int));
    gaussian_cluster*** models =
        calloc(num_templates, sizeof(gaussian_cluster**));
    

    // Initialize models
    printf("\nInitializing models...\n");
    gaussian_cluster** initial_models = calloc(12,sizeof(gaussian_cluster*));
    char* model = argv[1];

    char zero_model[32]; sprintf(zero_model, "zero-%s", model);
    initial_models[0] = read_cluster_from_file(zero_model);

    char one_model[32]; sprintf(one_model, "one-%s", model);
    initial_models[1] = read_cluster_from_file(one_model);

    char two_model[32]; sprintf(two_model, "two-%s", model);
    initial_models[2] = read_cluster_from_file(two_model);

    char three_model[32]; sprintf(three_model, "three-%s", model);
    initial_models[3] = read_cluster_from_file(three_model);

    char four_model[32]; sprintf(four_model, "four-%s", model);
    initial_models[4] = read_cluster_from_file(four_model);

    char five_model[32]; sprintf(five_model, "five-%s", model);
    initial_models[5] = read_cluster_from_file(five_model);

    char six_model[32]; sprintf(six_model, "six-%s", model);
    initial_models[6] = read_cluster_from_file(six_model);

    char seven_model[32]; sprintf(seven_model, "seven-%s", model);
    initial_models[7] = read_cluster_from_file(seven_model);

    char eight_model[32]; sprintf(eight_model, "eight-%s", model);
    initial_models[8] = read_cluster_from_file(eight_model);

    char nine_model[32]; sprintf(nine_model, "nine-%s", model);
    initial_models[9] = read_cluster_from_file(nine_model);

    char sil_model[32]; sprintf(sil_model, "sil-%s", model);
    initial_models[10] = read_cluster_from_file(sil_model);

    char oh_model[32]; sprintf(oh_model, "oh-%s", model);
    initial_models[11] = read_cluster_from_file(oh_model);


    // Initialize corpus
    int num_words = 12;
    char** words = calloc(12,sizeof(char*));
    words[0] = "zero";
    words[1] = "one";
    words[2] = "two";
    words[3] = "three";
    words[4] = "four";
    words[5] = "five";
    words[6] = "six";
    words[7] = "seven";
    words[8] = "eight";
    words[9] = "nine";
    words[10] = "sil";
    words[11] = "oh";


    // Read in our recordings and create their model arrays
    printf("Reading training templates...\n");
    fclose(transcript);
    transcript = fopen("../hwdata/TRAIN.transcripts","r");

    for(int i = 0; i < num_templates; i++)
    {
        // Read line
        fgets(buffer, buffer_size, transcript);

        // Count number of models
        // Use a temp buffer because strtok is destructive
        char temp[buffer_size];
        strcpy(temp,buffer);
        int n = 0;
        char* tok = strtok(temp, " \n");
        while(strtok(NULL, " \n") != NULL)
            n++;

        // Read models and copy into template
        gaussian_cluster** model = calloc(n, sizeof(gaussian_cluster*));
        tok = strtok(buffer, " \n");
        for(int j = 0; j < n; j++)
        {
            model[j] = initial_models[wordToIndex(tok)];
            tok = strtok(NULL, " \n");
        }


        //Strip first and last char to get rid of parens
        char* name = tok+1;
        name[strlen(name)-1] = '\0';
        char filename[52];
        sprintf(filename, "../hwdata/train/%s.wav",name);
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

        // Free extra memory
        free(samples);
        free(samples_d);
        free(cepstra);


        // Store results and continue
        templates[i] = features;
        models[i] = model;
        num_models[i] = n;
    }


    // Actually run training
    printf("Running training and clustering algorithm...\n\n");
    gaussian_cluster** results =
        train_from_recordings(templates, num_templates, models, num_models,
                              words, num_words);


    // Save results
    printf("Now saving results...\n");

    for(int i = 0; i < num_words; i++)
    {
        printf("    hmms/%s-aurora.hmm\n", words[i]);

        char filename[128];
        sprintf(filename, "%s-aurora", words[i]);
        write_cluster_to_file(filename, results[i]);
    }

    printf("Done!\n\n");


    return 0;
}
