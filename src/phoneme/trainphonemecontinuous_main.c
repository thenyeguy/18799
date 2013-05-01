#include <stdlib.h>
#include <stdio.h>
#include "../training/training_utils.h"
#include "phoneme_utils.h"
#include "phoneme_continuous_trainer.h"

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
    if(argc < 2)
    {
        printf("USAGE: bin/trainphonemecontinous [initial models]\n");
        printf("       Initial model options:\n");
        printf("          init - naive isolated phoneme models\n");
        printf("          new  - our voices, continuously trained\n");
        printf("\n");
        exit(1);
    }

    // Read in initial models
    printf("\nInitializing models...\n");
    char* set = argv[1];

    // Initial phonemes
    gaussian_cluster** initials = read_phoneme_set(set);
    
    // Initial silence
    char silname[52]; sprintf(silname, "sil-%s", set);
    gaussian_cluster* sil_init = read_cluster_from_file(silname);



    // Read in our recordings and create their model arrays
    printf("Reading training templates...\n");

    feature_vectors* template1 =
        features_from_file("analysis/0123456789-40.out");
    char* words1[12] = {"sil","zero","one","two","three","four","five","six",
        "seven","eight","nine","sil"};

    feature_vectors* template2 =
        features_from_file("analysis/1234567890-40.out");
    char* words2[12] = {"sil","one","two","three","four","five","six",
        "seven","eight","nine","zero","sil"};

    feature_vectors* template3 =
        features_from_file("analysis/9876543210-40.out");
    char* words3[12] = {"sil","nine","eight","seven","six","five","four","three",
        "two","one","zero","sil"};

    feature_vectors* template4 =
        features_from_file("analysis/0987654321-40.out");
    char* words4[12] = {"sil","zero","nine","eight","seven","six","five","four",
        "three","two","one","sil"};

    feature_vectors* template5 =
        features_from_file("analysis/1357902468-40.out");
    char* words5[12] = {"sil","one","three","five","seven","nine","zero","two",
        "four","six","eight","sil"};
        
    feature_vectors* template6 =
        features_from_file("analysis/8642097531-40.out");
    char* words6[12] = {"sil","eight","six","four","two","zero","nine","seven",
        "five","three","one","sil"};


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

    int num_words[6] = {12,12,12,12,12,12};
    char** words[6] = {words1,words2,words3,words4,words5,words6};


    // Actually run trainer
    printf("\nTraining...\n");
    gaussian_cluster** phoneme_models =
        train_continuous_phoneme_models(recordings, words, num_words,
            num_recordings, initials, sil_init);
    gaussian_cluster** word_models = phonemes_to_word_models(phoneme_models);


    // Save results
    printf("Now saving results...\n");

    for(int i = 0; i < NUM_PHONEMES; i++)
    {
        printf("    hmms/phonemes/%s-new.hmm\n", phoneme_names[i]);

        char filename[128];
        sprintf(filename, "phonemes/%s-new", phoneme_names[i]);
        write_cluster_to_file(filename, phoneme_models[i]);
    }

    for(int i = 0; i < NUM_WORDS; i++)
    {
        printf("    hmms/%s-phoneme-new.hmm\n", word_names[i]);

        char filename[128];
        sprintf(filename, "%s-phoneme-new", word_names[i]);
        write_cluster_to_file(filename, word_models[i]);
    }

    printf("Done!\n\n");


    return 0;
}
