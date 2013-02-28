#include "cepstrum_vectors.h"


/* Number of cepstrum values used in recording. Should be 13. 
 */
#define CEPSTRUM_DIMENSION 13


/* Struct to store a feature vector. A feature vector is composed of the 
 * normalized cepstrum values, their derivatives, and their double derivatives
 */
typedef struct {
    double values[CEPSTRUM_DIMENSION];
    double deltas[CEPSTRUM_DIMENSION];
    double doubles[CEPSTRUM_DIMENSION];
} feature;


/* Struct to contain a set of feature vectors
 */
typedef struct {
    int num_vectors;
    feature* features;
} feature_vectors;


/* read_features_from_file - given a filename, imports a set of raw cepstrum
 *                           vectors, converts it to feature vectors, and 
 *                           returns the result.
 */
feature_vectors* features_from_file(char* filename);


/* raw_cepstrum_to_features - given the series of raw cepstrum vectors, constructs
 *                            the feature vectors by normalizing the cepstra and
 *                            finding its deltas, then stores the result in the
 *                            specified output. Raw cepstrum is calculated by
 *                            the cepstrum_vectors library */
void raw_cepstrum_to_features(cepstrum_vectors* raws, feature_vectors* output);


/* print_* - given a type, prints a human-readable version of that type
 */
void print_feature(feature* f);
void print_feature_vectors(feature_vectors* fs);

/* free_feature_vectors - frees the memory allocated for feature_vectors, duh
 */
void free_feature_vectors(feature_vectors* fs);
