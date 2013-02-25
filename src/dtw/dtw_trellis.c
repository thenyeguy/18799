#include <stdlib.h>
#include <stdio.h>
#include "dtw_trellis.h"


dtw_t* new_dtw(void* template_data, int template_length,
               void* test_data,     int test_length,
               dtw_prune_t prune, double (*scorer)(void*,void*,dtw_trellis_dir))
{
    //Allocate dtw
    dtw_t* dtw = malloc(sizeof(dtw_t));

    //Store data info
    dtw->template_data = template_data;
    dtw->test_data = test_data;
    dtw->template_length = template_length;
    dtw->test_length = test_length;

    //Store evaluation info
    dtw->pruning = prune;
    dtw->scorer = scorer;
    dtw->last_column_i = 0;
    
    //Allocate columns for scoring
    dtw->last_col = malloc(template_length*sizeof(dtw_trellis_node));
    dtw->next_col = malloc(template_length*sizeof(dtw_trellis_node));
    for(int i = 0; i < template_length; i++)
    {
        //Score the first column with no travel direction
        dtw->last_col[i].score =
            scorer(test_data, template_data+i, DTW_DIR_NONE);
        dtw->last_col[i].pruned = false;
        dtw->last_col[i].dir = DTW_DIR_NONE;

        //Don't start pruning until next iteration
        dtw->next_col[i].score = DTW_MIN_SCORE;
        dtw->next_col[i].pruned = false;
    }
    return dtw;
}


bool dtw_fill_next_col(dtw_t* dtw)
{
    //If we have searched the whole trellis, return
    if(dtw->last_column_i >= dtw->test_length)
        return true;

    //Score entire column
    int column_max = DTW_MIN_SCORE;
    for(int i = 0; i < dtw->template_length; i++)
    {
        //If the node has been pruned, carry on
        if(dtw->next_col[i].pruned) continue;

        //Score this node
        double score = dtw_score_node(dtw,i);

        //Keep track of the beam threshold
        if(dtw->pruning == DTW_BEAM_PRUNE && score > column_max)
            column_max = score;
    }

    //Once we have scored the next column, it is the last column scored,
    //so swap it and reuse the memory allocated for the last column to store
    //pruning info for the next column
    dtw_trellis_node* temp = dtw->last_col;
    dtw->last_col = dtw->next_col;
    dtw->next_col = temp;

    //Perform pruning if enabled
    if(dtw->pruning == DTW_BEAM_PRUNE)
    {
        double threshold = column_max / DTW_THRESHOLD;
        for(int i = 0; i < dtw->template_length; i++)
        {
            bool check = false;
            //Left
            if(dtw->last_col[i].score > threshold)
                check = true;
            //Left and down one
            if(i > 0 && dtw->last_col[i-1].score > threshold)
                check = true;
            //Left and down two
            if(i > 1 && dtw->last_col[i-2].score > threshold)
                check = true;

            //Fill node
            dtw->next_col[i].score = DTW_MIN_SCORE;
            dtw->next_col[i].pruned = !check;
            dtw->next_col[i].dir = DTW_DIR_NONE;
        }
    }
    //Otherwise just blindly fill new column
    else
    {
        for(int i = 0; i < dtw->template_length; i++)
        {
            //Fill node
            dtw->next_col[i].score = DTW_MIN_SCORE;
            dtw->next_col[i].pruned = false;
            dtw->next_col[i].dir = DTW_DIR_NONE;
        }
    }

    //Store best score so far and update column count
    dtw->score = dtw->last_col[dtw->template_length].score;
    dtw->last_column_i++;
    //Return whether we scored the last column
    return dtw->last_column_i >= dtw->test_length;
}


double dtw_score_node(dtw_t* dtw, int n)
{
    double left, downone, downtwo;
    //Directly left
    left =
        dtw->scorer((dtw->test_data+n), (dtw->template_data+n), DTW_DIR_LEFT);
    //Left and down 1
    if(n > 0)
    {
        downone = dtw->scorer((dtw->test_data+n), (dtw->template_data+n-1),
            DTW_DIR_DOWNONE);
    }
    //Left and down 2
    if(n > 1)
    {
        downtwo = dtw->scorer((dtw->test_data+n), (dtw->template_data+n-2),
            DTW_DIR_DOWNTWO);
    }

    //Determine the highest score and use that
    double score = DTW_MIN_SCORE;
    dtw_trellis_dir dir = DTW_DIR_NONE;
    if(left > downone && left > downtwo)
    {
        score = left;
        dir = DTW_DIR_LEFT;
    }
    else if(downone > downtwo)
    {
        score = downone;
        dir = DTW_DIR_DOWNONE;
    }
    else
    {
        score = downtwo;
        dir = DTW_DIR_DOWNTWO;
    }

    //Store info back and finish
    dtw->next_col[n].score = score;
    dtw->next_col[n].dir = dir;
    return score;
}


void print_dtw_trellis(dtw_t* dtw)
{
    printf("Scoring DTW with (template:%p, len:%d) and (test: %p, len:%d)\n",
           dtw->template_data, dtw->template_length,
           dtw->test_data, dtw->test_length);

    char* pruning;
    if(dtw->pruning == DTW_NO_PRUNE)
        pruning = "none";
    else
        pruning = "beam";
    printf("Using pruning method: %s\n", pruning);
    printf("So far, we have scored up to time t=%d, ", dtw->last_column_i);
    printf("with a best score of: %1.4f\n\n", dtw->score);
}


void print_dtw_col(dtw_t* dtw)
{
    printf("Time t=%d  |  ",dtw->last_column_i);
    for(int i = 0; i<dtw->template_length; i++)
    {
        char* dir;
        switch(dtw->last_col[i].dir)
        {
            case DTW_DIR_NONE:
                dir = "xx ";
                break;
            case DTW_DIR_LEFT:
                dir = "-- ";
                break;
            case DTW_DIR_DOWNONE:
                dir = "/  ";
                break;
            case DTW_DIR_DOWNTWO:
                dir = "// ";
                break;
        }
        double score = dtw->last_col[i].score;
        if(score == DTW_MIN_SCORE) score = -1.0;
        printf("%s%1.4f\t",dir,score);
    }
    printf("\n");
}


void free_dtw(dtw_t* dtw)
{
    //Free the nodes of each col
    for(int i = 0; i < dtw->template_length; i++)
    {
        free(dtw->last_col+i);
        free(dtw->next_col+i);
    }
    //Free the cols and struct itself
    free(dtw->last_col);
    free(dtw->next_col);
    free(dtw);
}
