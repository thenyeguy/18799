#ifndef VITERBI_H
#define VITERBI_H

#include "../dtw/feature.h"
#include "../dtw/dtw_trellis.h"
#include "grammar.h"


/* Default threshold that seems to work well
 */
#define VITERBI_THRESHOLD 1.0


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
    bool terminal;
    viterbi_edge** edges;

    double best_score;
    void* best_backpointer;
    char* best_word;
};


/* A viterbi_edge contains evaluation information connected each node in the
 * grammar graph.
 *     trellis represents the DTW trellis structure describing this edge.
 *         This may be NULL. This signifies to immediately visit its child
 *         next represents where this edge lets out in the grammar graph.
 */
struct viterbi_edge {
    dtw_t* trellis; 
    double entrance_cost;
    viterbi_node* next;
};


/* A backpointer is simply a linked list that traces back to a root node.
 * Contains the word along this edge, and the total length of the string so far
 * Contains the time it was created and its score.
 */
typedef struct backpointer {
    int timestamp;
    char* word;
    int len;

    struct backpointer* prev;
    double score;
    struct grammar_node * gn;
    int hmm_path;
} backpointer;


/* viterbi_search - given a grammar to describe our HMM models, finds the best
 *                  spoken string that matches this test.
 *
 *                  Uses viterbi_backtrace to get backpointers and recontrsuct
 *                  word. See below for argument documenation.
 *
 */
char* viterbi_search(grammar* grammar, feature_vectors* test, double threshold);


/* viterbi_backtrace - given a grammar to describe our HMM models and their
 *                     interconnections, find the path through this grammar with
 *                     the maximal score for the given test input.
 *
  *                    Uses pruning threshold given.
  *
 *                     Returns the backpointer with the best score at end of time.
 */
backpointer* viterbi_backtrace(grammar* grammar, feature_vectors* test,
                               double threshold);

#endif
