#ifndef CLUSTER_H
#define CLUSTER_H

#include <stdbool.h>
#include "feature.h"
#include "gaussian.h"


/* NUM_CLUSTERS - number of time periods we break the template into to classify
 * CONVERGE_THRESHOLD - the number of nodes that can be moved and still consider
 *                      the k-means to be converged
 * CONVERGE_ITERATIONS - a hard cap on the number of iterations we iterate
 *                       before deciding it won't converge
 */
#define NUM_CLUSTERS 5
#define CONVERGE_THRESHOLD 3
#define CONVERGE_ITERATIONS 1000

#define MININT (1<<31)
#define MAXINT (~MININT)


/* Container to hold a set of gaussian_params that represents one template.
 *     Stores the number of templates inside, and the word used to generate
 *     this cluster.
 *
 *     stationary_probs[i] contains the probability of remaining in state i
 *     transition_probs[i] contains the probability of transitioning from
 *     state i to state i+1
 *     
 */
typedef struct {
    single_gaussian_params** params;

    double* stationary_probs;
    double* transition_probs;

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


/* read_cluster_from_file - Given a file containing a gaussian cluster,
 *							returns a gaussian_cluster with that cluster
 */
gaussian_cluster* read_cluster_from_file(char* filename);


/* write_cluster_to_file - write a gaussian_cluster to the filename given
*/
bool write_cluster_to_file(char* filename, gaussian_cluster* cluster);

/* print_cluster_count - Prints out the number of points in each cluster
*/
void print_cluster_count(int* cluster_count);


#endif
