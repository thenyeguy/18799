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


/* Useful constants for phoneme stuff
 */
#define NUM_PHONEMES 19
#define NUM_WORDS 10 //FIXME? is this used already
char* phoneme_names[NUM_PHONEMES] = {"AX", "AH", "AY", "EH", "EY", "F", "IH",
        "IY", "K", "N", "OW", "R", "S", "T", "TH", "UW", "V", "W", "Z"};

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

int phonemes_in_word[NUM_WORDS] = {4,3,2,3,3,3,4,5,2,3};
char** word_phonemes[NUM_WORDS] = {zero,one,two,three,four,five,six,seven,eight,nine};


/* get_phoneme_initial_models - given a set of recordings of digits, splits
 *     those digits into phonemes and trains an initial set of gaussian clusters
 *     for those phonemes
 */
gaussian_cluster** get_phoneme_initial_models(recording_set** recordings,
                                              int num_digits);


/* Used to convert a string like "AH" into the index of that phoneme
*/
int phonemeToModelIndex(char* phoneme);

/* Used to convert a string like "one" to the index to get phonemes
 */
int wordToModelIndex(char* word);

#endif
