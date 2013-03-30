#ifndef GRAPH_UTILS_H
#define GRAPH_UTILS_H

#include "../dtw/cluster.h"

//Forward declare
typedef struct grammar grammar;
typedef struct grammar_node grammar_node;
typedef struct grammar_transition grammar_transition;


/* grammar  - Represents a grammar graph. Holds an array of each node 
 *            contained in the graph. The interconnections are specified
 *            within each graph node. Always start at node 0.
 */
struct grammar {
    grammar_node* nodes;
    int num_nodes;
    int num_edges;
};


/* grammar_node - Represents an individual node in the grammar graph. Contains
 * 	              a list of potential transitions from the current node through
   * 	          an HMM to another node in the graph.
 */
struct grammar_node {
    int node_id;
    int num_edges;
    grammar_transition* edges;
};


/* grammar_transition - A struct representing the transition from one node,
 * 	                    through an HMM into another node.
 */
struct grammar_transition {
    double transition_prob;
    gaussian_cluster* hmm;
    int next_node_id;
};


/* build_grammar - constructs a grammar graph from the text file passed in.
 */
grammar* build_grammar(char* filename);


/* grammar_build_hmm - given a list of space seperated files and a word that
 *                     identifies them, builds an HMM from the given files
 */
gaussian_cluster* grammar_build_hmm(char* files, char* word);


/* print_grammar_*- prints a human readable form of the grammar structure
 */
void print_grammar(grammar* g);
void print_grammar_node(grammar_node* n);
void print_grammar_edge(grammar_transition* e);


#endif
