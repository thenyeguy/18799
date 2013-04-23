#include <stdlib.h>
#include <stdio.h>
#include "training_utils.h"

int main()
{
    char* one_files[5] =
    {
        "analysis/one1-40.out",
        "analysis/one2-40.out",
        "analysis/one3-40.out",
        "analysis/one4-40.out",
        "analysis/one5-40.out"
    };
    feature_vectors** one_features = features_from_all_files(one_files, 5);
    gaussian_cluster* one_init = cluster_templates(one_features, 5, "one");


    // set up continuous trainer
    printf("Training...\n");
    feature_vectors** recordings = one_features;
    int num_recordings = 5;

    gaussian_cluster*** models = calloc(5,sizeof(gaussian_cluster**));
    int num_models[5];
    for(int i = 0; i < 5; i++)
    {
        models[i] = calloc(1,sizeof(gaussian_cluster*));
        models[i][0] = one_init;
        num_models[i] = 1;
    }

    char* words[1] = {"one"};
    int num_words = 1;

    gaussian_cluster** results =
        train_from_recordings(recordings, num_recordings, models, num_models,
                              words, num_words);
    gaussian_cluster* one_new = results[0];

    // get naive result
    gaussian_cluster* one = cluster_templates(one_features,5,"one");


    // print results
    printf("Done!\n\n");

    printf("Printing difference between continuous and static trainer...\n");
    for(int j = 0; j < CEPSTRUM_DIMENSION; j++)
    {
        for(int i = 0; i < one_new->num_clusters; i++)
        {
            double temp = one_new->params[i]->means.values[j] -
                              one->params[i]->means.values[j];
            printf("%f ",temp);

        }
        printf("\n");
    }
    printf("\n\n");

    return 0;
}
