#ifndef VITERBI_H
#define VITERBI_H

#include "../dtw/feature.h"
#include "../dtw/dtw_trellis.h"
#include "grammar.h"


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
    char* best_word;
};


/* A viterbi_edge contains evaluation information connected each node in the
 * grammar graph.
 *     trellis represents the DTW trellis structure describing this edge.
 *        This may be NULL. This signifies to immediately visit its child
 *     next represents where this edge lets out in the grammar graph.
 */
struct viterbi_edge {
    dtw_t* trellis; 
    viterbi_node* next;
};


/* A backpointer is simply a linked list that traces back to a root node.
 * Contains the word along this edge, and the total length of the string so far
 * Contains the time it was created and its score.
 */
typedef struct backpointer {
    char* word;
    int len;
    double score;
    struct backpointer* prev;
} backpointer;


/* viterbi_search - given a grammar to describe our HMM models and their
 *                  interconnections, find the path through this grammar with
 *                  the maximal score for the given test input. Returns a string
 *                  describing the recognized speech.
 *
 *                  Uses pruning threshold given, if prune is true. Returns best
 *                  n results found.
 */
char** viterbi_search(grammar* grammar, feature_vectors* test,
                      bool prune, double threshold, int n);


/* fill_viterbi_graph - given a grammar, and arrays to store all the nodes and
 *                      edges in, fills these arrays with data from the grammar
 */
void fill_viterbi_graph(graph* grammer, viterbi_node* nodes, int num_nodes,
                                        viterbi_edge* edges, int num_edges);


/* add_backpointer_to_results - given a backpointer list and the backpointer,
 *                              adds to the list, sorted by score
 */
void add_backpointer_to_results(backpointer** results, int n,
                                backpointer* backpointer);

#endif
