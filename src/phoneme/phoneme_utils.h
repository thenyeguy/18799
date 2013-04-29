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
 *     phoneme_names is a list of the names for each phoneme.
 *     phonemes_in_word is a list of how many phonemes each word contains.
 *     word_phonemes is a list of a list of phoneme names for each word.
 */
#define NUM_PHONEMES 19
#define NUM_WORDS 10 //FIXME? is this used already
char* phoneme_names[NUM_PHONEMES];
int phonemes_in_word[NUM_WORDS];
char** word_phonemes[NUM_WORDS];



/* get_phoneme_initial_models - given a set of recordings of digits, splits
 *     those digits into phonemes and trains an initial set of gaussian clusters
 *     for those phonemes
 */
gaussian_cluster** get_phoneme_initial_models(recording_set** recordings,
                                              int num_digits);


/* split_feature_vectors - splits a feature_vectors into num_segments segments
	of equal length.
*/
feature_vectors** split_feature_vectors(feature_vectors* input_vectors, int num_segments);

/* Used to convert a string like "AH" into the index of that phoneme
*/
int phonemeToModelIndex(char* phoneme);

/* Used to convert a string like "one" to the index to get phonemes
 */
int wordToModelIndex(char* word);

#endif
