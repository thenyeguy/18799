#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../libraries/queue.h"
#include "../dtw/cluster.h"
#include "../dtw/dtw_gaussian.h"
#include "../dtw/dtw_unity.h"
#include "viterbi.h"


char* viterbi_search3(grammar* grammar, feature_vectors* test, double threshold){
	
	//Initialize a backpointer table array based on the number of timesteps
	int time_steps = test->num_vectors;
        backpointer * backpointer_table = (backpointer*) malloc(time_steps*sizeof(backpointer));

	for(int i=0; i < time_steps; i++){
		backpointer_table[i].prev = NULL;
		backpointer_table[i].score = DTW_MIN_SCORE;
		backpointer_table[i].timestamp = i;
	}

	//Set the first backpointer to hold initial grammar node which holds next possible transitions
        backpointer_table[0].gn = &(grammar->nodes[0]);

	//Initialize an array of trellises
	int num_hmms = grammar->num_hmms;
	dtw_t ** ts = (dtw_t**) malloc(sizeof(dtw_t*) * num_hmms );

	//Each trellis is built using the gauss func; test and hmm
	gaussian_cluster ** hmms = grammar->hmms;
	for(int i=0; i < num_hmms; i++){
		ts[i] = *get_gaussian_trellis(test, &(hmms[i]), 1,DTW_BEAM_PRUNE, threshold);

		//In reality, we only want to set to the first backpointer if it's an initial trans
		dtw_set_incoming(ts[i],0,&(backpointer_table[0])); //FIXME
	}

	//Each trellis should be initialized with a backpointer

		//If the HMM is part of the initial grammar node's possible transitions

			//Initialize the trellis with a backpointer to the first backpointer table entry

		//Otherwise
			
			//Initialize the trellis with a backpointer set to NULL

	//For 0 to test length
	for(int t = 0; t< time_steps; t++){
		printf("t=%d\n",t);
		//For each trellis in the HMM/trellis array thing
		for(int i=0; i < num_hmms; i++){
			
			//Step forward in time by: 
			//If there is a backpointer at t-1:
			if( t>0	&& backpointer_table[t-1].score > DTW_MIN_SCORE){
			        double bp_score = backpointer_table[t-1].score;
				printf("BP_SCORE: %f\n",bp_score);

				//Attempt to add a come from below using the backpointer and score
				dtw_set_incoming(ts[i],bp_score,&(backpointer_table[t-1]));				
			}

			//Then fill a new column in the trellis
			dtw_fill_next_col(ts[i]);

			//If the word finish ie. non -inf score. 
			double score = ts[i]->score;
			printf("Trellis current score: %f\n",score);

			if(score!=DTW_MIN_SCORE){
				printf("Word ended\n");
				//If first word to finish, add myself
				//Set score, back pointer, and new grammar node based on back pointer
				if(score > backpointer_table[t].score){
					backpointer_table[t].prev = ts[i]-> backpointer;
                                        backpointer_table[t].score = score;
				}
			}
		}
	}

	//When the word ends, look at the backpointer table's last entry and trace it back
	print_bpt(backpointer_table,time_steps);
	return NULL;
}


void print_bpt(backpointer * bpt, int bp_size){
	printf("====bp====\n");
	for(int i=0; i<bp_size; i++){
		printf("t=%d:\t%f\tBackTrace: ",i,bpt[i].score);
		backpointer * temp = bpt[i].prev;
		while(temp){
			printf("%d ",temp->timestamp);
			temp = temp->prev;
			
		}
		printf("\n");
	}
	printf("==========\n\n");
}




















char* viterbi_search2(grammar* grammar, feature_vectors* test, double threshold){
	//Initialize viterbi_queue
	viterbi_queue * q = (viterbi_queue *) malloc(sizeof(viterbi_queue));
	q->length =0;
	q->head = NULL; 

	//Initialize back_pointer array
	int time_steps = test->num_vectors;
	backpointer * backpointer_table = (backpointer*) calloc(time_steps,sizeof(backpointer));
	
	//Push initial back_pointer into its array
	for(int i =0; i<time_steps; i++){
		backpointer_table[i].score = DTW_MIN_SCORE;	//sort of taken care of by calloc...
		backpointer_table[i].prev = NULL;
	}

	//For each grammar start initialize first viterbi_queue_node & fill in details
	grammar_node * initial_node = &(grammar->nodes[0]);
	int initial_transitions = initial_node->num_edges;

	//FIXME: For the initial grammar node, push a queue_node for each potential transition
	//We make the assumption right now that no new HMM's will be introduced in later
	//transitions. This should be changed to build a trellis for EACH HMM.
	for(int i=0; i<initial_transitions; i++){

		//Generate and initialize the first group of nodes
		viterbi_queue_node * first = (viterbi_queue_node*)malloc(sizeof(viterbi_queue_node));
		first->next = NULL;
		first->parent = &backpointer_table[0];
		first->time_step = 0;
		first->score = DTW_MIN_SCORE; //FIXME should this be the MIN_SCORE or 0?
		first->transition = &(initial_node->edges[i]);
		//FIXME, totally guessed on this next line
		first->trellis = *get_gaussian_trellis(test, &(first->transition->hmm), 1,DTW_BEAM_PRUNE, threshold);

		//Push initial viterbi_queue_nodes into the queue
		push_back_v(q, first);
	}

	//Loop while entries in queue:
	while(q->length>0){

		//pop off first node
		viterbi_queue_node * popped = pop_front_v(q);
		double node_score = popped->score;		
		printf("Score: %f\n", node_score);

		//decide to prune it or not

		//if word has ended:
		if(node_score!= DTW_MIN_SCORE){	
			
			//Figure out when the word ended so we may add it to the back pointer table
			int ended_time = popped->time_step;
			printf("Word ended at: %d\n",ended_time);

			printf("Current entry: %f\n",backpointer_table[ended_time].score);
			//No word has finished at this time instant yet, so create this backpointer entry
			if(backpointer_table[ended_time].score == DTW_MIN_SCORE){
				
				//First time a word has finished for this time instant
				backpointer_table[ended_time].score = popped-> score;
                                backpointer_table[ended_time].prev  = popped-> parent;
				//FIXME add a char * to backpointer to keep track of which HMM
				//Then add the HMM's name to the backpointer table entry
				printf("First time seeing a word end\n");
			}			
			//We want to kick out the previous best score
			else if(node_score > backpointer_table[ended_time].score){

				//Just update params as we have found a better path
				backpointer_table[ended_time].score = popped-> score;
				backpointer_table[ended_time].prev  = popped-> parent;
				//FIXME add the HMM's name to the backpointer entry
				printf("Kicked out a previous best score\n");

			}
			//Our attempt at being the best score has been beaten, this node is useless
			else{
				//Not sure if we need to do anything here, I don't think so
				printf("What.\n");
			}

		}

		//If this node is allowed to follow the backpointer[time] in the grammar
		//Then we need to enter from beneath trellis using that backpointer

		//Continue to fill up the trellis
		dtw_fill_next_col(popped->trellis);
		popped->time_step ++;
		popped->score = popped->trellis->score;	

		//Push the popped node back into the queue with updated trellis and time steps
		push_back_v(q,popped);
	}

	return NULL;
}

viterbi_queue_node* pop_front_v(viterbi_queue * q){
	if(q->head==NULL){
		return NULL;
	}
	else{
		viterbi_queue_node * t = q->head;
		q->head = t->next;
		q->length--;
		return t;
	}

}

void push_back_v(viterbi_queue * q, viterbi_queue_node * n){
	if(q->head ==NULL){
		q->head = n;
	}
	else{
		//FIXME use tail pointer
		viterbi_queue_node * temp = q->head;
		while(temp->next){
			temp = temp->next;
		}
		temp->next = n;
		n->next = NULL;
	}	
	q->length++;
}


// Maybe TODO: refactor this function? Its really long right now...
// FYI: leaks lots of memory in backpointers currently
char* viterbi_search(grammar* grammar, feature_vectors* test, double threshold){
    /* First, we have to traverse the grammar we read in, and use it to
     * construct the equivalent evaluation data so we can track state.
     */
    int num_nodes = grammar->num_nodes;
    viterbi_node* nodes = malloc(num_nodes*sizeof(viterbi_node));

    int num_edges = grammar->num_edges;
    viterbi_edge* edges = malloc(num_edges*sizeof(viterbi_edge));


    //printf("traversing grammar\n");
    //Traverse the grammar graph with a queue
    //And create the analog viterbi nodes and edges
    int edge_i = 0;
    for(int i = 0; i < num_nodes; i++)
    {
        //Get next node to fill
        grammar_node* gn = &grammar->nodes[i];
        viterbi_node* vn = &nodes[i];

        //Get the edges and create new edges
        vn->num_edges = gn->num_edges;
        vn->edges = malloc(vn->num_edges*sizeof(viterbi_edge));
        vn->best_score = DTW_MIN_SCORE;
        vn->best_backpointer = NULL;
        vn->best_word = "";
        for(int i = 0; i < vn->num_edges; i++)
        {
            grammar_transition* ge = &gn->edges[i];
            viterbi_edge* ve = &edges[edge_i];

            //If we have an HMM, we want to score against it
            //Otherwise, we want to just pass straight through, so have a unity
            //trellis that always returns its incoming score
            if(ge->hmm == NULL)
                ve->trellis = get_unity_trellis(test->num_vectors, DTW_BEAM_PRUNE,
                                                threshold);
            else
                ve->trellis = *get_gaussian_trellis(test, &ge->hmm, 1,
                                                    DTW_BEAM_PRUNE, threshold);

            //Set trellis to have no incoming score
            dtw_set_incoming(ve->trellis,DTW_MIN_SCORE,NULL);

            ve->entrance_cost = (ge->transition_prob);
            ve->next = &nodes[ge->next_node_id];

            //Advance
            vn->edges[i] = ve;
            edge_i++;
        }
    }

    //Just in case for testing
    assert(edge_i == num_edges);


    //printf("prepping backpointer\n");
    /* Finally, prep the nodes from the head of the grammar to have an incoming 
     * score of zero. TODO: change to entrance costs?
     */
    backpointer* bp = malloc(sizeof(backpointer));
    bp->timestamp = 0;
    bp->word = "";
    bp->len = 0;
    bp->score = 0;
    bp->prev = NULL;
    for(int i = 0; i < nodes[0].num_edges; i++)
    {
        viterbi_edge* edge = nodes[0].edges[i];
        dtw_set_incoming(edge->trellis, 0, bp);
    }

    //Create results lists
    backpointer* best = malloc(sizeof(backpointer));
    best->timestamp = 0;
    best->word = "NO RESULTS";
    best->len = strlen(best->word)+1;
    best->score = DTW_MIN_SCORE;
    best->prev = NULL;
    
    
    //printf("scoring!\n");
    /* Now, we traverse the graph, one time instant per step.
     */
    for(int t = 0; t < test->num_vectors; t++)
    {
        printf("\n============================");
        printf("\nTime t=%d\n",t);

        //Reset node entry scores
        for(int i = 0; i < num_nodes; i++)
        {
            nodes[i].best_score = DTW_MIN_SCORE;
            nodes[i].best_backpointer = NULL;
            nodes[i].best_word = "";
        }


        //printf("Score edges...\n");
        //Score every edge
        double column_max = DTW_MIN_SCORE;
        for(int i = 0; i < num_edges; i++)
        {
            viterbi_edge* edge = &edges[i];
            dtw_fill_next_col(edge->trellis);

            //Get score from trellis and visit a node
            //If we have the best score seen into this node, then update it
            double score = edge->trellis->score;
            printf("score %d %f\n",i,score);
            if(score > DTW_MIN_SCORE && score > edge->next->best_score)
            {
                score += edge->entrance_cost;
                //Get the template used for its word
                gaussian_cluster* template = edge->trellis->template_data;

                //Update the best seen score into this node
                edge->next->best_score = score;
                edge->next->best_backpointer = edge->trellis->backpointer;
                if(template == NULL)
                    edge->next->best_word = "";
                else
                    edge->next->best_word = template->word_id;

                //printf("Setting %p to backpointer %p\n",
                    //edge->next,edge->trellis->backpointer);
            }
            
            //Keep pruning information
            double this_max = edge->trellis->column_max;
            if(this_max > column_max)
                column_max = this_max;
        }


        //printf("prune...\n");
        //Prune trellis structures
        double window = abs(column_max)*threshold;
        double abs_threshold = column_max - window;
        for(int i = 0; i < num_edges; i++)
        {
            dtw_prune_next_column(edges[i].trellis, abs_threshold);
        }


        //printf("seed...\n");
        //Use the node information to seed the trellises
        for(int i = 0; i < num_nodes; i++)
        {
            viterbi_node* node = &nodes[i];
            backpointer* prev = node->best_backpointer;

            //Create backpointers
            backpointer* bp = malloc(sizeof(backpointer));
            bp->timestamp = t;
            bp->word = node->best_word;
            bp->len = strlen(node->best_word)+1;
            if(prev != NULL) bp->len += prev->len;
            bp->score = node->best_score;
            bp->prev = node->best_backpointer;

            //Replace our best if we are at the end of time
            if(t == test->num_vectors-1 && bp->score > best->score)
                best = bp;

            //printf("%d %p '%s' %f %p %p\n", i, &nodes[i], bp->word, bp->score, bp, bp->prev);
            //Set incoming to each edge
            for(int j = 0; j < node->num_edges; j++)
            {
                dtw_set_incoming(node->edges[j]->trellis, node->best_score,
                                 bp);
            }
        }
    }


    /* Finally, convert the best backpointer to a string
     */
    backpointer* p = best;
    char* s = malloc(p->len*sizeof(char));
    int len = p->len;

    printf("\n\nRebuild\n");
    //Follow back, copy into word
    while(p != NULL)
    {
        printf("Word %s ended at t=%d\n",p->word,p->timestamp);
        int segment_start = p->len - strlen(p->word);
        strncpy(&s[segment_start], p->word, strlen(p->word));
        s[segment_start + strlen(p->word)] = ' ';
        p = p->prev;
    }

    //Return it
    s[len] = '\0';
    return s;
}
