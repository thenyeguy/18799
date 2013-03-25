#include "graph_utils.h"

graph * build_graph(char * filename){
	graph * grammar_graph = init_graph(4);
	FILE * file = fopen(filename,"r");
	if(NULL==file){
		printf("Error opening file: %s\n",filename);
		exit(0);
	}
	int buffer_size = 100;
	char buffer[buffer_size];
	while (fgets(buffer, buffer_size, file) != NULL){
		printf("%s\n",buffer);
	}


	fclose(file);
	return grammar_graph;
}

graph * init_graph(int num_nodes){
	graph * grammar_graph = (graph *)malloc(sizeof(graph));
	graph_node * init_nodes = (graph_node *) malloc(sizeof(graph_node) * num_nodes);
	grammar_graph->nodes = init_nodes;
	grammar_graph->num_nodes = num_nodes;
	return grammar_graph;
}



