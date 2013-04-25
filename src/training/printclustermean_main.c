#include <stdlib.h>
#include <stdio.h>
#include "../dtw/cluster.h"

/* Currently hardcoded to the following six templates:
 *     0 1 2 3 4 5 6 7 8 9
 *     1 2 3 4 5 6 7 8 9 0
 *     9 8 7 6 5 4 3 2 1 0
 *     0 9 8 7 6 5 4 3 2 1
 *     1 3 5 7 9 0 2 4 6 8
 *     8 6 4 2 0 9 7 5 3 1
 *
 */

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        printf("\nUsage: printclustermean [hmm name]\n");
        printf("       Prints the mean vectors for an hmm.\n\n");
        printf("       Each column is one state.\n");
        printf("       This format was chosen to make it easy to cop paste");
        printf(" into MATLAB. :P\n\n");
        exit(1);
    }

    // Read model
    gaussian_cluster* model = read_cluster_from_file(argv[1]);

    // Print
    printf("\nPrinting out mean vectors for HMM: %s\n", argv[1]);
    printf("Each column vector is one state.\n\n");
    for(int i = 0; i < CEPSTRUM_DIMENSION; i++)
    {
        for(int j = 0; j < model->num_clusters; j++)
            printf("%f ", model->params[j]->means.values[i]);
        printf("\n");
    }
    printf("\n");


    return 0;
}
