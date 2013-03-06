#ifndef DTWSYNC_H 
#define DTWSYNC_H

/* NOTE: This file is not sufficient to perform time-synchronous dtw. It will
 *       handle any type of trellis if given those structures already, but other
 *       code is required to initialize the trellis from different kinds of
 *       templates, and to print out results. Files that complete this library
 *       include:
 *           naivedtw.h
 *           gaussiandtw.h
 */


#include "dtw_trellis.h"
#include "feature.h"


/* get_best_n_from_trellis - given an array of trellises already initialized,
 *                           performs the time-synchronous dtw, and stores the
 *                           n highest scoring results
 */
dtw_t** get_best_n_from_trellis(dtw_t** trellis, int trellis_length, int n);


/* insert - helper function for get_best_n... inserts the input into list in 
 *          order if it is greater than one of length entries already in list.
 *
 *          Length refers to the maximum length of the list, not necessarily
 *          the current number of non-null entries in list.
 */
bool insert_into_n_best(dtw_t* input, dtw_t** list, int length);


/* free_n_best - frees the memory allocated for n_best
 */
void free_n_best(dtw_t** dtws, int n);


#endif
