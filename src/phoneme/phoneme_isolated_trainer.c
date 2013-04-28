#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "phoneme_isolated_trainer.h"


gaussian_cluster** train_isolated_phoneme_models(recording_set** recordings,
                                                 int num_digits)
{
    //First get initial models to seed the continuous training
    gaussian_cluster** initials = get_phoneme_initial_models(recordings, num_digits);
}
