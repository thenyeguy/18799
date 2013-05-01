#ifndef PHONEME_UTILS_H
#define PHONEME_UTILS_H

#include "../dtw/feature.h"
#include "../dtw/cluster.h"


/* Encapsulate a set of recordings for a single digit or phoneme
 */
typedef struct {
    char name[28];
    int num_recordings;
    feature_vectors** recordings;
} recording_set;


/* Useful constants for mapping between words and phonemes.
 *     states_per_phoneme is how many hmm states each phoneme model should have
 *     phoneme_names is a list of the names for each phoneme.
 *     phonemes_in_word is a list of how many phonemes each word contains.
 *     word_phonemes is a list of a list of phoneme names for each word.
 */
#define STATES_PER_PHONEME 3
#define NUM_PHONEMES 19
#define NUM_WORDS 11
char* phoneme_names[NUM_PHONEMES];
char* word_names[NUM_WORDS];
int phonemes_in_word[NUM_WORDS];
char** word_phonemes[NUM_WORDS];



/* get_phoneme_initial_models - given a set of recordings of digits, splits
 *     those digits into phonemes and trains an initial set of gaussian clusters
 *     for those phonemes
 */
gaussian_cluster** get_phoneme_initial_models(recording_set** recordings,
                                              int num_digits);


/* phonemes_to_word_models - given a set of trained phoneme models, pastes the
 *      models together into models for each word
 */
gaussian_cluster** phonemes_to_word_models(gaussian_cluster** phoneme_models);


/* split_feature_vectors - splits a feature_vectors into num_segments segments
	of equal length.
*/
feature_vectors** split_feature_vectors(feature_vectors* input_vectors, int num_segments,char **new_names);

/* Used to convert a string like "AH" into the index of that phoneme
*/
int phonemeToModelIndex(char* phoneme);

/* Used to convert a string like "one" to the index to get phonemes
 */
int wordToModelIndex(char* word);

#endif
