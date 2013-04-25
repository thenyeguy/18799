#include <stdlib.h>
#include <stdio.h>
#include "training_utils.h"

/* Currently hardcoded to the following six templates:
 *     0 1 2 3 4 5 6 7 8 9
 *     1 2 3 4 5 6 7 8 9 0
 *     9 8 7 6 5 4 3 2 1 0
 *     0 9 8 7 6 5 4 3 2 1
 *     1 3 5 7 9 0 2 4 6 8
 *     8 6 4 2 0 9 7 5 3 1
 *
 */

int main()
{
    // Read in initial models
    printf("\nInitializing models...\n");
    gaussian_cluster* sil_init = read_cluster_from_file("sil");
    gaussian_cluster* zero_init = read_cluster_from_file("zero");
    gaussian_cluster* one_init = read_cluster_from_file("one");
    gaussian_cluster* two_init = read_cluster_from_file("two");
    gaussian_cluster* three_init = read_cluster_from_file("three");
    gaussian_cluster* four_init = read_cluster_from_file("four");
    gaussian_cluster* five_init = read_cluster_from_file("five");
    gaussian_cluster* six_init = read_cluster_from_file("six");
    gaussian_cluster* seven_init = read_cluster_from_file("seven");
    gaussian_cluster* eight_init = read_cluster_from_file("eight");
    gaussian_cluster* nine_init = read_cluster_from_file("nine");



    // Read in our recordings and create their model arrays
    printf("Reading training templates...\n");

    feature_vectors* template1 =
        features_from_file("analysis/0123456789-40.out");
    gaussian_cluster** model1 = calloc(12, sizeof(gaussian_cluster*));
    model1[0] = sil_init;
    model1[1] = zero_init;
    model1[2] = one_init;
    model1[3] = two_init;
    model1[4] = three_init;
    model1[5] = four_init;
    model1[6] = five_init;
    model1[7] = six_init;
    model1[8] = seven_init;
    model1[9] = eight_init;
    model1[10] = nine_init;
    model1[11] = sil_init;

    feature_vectors* template2 =
        features_from_file("analysis/1234567890-40.out");
    gaussian_cluster** model2 = calloc(12, sizeof(gaussian_cluster*));
    model2[0] = sil_init;
    model2[1] = one_init;
    model2[2] = two_init;
    model2[3] = three_init;
    model2[4] = four_init;
    model2[5] = five_init;
    model2[6] = six_init;
    model2[7] = seven_init;
    model2[8] = eight_init;
    model2[9] = nine_init;
    model2[10] = zero_init;
    model2[11] = sil_init;

    feature_vectors* template3 =
        features_from_file("analysis/9876543210-40.out");
    gaussian_cluster** model3 = calloc(12, sizeof(gaussian_cluster*));
    model3[0] = sil_init;
    model3[1] = nine_init;
    model3[2] = eight_init;
    model3[3] = seven_init;
    model3[4] = six_init;
    model3[5] = five_init;
    model3[6] = four_init;
    model3[7] = three_init;
    model3[8] = two_init;
    model3[9] = one_init;
    model3[10] = zero_init;
    model3[11] = sil_init;

    feature_vectors* template4 =
        features_from_file("analysis/0987654321-40.out");
    gaussian_cluster** model4 = calloc(12, sizeof(gaussian_cluster*));
    model4[0] = sil_init;
    model4[1] = zero_init;
    model4[2] = nine_init;
    model4[3] = eight_init;
    model4[4] = seven_init;
    model4[5] = six_init;
    model4[6] = five_init;
    model4[7] = four_init;
    model4[8] = three_init;
    model4[9] = two_init;
    model4[10] = one_init;
    model4[11] = sil_init;

    feature_vectors* template5 =
        features_from_file("analysis/1357902468-40.out");
    gaussian_cluster** model5 = calloc(12, sizeof(gaussian_cluster*));
    model5[0] = sil_init;
    model5[1] = one_init;
    model5[2] = three_init;
    model5[3] = five_init;
    model5[4] = seven_init;
    model5[5] = nine_init;
    model5[6] = zero_init;
    model5[7] = two_init;
    model5[8] = four_init;
    model5[9] = six_init;
    model5[10] = eight_init;
    model5[11] = sil_init;

    feature_vectors* template6 =
        features_from_file("analysis/8642097531-40.out");
    gaussian_cluster** model6 = calloc(12, sizeof(gaussian_cluster*));
    model6[0] = sil_init;
    model6[1] = eight_init;
    model6[2] = six_init;
    model6[3] = four_init;
    model6[4] = two_init;
    model6[5] = zero_init;
    model6[6] = nine_init;
    model6[7] = seven_init;
    model6[8] = five_init;
    model6[9] = three_init;
    model6[10] = one_init;
    model6[11] = sil_init;



    // Stuff the models together
    printf("Packaging training data...\n");
    int num_recordings = 6;
    feature_vectors** recordings = calloc(6,sizeof(feature_vectors*));
    recordings[0] = template1;
    recordings[1] = template2;
    recordings[2] = template3;
    recordings[3] = template4;
    recordings[4] = template5;
    recordings[5] = template6;

    int num_models[6] = {12,12,12,12,12,12};
    gaussian_cluster*** models = calloc(6,sizeof(gaussian_cluster**));
    models[0] = model1;
    models[1] = model2;
    models[2] = model3;
    models[3] = model4;
    models[4] = model5;
    models[5] = model6;

    int num_words = 11;
    char** words = calloc(11,sizeof(char*));
    words[0] = "zero";
    words[1] = "one";
    words[2] = "two";
    words[3] = "three";
    words[4] = "four";
    words[5] = "five";
    words[6] = "six";
    words[7] = "seven";
    words[8] = "eight";
    words[9] = "nine";
    words[10] = "sil";


    
    // Actually run training
    printf("Running training and clustering algorithm...\n\n");
    gaussian_cluster** results =
        train_from_recordings(recordings, num_recordings, models, num_models,
                              words, num_words);


    // Save results
    printf("Now saving results...\n");

    for(int i = 0; i < num_words; i++)
    {
        printf("    hmms/%s-new.hmm\n", words[i]);

        char filename[128];
        sprintf(filename, "%s-new", words[i]);
        write_cluster_to_file(filename, results[i]);
    }

    printf("Done!\n\n");



    return 0;
}
