#ifndef TRAINING_UTILS_H
#define TRAINING_UTILS_H


#include "../dtw/feature.h"
#include "../dtw/cluster.h"


/* train_from_recordings -
 *      Takes in an array of feature_vectors* representing recordings, and the
 *      size of said array.

 *      Also takes in an array of gaussiancluster*** representing an array of
 *      model arrays for each recording, in time order, and the number of
 *      models in each recording.
 *
 *      Returns an array of clusters for each word in our corpus, in the order
 *      they are named in words_in_corpus.
 *
 *      The word names in this array MUST
 *      be identical to the names contained in the initial gaussian clusters.
 */
gaussian_cluster** train_from_recordings(feature_vectors** recordings,
        int num_recordings, gaussian_cluster*** models, int* num_models,
        char** words_in_corpus, int num_words);

/* extract_words_from_recording -
 *      Takes in a recording of training data, and an array of num_models
 *      initial models for the words spoken in the recording, in order.
 *
 *      Returns an array of num_models feature_vectors that are the segmented
 *      portion of the recording containing just one word. These can be used
 *      as templates for further training.
 */
feature_vectors** extract_words_from_recording(feature_vectors* recording,
        gaussian_cluster** models, int num_models);


#endif
