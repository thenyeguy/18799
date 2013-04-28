#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "feature.h"
#include "dtw_sync.h"
#include "dtw_naive.h"


int main(int argc, char **argv)
{
    if(argc < 4)
    {
        printf("Usage: bin/naivedtw [threshold] [path/to/test.out] ");
        printf("[paths/to/templates.out]\n");
        printf("    Takes in the pruning threshold (0 for no pruning),\n");
        printf("    the test output file generated by bin/cepstrum\n");
        printf("    and one or more template output files.\n\n");
        printf("    Uses a naive DTW to compare against each template.\n\n");
        exit(1);
    }

    // Read in test file
    feature_vectors* test = features_from_file(argv[2]);

    // Read in arbitrary number of templates
    int num_templates = argc - 3;
    feature_vectors** templates =
        features_from_all_files(&argv[3], num_templates);
    
    double threshold = strtod(argv[1],NULL);
    dtw_prune_t prune;
    if(threshold == 0.0)
        prune = DTW_NO_PRUNE;
    else
        prune = DTW_BEAM_PRUNE;

    printf("Comparing %s against %d templates with threshold %1.4lf\n\n",
           argv[2],num_templates,threshold);

    //Create DTW structure
    dtw_t** dtws = get_naive_trellis(test, templates, num_templates,
                                     prune, threshold);

    //Get n best
    dtw_t** results = get_best_n_from_trellis(dtws, num_templates, 5, threshold);

    //Print results
    print_naive_results(results, 5);

    return 0;
}
