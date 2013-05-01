#ifndef PHONEME_CONTINUOUS_TRAINER_H
#define PHONEME_CONTINUOUS_TRAINER_H

#include "../dtw/feature.h"
#include "../dtw/gaussian.h"

/* train_continuous_phoneme_models - given sets of recordings of digits,
 *      and list of those digits in each recordings, retrains new
 *      phoneme models.
 *
 * Uses initials as the starting phoneme models. Needs a sil_init as well
 *      since we have no sil phoneme
 */
gaussian_cluster** train_continuous_phoneme_models(feature_vectors** recordings,
    char*** word_names, int* num_words, int num_recordings,
    gaussian_cluster** initials, gaussian_cluster* sil_init);


/* read_phoneme_set - given a setname, reads that set of phoneme models from
 *      their files into gaussian clusters.
 *      Setname can be init or new.
 */
gaussian_cluster** read_phoneme_set(char* setname);

#endif
