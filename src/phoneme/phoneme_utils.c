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
			feature_vectors_per_phoneme[phoneme_index]++;
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
			feature_vectors** split = 
				split_feature_vectors(recordings[i]->recordings[j], num_phonemes);

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

		phoneme_clusters[i] = cluster_templates(feature_vectors_for_phonemes[i],
					feature_vectors_per_phoneme[i],3,phoneme_names[i]);
		
	}
	
	return phoneme_clusters;
}

feature_vectors** split_feature_vectors(feature_vectors* input_vectors, 
										int num_segments) {
	/// determine the number of segments per split vector 
	int segment_length = (input_vectors -> num_vectors) / num_segments;
	int remaining_vectors = input_vectors->num_vectors;

	/// identify the phoneme breakdown of the input word
	char* word = input_vectors->word_id;
	char** phonemes = word_phonemes[wordToModelIndex(word)];

	/// allocate memory for the new feature_vectors list
	feature_vectors** segment_list = 
					malloc(num_segments*sizeof(feature_vectors*));

	/// fill in all but the last feature vector (to make sure we account for all 
	/// vectors in the original feature_vectors)
	for (int i = 0; i < num_segments-1; i++) {
		segment_list[i] = malloc(sizeof(feature_vectors));
		segment_list[i]->num_vectors = segment_length;
		strcpy(segment_list[i]->word_id, phonemes[i]);
		segment_list[i]->features = malloc(segment_length*sizeof(feature));
		for (int j = 0; j < segment_length; j++) {
			/// copy features from input_vectors to new segment
			memcpy(&(segment_list[i]->features[j]),
					&(input_vectors->features[i*segment_length+j]),
					sizeof(feature));
		}
		remaining_vectors -= segment_length;
	}

	/// put the remaining vectors in the last segment.
	segment_list[num_segments-1] = malloc(sizeof(feature_vectors));
	segment_list[num_segments-1]->num_vectors = remaining_vectors;
	strcpy(segment_list[num_segments-1]->word_id, phonemes[num_segments-1]);
	for (int j = 0; j < remaining_vectors; j++) {
		/// copy features from input_vectors to new segment
		memcpy(&(segment_list[num_segments-1]->features[j]), 
				&(input_vectors->features[(num_segments-1)*segment_length]),
				sizeof(feature));
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
