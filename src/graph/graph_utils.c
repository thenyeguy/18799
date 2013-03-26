#include "graph_utils.h"

graph * build_graph(char * filename){
	graph * grammar_graph;
	FILE * file = fopen(filename,"r");
	if(NULL==file){
		printf("Error opening file: %s\n",filename);
		exit(0);
	}
	int buffer_size = 100;
	char buffer[buffer_size];
	int line_number = 0;
	int states;
	int start;
	int end;
	while (fgets(buffer, buffer_size, file) != NULL){
		//printf("%s\n",buffer);
		//FIXME This needs to be done using STRTOK rather than indexing in case double digit
		if(line_number==0){
			states = buffer[10]-'0';
			printf("Number of states: %d\n", states);
		}
		else if(line_number==1){
			start = buffer[13]-'0';
			printf("Start node: %d\n",start);
		}
		else if(line_number==2){
			end = buffer[17]-'0';
			printf("Terminal node: %d\n",end);
		}
		else{
			int from = buffer[5]-'0';
			int to = buffer[7]-'0';
			int hm = buffer[10]-'0';
			printf("Connect: %d to %d using: %d\n",from,to,hm);
		}
		line_number++;
	}


	fclose(file);
	return grammar_graph;
}

/*
void connect(graph * gr, int from, int to, hidden_markov hmm){
	graph->nodes[from]	


}
*/

graph * init_graph(int num_nodes){
	graph * grammar_graph = (graph *)malloc(sizeof(graph));
	graph_node * init_nodes = (graph_node *) malloc(sizeof(graph_node) * num_nodes);
	grammar_graph->nodes = init_nodes;
	grammar_graph->num_nodes = num_nodes;
	return grammar_graph;
}



