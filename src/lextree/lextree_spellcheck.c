#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lextree_spellcheck.h"


/* TODO: Implement pruning.
 */
lextree_scored_word** lextree_closest_n_words(lextree* lex, char* word, int n)
{
    //Null prefix the word
    char test_word[64] = "*";
    strcpy(&test_word[1],word);

    //Allocate results array
    lextree_scored_word** words = calloc(n, sizeof(lextree_scored_word));

    //Create first node to search, use calloc to zero fields
    pq_node* first = calloc(1,sizeof(pq_node));
    first->cur_node = lex->head;

    pq_t pq = pq_new_queue(lex->size, compare_priority, NULL);
    pq_enqueue(pq, first, NULL);

    //Initialize current column, and the min edit distance in that column
    //for pruning purposes
    int current_col = 1;

    // What we do here is somewhat clever. Since we have lots of branching,
    // directly visualizing the stacked trellis is really fucking hard. Instead,
    // we generate the next nodes to check from the current node, and add them
    // to a priority queue. We will then check them in a given order: everything
    // in the same "column" of our "trellis", and then within that column the
    // nodes with the lowest score first
    while(pq_number_of_entries(pq) > 0)
    {
        printf("next...\n");
        //Get next node, and potentially throw it away
        printf("%d %p\n", pq_number_of_entries(pq), pq_inspect_next(pq,NULL));
        pq_node* next = pq_dequeue(pq, NULL);
        if(next == NULL)
        {
            printf("WTF\n");
            continue;
        }
        if(next->cur_word_i > strlen(test_word) ||
           next->cur_depth > lex->depth)
        {
            printf("carry on\n");
            continue;
        }

        printf("a\b");
        //If this is a full word, then add it to our results
        //We may need to delete the rest of the word to make it match
        if(next->cur_node->is_full_word)
        {
            int num_deletions = strlen(test_word) - next->cur_word_i - 1;
            lextree_add_to_result(words, n, next->word_so_far,
                next->score + num_deletions);
        }

        printf("b\b");
        //Update current column and pruning
        if(next->cur_word_i > current_col)
            current_col = next->cur_word_i;

        /* TODO: generate the next set of nodes to visit
         */
        char test_char = test_word[next->cur_word_i] - 'a';

        printf("c\b");
        //Generate substituions
        for(int i = 0; i < 26; i++)
        {
            //Only check next child if we have a child node
            if(next->cur_node->children[i] == NULL) continue;

            // What to add - either 1 point for subsitution, or none for
            // exact match
            int score = 1;
            if(test_char == i)
                score = 0;

            pq_node* new_node = malloc(sizeof(pq_node));
            new_node->cur_word_i = next->cur_word_i + 1;
            new_node->cur_depth = next->cur_depth + 1;
            new_node->insertions = next->insertions;
            new_node->deletions = next->deletions;
            new_node->substitutions = next->substitutions + score;
            new_node->score = next->score + score;
            new_node->cur_node = next->cur_node->children[i];

            //Build next word
            strcpy(new_node->word_so_far, next->word_so_far);
            new_node->word_so_far[next->cur_depth] = i+'a';
            new_node->word_so_far[next->cur_depth+1] = '\0';
        }

        printf("d\b");
        //Generate insertions
        for(int i = 0; i < 26; i++)
        {
            //Only check next child if we have a child node
            if(next->cur_node->children[i] == NULL) continue;

            pq_node* new_node = malloc(sizeof(pq_node));
            new_node->cur_word_i = next->cur_word_i;
            new_node->cur_depth = next->cur_depth + 1;
            new_node->insertions = next->insertions + 1;
            new_node->deletions = next->deletions;
            new_node->substitutions = next->substitutions;
            new_node->score = next->score + 1;
            new_node->cur_node = next->cur_node->children[i];

            //Build next word
            strcpy(new_node->word_so_far, next->word_so_far);
            new_node->word_so_far[next->cur_depth] = i+'a';
            new_node->word_so_far[next->cur_depth+1] = '\0';
        }

        printf("e\b");
        //Generate deletions
        for(int i = 0; i < 26; i++)
        {
            pq_node* new_node = malloc(sizeof(pq_node));
            new_node->cur_word_i = next->cur_word_i + 1;
            new_node->cur_depth = next->cur_depth;
            new_node->insertions = next->insertions;
            new_node->deletions = next->deletions + 1;
            new_node->substitutions = next->substitutions;
            new_node->score = next->score + 1;
            new_node->cur_node = next->cur_node;

            //Build next word
            strcpy(new_node->word_so_far, next->word_so_far);
        }

        printf("f\b");
        //Dispose of this node
        free(next);
    }

    //Clean up
    pq_dispose_queue(pq);

    return words;
}


void lextree_add_to_result(lextree_scored_word** words, int n,
                           char* word, int score)
{
    lextree_scored_word* node = malloc(sizeof(lextree_scored_word));
    strcpy(node->word, word);
    node->score = score;

    for(int i = 0; i < n; i++)
    {
        //If we see an empty slot, then put our item at the bottom
        if(words[i] == NULL)
        {
            words[i] = node;
            return;
        }

        if(score > words[i]->score)
        {
            lextree_scored_word* temp1 = node;
            lextree_scored_word* temp2 = words[i];
            for (int j = i; j < n; j++)
            {
                words[j] = temp1;
                temp1 = temp2;
                if (j+1 < n) temp2 = words[j+1];
            }
            return;
        }
    }
}


bool compare_priority(pq_data_t left_in, pq_data_t right_in)
{
    pq_node* left = (pq_node*) left_in;
    pq_node* right = (pq_node*) right_in;

    /* We want to the same column in each "trellis" in a row
     * Do this by first priortizing the word index.
     *
     * Otherwise prioritize by lowest score so far
     */
    if(left->cur_word_i < right->cur_word_i)
        return true;
    else if(right->cur_word_i > left->cur_word_i)
        return false;
    else
        return left->score <= right->score;
}


void lextree_print_n_best(lextree_scored_word** words, int n)
{
    printf("\nBest %d words found:\n", n);
    for(int i = 0; i < n; i++)
    {
        //If we ran out of words...
        if(words[i] == NULL)
            return;

        printf("\t%s (score %d)\n", words[i]->word, words[i]->score);
    }
    printf("\n");
}


void lextree_free_n_best(lextree_scored_word** words, int n)
{
    for(int i = 0; i < n; i++)
    {
        //If we ran out of words...
        if(words[i] == NULL)
            break;
        
        free(words[i]);
    }
    free(words);
}
