#ifndef DTWUNITY_H 
#define DTWUNITY_H


/* NOTE: This file extends the behavior of dtw_sync.h, to perform the
 *       the time-synchrnous dtw of feature vectors using clustering.
 *       It simply returns the last incoming score it received
 */


#include "dtw_trellis.h"


/* get_unity_trellis - sets up a trellis that doesn't score anything
 */
dtw_t* get_unity_trellis(int length, dtw_prune_t prune, double threshold);


/* unity - a scorer for the dtw_t that doesn't do anything
 */
double unity_scorer(void* test_p, void* template_p, int row, int col,
                    dtw_trellis_dir dir);


#endif
