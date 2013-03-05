#ifndef CLUSTER_H

#define CLUSTER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "feature.h"
#include "gaussian.h"

#define MININT (1<<31)
#define MAXINT (~MININT)
#define NUM_CLUSTERS 4
#define CONVERGE_THRESHOLD 3
#define CONVERGE_ITERATIONS 1000


/* cluster_templates - 	Runs Kmeans segmentation clustering on 
 *			input feature_vectors */
single_gaussian_params ** cluster_templates(feature_vectors ** templates,int num_templates);


/* print_cluster_count - 	Prints out the number of points in each cluster */
void print_cluster_count(int * cluster_count);


#endif
