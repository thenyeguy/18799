#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <math.h>
#include "dtw_sync.h"
#include "feature.h"


dtw_t** get_best_n_from_trellis(dtw_t** trellis, int trellis_length, int n,
                                double threshold)
{
    bool done = false;
    while (!done)
    {
        double column_max = DTW_MIN_SCORE;
        done = true;

        //Score
        for (int i = 0; i<trellis_length; i++)
        {
            if(dtw_fill_next_col(trellis[i]) != true)
                done = false;
            double this_max = trellis[i]->column_max;
            if(this_max > column_max)
                column_max = this_max;
        }

        //Reprune
        double window = abs(column_max) * threshold;
        double abs_threshold = column_max - window;
        for (int i = 0; i<trellis_length; i++)
        {
            dtw_prune_next_column(trellis[i], abs_threshold);
        }
    }

    // Allocate space for score list and insert into list
    dtw_t** scores = (dtw_t**) calloc(sizeof(void*), n);

    for (int i = 0; i<trellis_length; i++) {
        insert_into_n_best(trellis[i], scores, n);
    }

    return scores;
}


bool insert_into_n_best(dtw_t* input, dtw_t** list, int length)
{
    int i = 0;
    while (i<length)
    {
        /// if we've gotten to empty space, add the trellis by default
        if (!list[i]) {
            list[i] = input;
            return true;
        }

        if (input->score > list[i]->score)
        {
            dtw_t* temp1 = input;
            dtw_t* temp2 = list[i];
            for (int j = i; j<length; j++)
            {
                list[j] = temp1;
                temp1 = temp2;
                if (j+1 < length) temp2 = list[j+1];
            }
            return true;
        }
        i++;
    }
    return false;
}


void free_n_best(dtw_t** dtws, int n)
{
    free(dtws);
}
