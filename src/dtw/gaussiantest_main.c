#include <stdlib.h>
#include <stdio.h>
#include "gaussian.h"
#include "feature.h"


int main()
{
    feature_vectors* fs = malloc(sizeof(feature_vectors));
    fs->num_vectors = 3;
    fs->features = malloc(sizeof(feature)*3);

    fs->features[0].values[0] = 3;
    fs->features[1].values[0] = 5;
    fs->features[2].values[0] = 7;

    single_gaussian_params* gp = compute_single_gaussian_params(fs);

    printf("Evaluating gaussian of three points (only using first row).\n");
    printf("This is the gaussian of list [3,5,7].\n");
    printf("    This should have mean (3+5+7)/3 == 5...\n");
    printf("    and variance ((3-5)^2 + (5-5)^2 + (7-5)^2)/3 == 8/3\n");
    printf("    if my math is correct.\n\n");
    print_single_gaussian_params(gp);

    return 0;
}
