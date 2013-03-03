#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "dtw_trellis.h"
#include "feature.h"

// Euclydian distance scorer
double score(void* test, void* template, int row, int col, dtw_trellis_dir dir);


int main(int argc, char **argv)
{
    if(argc < 4)
    {
        printf("Usage: bin/naivedtw [threshold] [path/to/test.out] ");
        printf("[paths/to/templates.out]\n");
        printf("    Takes in the pruning threshold (0 for no pruning),\n");
        printf("    the test output file generated by bin/cepstrum\n");
        printf("    and one or more template output files.\n\n");
        printf("    Uses a naive DTW to compare against each template and ");
        printf("return the score of the best results.\n\n");
        exit(1);
    }

    //Currently only handles a single template, sorry
    if(argc > 4){
        printf("IGNORING EXTRA TEMPLATES... fix me :(\n\n");
	int num_templates = argc - FIRST_TEMPLATE_INDEX;
	feature_vectors ** templates = features_from_all_files(argc,argv);
	cluster_templates(templates,num_templates);
    }
    feature_vectors* test     = features_from_file(argv[2]);
    feature_vectors* template = features_from_file(argv[3]);

    double threshold = strtod(argv[1],NULL);
    dtw_prune_t prune;
    if(threshold == 0.0)
        prune = DTW_NO_PRUNE;
    else
        prune = DTW_BEAM_PRUNE;

    printf("Comparing %s against %s with threshold %1.4lf\n\n",argv[2],argv[3],threshold);

    //Create DTW structure
    dtw_t* dtw = new_dtw(test->features, test->num_vectors,
                         template->features, template->num_vectors,
                         prune, threshold, score);

    //Run until it completes
    while(!dtw_fill_next_col(dtw));
    //Print results
    //print_feature_vectors(test);
    //print_feature_vectors(template);
    dtw_print_struct(dtw);
    return 0;
}


double score(void* test, void* template, int row, int col, dtw_trellis_dir dir)
{
    feature* testF = (feature*) test;
    feature* tempF = (feature*) template;
    //At the beginning force us to start at the bottom left
    if(dir == DTW_DIR_NONE)
    {
        if(row == 0)
            return 0.0;
        else
            return log(0.0);
    }

    double score = 0;
    for(int i = 0; i < CEPSTRUM_DIMENSION; i++)
    {
        score += pow(testF[col].values[i] - tempF[row].values[i], 2);
        score += pow(testF[col].deltas[i] - tempF[row].deltas[i], 2);
        score += pow(testF[col].doubles[i] - tempF[row].doubles[i], 2);
    }

    //printf("Return: %1.4lf %1.4lf\n", score, -0.5*log(score));
    return -1.0*score;
}