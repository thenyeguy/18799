#include <stdlib.h>
#include <stdio.h>
#include "../dtw/cluster.h"

int main(int argc, char** argv)
{
    if(argc < 3)
    {
        printf("\n");
        printf("USAGE: ./trainnaive [name] [template list]\n");
        printf("       Takes a list of template files and performs");
        printf(" a gaussian clustering on them.\n\n");
        printf("       Saves its output to hmms/name.hmm\n\n");
        exit(1);
    }


    //Get the args from command line
    char* name = argv[1];
    int num_templates     = argc-2;
    char** template_files = argv+2;


    //Get the templates
    printf("\nReading templates...\n");
    feature_vectors** templates =
        features_from_all_files(template_files, num_templates);


    //Cluster
    printf("Clustering...\n");
    gaussian_cluster* cluster =
        cluster_templates(templates, num_templates, NUM_CLUSTERS, name);


    //Save output
    char filename[256];
    sprintf(filename, "hmms/%s-init.hmm", name);
    printf("Saving results to: %s\n",filename);

    char appended[256];
    sprintf(appended, "%s-init", name);
    write_cluster_to_file(appended, cluster);

    printf("Done!\n\n");

    return 0;
}
