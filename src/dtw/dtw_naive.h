#ifndef DTWNAIVE_H 
#define DTWNAIVE_H


/* NOTE: This file extends the behavior of dtw_sync.h, to perform the
 *       the time-synchrnous dtw of feature vectors using euclidian distance
 */


#include "dtw_trellis.h"
#include "feature.h"


/* get_naive_trellis - given a test vector and a set of template vectors,
 *                     creates an array of dtw_t to pass to get_best_n.
 *                     Prunes using specified method and threshold
 */
dtw_t** get_naive_trellis(feature_vectors* test, feature_vectors** templates,
                          int num_templates,
                          dtw_prune_t prune, double threshold);


/* naive_scorer - a scorer for the dtw_t that uses euclydian distance
 *                between two vectors
 */
double naive_scorer(void* test, void* template, int row, int col,
                    dtw_trellis_dir dir);


/* print_naive_results - given a list of scored trellises generated using
 *                       this file's naive method, and the number of trellises,
 *                       prints out the resulting list
 */
void print_naive_results(dtw_t** trellis, int n);


/* free_naive_results - free the memory for these trellises
 */
void free_naive_results(dtw_t** trellis, int n);


#endif
