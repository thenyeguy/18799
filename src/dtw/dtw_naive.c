#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "dtw_naive.h"


dtw_t** get_naive_trellis(feature_vectors* test, feature_vectors** templates,
                          int num_templates,
                          dtw_prune_t prune, double threshold)
{
    dtw_t** dtws = malloc(num_templates * sizeof(dtw_t*));

    for(int i = 0; i < num_templates; i++)
    {
        dtws[i] = new_dtw(test, test->num_vectors,
                          templates[i], templates[i]->num_vectors,
                          prune, threshold, naive_scorer);
    }

    return dtws;
}


double naive_scorer(void* test, void* template, int row, int col,
                    dtw_trellis_dir dir)
{
    feature_vectors* testF = (feature_vectors*) test;
    feature_vectors* tempF = (feature_vectors*) template;
    //At the beginning force us to start at the bottom left
    if(dir == DTW_DIR_NONE)
    {
        if(row == 0)
            return 0.0;
        else
            return log(0.0);
    }

    double score =
        feature_distance(&testF->features[col], &tempF->features[row]);

    //printf("Return: %1.4lf %1.4lf\n", score, -0.5*log(score));
    return -1.0*score;
}


void print_naive_results(dtw_t** trellis, int n)
{
    for(int i = 0; i < n && trellis[i] != NULL; i++)
    {
        feature_vectors* template = trellis[i]->template_data;
        printf("WORD MATCH: %s\n", template->word_id);
        dtw_print_struct(trellis[i]);
    }
}


void free_naive_trellis(dtw_t** trellis, int n)
{
    for(int i = 0; i < n; i++)
    {
        free_dtw(trellis[i]);
    }
    free(trellis);
}
