#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "cluster.h"
#include "dtw_gaussian.h"


dtw_t** get_gaussian_trellis(feature_vectors* test,
                             gaussian_cluster** templates,
                             int num_templates,
                             dtw_prune_t prune, double threshold)
{
    dtw_t** dtws = malloc(num_templates * sizeof(dtw_t*));

    for(int i = 0; i < num_templates; i++)
    {
        dtws[i] = new_dtw(test, test->num_vectors,
                          templates[i], templates[0]->num_clusters,
                          prune, threshold, gaussian_scorer);
    }

    return dtws;
}


double gaussian_scorer(void* test_p, void* template_p, int row, int col,
                       dtw_trellis_dir dir)
{
    //Handle the case for time t=0
    if(dir == DTW_DIR_NONE)
    {
        if(row == 0)
            return 0.0;
        else
            return log(0.0);
    }

    //Get specified elements to compare
    feature_vectors* tests = (feature_vectors*) test_p;
    gaussian_cluster* templates = (gaussian_cluster*) template_p;

    feature* test = &tests->features[col];
    single_gaussian_params* ps = templates->params[row];

    double score = single_gaussian_log_pdf(ps, test);

    //Get transition cost
    if(dir == DTW_DIR_LEFT)
        score += log(templates->stationary_probs[row]);
    else if(dir == DTW_DIR_DOWNONE)
        score += log(templates->transition_probs[row]) + TRANSITION_PENALTY;
    else
        score += log(0.0);

    return score;
}


void print_gaussian_results(dtw_t** trellis, int n)
{
    for(int i = 0; i < n && trellis[i] != NULL; i++)
    {
        gaussian_cluster* template = trellis[i]->template_data;
        printf("WORD MATCH: %s\n", template->word_id);
        dtw_print_struct(trellis[i]);
    }
}


void free_gaussian_results(dtw_t** trellis, int n)
{
    for(int i = 0; i < n; i++)
    {
        free_dtw(trellis[i]);
    }
    free(trellis);
}

