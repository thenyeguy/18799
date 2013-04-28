#ifndef PHONEME_UTILS_H
#define PHONEME_UTILS_H


#include "../dtw/feature.h"


/* Encapsulate a set of recordings for a digit
 */
typedef struct {
    char name[28];
    int num_recordings;
    feature_vectors** recordings;
} digit_recordings;


#define NUM_PHONEMES 19
char* phoneme_names[NUM_PHONEMES] = {"AX", "AH", "AY", "EH", "EY", "F", "IH",
        "IY", "K", "N", "OW", "R", "S", "T", "TH", "UW", "V", "W", "Z"};

/* get_phoneme_initial_models - given a set of recordings of digits, splits
 *     those digits into phonemes and trains an initial set of gaussian clusters
 *     for those phonemes
 */
gaussian_cluster** get_phoneme_initial_models(digit_recordings** recordings,
                                              int num_digits);


#endif
