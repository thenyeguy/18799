#include "graph_utils.h"

graph * build_graph(char * filename){
	filename = filename;
	graph * grammar_graph = init_graph(4);
	return grammar_graph;
}

graph * init_graph(int num_nodes){
	graph * grammar_graph = (graph *)malloc(sizeof(graph));
	graph_node * init_nodes = (graph_node *) malloc(sizeof(graph_node) * num_nodes);
	grammar_graph->nodes = init_nodes;
	grammar_graph->num_nodes = num_nodes;
	return grammar_graph;
}



