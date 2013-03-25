#ifndef GRAPH_UTILS_H
#define GRAPH_UTILS_H

/* graph	Represents a grammar graph. Holds an array of each node 
 * 		contained in the graph. The interconnections are specified
 * 		within each graph node.
 */
typedef struct graph{
	graph_node * nodes;
	int num_nodes;
}graph;

/* graph_node	Represents an individual node in the grammar graph. Contains
 * 		a list of potential transitions from the current node through
 * 		an HMM to another node in the graph.
 */
typedef struct graph_node{
	transition * transition_list;
	int num_transitions;
}graph_node;

/* transition	A struct representing the transition from one node, through
 * 		an HMM into another node.
 */
typedef struct transition{
	hidden_markov hmm;
	graph_node next;
}transition;

/* hidden_markov	Place holder for the markov model developed previously.
 *			The markov model will be developed using segmental K-means
 * 			and split into N states with an array of means and covariances
 * 			as we traverse through the graph, the HMM for a word will be 
 *			used to obtain a score through that path in the graph.
 */ 
typedef struct hidden_markov{

}hidden_markov;

#endif





