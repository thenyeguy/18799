#include <stdlib.h>
#include <stdio.h>
#include "../libraries/queue.h"
#include "viterbi.h"


char* viterbi_search(graph grammar, feature_vectors* test,
                     bool prune, double threshold)
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
    

    /* Now, we traverse the graph, one time instant per step
     */
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
            double score = edge.trellis->score;
            if(score > DTW_MIN_SCORE)
            {
                edge.next->best_score = score;
                edge.next->best_backpointer = edge.trellis->backpointer;
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

            //TODO: Add to backpointer table
            void* backpointer;

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


    /* Finally, search the backpointer table for the best score that terminated
     * at the last time instant, then follow it to reconstruct our result.
     * TODO
     */
    char* result = "";

    return result;
}
