#ifndef VITERBI_H
#define VITERBI_H

#include "../dtw/feature.h"
#include "../dtw/dtw_trellis.h"
#include "graph_utils.h"


/* Declare these structs ahead of time, because the actual definitions are
 * mutually recursive
 */
typedef struct viterbi_node viterbi_node;
typedef struct viterbi_edge viterbi_edge;


/* A viterbi_node contains evaluation information for the grammar graph.
 *     edges represents an edge from one node to the next, and is described
 *         below.
 *     best_score and best_backpointer contain information on the best path so
 *         far into this node at this time instant.
 */
struct viterbi_node {
    int num_edges;
    viterbi_edge* edges;

    double best_score;
    void* best_backpointer;
};


/* A viterbi_edge contains evaluation information connected each node in the
 * grammar graph.
 *     trellis represents the DTW trellis structure describing this edge.
 *     next represents where this edge lets out in the grammar graph.
 */
struct viterbi_edge {
    dtw_t* trellis; 
    viterbi_node* next;
};


/* visit_request - represents a request to update the score at a given node.
 *                 If it has a higher score than the last visit to this node,
 *                 update the visited information
 */
typedef struct {
    viterbi_node* node;
    double score;
    void* backpointer;
} viterbi_visit_requst;


/* viterbi_search - given a grammar to describe our HMM models and their
 *                  interconnections, find the path through this grammar with
 *                  the maximal score for the given test input. Returns a string
 *                  describing the recognized speech.
 *
 *                  Uses pruning threshold given, if prune is true
 */
char* viterbi_search(graph grammar, feature_vectors* test,
                     bool prune, double threshold);

#endif
