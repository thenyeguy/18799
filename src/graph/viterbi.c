#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../libraries/queue.h"
#include "../dtw/cluster.h"
#include "viterbi.h"


char** viterbi_search(graph grammar, feature_vectors* test,
                      bool prune, double threshold, int n)
{
    /* First, we have to traverse the grammar we read in, and use it to
     * construct the equivalent evaluation data so we can track state
     */
    viterbi_node* nodes;
    int num_nodes;
    viterbi_edge* edges;
    int num_edges;
    //TODO: go back add the grammar parsing
    //Initialize dtw_set_incoming with zeros
    

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
            backpointer* backpointer = malloc(sizeof(backpointer));
            backpointer->word = node.best_word;
            backpointer->len = strlen(node.best_word) + prev->len;
            backpointer->score = node.best_score;
            backpointer->prev = node.best_backpointer;

            //Add to our best table if we are at the end of time
            if(t == test->num_vectors-1)
                add_backpointer_to_results(results, n, backpointer);

            //Set incoming to each edge
            for(int j = 0; j < node.num_edges; j++)
            {
                dtw_set_incoming(node.edges[j].trellis, node.best_score,
                                 backpointer);
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
