#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "phoneme_utils.h"


/* Define the phonemes used in each word
 */
char* zero[4] = {"Z","IY","R","OW"};
char* one[3] = {"W","AX","N"};
char* two[2] = {"T","UW"};
char* three[3] = {"TH","R","IY"}; 
char* four[3] = {"F","OW","R"};
char* five[3] = {"F","AY","V"};
char* six[4] = {"S","IH","K","S"};
char* seven[5] = {"S","EH","V","EH","N"};
char* eight[2] = {"EY","T"};
char* nine[3] = {"N","AY","N"};


/* Packge the phoneme names and word models together
 */
char* phoneme_names[NUM_PHONEMES] = {"AX", "AH", "AY", "EH", "EY", "F", "IH",
        "IY", "K", "N", "OW", "R", "S", "T", "TH", "UW", "V", "W", "Z"};
char* word_names[NUM_WORDS] =
    {"zero","one","two","three","four","five","six","seven","eight","nine"};

int phonemes_in_word[NUM_WORDS] = {4,3,2,3,3,3,4,5,2,3};
char** word_phonemes[NUM_WORDS] = {zero,one,two,three,four,five,six,seven,eight,nine};



gaussian_cluster** get_phoneme_initial_models(recording_set** recordings,int num_digits) {
	
	// Initialize a count of the number of feature vectors needed for each phoneme
	int feature_vectors_per_phoneme[NUM_PHONEMES];
	for (int i=0; i<NUM_PHONEMES; i++) {
		feature_vectors_per_phoneme[i] = 0;
	}

	// Loop through each digit in our array of recordings
	for (int i=0; i<num_digits; i++) {
		
		// Get list of phonemes in that digit
		char * digit_name = recordings[i]->name;
		int digit_index = wordToModelIndex(digit_name);
		char ** digit_phonemes = word_phonemes[digit_index];
		int num_phonemes_in_digit = phonemes_in_word[digit_index];
		
		// For each recording in our recording set, increment that phoneme count
		for (int j=0; j<num_phonemes_in_digit; j++) {
			int phoneme_index = phonemeToModelIndex(digit_phonemes[j]);
			feature_vectors_per_phoneme[phoneme_index] +=
            recordings[i]->num_recordings;
		}
	}	

	// Initialize array of feature vector arrays to ship off for clustering
	feature_vectors** feature_vectors_for_phonemes[NUM_PHONEMES];

	// Initializes the array of feature vector arrays for each phoneme to fill in by splitting
	for (int i=0; i<NUM_PHONEMES; i++) {
		feature_vectors_for_phonemes[i] = 
			(feature_vectors**)malloc(sizeof(feature_vectors*) * 
									feature_vectors_per_phoneme[i]);
	}

	// Initialize an array of indexes into the Phoneme array to keep track of which 
	// feature_vector we are on
	int phoneme_feature_vector_index[NUM_PHONEMES];
	for (int i=0; i<NUM_PHONEMES; i++) {
		phoneme_feature_vector_index[i]=0;
	}

	// Loop through each digit
	for (int i=0; i<num_digits; i++) {
		
		// Get the word, number of phonemes, and list of phonemes for each digit
		int word_index = wordToModelIndex(recordings[i]->name);
		int num_phonemes = phonemes_in_word[word_index];
		char** phonemes = word_phonemes[word_index];

		// Loop through each recording in the digit's recording set
		for (int j=0; j<(recordings[i]->num_recordings); j++) {
		
			// Split the recording into num_phoneme different phoneme slices
			feature_vectors** split = split_feature_vectors(recordings[i]->recordings[j], num_phonemes,phonemes);

			// Add the newly split phoneme into the phoneme feature vector array
			for(int k=0; k<num_phonemes; k++){
				
				// Figure out which phoneme this feature_vector is a member of
				char *phoneme_name = phonemes[k];
				int phoneme_index = phonemeToModelIndex(phoneme_name);
			
				// Figure out which feature vector within the phoneme array to put it	
				int feature_vector_index = phoneme_feature_vector_index[phoneme_index];
				
				// Actually set the feature vector to point to that slice of the recording
				feature_vectors_for_phonemes[phoneme_index][feature_vector_index] = split[k];

				// Increment the feature vector index for that phoneme
				phoneme_feature_vector_index[phoneme_index] ++;
			}			
		}

	}

	
	/* feature_vectors_for_phonemes now holds an array of phonemes where each 
	 * index is an array of feature vectors, now we must cluster each of these 
	 * indexes to get a gaussian params for each phoneme */


	gaussian_cluster ** phoneme_clusters = 
		(gaussian_cluster **) malloc(sizeof(gaussian_cluster*)*NUM_PHONEMES);
	for(int i=0; i<NUM_PHONEMES; i++){
		
		//Actually send off each array of feature vectors to be clustered and store in the phoneme cluster array
		phoneme_clusters[i] = cluster_templates(feature_vectors_for_phonemes[i],
					feature_vectors_per_phoneme[i],STATES_PER_PHONEME,phoneme_names[i]);
		
		//Test to see whether everything worked:
		/*
		single_gaussian_params ** phoneme_cluster_params = phoneme_clusters[i]->params;
		int num_clusters = phoneme_clusters[i]-> num_clusters;
		for(int k=0; k<num_clusters; k++){
			print_single_gaussian_params(phoneme_cluster_params[k]);
		}
		*/
	}
	
	return phoneme_clusters;
}


gaussian_cluster** phonemes_to_word_models(gaussian_cluster** phoneme_models)
{
    gaussian_cluster** words = calloc(NUM_WORDS, sizeof(gaussian_cluster*));

    // For each word, figure out what its phonemes are, then get the models and
    // concactenate the states
    for(int i = 0; i < NUM_WORDS; i++)
    {
        int states = STATES_PER_PHONEME * phonemes_in_word[i];

        // Allocate space and fill identifiers
        gaussian_cluster* word = malloc(sizeof(gaussian_cluster));
        strcpy(word->word_id, phoneme_names[i]);
        word->num_clusters = states;
        word->params = calloc(states, sizeof(single_gaussian_params*));
        word->stationary_probs = calloc(states, sizeof(double));
        word->transition_probs = calloc(states, sizeof(double));

        // Get phonemes and copy their data
        char** phonemes = word_phonemes[i];
        for(int j = 0; j < phonemes_in_word[i]; j++)
        {
            gaussian_cluster* phoneme =
                phoneme_models[phonemeToModelIndex(phonemes[j])];
            for(int k = 0; k < STATES_PER_PHONEME; k++)
            {
                word->params[STATES_PER_PHONEME*j + k] = phoneme->params[k];
                word->stationary_probs[STATES_PER_PHONEME*j + k] =
                    phoneme->stationary_probs[k];
                word->transition_probs[STATES_PER_PHONEME*j + k] =
                    phoneme->transition_probs[k];
            }
        }

        // Add to results
        words[i] = word;
    }

    return words;
}


feature_vectors** split_feature_vectors(feature_vectors* input_vectors, int num_segments,char **new_names) {

	/// determine the number of segments per split vector 
	int segment_length = (input_vectors -> num_vectors) / num_segments;
	int remaining_vectors = (input_vectors->num_vectors) % num_segments;

	/// allocate memory for the new feature_vectors list
	feature_vectors** segment_list = 
					(feature_vectors**)malloc(num_segments*sizeof(feature_vectors*));

	/// fill in all but the last feature vector (to make sure we account for all 
	/// vectors in the original feature_vectors)
	for (int i = 0; i < num_segments; i++) {
        // Add last segment onto end
        if(i == num_segments-1) segment_length += remaining_vectors;
		segment_list[i] = malloc(sizeof(feature_vectors)); 
		segment_list[i]->num_vectors = segment_length;

		strcpy(segment_list[i]->word_id, new_names[i]);
		segment_list[i]->features = malloc(segment_length*sizeof(feature));
		for (int j = 0; j < segment_length; j++) {
			/// copy features from input_vectors to new segment
			memcpy(&(segment_list[i]->features[j]),
					&(input_vectors->features[i*segment_length+j]),
					sizeof(feature));
		}
	}
	
	return segment_list;
}

int phonemeToModelIndex(char* phoneme) {
	if (!strcmp(phoneme,"AX"))
		return 0;
	else if (!strcmp(phoneme,"AH"))
		return 1;
	else if (!strcmp(phoneme,"AY"))
		return 2;
	else if (!strcmp(phoneme,"EH"))
		return 3;
	else if (!strcmp(phoneme,"EY"))
		return 4;
 	else if (!strcmp(phoneme,"F"))
		return 5;
 	else if (!strcmp(phoneme,"IH"))
		return 6;
 	else if (!strcmp(phoneme,"IY"))
		return 7;
 	else if (!strcmp(phoneme,"K"))
		return 8;
 	else if (!strcmp(phoneme,"N"))
		return 9;
 	else if (!strcmp(phoneme,"OW"))
		return 10;
 	else if (!strcmp(phoneme,"R"))
		return 11;
 	else if (!strcmp(phoneme,"S"))
		return 12;
 	else if (!strcmp(phoneme,"T"))
		return 13;
 	else if (!strcmp(phoneme,"TH"))
		return 14;
 	else if (!strcmp(phoneme,"UW"))
		return 15;
 	else if (!strcmp(phoneme,"V"))
		return 16;
 	else if (!strcmp(phoneme,"W"))
		return 17;
 	else if (!strcmp(phoneme,"Z"))
		return 18; 
	
	return -1;
}

int wordToModelIndex(char* word)
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
