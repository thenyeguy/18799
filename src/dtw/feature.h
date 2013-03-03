#ifndef FEATURE_H

#define FEATURE_H

#include "../cepstrum/cepstrum_vectors.h"


/* Number of cepstrum values used in recording. Should be 13. 
 */
#define CEPSTRUM_DIMENSION 13
#define NULL_PREFIX NULL

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


/* alloc_feature_vectors - allocates space for a feature */


#define NUM_CLUSTERS 3
/* cluster_templates - 	Runs Kmeans segmentation clustering on 
 *			input feature_vectors */
void cluster_templates(feature_vectors ** templates,int num_templates);


#define FIRST_TEMPLATE_INDEX 3
/* features_from_all_files -	Builds feature vectors for each template file passed
 * 				in on the command line by calling 
 * 				read_features_from_file ARGC number of times*/
feature_vectors** features_from_all_files(int argc, char** argv);


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

#endif