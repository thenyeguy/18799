#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../libraries/string_utils.h"
#include "grammar.h"


grammar* build_grammar(char* filename)
{
    FILE* file = fopen(filename,"r");
    int buffer_size = 512;
    char buffer[buffer_size];
    char* temp;

    if(file == NULL)
    {
        printf("Error opening grammar file: %s\n", filename);
        exit(1);
    }


    //First read the important line
    grammar* g = malloc(sizeof(grammar));
    g->num_nodes = 0;
    g->num_edges = 0;
    int num_hmms;
    
    fgets(buffer, buffer_size, file);
    temp = strtok(buffer, " \n");
    temp = strtok(NULL, " \n");
    g->num_nodes= atoi(temp);

    fgets(buffer, buffer_size, file);
    temp = strtok(buffer, " \n");
    temp = strtok(NULL, " \n");
    num_hmms = atoi(temp);


    //Create nodes and HMM space
    g->nodes = malloc(g->num_nodes * sizeof(grammar_node));
    for(int i = 0; i < g->num_nodes; i++)
    {
        g->nodes[i].node_id = i;
        g->nodes[i].num_edges = 0;
    }
    gaussian_cluster** hmms = malloc(num_hmms*sizeof(gaussian_cluster*));
    (void) hmms;


    //Count edges from each node
    while(fgets(buffer, buffer_size, file) != NULL)
    {
        temp = strtok(buffer, " \n");
        if(temp != NULL && strcmp(temp,"Edge") == 0)
        {
            temp = strtok(NULL, " \n");
            int from = atoi(temp);
            g->nodes[from].num_edges++;
        }
    }

    
    //Allocate space for edges now. Reset count to zero, we rebuild it later
    for(int i = 0; i < g->num_nodes; i++)
    {
        g->nodes[i].edges =
            malloc(g->nodes[i].num_edges * sizeof(grammar_transition));
        g->nodes[i].num_edges = 0;
    }


    //Populate HMMs and edges
    //Reopen file to start from top
    fclose(file);
    file = fopen(filename, "r");

    while(fgets(buffer, buffer_size, file) != NULL)
    {
        temp = strtok(buffer, " \n");
        if(temp == NULL)
            continue;
        else if(strcmp(temp,"HMM") == 0)
        {
            temp = strtok(NULL, " \n");
            int i = atoi(temp);

            //Build HMM
            char* word = strtok(NULL, " \n");
            char* files = strtok(NULL, "\n");
            gaussian_cluster* hmm = grammar_build_hmm(files,word);
            hmms[i] = hmm;

            //printf("Built HMM for %s, id %d.\n",word,i);
        }
        else if(strcmp(temp,"Edge") == 0)
        {
            //Get to/from
            temp = strtok(NULL, " \n");
            int from = atoi(temp);
            temp = strtok(NULL, " \n");
            int to = atoi(temp);
            
            //Get HMM
            gaussian_cluster* hmm;
            temp = strtok(NULL, " \n");
            temp = &temp[4];
            if(strcmp(temp,"none") == 0)
                hmm = NULL;
            else
                hmm = hmms[atoi(&temp[4])];

            //Get edge prob
            temp = strtok(NULL, " \n");
            double prob = atof(&temp[5]);

            //Populate edge
            grammar_node* n = &g->nodes[from];
            grammar_transition* e = &n->edges[n->num_edges];
            e->hmm = hmm;
            e->next_node_id = to;
            e->transition_prob = prob;

            //Reupdate edge counts
            n->num_edges++;
            g->num_edges++;
        }
    }


    return g;
}


gaussian_cluster* grammar_build_hmm(char* files, char* word)
{
	gaussian_cluster* hmm;
	string_array* file_array = split_string(files,' ');
	
	//Get feature vectors from files
	feature_vectors** features =
        features_from_all_files(file_array->strings,file_array->num_strings);
	
	// Send of feature vectors to be clustered
	hmm = cluster_templates(features,file_array->num_strings,word);

	//return gaussian cluster	
	return hmm;
}


void print_grammar(grammar* g)
{
    printf("Grammar with %d nodes and %d edges.\n", g->num_nodes, g->num_edges);

    //Print nodes
    for(int i = 0; i < g->num_nodes; i++)
        print_grammar_node(&g->nodes[i]);
}


void print_grammar_node(grammar_node* n)
{
    printf("Node %d:\n", n->node_id);
    
    //Print edges
    if(n->num_edges == 0)
        printf("    This is a terminating node.\n");
    for(int i = 0; i < n->num_edges; i++)
        print_grammar_edge(&n->edges[i]);
}


void print_grammar_edge(grammar_transition* e)
{
    if(e->hmm == NULL)
        printf("    Edge to %d, edge probability %f, no HMM\n",
                e->next_node_id, e->transition_prob);
    else
        printf("    Edge to %d, edge probability %f, HMM for \"%s\"\n",
                e->next_node_id, e->transition_prob, e->hmm->word_id);
}
