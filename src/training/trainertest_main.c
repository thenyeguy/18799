#include <stdlib.h>
#include <stdio.h>
#include "training_utils.h"

int main()
{
    char* sil_files[5] =
    {
        "analysis/silence1-40.out",
        "analysis/silence2-40.out",
        "analysis/silence3-40.out",
        "analysis/silence4-40.out",
        "analysis/silence5-40.out"
    };
    feature_vectors** sil_features = features_from_all_files(sil_files, 5);
    gaussian_cluster* sil_init = cluster_templates(sil_features, 5, "...");

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

    char* two_files[5] =
    {
        "analysis/two1-40.out",
        "analysis/two2-40.out",
        "analysis/two3-40.out",
        "analysis/two4-40.out",
        "analysis/two5-40.out"
    };
    feature_vectors** two_features = features_from_all_files(two_files, 5);
    gaussian_cluster* two_init = cluster_templates(two_features, 5, "two");


    // set up continuous trainer
    printf("Training...\n");

    feature_vectors** recordings = calloc(1,sizeof(feature_vectors*));
    recordings[0] = features_from_file("analysis/onetwo-40.out");
    int num_recordings = 1;

    gaussian_cluster*** models = calloc(1,sizeof(gaussian_cluster**));
    models[0] = calloc(5,sizeof(gaussian_cluster*));
    models[0][0] = sil_init;
    models[0][1] = one_init;
    models[0][2] = sil_init;
    models[0][3] = two_init;
    models[0][4] = sil_init;

    int num_models[1] = {5};

    char** words = calloc(3,sizeof(char*));
    words[0] = "...";
    words[1] = "one";
    words[2] = "two";
    int num_words = 3;

    gaussian_cluster** results =
        train_from_recordings(recordings, num_recordings, models, num_models,
                              words, num_words);


    // print results
    printf("Done!\n\n");
    gaussian_cluster* sil_new = results[0];
    gaussian_cluster* one_new = results[1];
    gaussian_cluster* two_new = results[2];
    (void) sil_new; (void) one_new; (void) two_new;

    printf("New gaussian model for two:\n");
    for(int j = 0; j < CEPSTRUM_DIMENSION; j++)
    {
        for(int i = 0; i < two_new->num_clusters; i++)
        {
            double temp = two_new->params[i]->means.values[j];
            printf("%f ",temp);

        }
        printf("\n");
    }
    printf("\n\n");

    printf("Old gaussian model for two:\n");
    for(int j = 0; j < CEPSTRUM_DIMENSION; j++)
    {
        for(int i = 0; i < two_new->num_clusters; i++)
        {
            double temp = two_init->params[i]->means.values[j];
            printf("%f ",temp);

        }
        printf("\n");
    }
    printf("\n\n");

    return 0;
}
