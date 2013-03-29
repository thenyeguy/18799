#include "graph_utils.h"


graph * build_graph(char * filename){
	graph * grammar_graph;
	FILE * file = fopen(filename,"r");
	if(NULL==file){
		printf("Error opening file: %s\n",filename);
		exit(0);
	}
	int buffer_size = 512;
	char buffer[buffer_size];
	int line_number = 0;
	int states;
	int start;
	int end;
	char* temp;

	while (fgets(buffer, buffer_size, file) != NULL){
		if(line_number==0){
			temp = strtok(buffer, " ");
			temp = strtok(NULL, " ");
			states = atoi(temp);

			//printf("Number of states: %d\n", states);
			grammar_graph = init_graph(states);
		}
		else if(line_number==1){
			temp = strtok(buffer, " ");
			temp = strtok(NULL, " ");
			start = atoi(temp);
			//printf("Start node: %d\n",start);
		}
		else if(line_number==2){
			temp = strtok(buffer, " ");
			temp = strtok(NULL, " ");
			end = atoi(temp);
			//printf("Terminal node: %d\n",end);
		}
		else{
			temp = strtok(buffer, " ");
			temp = strtok(NULL, " ");
			int from = atoi(temp);

			temp = strtok(NULL, " ");
			int to = atoi(temp);

			temp = strtok(NULL, " "); /// form "a.out","b.out","c.out"...
			printf("temp = %s\n", temp);

			gaussian_cluster * hmm = build_hmm_from_arg(temp);
			connect(grammar_graph,from,to,hmm);
			//printf("Connect: %d to %d using: %d\n",from,to,hm);
		}

		line_number++;
	}
	print_graph(grammar_graph);
	fclose(file);
	return grammar_graph;
}


gaussian_cluster * build_hmm_from_arg(char * files){
	gaussian_cluster * hmm;
	printf("Need to parse: %s\n",files);
	string_array * file_array = split_string(files,',');
	char* temp;
	for (int i = 0; i < file_array->num_strings; i++) {
		/// remove the quotes, if there are quotes
		if ('"' != (file_array->strings[i])[0]) 
			continue;
		temp = strtok(file_array->strings[i], "\"");
		strcpy(file_array->strings[i], temp);
	}
	string_array_print(file_array);
	
	//Get feature vectors from files
	feature_vectors** features = features_from_all_files(file_array->strings,file_array->num_strings);
	
	// Send of feature vectors to be clustered
	hmm = cluster_templates(features,file_array->num_strings,"three");

	//free dynamic string array

	//return gaussian cluster	
	return hmm;
}

void connect(graph * gr, int from, int to, gaussian_cluster * hmm){
	//Generate the new transition with corresponding HMM
	transition * new_trans = (transition*) malloc(sizeof(transition));
	new_trans->hmm=hmm; 
	new_trans->next_node = &(gr->nodes[to]);
	new_trans->next_node_index = to;
	new_trans->next_trans =NULL;

	//Append new transition to the graph_node's linked list of transitions
	graph_node * from_node = &(gr->nodes[from]);
	append(from_node,new_trans);
	from_node->num_transitions++;
}

void append(graph_node * from_node, transition * new_trans){
	transition *temp = from_node->head;
	if(temp==NULL){
		from_node->head = new_trans;
	}
	else{	
		while(temp->next_trans){
			temp = temp->next_trans;
		}
		temp->next_trans = new_trans;
		new_trans->next_trans=NULL;
	}
}

void print_graph(graph * gr){
	for(int i=0; i<gr->num_nodes; i++){
		printf("Node: %d\n",i);
		print_graph_node(&(gr->nodes[i]));
	}
}

void print_graph_node(graph_node * gn){
	printf("=======NODE======\n");
	//printf("num trans: %d\n",gn->num_transitions);
	transition *temp = gn->head;
	while(temp){
		printf("%d with HMM: \n",temp->next_node_index);//FIXME add HMM to graph print out
		temp = temp->next_trans;
	}
	printf("=================\n");
}


graph * init_graph(int num_nodes){
	graph * grammar_graph = (graph *)malloc(sizeof(graph));
	graph_node * init_nodes = (graph_node *) malloc(sizeof(graph_node) * num_nodes);
	grammar_graph->nodes = init_nodes;
	grammar_graph->num_nodes = num_nodes;
	for(int i=0; i<num_nodes; i++){
		grammar_graph->nodes[i].head = NULL;
	}
	return grammar_graph;
}



