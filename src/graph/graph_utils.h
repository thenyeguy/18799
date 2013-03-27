#ifndef GRAPH_UTILS_H
#define GRAPH_UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "../dtw/cluster.h"

/* graph	Represents a grammar graph. Holds an array of each node 
 * 		contained in the graph. The interconnections are specified
 * 		within each graph node.
 */
typedef struct graph{
	struct graph_node * nodes;
	int num_nodes;
}graph;

/* graph_node	Represents an individual node in the grammar graph. Contains
 * 		a list of potential transitions from the current node through
 * 		an HMM to another node in the graph.
 */
typedef struct graph_node{
	struct transition * head;
	int num_transitions;
}graph_node;

/* transition	A struct representing the transition from one node, through
 * 		an HMM into another node.
 */
typedef struct transition{
	gaussian_cluster * hmm;
	struct graph_node * next_node;
	struct transition * next_trans;
	//Used for debugging/displaying structure
	int next_node_index;
}transition;


/* build_graph	Constructs a grammar graph from the textfile passed in
 *		After initializing an array of nodes according to the 
 *		first line in the text file, the build_graph function
 *		loops through each line specifying an edge and "connects"
 *		them. 
 */
graph * build_graph(char * filename);

/* init_graph	Initializes a graph by allocating an array of nodes. The
 *		number of nodes corresponds to the first line in the text 
 *		file.
 */
graph * init_graph(int num_nodes);

/* connect	Connect takes the current state of the graph and adds
 * 		a transition. The 'from' node contains a link list of 
 *		transitions and connect appends a transition to this 
 *		The 'hm' parameter must be changed once we decide how 
 * 		hm's should be held in the graph. I believe there 
 *		should be an hmm struct somewhere from assignemnt 3?
 */
void connect(graph * gr, int from, int to, gaussian_cluster * hmm);

/* append	append takes the current node and adds a new transition
 *		to its linked list. This indicates a new possible path
 * 		that can be taken when traversing the graph.
 */
void append(graph_node * from_node, transition * new_trans);

/* print_graph	prints out the graph by printing individual nodes
 */
void print_graph(graph * gr);

/* print_graph_node	prints a graph node :/
 */
void print_graph_node(graph_node * gn);


/* build_hmm_from_arg	Helper function for parsing graph text file.
 *			Takes strings from the text file and passes 
 *			them off to be clustered and returned as
 *			an HMM.
 */
gaussian_cluster * build_hmm_from_arg(char * files);

#endif





