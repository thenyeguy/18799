#ifndef PHONEME_UTILS_H
#define PHONEME_UTILS_H


#include "../feature.h"

#define NUM_PHONEMES
char phoneme_names[NUM_PHONEMES] = {"AX", "AH", "AY", "EH", "EY", "F", "IH",
        "IY", "K", "N", "OW", "R", "S", "T", "TH", "UW", "V", "W", "Z"};

/* get_phoneme_initial_models - given a set of recordings of digits, splits
 *     those digits into phonemes and trains an initial set of gaussian clusters
 *     for those phonemes
 */
gaussian_cluster** get_phoneme_initial_models(feature_vectors** recordings,
                                              int num_recordings);


#endif
