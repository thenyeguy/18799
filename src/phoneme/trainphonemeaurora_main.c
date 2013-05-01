#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../record/portaudio_utils.h"
#include "../cepstrum/cepstrum_vectors.h"
#include "../training/training_utils.h"
#include "phoneme_utils.h"
#include "phoneme_continuous_trainer.h"

/* 
 * Reads in data from the aurora data set and runs it through the continuous
 * trainer. Saves its hmm results as template-aurora.hmm.
 */



int main(int argc, char** argv)
{
    if(argc < 2)
    {
        printf("USAGE: bin/trainphonemeaurora [initial models]\n");
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

    int* num_words = calloc(num_templates, sizeof(int));
    char*** words =
        calloc(num_templates, sizeof(gaussian_cluster**));
    

    // Initialize models
    // Read in initial models
    printf("\nInitializing models...\n");
    char* set = argv[1];

    // Initial phonemes
    gaussian_cluster** initials = read_phoneme_set(set);
    
    // Initial silence
    char silname[52]; sprintf(silname, "sil-%s", set);
    gaussian_cluster* sil_init = read_cluster_from_file(silname);



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
        char** model = calloc(n, sizeof(gaussian_cluster*));
        tok = strtok(buffer, " \n");
        for(int j = 0; j < n; j++)
        {
            model[j] = calloc(52,sizeof(char));
            strcpy(model[j],tok);
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
        strcpy(features->word_id, filename);
        raw_cepstrum_to_features(cepstra,features);

        // Free extra memory
        free(samples);
        free(samples_d);
        free(cepstra);


        // Store results and continue
        templates[i] = features;
        words[i] = model;
        num_words[i] = n;
    }


    // Actually run training
    printf("Running training and clustering algorithm...\n\n");
    gaussian_cluster** phoneme_models =
        train_continuous_phoneme_models(templates, words, num_words,
            num_templates, initials, sil_init);
    gaussian_cluster** word_models = phonemes_to_word_models(phoneme_models);


    // Save results
    printf("Now saving results...\n");

    for(int i = 0; i < NUM_PHONEMES; i++)
    {
        printf("    hmms/phonemes/%s-aurora.hmm\n", phoneme_names[i]);

        char filename[128];
        sprintf(filename, "phonemes/%s-aurora", phoneme_names[i]);
        write_cluster_to_file(filename, phoneme_models[i]);
    }

    for(int i = 0; i < NUM_WORDS; i++)
    {
        printf("    hmms/%s-phoneme-aurora.hmm\n", word_names[i]);

        char filename[128];
        sprintf(filename, "%s-phoneme-aurora", word_names[i]);
        write_cluster_to_file(filename, word_models[i]);
    }

    printf("Done!\n\n");


    return 0;
}
