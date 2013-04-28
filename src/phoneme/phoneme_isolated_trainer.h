#ifndef PHONEME_ISOLATED_TRAINER_H
#define PHONEME_ISOLATED_TRAINER_H

#include "phoneme_utils.h"


/* train_phoneme_models - given sets of recordings of isolated digits, uses
 *      continuous training to split recordings into phonemes, then recombines
 *      those phonemes into words models.
 */
gaussian_cluster** train_isolated_phoneme_models(recording_set** recordings,
                                                 int num_digits);

#endif
