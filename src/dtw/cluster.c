#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "cluster.h"


gaussian_cluster* cluster_templates(feature_vectors** templates,
        int num_templates, char* word_id)
{
    int iterations =0;
    //Initialize K means cluster by assigning each vector to a cluster
    //printf("initializing clusters...\n");
    int** cluster_array = (int**) malloc(num_templates*sizeof(int*));

    //Initialize counter for each cluster
    int* cluster_count = (int *) malloc(NUM_CLUSTERS*sizeof(int));
    for(int i=0; i<NUM_CLUSTERS; i++)
    {
        cluster_count[i] = 0;
    }

    //Set each point to an initial cluster	
    for(int i=0; i<num_templates; i++)
    {
        int num_vectors = templates[i]->num_vectors;	
        //Holds the cluster that each feature belongs to
        int* vector_clusters = (int*) malloc(sizeof(int)*num_vectors);
        int segment_size = num_vectors/NUM_CLUSTERS;
        for(int j=0; j<num_vectors; j++)
        {
            if(j<(NUM_CLUSTERS*segment_size))
            {
                vector_clusters[j] = j/segment_size;
            }
            else
            {
                vector_clusters[j] = NUM_CLUSTERS - 1;
            }
            cluster_count[vector_clusters[j]] +=1;
        }
        cluster_array[i] = vector_clusters;
    }

    //Initialize gaussian statistics for each cluster
    single_gaussian_params** cluster_stats = (single_gaussian_params**)
        malloc(NUM_CLUSTERS*sizeof(single_gaussian_params*));

    //Create feature_vectors array for each cluster to send off for gaussians
    feature_vectors* vectors_array = (feature_vectors*)
        malloc(NUM_CLUSTERS*sizeof(feature_vectors));

    //Keep track of the index in each cluster we are building
    int* cluster_index = (int*) malloc(NUM_CLUSTERS * sizeof(int));

    //Number of reclassifications needed on each iteration
    int reclassify;

    //Iterate clustering until convergence
    do
    {
        reclassify = 0;

        //Zero out the index for each cluster we will build
        for(int i=0; i < NUM_CLUSTERS; i++)
        {
            cluster_index[i] = 0;
        }

        //Initialize NUM_CLUSTERS different feature vector arrays to be sent
        //for gaussian evaluation
        for(int i=0; i < NUM_CLUSTERS; i++)
        {
            vectors_array[i].features = (feature*)
                malloc(sizeof(feature)*cluster_count[i]);
            vectors_array[i].num_vectors = cluster_count[i];
        }

        //Build arrays of features to pass into gaussian calculation 
        for(int i=0; i < num_templates; i++)
        {
            int num_vectors = templates[i]->num_vectors;			
            for(int j=0; j<num_vectors; j++)
            {
                int my_cluster = cluster_array[i][j];

                //Feature goes here
                feature* new_feature = &(vectors_array[my_cluster].
                        features[cluster_index[my_cluster]]);

                //Grab feature from here
                feature* old_feature = &(templates[i]->features[j]);

                //Copy feature into feature array to pass into gaussian evaluation
                memcpy(new_feature,old_feature,sizeof(feature));

                //Increment the cluster index to keep track of cluster position
                cluster_index[my_cluster]+=1;
            }
        }

        //Get gaussian parameters
        for(int i =0; i<NUM_CLUSTERS; i++)
        {
            single_gaussian_params* cluster_gaus_params;
            cluster_gaus_params =
                compute_single_gaussian_params(&(vectors_array[i]));
            cluster_stats[i] = cluster_gaus_params;
        }

        //Reevaluate each point given the gaussians
        for(int i=0; i < num_templates; i++)
        {
            int num_vectors = templates[i]->num_vectors;
            for(int j=0; j<num_vectors; j++)
            {
                int my_cluster = cluster_array[i][j];
                int new_cluster = my_cluster; //initialize to original cluster
                double best = log(0.0);
                feature* point = &(templates[i]->features[j]);
                for(int k=my_cluster; k<my_cluster+1; k++)
                {
                    single_gaussian_params* dist = cluster_stats[k];

                    double distance = single_gaussian_log_pdf(dist,point);

                    if(distance>best)
                    {
                        best = distance;
                        new_cluster = k;
                    }

                }

                //Reclassify point if it's in the wrong cluster
                if(new_cluster != my_cluster && cluster_count[my_cluster] > 1)
                {
                    reclassify++;	
                    cluster_array[i][j] = new_cluster;
                    cluster_count[my_cluster]-=1;
                    cluster_count[new_cluster]+=1;	
                }
            }
        }

        //Free Memory used in recalculating gaussians
        for(int i=0; i <NUM_CLUSTERS; i++)
        {
            free(vectors_array[i].features);
        }

        iterations++;
    } while(iterations<CONVERGE_ITERATIONS && reclassify>CONVERGE_THRESHOLD);


    //Create container
    gaussian_cluster* cluster = malloc(sizeof(gaussian_cluster));
    cluster->params = cluster_stats;
    cluster->num_clusters = NUM_CLUSTERS;
    strcpy(cluster->word_id, word_id);

    //Set the transition probabilities
    cluster->stationary_probs = malloc(NUM_CLUSTERS*sizeof(double));
    cluster->transition_probs = malloc(NUM_CLUSTERS*sizeof(double));
    for(int i = 0; i < NUM_CLUSTERS; i++)
    {
        cluster->transition_probs[i] = ((double) num_templates)/ cluster_count[i];
        cluster->stationary_probs[i] = 1 - cluster->transition_probs[i];
    }

    //Free everything!
    for(int i=0; i<num_templates; i++){
        free(cluster_array[i]);
    }	
    free(cluster_array);
    free(cluster_count);
    free(cluster_index);

    return cluster;
}

gaussian_cluster* read_cluster_from_file(char* filename) {

    //Open the file for reading
    char fname[128];
    sprintf(fname, "hmms/%s.hmm", filename);
    FILE* infile = fopen(fname,"r");
    if(!infile){
        printf("Failed to open file: %s in read_cluster_from_file.\n",fname);
        exit(0);
    }

    //Initialize the gaussian cluster that will be returned
    gaussian_cluster * cluster = (gaussian_cluster *)malloc(sizeof(gaussian_cluster));
    cluster=cluster;

    //Initialize buffer for reading
    int buffer_size = 256;
    char buffer[buffer_size];

    //word_id
    fgets(buffer,buffer_size,infile); //Once to move to the next line
    fgets(buffer,buffer_size,infile);
    strcpy(cluster->word_id,buffer);
    cluster->word_id[strlen(buffer)-1]='\0';

    //number of clusters
    fgets(buffer,buffer_size,infile); //Once to move to the next line
    fgets(buffer,buffer_size,infile);
    cluster->num_clusters = atoi(buffer);	

    //stationary_probs
    fgets(buffer,buffer_size,infile); //Once to move to the next line
    fgets(buffer,buffer_size,infile);
    double * stationary_probs = (double*) malloc(sizeof(double)*cluster->num_clusters);
    char * st_ptr;
    int i=0; 
    st_ptr = strtok(buffer," ");
    while(st_ptr!=NULL){
        double temp = atof(st_ptr);
        stationary_probs[i] = temp;
        i++;
        st_ptr = strtok(NULL," ");
    }
    cluster->stationary_probs = stationary_probs;

    //transition_probs
    fgets(buffer,buffer_size,infile); //Once to move to the next line
    fgets(buffer,buffer_size,infile);
    double * transition_probs = (double*) malloc(sizeof(double)*cluster->num_clusters);
    char * tr_ptr;
    int j=0; 
    tr_ptr = strtok(buffer," ");
    while(tr_ptr!=NULL){
        double temp = atof(tr_ptr);
        transition_probs[j] = temp;
        j++;
        tr_ptr = strtok(NULL," ");
    }   
    cluster->transition_probs = transition_probs;

    //single_gaussian_params
    cluster->params = (single_gaussian_params**) malloc(sizeof(single_gaussian_params*) * cluster->num_clusters);
    for(int i=0; i<cluster->num_clusters; i++){
        cluster->params[i] = (single_gaussian_params*) malloc(sizeof(single_gaussian_params));
    }

    for(int i=0; i<cluster->num_clusters; i++){

        //means
        fgets(buffer,buffer_size,infile); //Once to move to the next line

        //values
        int j=0;
        fgets(buffer,buffer_size,infile);
        char * tok = strtok(buffer," ");
        while(tok!=NULL){
            double temp = atof(tok);
            cluster->params[i]->means.values[j] = temp;
            j++;
            tok = strtok(NULL," ");
        }

        //deltas
        j=0;
        fgets(buffer,buffer_size,infile);
        tok = strtok(buffer," ");
        while(tok!=NULL){
            double temp = atof(tok);
            cluster->params[i]->means.deltas[j] = temp;
            j++;
            tok = strtok(NULL," ");
        } 

        //doubles
        j=0;
        fgets(buffer,buffer_size,infile);
        tok = strtok(buffer," ");
        while(tok!=NULL){
            double temp = atof(tok);
            cluster->params[i]->means.doubles[j] = temp;
            j++;
            tok = strtok(NULL," ");
        }

        //covs
        fgets(buffer,buffer_size,infile); //Once to move to the next line

        //values
        j=0;
        fgets(buffer,buffer_size,infile);
        tok = strtok(buffer," ");
        while(tok!=NULL){
            double temp = atof(tok);
            cluster->params[i]->covariances.values[j] = temp;
            j++;
            tok = strtok(NULL," ");
        }

        //deltas
        j=0;
        fgets(buffer,buffer_size,infile);
        tok = strtok(buffer," ");
        while(tok!=NULL){
            double temp = atof(tok);
            cluster->params[i]->covariances.deltas[j] = temp;
            j++;
            tok = strtok(NULL," ");
        }

        //doubles
        j=0;
        fgets(buffer,buffer_size,infile);
        tok = strtok(buffer," ");
        while(tok!=NULL){
            double temp = atof(tok);
            cluster->params[i]->covariances.doubles[j] = temp;
            j++;
            tok = strtok(NULL," ");
        }

    }
    fclose(infile);
    return cluster;
}

bool write_cluster_to_file(char* filename, gaussian_cluster* cluster) {
    char fname[128];
    sprintf(fname, "hmms/%s.hmm", filename);

    FILE* outfile = fopen(fname, "w");
    if(!outfile){
        return false;
    }
    int num_clusters = cluster->num_clusters;

    //word_id
    fprintf(outfile, "WORD_ID\n");
    fprintf(outfile, "%s\n", cluster->word_id);

    //num_clusters
    fprintf(outfile, "NUM_CLUSTERS\n");
    fprintf(outfile, "%d\n", num_clusters);

    //stationary_probs
    fprintf(outfile, "STATIONARY_PROBS\n");
    for (int i = 0; i<num_clusters; i++) {
        fprintf(outfile, "%f ", cluster->stationary_probs[i]);
    }
    fprintf(outfile, "\n");

    //transition_probs
    fprintf(outfile, "TRANSITION_PROBS\n");
    for (int i = 0; i<num_clusters; i++) {
        fprintf(outfile, "%f ", cluster->transition_probs[i]);
    }
    fprintf(outfile, "\n");

    //single_gaussian_params
    for(int i=0; i<num_clusters; i++){

        //word_id

        //means separated by spaces
        fprintf(outfile, "MEANS\n");

        //values
        for(int j=0; j<CEPSTRUM_DIMENSION; j++){
            fprintf(outfile,"%f ",cluster->params[i]->means.values[j]);
        }
        fprintf(outfile,"\n");

        //deltas
        for(int j=0; j<CEPSTRUM_DIMENSION; j++){
            fprintf(outfile,"%f ",cluster->params[i]->means.deltas[j]);
        }   
        fprintf(outfile,"\n");

        //doubles
        for(int j=0; j<CEPSTRUM_DIMENSION; j++){
            fprintf(outfile,"%f ",cluster->params[i]->means.doubles[j]);
        }   
        fprintf(outfile,"\n");


        //cov separated by spaces
        fprintf(outfile, "COVS\n");

        //values
        for(int j=0; j<CEPSTRUM_DIMENSION; j++){
            fprintf(outfile,"%f ",cluster->params[i]->covariances.values[j]);
        }   
        fprintf(outfile,"\n");

        //deltas
        for(int j=0; j<CEPSTRUM_DIMENSION; j++){
            fprintf(outfile,"%f ",cluster->params[i]->covariances.deltas[j]);
        }   
        fprintf(outfile,"\n");

        //doubles
        for(int j=0; j<CEPSTRUM_DIMENSION; j++){
            fprintf(outfile,"%f ",cluster->params[i]->covariances.doubles[j]);
        }   
        fprintf(outfile,"\n");	


    }
    fclose(outfile);
    return true;
}


void print_cluster_count(int* cluster_count)
{
    for(int i=0; i<NUM_CLUSTERS; i++)
    {
        printf("%d: %d\t",i,cluster_count[i]);
    }	
    printf("\n");
}
