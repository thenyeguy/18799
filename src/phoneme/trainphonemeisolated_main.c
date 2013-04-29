#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "phoneme_utils.h"
#include "phoneme_isolated_trainer.h"


#define RECORDINGS_PER_WORD 5

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        printf("USAGE: bin/trainphonemeisolated [recursion depth]\n");
        printf("       Trains our isolated phoneme models.\n");
        printf("       Runs recursion depth iterations of training\n");
        printf("\n");
        exit(1);
    }

    // Read in initial models
    printf("\nInitializing recordings...\n");
    recording_set** recordings = calloc(NUM_WORDS, sizeof(recording_set*));

    char* zero_files[RECORDINGS_PER_WORD] = {"analysis/zero1-40.out",
        "analysis/zero2-40.out", "analysis/zero3-40.out", "analysis/zero4-40.out",
        "analysis/zero5-40.out"};
    recording_set* zero = malloc(sizeof(recording_set));
    strcpy(zero->name, "zero");
    zero->num_recordings = RECORDINGS_PER_WORD;
    zero->recordings = features_from_all_files(zero_files,RECORDINGS_PER_WORD);

    char* one_files[RECORDINGS_PER_WORD] = {"analysis/one1-40.out",
        "analysis/one2-40.out", "analysis/one3-40.out", "analysis/one4-40.out",
        "analysis/one5-40.out"};
    recording_set* one = malloc(sizeof(recording_set));
    strcpy(one->name, "one");
    one->num_recordings = RECORDINGS_PER_WORD;
    one->recordings = features_from_all_files(one_files,RECORDINGS_PER_WORD);

    char* two_files[RECORDINGS_PER_WORD] = {"analysis/two1-40.out",
        "analysis/two2-40.out", "analysis/two3-40.out", "analysis/two4-40.out",
        "analysis/two5-40.out"};
    recording_set* two = malloc(sizeof(recording_set));
    strcpy(two->name, "two");
    two->num_recordings = RECORDINGS_PER_WORD;
    two->recordings = features_from_all_files(two_files,RECORDINGS_PER_WORD);

    char* three_files[RECORDINGS_PER_WORD] = {"analysis/three1-40.out",
        "analysis/three2-40.out", "analysis/three3-40.out", "analysis/three4-40.out",
        "analysis/three5-40.out"};
    recording_set* three = malloc(sizeof(recording_set));
    strcpy(three->name, "three");
    three->num_recordings = RECORDINGS_PER_WORD;
    three->recordings = features_from_all_files(three_files,RECORDINGS_PER_WORD);

    char* four_files[RECORDINGS_PER_WORD] = {"analysis/four1-40.out",
        "analysis/four2-40.out", "analysis/four3-40.out", "analysis/four4-40.out",
        "analysis/four5-40.out"};
    recording_set* four = malloc(sizeof(recording_set));
    strcpy(four->name, "four");
    four->num_recordings = RECORDINGS_PER_WORD;
    four->recordings = features_from_all_files(four_files,RECORDINGS_PER_WORD);
    
    char* five_files[RECORDINGS_PER_WORD] = {"analysis/five1-40.out",
        "analysis/five2-40.out", "analysis/five3-40.out", "analysis/five4-40.out",
        "analysis/five5-40.out"};
    recording_set* five = malloc(sizeof(recording_set));
    strcpy(five->name, "five");
    five->num_recordings = RECORDINGS_PER_WORD;
    five->recordings = features_from_all_files(five_files,RECORDINGS_PER_WORD);

    char* six_files[RECORDINGS_PER_WORD] = {"analysis/six1-40.out",
        "analysis/six2-40.out", "analysis/six3-40.out", "analysis/six4-40.out",
        "analysis/six5-40.out"};
    recording_set* six = malloc(sizeof(recording_set));
    strcpy(six->name, "six");
    six->num_recordings = RECORDINGS_PER_WORD;
    six->recordings = features_from_all_files(six_files,RECORDINGS_PER_WORD);

    char* seven_files[RECORDINGS_PER_WORD] = {"analysis/seven1-40.out",
        "analysis/seven2-40.out", "analysis/seven3-40.out", "analysis/seven4-40.out",
        "analysis/seven5-40.out"};
    recording_set* seven = malloc(sizeof(recording_set));
    strcpy(seven->name, "seven");
    seven->num_recordings = RECORDINGS_PER_WORD;
    seven->recordings = features_from_all_files(seven_files,RECORDINGS_PER_WORD);

    char* eight_files[RECORDINGS_PER_WORD] = {"analysis/eight1-40.out",
        "analysis/eight2-40.out", "analysis/eight3-40.out", "analysis/eight4-40.out",
        "analysis/eight5-40.out"};
    recording_set* eight = malloc(sizeof(recording_set));
    strcpy(eight->name, "eight");
    eight->num_recordings = RECORDINGS_PER_WORD;
    eight->recordings = features_from_all_files(eight_files,RECORDINGS_PER_WORD);

    char* nine_files[RECORDINGS_PER_WORD] = {"analysis/nine1-40.out",
        "analysis/nine2-40.out", "analysis/nine3-40.out", "analysis/nine4-40.out",
        "analysis/nine5-40.out"};
    recording_set* nine = malloc(sizeof(recording_set));
    strcpy(nine->name, "nine");
    nine->num_recordings = RECORDINGS_PER_WORD;
    nine->recordings = features_from_all_files(nine_files,RECORDINGS_PER_WORD);



    // Actually run training
    printf("Running training and clustering algorithm...\n\n");
    int iterations = atoi(argv[1]);
    gaussian_cluster** results =
        train_isolated_phoneme_models(recordings, NUM_WORDS, iterations);


    /* TODO - recombine phonemes into words and save those
     */

    // Save results
    printf("Now saving results...\n");

    for(int i = 0; i < NUM_PHONEMES; i++)
    {
        printf("    hmms/%s-init.hmm\n", phoneme_names[i]);

        char filename[128];
        sprintf(filename, "%s-init", phoneme_names[i]);
        write_cluster_to_file(filename, results[i]);
    }

    printf("Done!\n\n");



    return 0;
}
