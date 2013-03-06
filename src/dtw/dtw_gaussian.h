#ifndef DTWGAUSSIAN_H 
#define DTWGAUSSIAN_H


/* NOTE: This file extends the behavior of dtw_sync.h, to perform the
 *       the time-synchrnous dtw of feature vectors using clustering
 */


#include "dtw_trellis.h"
#include "feature.h"
#include "gaussian.h"
#include "cluster.h"


/* get_gaussian_trellis - given a test vector and a set of template gaussians,
 *                     creates an array of dtw_t to pass to get_best_n.
 *                     Prunes using specified method and threshold.
 */
dtw_t** get_gaussian_trellis(feature_vectors* test,
                             gaussian_cluster** templates,
                             int num_templates,
                             dtw_prune_t prune, double threshold);


/* gaussian_scorer - a scorer for the dtw_t that uses gaussian log likelihood
 */
double gaussian_scorer(void* test_p, void* template_p, int row, int col,
                       dtw_trellis_dir dir);


/* print_gaussian_results - given a list of scored trellises generated using
 *                       this file's gaussian method, and the number of trellises,
 *                       prints out the resulting list
 */
void print_gaussian_results(dtw_t** trellis, int n);


/* free_gaussian_results - free the memory for these trellises
 */
void free_gaussian_results(dtw_t** trellis, int n);


#endif
