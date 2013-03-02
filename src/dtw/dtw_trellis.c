#include <stdlib.h>
#include <stdio.h>
#include "dtw_trellis.h"


dtw_t* new_dtw(void* test_data,     int test_length,
               void* template_data, int template_length,
               dtw_prune_t prune,   double pruning_threshold,
               double (*scorer)(void*,void*,int,int,dtw_trellis_dir))
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
    dtw->pruning_threshold = pruning_threshold;
    dtw->scorer = scorer;
    dtw->last_column_i = 0;
    
    //Allocate columns for scoring
    dtw->last_col = malloc(template_length*sizeof(dtw_trellis_node));
    dtw->next_col = malloc(template_length*sizeof(dtw_trellis_node));
    for(int i = 0; i < template_length; i++)
    {
        //Score the first column with no travel direction
        dtw->last_col[i].score =
            scorer(test_data, template_data, i, 0, DTW_DIR_NONE);
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
    double column_min = DTW_MAX_SCORE;
    double column_max = DTW_MIN_SCORE;
    for(int i = 0; i < dtw->template_length; i++)
    {
        //If the node has been pruned, carry on
        if(dtw->next_col[i].pruned) continue;

        //Score this node
        double score = dtw_score_node(dtw,i);

        //Keep track of the beam threshold
        if(score < column_min)
            column_min = score;
        if(score > column_max)
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
        double window = abs(column_max) * dtw->pruning_threshold;
        double threshold = column_max - window;
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
    dtw->score = dtw->last_col[dtw->template_length-1].score;
    dtw->last_column_i++;
    //Return whether we scored the last column
    return dtw->last_column_i >= dtw->test_length;
}


double dtw_score_node(dtw_t* dtw, int row)
{
    int col = dtw->last_column_i+1;
    double left = DTW_MIN_SCORE;
    double downone = DTW_MIN_SCORE;
    double downtwo = DTW_MIN_SCORE;
    //Directly left
    left = dtw->scorer(dtw->test_data, dtw->template_data,
        row, col, DTW_DIR_LEFT) + dtw->last_col[row].score;
    //Left and down 1
    if(row > 0)
    {
        downone = dtw->scorer(dtw->test_data, dtw->template_data,
            row-1, col, DTW_DIR_DOWNONE) + dtw->last_col[row-1].score;
    }
    //Left and down 2
    if(row > 1)
    {
        downtwo = dtw->scorer(dtw->test_data, dtw->template_data,
            row-2, col, DTW_DIR_DOWNTWO) + dtw->last_col[row-2].score;
    }

    //Determine the highest score and use that
    double score = DTW_MIN_SCORE;
    dtw_trellis_dir dir = DTW_DIR_NONE;
    if(left > downone && left > downtwo && left > score)
    {
        score = left;
        dir = DTW_DIR_LEFT;
    }
    else if(downone > downtwo && downone > score)
    {
        score = downone;
        dir = DTW_DIR_DOWNONE;
    }
    else if(downtwo > score)
    {
        score = downtwo;
        dir = DTW_DIR_DOWNTWO;
    }

    //Store info back and finish
    dtw->next_col[row].score = score;
    dtw->next_col[row].dir = dir;
    return score;
}


void dtw_print_struct(dtw_t* dtw)
{
    printf("Scoring DTW with (test: %p, len:%d) and (template:%p, len:%d)\n",
           dtw->test_data, dtw->test_length,
           dtw->template_data, dtw->template_length);

    char* pruning;
    if(dtw->pruning == DTW_NO_PRUNE)
        pruning = "NONE";
    else
        pruning = "BEAM";
    printf("Using pruning method: %s, threshold: %1.4lf\n", pruning,
           dtw->pruning_threshold);
    printf("So far, we have scored up to time t=%d, ", dtw->last_column_i);
    printf("with a best score of: %1.4f\n\n", dtw->score);
}


void dtw_print_col(dtw_t* dtw)
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
        printf("%s%1.4f    ",dir,score);
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
