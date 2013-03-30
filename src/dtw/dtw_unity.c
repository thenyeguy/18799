#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "dtw_unity.h"


dtw_t* get_unity_trellis(int length, dtw_prune_t prune, double threshold)
{
    dtw_t* dtw = malloc(sizeof(dtw_t));

    dtw = new_dtw(NULL, length, NULL, 0,
                  prune, threshold, unity_scorer);

    return dtw;
}


double unity_scorer(void* test_p, void* template_p, int row, int col,
             dtw_trellis_dir dir)
{
    return DTW_MIN_SCORE;
}
