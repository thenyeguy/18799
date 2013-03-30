#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../libraries/queue.h"
#include "../dtw/cluster.h"
#include "../dtw/dtw_gaussian.h"
#include "../dtw/dtw_unity.h"
#include "viterbi.h"


// Maybe TODO: refactor this function? Its really long right now...
char** viterbi_search(grammar* grammar, feature_vectors* test,
                      bool prune, double threshold, int n)
{
    /* First, we have to traverse the grammar we read in, and use it to
     * construct the equivalent evaluation data so we can track state.
     */
    int num_nodes = grammar->num_nodes;
    viterbi_node* nodes = malloc(num_nodes*sizeof(viterbi_node));

    int num_edges;
    viterbi_edge* edges = malloc(num_edges*sizeof(viterbi_edge));


    //Traverse the grammar graph with a queue
    //And create the analog viterbi nodes and edges
    int node_i = 0;
    int edge_i = 0;
    queue* q = new_queue();
    enqueue(q, &grammar->nodes[0]);

    while(q->size > 0)
    {
        //Get next node to fill
        grammar_node* gn = dequeue(q);
        viterbi_node* vn = &nodes[node_i];

        //Get the edges and create new edges
        vn->num_edges = gn->num_edges;
        vn->edges = malloc(vn->num_edges*sizeof(viterbi_edge));
        for(int i = 0; i < vn->num_edges; i++)
        {
            grammar_transition* ge = &gn->edges[i];
            viterbi_edge* ve = &vn->edges[i];

            //If we have an HMM, we want to score against it
            //Otherwise, we want to just pass straight through, so have a unity
            //trellis that always returns its incoming score
            if(ge->hmm == NULL)
                ve->trellis = get_unity_trellis(test->num_vectors, DTW_BEAM_PRUNE,
                                                threshold);
            else
                ve->trellis = *get_gaussian_trellis(test, &ge->hmm, 1,
                                                    DTW_BEAM_PRUNE, threshold);
            ve->next = &nodes[ge->next_node_id];

            //Prep the next node to check if it hasn't been seen yet
            if(ge->next_node_id > node_i)
                enqueue(q, &grammar->nodes[ge->next_node_id]);

            //Advance
            edge_i++;
        }

        //Advance
        node_i++;
    }

    //Just in case for testing
    assert(node_i == num_nodes);
    assert(edge_i == num_edges);


    /* Finally, prep the nodes from the head of the grammar to have an incoming 
     * score of zero. TODO: change to entrance costs?
     */
    backpointer* bp = malloc(sizeof(backpointer));
    bp->word = "";
    bp->len = 0;
    bp->score = 0;
    bp->prev = NULL;
    for(int i = 0; i < nodes[0].num_edges; i++)
    {
        viterbi_edge edge = nodes[0].edges[0];
        dtw_set_incoming(edge.trellis, 0, bp);
    }
    


    /* Now, we traverse the graph, one time instant per step.
     */
    backpointer** results = malloc(n*sizeof(backpointer*));
    for(int t = 0; t < test->num_vectors; t++)
    {
        //Reset node entry scores
        for(int i = 0; i < num_nodes; i++)
        {
            nodes[i].best_score = DTW_MIN_SCORE;
            nodes[i].best_backpointer = NULL;
        }


        //Score every edge
        //TODO: add transition costs?
        double column_max = DTW_MIN_SCORE;
        for(int i = 0; i < num_edges; i++)
        {
            viterbi_edge edge = edges[i];
            dtw_fill_next_col(edge.trellis);

            //Get score from trellis and visit a node
            //If we have the best score seen into this node, then update it
            double score = edge.trellis->score;
            if(score > DTW_MIN_SCORE && score > edge.next->best_score)
            {
                //Get the template used for its word
                gaussian_cluster* template = edge.trellis->template_data;

                //Update the best seen score into this node
                edge.next->best_score = score;
                edge.next->best_backpointer = edge.trellis->backpointer;
                edge.next->best_word = template->word_id;
            }
            
            //Keep pruning information
            double this_max = edge.trellis->column_max;
            if(this_max > column_max)
                column_max = this_max;
        }


        //Use the node information to seed the trellises
        for(int i = 0; i < num_nodes; i++)
        {
            viterbi_node node = nodes[i];
            backpointer* prev = node.best_backpointer;

            //Create backpointers
            backpointer* bp = malloc(sizeof(backpointer));
            bp->word = node.best_word;
            bp->len = strlen(node.best_word) + prev->len;
            bp->score = node.best_score;
            bp->prev = node.best_backpointer;

            //Add to our best table if we are at the end of time
            if(t == test->num_vectors-1)
                add_backpointer_to_results(results, n, bp);

            //Set incoming to each edge
            for(int j = 0; j < node.num_edges; j++)
            {
                dtw_set_incoming(node.edges[j].trellis, node.best_score,
                                 bp);
            }
        }


        //Prune trellis structures
        double window = abs(column_max)*threshold;
        double abs_threshold = column_max - window;
        for(int i = 0; i < num_edges; i++)
        {
            dtw_prune_next_column(edges[i].trellis, abs_threshold);
        }
    }


    /* Finally, convert the best n backpointers to string
     */
    char** result_strings = malloc(10*sizeof(char*));
    for(int i = 0; i < n; i++)
    {
        backpointer* p = results[i];
        char* s = malloc(p->len*sizeof(char) + 1);

        //Follow back, copy into word
        while(p != NULL)
        {
            int segment_start = p->len - strlen(p->word);
            strncpy(&s[segment_start], p->word, strlen(p->word));
            p = p->prev;
        }
    }

    return result_strings;
}


void add_backpointer_to_results(backpointer** results, int n,
                                backpointer* bp)
{
    for(int i = 0; i < n; i++)
    {
        //If we see an empty slot, then put our item at the bottom
        if(results[i] == NULL)
        {
            results[i] = bp;
            return;
        }

        if(bp->score > results[i]->score)
        {
            backpointer* temp1 = bp;
            backpointer* temp2 = results[i];
            for (int j = i; j < n; j++)
            {
                results[i] = temp1;
                temp1 = temp2;
                if (j+1 < n) temp2 = results[j+1];
            }

            free(temp2);
            return;
        }
    }
}
