#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "grammar.h"
#include "../dtw/dtw_gaussian.h"

int main()
{
    grammar* g = build_grammar("grammar/simple.txt");

    gaussian_cluster* sil = g->hmms[10];
    gaussian_cluster* one = g->hmms[1];
    gaussian_cluster* two = g->hmms[2];
    (void) sil; (void) one; (void) two;

    //Print out params in matrix
    /*
    for(int j = 0; j < CEPSTRUM_DIMENSION; j++)
    {
        for(int i = 0; i < sil->num_clusters; i++)
            printf("%f ",sil->params[i]->means.values[j]);
        printf("\n");
    }
    printf("\n\n");
    */
    for(int j = 0; j < CEPSTRUM_DIMENSION; j++)
    {
        for(int i = 0; i < one->num_clusters; i++)
            printf("%f ",one->params[i]->means.values[j]);
        printf("\n");
    }
    printf("\n\n");
    /*
    for(int j = 0; j < CEPSTRUM_DIMENSION; j++)
    {
        for(int i = 0; i < two->num_clusters; i++)
            printf("%f ",two->params[i]->means.values[j]);
        printf("\n");
    }
    */

    /*
    //Append models
    gaussian_cluster* cat = malloc(sizeof(gaussian_cluster));
    cat->params = calloc(25,sizeof(single_gaussian_params*));
    cat->stationary_probs = calloc(25,sizeof(double));
    cat->transition_probs = calloc(25,sizeof(double));
    cat->num_clusters = 25;

    gaussian_cluster* wordone = g->hmms[8];
    gaussian_cluster* wordtwo = g->hmms[4];
    for(int i = 0; i < 5; i++)
    {
        cat->params[i] = sil->params[i];
        cat->params[i+5] = wordone->params[i];
        cat->params[i+10] = sil->params[i];
        cat->params[i+15] = wordtwo->params[i];
        cat->params[i+20] = sil->params[i];

        cat->stationary_probs[i] = sil->stationary_probs[i];
        cat->stationary_probs[i+5] = wordone->stationary_probs[i];
        cat->stationary_probs[i+10] = sil->stationary_probs[i];
        cat->stationary_probs[i+15] = wordtwo->stationary_probs[i];
        cat->stationary_probs[i+20] = sil->stationary_probs[i];

        cat->transition_probs[i] = sil->transition_probs[i];
        cat->transition_probs[i+5] = wordone->transition_probs[i];
        cat->transition_probs[i+10] = sil->transition_probs[i];
        cat->transition_probs[i+15] = wordtwo->transition_probs[i];
        cat->transition_probs[i+20] = sil->transition_probs[i];
    }

    //Read in test
    feature_vectors* test = features_from_file("analysis/eightfour-40.out");

    //Score
    dtw_t* trellis = *get_gaussian_trellis(test, &cat, 1, false, .1);
    while(!dtw_fill_next_col(trellis));
    dtw_print_struct(trellis);
    */

    return 0;
}
