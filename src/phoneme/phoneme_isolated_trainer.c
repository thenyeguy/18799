#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../training/training_utils.h"
#include "phoneme_utils.h"
#include "phoneme_isolated_trainer.h"



gaussian_cluster** train_isolated_phoneme_models(recording_set** recordings,
    int num_digits, int num_iterations)
{

    // Get initial seeds
    gaussian_cluster** models =
        get_phoneme_initial_models(recordings, num_digits);

    // Run training with previous result as seed
    for(int i = 0; i < num_iterations; i++)
    {
	//FIXME, something seems to go wrong starting here according to backtrace
        models = train_isolated_phoneme_step(recordings, num_digits, models);
    }

    return models;
}


gaussian_cluster** train_isolated_phoneme_step(recording_set** recordings,
    int num_digits, gaussian_cluster** initials)
{
    /* Rejigger our data to plug into the continuous training code
     */
    int num_recordings = 0;
    for(int i = 0; i < num_digits; i++)
        num_recordings += recordings[i]->num_recordings;

    //Then create our flat arrays
    feature_vectors** all_recordings =
        calloc(num_recordings, sizeof(feature_vectors*));
    gaussian_cluster*** models =
        calloc(num_recordings, sizeof(gaussian_cluster**));
    int* num_models = calloc(num_recordings, sizeof(int));

    //Fill the arrays from our recording sets
    int next_i = 0;
    for(int i = 0; i < num_digits; i++)
    {
        //Get our set
        recording_set* set = recordings[i];

        //Create the set of models
        int word_index = wordToModelIndex(set->name);
        int set_num_models = phonemes_in_word[word_index];;
        gaussian_cluster** set_model =
            calloc(set_num_models, sizeof(gaussian_cluster*));

        //Find phonemes and get their intial models
        char** set_phoneme_names = word_phonemes[word_index];
        for(int j = 0; j < set_num_models; j++)
        {
            gaussian_cluster* phoneme_model = NULL;
            for(int k = 0; k < NUM_PHONEMES; k++)
            {
                if(strcmp(set_phoneme_names[j],phoneme_names[k]) == 0)
                {
                    phoneme_model = initials[k];
                }
            }
            set_model[j] = phoneme_model;
        }

        //Fill the flat arrays for each isolated recording
        for(int j = 0; j < set->num_recordings; j++)
        {
            all_recordings[next_i] = set->recordings[j];
            models[next_i] = set_model;
            num_models[next_i] = set_num_models;

            // Go to next index in flat array
            next_i++;
        }
    }



    /* Run the continuous trainer using these flattened arrays of data
     */
    gaussian_cluster** result;
    result = train_from_recordings(all_recordings, num_recordings,
        models, num_models, phoneme_names, NUM_PHONEMES);

    return result;
}
