#ifndef CLUSTER_H
#define CLUSTER_H


#include "feature.h"
#include "gaussian.h"


/* NUM_CLUSTERS - number of time periods we break the template into to classify
 * CONVERGE_THRESHOLD - the number of nodes that can be moved and still consider
 *                      the k-means to be converged
 * CONVERGE_ITERATIONS - a hard cap on the number of iterations we iterate
 *                       before deciding it won't converge
 */
#define MININT (1<<31)
#define MAXINT (~MININT)
#define NUM_CLUSTERS 5
#define CONVERGE_THRESHOLD 3
#define CONVERGE_ITERATIONS 1000


/* Container to hold a set of gaussian_params that represents one template.
 *     Stores the number of templates inside, and the word used to generate
 *     this cluster
 */
typedef struct {
    single_gaussian_params** params;
    int num_clusters;
    char word_id[256];
} gaussian_cluster;


/* cluster_templates - 	Runs Kmeans segmentation clustering on the input
 *                      feature_vectors and returns NUM_CLUSTERS gaussian
 *                      distributions describing the data.
 *
 *                      Uses the word_id to name the cluster.
 */
gaussian_cluster* cluster_templates(feature_vectors** templates,
                                    int num_templates, char* word_id);


/* print_cluster_count - Prints out the number of points in each cluster
*/
void print_cluster_count(int* cluster_count);


#endif
