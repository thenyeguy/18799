#ifndef PHONEME_CONTINUOUS_TRAINER_H
#define PHONEME_CONTINUOUS_TRAINER_H


/* train_isolated_phoneme_models - given sets of recordings of isolated digits,
 *      trains phoneme models. Uses num_iterations of iterations of the
 *      training to seek convergence.
 */
gaussian_cluster** train_isolated_phoneme_models(recording_set** recordings,
    int num_digits, int num_iterations);


/* train_phoneme_models - given sets of recordings of isolated digits, uses
 *      continuous training to split recordings into phonemes, then recombines
 *      those phonemes into words models.
 */
gaussian_cluster** train_isolated_phoneme_step(recording_set** recordings,
    int num_digits, gaussian_cluster** initials);

#endif
