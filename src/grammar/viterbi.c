#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../libraries/queue.h"
#include "../dtw/cluster.h"
#include "../dtw/dtw_gaussian.h"
#include "../dtw/dtw_unity.h"
#include "viterbi.h"


char* viterbi_search(grammar* grammar, feature_vectors* test, double threshold)
{
    //Use backtrace to get the best backpointer
    backpointer* best = viterbi_backtrace(grammar, test, threshold);

    //Reconstruct string from backtrace
    backpointer* p = best;
    int len = p->len;
    char* s = malloc(len*sizeof(char));
    strcpy(s," ");

    printf("\n\nRebuild\n");
    //Follow back, copy into word
    while(p != NULL)
    {
        if(p->prev != NULL)
            printf("Word %s ended at \tt=%d \twith score:%1.4f\n",
                   p->word,p->timestamp,p->score);
        int segment_start = p->len - strlen(p->word);
        strncpy(&s[segment_start], p->word, strlen(p->word));
        s[segment_start + strlen(p->word)] = ' ';
        p = p->prev;
    }
    printf("\nFINAL SCORE: %1.4f\n\n", best->score);

    //Return it
    s[len] = '\0';
    return s;
}


// Maybe TODO: refactor this function? Its really long right now...
// FYI: leaks lots of memory in backpointers currently
backpointer* viterbi_backtrace(grammar* grammar, feature_vectors* test,
                               double threshold)
{
    /* First, we have to traverse the grammar we read in, and use it to
     * construct the equivalent evaluation data so we can track state.
     */
    int num_nodes = grammar->num_nodes;
    viterbi_node* nodes = malloc(num_nodes*sizeof(viterbi_node));

    int num_edges = grammar->num_edges;
    viterbi_edge* edges = malloc(num_edges*sizeof(viterbi_edge));


    /*Traverse the grammar graph with a queue
     *And create the analog viterbi nodes and edges
     */
    int edge_i = 0;
    for(int i = 0; i < num_nodes; i++)
    {
        //Get next node to fill
        grammar_node* gn = &grammar->nodes[i];
        viterbi_node* vn = &nodes[i];

        //Get the edges and create new edges
        vn->num_edges = gn->num_edges;
        vn->terminal = gn->terminal;
        vn->edges = malloc(vn->num_edges*sizeof(viterbi_edge));
        vn->best_score = DTW_MIN_SCORE;
        vn->best_backpointer = NULL;
        vn->best_word = "";
        for(int i = 0; i < vn->num_edges; i++)
        {
            grammar_transition* ge = &gn->edges[i];
            viterbi_edge* ve = &edges[edge_i];

            //If we have an HMM, we want to score against it
            if(ge->hmm != NULL)
            {
                ve->trellis = *get_gaussian_trellis(test, &ge->hmm, 1,
                                                    DTW_BEAM_PRUNE, threshold);

                //Set trellis to have no incoming score
                dtw_set_incoming(ve->trellis,DTW_MIN_SCORE,NULL);

            }
            else
            {
                ve->trellis = NULL;
            }

            ve->entrance_cost = log(ge->transition_prob);
            ve->next = &nodes[ge->next_node_id];

            //Advance
            vn->edges[i] = ve;
            edge_i++;
        }
    }


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
        if(edge->trellis != NULL)
            dtw_set_incoming(edge->trellis, 0, bp);
    }

    //Create results lists
    backpointer* best = malloc(sizeof(backpointer));
    best->timestamp = 0;
    best->word = "NO RESULTS";
    best->len = strlen(best->word)+1;
    best->score = DTW_MIN_SCORE;
    best->prev = NULL;
    
    
    /* Now, we traverse the graph, one time instant per step.
     */
    for(int t = 0; t < test->num_vectors; t++)
    {
        //printf("\n\n=================================\n");

        //Reset node entry scores
        for(int i = 0; i < num_nodes; i++)
        {
            nodes[i].best_score = DTW_MIN_SCORE;
            nodes[i].best_backpointer = NULL;
            nodes[i].best_word = "";
        }


        //Score every non-NULL edge
        double column_max = DTW_MIN_SCORE;
        for(int i = 0; i < num_edges; i++)
        {
            viterbi_edge* edge = &edges[i];

            //If we have a trellis, score it
            //Otherwise, continue
            if(edge->trellis != NULL)
            {
                dtw_fill_next_col(edge->trellis);
                double up_score = edge->trellis->score;
                void* up_bp = edge->trellis->backpointer;
                //dtw_print_col(edge->trellis);

                //If we have the best score seen into this node, then update it
                //printf("score %d %f\n",i,score);
                if(up_score > DTW_MIN_SCORE && up_score > edge->next->best_score)
                {
                    up_score += edge->entrance_cost;
                    //Get the template used for its word
                    gaussian_cluster* template = edge->trellis->template_data;

                    //Update the best seen score into this node
                    edge->next->best_score = up_score;
                    edge->next->best_backpointer = up_bp;
                    edge->next->best_word = template->word_id;
                }

                //Keep pruning information
                double this_max = edge->trellis->column_max;
                if(this_max > column_max)
                    column_max = this_max;
            }
        }


        //Prune every non-NULL trellis structures
        double window = abs(column_max)*threshold;
        double abs_threshold = column_max - window;
        for(int i = 0; i < num_edges; i++)
        {
            if(edges[i].trellis != NULL)
                dtw_prune_next_column(edges[i].trellis, abs_threshold);
        }


        //If a node has NULL transitions, then we need to send its score
        //across the NULL edge now
        for(int i = 0; i < num_nodes; i++)
        {
            // For every node...
            viterbi_node* node = &nodes[i];
            for(int j = 0; j < node->num_edges; j++)
            {
                // For each NULL edge...
                viterbi_edge* e = node->edges[j];
                if(e->trellis == NULL)
                {
                    // If this edge score is better, update its best
                    double score = node->best_score + e->entrance_cost;
                    if(score > e->next->best_score)
                    {
                        e->next->best_score = score;
                        e->next->best_backpointer = node->best_backpointer;
                        e->next->best_word = node->best_word;
                    }
                }
            }
        }

        //Use the node information to seed the trellises for next round
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
            if(t == test->num_vectors-1 && node->terminal &&
                bp->score > best->score)
                best = bp;

            //Set incoming to each edge
            for(int j = 0; j < node->num_edges; j++)
            {
                if(node->edges[j]->trellis != NULL)
                    dtw_set_incoming(node->edges[j]->trellis, node->best_score,
                                     bp);
            }
        }
    }

    return best;
}
