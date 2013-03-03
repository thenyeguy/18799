#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "feature.h"

//Want to include this for clustering :(
#include "gaussian.h"

void cluster_templates(feature_vectors ** templates,int num_templates){
	//Initialize K means cluster by assigning each vector to a cluster
	printf("initializing clusters...\n");
	int ** cluster_array = (int **)malloc(num_templates*sizeof(int*));

	//Initialize counter for each cluster
	int * cluster_count = (int *)malloc(NUM_CLUSTERS*sizeof(int));
	for(int i=0; i<NUM_CLUSTERS; i++){
		cluster_count[i] = 0;
	}
	
	for(int i=0; i<num_templates; i++){
		int num_vectors = templates[i]->num_vectors;	
		//Holds the cluster that each feature belongs to
		int * vector_clusters = (int *)malloc(sizeof(int)*num_vectors);
		int segment_size = num_vectors/NUM_CLUSTERS;
		for(int j=0; j<num_vectors; j++){
			if(j<(NUM_CLUSTERS*segment_size)){
				vector_clusters[j] = j/segment_size;
			}
			else{
				vector_clusters[j] = NUM_CLUSTERS - 1;
			}
			cluster_count[vector_clusters[j]] +=1;
			printf("%d",  vector_clusters[j]);
		}
		printf("\n");
		cluster_array[i] = vector_clusters;
	}
	
	//Initialize gaussian statistics for each cluster
	single_gaussian_params ** cluster_stats = (single_gaussian_params**) malloc(NUM_CLUSTERS*sizeof(single_gaussian_params*));
	print_cluster_count(cluster_count);

	//Iterate clustering until convergence
	//do{
		//Initialize NUM_CLUSTERS different feature vector arrays to be sent for gaussian evaluation
		feature_vectors * vectors_array = (feature_vectors*) malloc(NUM_CLUSTERS*sizeof(feature_vectors));
		for(int i=0; i <NUM_CLUSTERS;i++){
			vectors_array[i].features = (feature*)malloc(sizeof(feature)*cluster_count[i]);
			vectors_array[i].num_vectors = cluster_count[i];
			printf("Allocating vector set %d to size: %d\n",i, cluster_count[i]);
		}

		//Keep track of the index in each cluster we are building
		int * cluster_index = (int*) malloc(NUM_CLUSTERS * sizeof(int));
		for(int i=0; i<NUM_CLUSTERS; i++){
			cluster_index[i] = 0;
		}

		//Build arrays of features to pass into gaussian calculation 
		for(int i=0; i < num_templates; i++){
			int num_vectors = templates[i]->num_vectors;			
			for(int j=0; j<num_vectors; j++){
				int my_cluster = cluster_array[i][j];
				printf("My Cluster: %d\n",my_cluster);
				
				//Feature goes here
				feature * new_feature = &(vectors_array[i].features[cluster_index[my_cluster]]);
				feature * old_feature = &(templates[i]->features[j]);
				memcpy(new_feature,old_feature,sizeof(feature));
				
				//Increment the cluster index to keep track of cluster position
				cluster_index[my_cluster]+=1;
				printf("In: %d\n",cluster_index[my_cluster]);
			}
		}

		//Get gaussian parameters
		for(int i =0; i<NUM_CLUSTERS; i++){
			single_gaussian_params* cluster_gaus_params = compute_single_gaussian_params(&(vectors_array[i]));			
			print_single_gaussian_params(cluster_gaus_params);
		}

		//Free Memory used in recalculating gaussians
		/*
		for(int i=0; i <NUM_CLUSTERS;i++){
			free(feature_set[i]);
		}
		free(cluster_index);*/
		cluster_stats = cluster_stats;
//	}while(1);	

	//Free cluster array
	/*
	for(int i=0; i<num_templates; i++){
		free(vectors_array[i].features);
	}*/
	free(vectors_array);
	free(cluster_array);
}

void print_cluster_count(int * cluster_count){
	for(int i=0; i<NUM_CLUSTERS; i++){
		printf("%d: %d\n",i,cluster_count[i]);
	}	
}



feature_vectors** features_from_all_files(int argc, char** argv){
	//Reading .out files from the command line
	int number_of_feature_vectors = argc - FIRST_TEMPLATE_INDEX;

	//Malloc space for each template feature vector
	int malloc_size = number_of_feature_vectors*sizeof(feature_vectors *); 
	feature_vectors ** templates = (feature_vectors **) malloc(malloc_size);

	//Getting features from file for each template path on command line	
	for( int i = 0 ; i<number_of_feature_vectors; i++){
		int template_index = i + FIRST_TEMPLATE_INDEX;
		feature_vectors * vects = features_from_file(argv[template_index]);
		templates[i] = vects;
	}	
	
	return templates;
}


feature_vectors* features_from_file(char* filename)
{
    //First open file
    FILE* file = fopen(filename, "r");
    if(file == NULL)
    {
        printf("Could not open file: %s\n\n",filename);
        exit(1);
    }

    //Figure out the number of vectors we have by counting lines
    //Then reset to beginning to read
    char c;
    int lines = 0;
    while((c = fgetc(file)) != EOF)
    {
        if(c == '\n')
            lines++;
    }
    fseek(file, 0, SEEK_SET);

    //Allocate space for features
    feature_vectors* fs = malloc(sizeof(feature_vectors));
    fs->num_vectors = lines;
    fs->features = malloc(lines*sizeof(feature));

    //For each line, read in the raw cepstrum
    //char line[512];
    double** raws = malloc(fs->num_vectors*sizeof(double*));
    for(int i = 0; i < fs->num_vectors; i++)
    {
        raws[i] = malloc(CEPSTRUM_DIMENSION*sizeof(double));
        //fgets(line, 512, file);
        for(int j = 0; j < CEPSTRUM_DIMENSION; j++)
            fscanf(file,"%lf",&raws[i][j]);

        //Ignore infinite/nan lines
        if(!isfinite(raws[i][0]))
        {
            i--;
            fs->num_vectors--;
        }
    }

    //Process the raws
    cepstrum_vectors* raw_cepstra = malloc(sizeof(cepstrum_vectors));
    raw_cepstra->num_vectors = lines;
    raw_cepstra->num_points = CEPSTRUM_DIMENSION;
    raw_cepstra->cepstra = raws;
    raw_cepstrum_to_features(raw_cepstra, fs);

    //Free memory
    for(int i = 0; i < fs->num_vectors; i++)
        free(raws[i]); 
    free(raws);
    free(raw_cepstra);

    return fs;
}


void raw_cepstrum_to_features(cepstrum_vectors* raws, feature_vectors* output)
{
    //First copy in the raw cepstrum
    for(int i = 0; i < output->num_vectors; i++)
        memcpy(&(output->features[i].values), raws->cepstra[i],
               CEPSTRUM_DIMENSION*sizeof(double));

    //Then mean normalize
    double mean[CEPSTRUM_DIMENSION];
    for(int i = 0; i < CEPSTRUM_DIMENSION; i++)
        mean[i] = 0.0;

    for(int i = 0; i < output->num_vectors; i++)
        for(int j = 0; j < CEPSTRUM_DIMENSION; j++)
            mean[j] += output->features[i].values[j];

    for(int i = 0; i < CEPSTRUM_DIMENSION; i++)
        mean[i] /= output->num_vectors;

    for(int i = 0; i < output->num_vectors; i++)
        for(int j = 0; j < CEPSTRUM_DIMENSION; j++)
            output->features[i].values[j] -= mean[j];

    /*
    //Then variance normalize
    double var[CEPSTRUM_DIMENSION];
    for(int i = 0; i < CEPSTRUM_DIMENSION; i++)
        var[i] = 0.0;

    for(int i = 0; i < output->num_vectors; i++)
        for(int j = 0; j < CEPSTRUM_DIMENSION; j++)
            var[j] += pow(output->features[i].values[j], 2);

    for(int i = 0; i < CEPSTRUM_DIMENSION; i++)
        var[i] = sqrt(var[i]/output->num_vectors);

    for(int i = 0; i < output->num_vectors; i++)
        for(int j = 0; j < CEPSTRUM_DIMENSION; j++)
            output->features[i].values[j] /= var[j];
            */

    //Then compute single deltas - copy second single delta into first
    for(int i = 1; i < output->num_vectors; i++)
        for(int j = 0; j < CEPSTRUM_DIMENSION; j++)
            output->features[i].deltas[j] =
                output->features[i].values[j] - 
                output->features[i-1].values[j];

    for(int i = 0; i < CEPSTRUM_DIMENSION; i++)
        output->features[0].deltas[i] = 
            output->features[1].deltas[i];

    //Then compute double deltas - copy second double delta into first
    for(int i = 1; i < output->num_vectors; i++)
        for(int j = 0; j < CEPSTRUM_DIMENSION; j++)
            output->features[i].doubles[j] =
                output->features[i].deltas[j] - 
                output->features[i-1].deltas[j];

    for(int i = 0; i < CEPSTRUM_DIMENSION; i++)
        output->features[0].doubles[i] = 
            output->features[1].doubles[i];
}


void print_feature(feature* f)
{
    printf("FEATURE: Cepstra\t| Deltas\t| Double deltas\n");
    for(int i = 0; i < CEPSTRUM_DIMENSION; i++)
    {
        printf("\t %5.5f\t| %5.5f\t| %5.5f\n",
               f->values[i], f->deltas[i], f->doubles[i]);
    }
}


void print_feature_vectors(feature_vectors* fs)
{
    printf("FEATURE VECTORS PACKET {{{{{{{{{{{{\n");
    for(int i = 0; i < fs->num_vectors; i++)
        print_feature(&fs->features[i]);
    printf("}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}\n\n");
}


void free_feature_vectors(feature_vectors* fs)
{
    free(fs->features);
    free(fs);
}
