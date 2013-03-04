#include "cluster.h"

single_gaussian_params ** cluster_templates(feature_vectors ** templates,int num_templates){
	int iterations =0;
	//Initialize K means cluster by assigning each vector to a cluster
	//printf("initializing clusters...\n");
	int ** cluster_array = (int **)malloc(num_templates*sizeof(int*));

	//Initialize counter for each cluster
	int * cluster_count = (int *)malloc(NUM_CLUSTERS*sizeof(int));
	for(int i=0; i<NUM_CLUSTERS; i++){
		cluster_count[i] = 0;
	}

	//Set each point to an initial cluster	
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
		}
		cluster_array[i] = vector_clusters;
	}
	
	//Initialize gaussian statistics for each cluster
	single_gaussian_params ** cluster_stats = (single_gaussian_params**) malloc(NUM_CLUSTERS*sizeof(single_gaussian_params*));

	//Create feature_vectors array for each cluster to send off for gaussians
	feature_vectors * vectors_array = (feature_vectors*) malloc(NUM_CLUSTERS*sizeof(feature_vectors));

	//Keep track of the index in each cluster we are building
        int * cluster_index = (int*) malloc(NUM_CLUSTERS * sizeof(int));

	//Number of reclassifications needed on each iteration
	int reclassify;
		
	//Iterate clustering until convergence
	do{
		reclassify = 0;

		//Zero out the index for each cluster we will build
		for(int i=0; i<NUM_CLUSTERS; i++){
                	cluster_index[i] = 0;
        	}

		//Initialize NUM_CLUSTERS different feature vector arrays to be sent for gaussian evaluation
		for(int i=0; i <NUM_CLUSTERS;i++){
			vectors_array[i].features = (feature*)malloc(sizeof(feature)*cluster_count[i]);
			vectors_array[i].num_vectors = cluster_count[i];
		}

		//Build arrays of features to pass into gaussian calculation 
		for(int i=0; i < num_templates; i++){
			int num_vectors = templates[i]->num_vectors;			
			for(int j=0; j<num_vectors; j++){
				int my_cluster = cluster_array[i][j];
			
				//Feature goes here
				feature * new_feature = &(vectors_array[my_cluster].features[cluster_index[my_cluster]]);

				//Grab feature from here
				feature * old_feature = &(templates[i]->features[j]);

				//Copy feature into feature array to pass into gaussian evaluation
				memcpy(new_feature,old_feature,sizeof(feature));
				
				//Increment the cluster index to keep track of cluster position
				cluster_index[my_cluster]+=1;
			}
		}

		//Get gaussian parameters
		for(int i =0; i<NUM_CLUSTERS; i++){
			single_gaussian_params* cluster_gaus_params;
			cluster_gaus_params = compute_single_gaussian_params(&(vectors_array[i]));
			cluster_stats[i] = cluster_gaus_params;
		}

		//Reevaluate each point given the gaussians
		for(int i=0; i < num_templates; i++){
                        int num_vectors = templates[i]->num_vectors;
                        for(int j=0; j<num_vectors; j++){
                                int my_cluster = cluster_array[i][j];
				int new_cluster = my_cluster; //initialize to original cluster
				double best = MAXINT;
				feature * point = &(templates[i]->features[j]);
				for(int k=0; k<NUM_CLUSTERS; k++){
					single_gaussian_params * dist = cluster_stats[k];
					feature * mean = &(dist->means);

					double distance = feature_distance(*point,*mean);

					if(distance<best){
						best = distance;
						new_cluster = k;
					}
					
				}
				
				//Reclassify point if it's in the wrong cluster
				if(new_cluster!=my_cluster && cluster_count[my_cluster] > 1){
						reclassify++;	
						cluster_array[i][j] = new_cluster;
						cluster_count[my_cluster]-=1;
						cluster_count[new_cluster]+=1;	
				}
					
			}
		}

		//Free Memory used in recalculating gaussians
		for(int i=0; i <NUM_CLUSTERS;i++){
			free(vectors_array[i].features);
		}

		iterations++;
	}while(iterations<CONVERGE_ITERATIONS && reclassify>CONVERGE_THRESHOLD);
	
	//Free everything!
	for(int i=0; i<num_templates; i++){
		free(cluster_array[i]);
	}	
	free(cluster_array);
	free(cluster_count);
	free(cluster_index);

	//need to return cluster_stats
	printf("Trying to return clustered stats\n");
	return cluster_stats;
}



void print_cluster_count(int * cluster_count){
	for(int i=0; i<NUM_CLUSTERS; i++){
		printf("%d: %d\t",i,cluster_count[i]);
	}	
	printf("\n");
}



