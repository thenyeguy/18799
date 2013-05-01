#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../dtw/cluster.h"
#include "../training/training_utils.h"
#include "phoneme_utils.h"
#include "phoneme_continuous_trainer.h"


gaussian_cluster** train_continuous_phoneme_models(feature_vectors** recordings,
    char*** word_names, int* num_words, int num_recordings,
    gaussian_cluster** initials, gaussian_cluster* sil_init)
{
    // Initialize flat arrays
    int* num_models = calloc(num_recordings, sizeof(int));
    gaussian_cluster*** models =
        calloc(num_recordings, sizeof(gaussian_cluster**));

    // For each recording, create our array of models to train against
    for(int i = 0; i < num_recordings; i++)
    {
        // Count number of models
        int this_model_num = 0;
        for(int j = 0; j < num_words[i]; j++)
        {
            char* word = word_names[i][j];
            if(strcmp(word, "sil") == 0)
                this_model_num++;
            else
                this_model_num += phonemes_in_word[wordToModelIndex(word)];
        }

        gaussian_cluster** this_model =
            calloc(this_model_num, sizeof(gaussian_cluster*));

        // Add each phoneme model in
        // Special case silence
        int next_i = 0;
        for(int j = 0; j < num_words[i]; j++)
        {
            char* word = word_names[i][j];
            if(strcmp(word, "sil") == 0)
            {
                this_model[next_i] = sil_init;
                next_i++;
                continue;
            }

            // For each phoneme in the word, add its model
            int num_phonemes = phonemes_in_word[wordToModelIndex(word)];
            char** phonemes = word_phonemes[wordToModelIndex(word)];
            for(int k = 0; k < num_phonemes; k++)
            {
                this_model[next_i] = initials[phonemeToModelIndex(phonemes[k])];
                next_i++;
            }
        }

        // Add to training data
        models[i] = this_model;
        num_models[i] = this_model_num;
    }


    // Run continuous trainer
    gaussian_cluster** results =
        train_from_recordings(recordings, num_recordings, models, num_models,
            phoneme_names, NUM_PHONEMES);

    return results;
}


gaussian_cluster** read_phoneme_set(char* setname)
{
    // For each phoneme name, add the setname path and read the file
    gaussian_cluster** results = calloc(NUM_PHONEMES, sizeof(gaussian_cluster*));
    for(int i = 0; i < NUM_PHONEMES; i++)
    {
        char buffer[128];
        sprintf(buffer, "phonemes/%s-%s", phoneme_names[i], setname);
        results[i] = read_cluster_from_file(buffer);
    }

    return results;
}
