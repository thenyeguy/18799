#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lextree_spellcheck.h"
#include "lexqueue_utils.h"

char* lextree_closest(lextree* lex, char* string, bool segment)
{
    /* Parse grammar into scoring structure
     */
    lextree_spellcheck_node* head = get_lextree_eval_struct(lex->head, NULL);

    // Prep entry
    head->edit_distance = MAXINT/2;
    strcpy(head->word, "");
    head->word_length = 0;

    head->last_edit_distance = 0;
    strcpy(head->last_word,"");
    head->last_word_length = 0;


    /* Score one column at a time
     */
    for(int i = 0; i < strlen(string); i++)
    {
        // Score a column
        score_lextree_spellcheck_column(head, string, head, segment);

        //Push current column to last column
        prep_lextree_spellcheck_column(head);
    }


    /* Find best scoring word
     */
    lextree_spellcheck_node* backpointer = lextree_best_backpointer(head);
    char* result = backpointer->last_word; 
    printf("Distance: %d\n", backpointer->last_edit_distance);

    return result;
}


lextree_spellcheck_node* get_lextree_eval_struct(lextree_node* l,
    lextree_spellcheck_node* parent)
{
    lextree_spellcheck_node* result = calloc(1,sizeof(lextree_spellcheck_node));
    result->c = l->c;
    result->is_full_word = l->is_full_word;

    result->col = 0;

    result->word_length = 0;
    strcpy(result->word,"");
    result->edit_distance = MAXINT/2;

    result->last_word_length = 0;
    strcpy(result->last_word,"");
    result->last_edit_distance = MAXINT/2;
    
    for(int i = 0; i < 26; i++)
    {
        if(l->children[i] == NULL)
            result->children[i] = NULL;
        else
            result->children[i] = get_lextree_eval_struct(l->children[i], result);
    }
    result->parent = parent;

    return result;
}


void score_lextree_spellcheck_column(lextree_spellcheck_node* n, char* string,
    lextree_spellcheck_node* head, bool segment)
{
    // Set up self insertion
    if(n->last_edit_distance + 1 < n->edit_distance)
    {
        n->edit_distance = n->last_edit_distance+1;
        strcpy(n->word, n->last_word);
        n->word_length = n->last_word_length;
    }

    // Inject substitutions/deletions
    for(int i = 0; i < 26; i++)
    {
        lextree_spellcheck_node* c = n->children[i];
        if(c == NULL) continue;

        int sub_distance = n->last_edit_distance;
        if(c->c != string[n->col]) sub_distance++;
        if(sub_distance < c->edit_distance)
        {
            c->edit_distance = sub_distance;
            strcpy(c->word,n->last_word);
            c->word[n->last_word_length] = c->c;
            c->word[n->last_word_length+1] = '\0';
            c->word_length = n->last_word_length+1;
        }

        int del_distance = n->edit_distance + 1;
        if(del_distance < c->edit_distance)
        {
            c->edit_distance = del_distance;
            strcpy(c->word,n->word);
            c->word[n->word_length] = c->c;
            c->word[n->word_length+1] = '\0';
            c->word_length = n->word_length+1;
        }
    }

    // Delete into head if we can!
    if(segment && n->is_full_word)
    {
        int del_distance = n->edit_distance + 1;
        if(del_distance < head->edit_distance)
        {
            head->edit_distance = del_distance;
            strcpy(head->word,n->last_word);
            head->word[n->word_length] = head->c;
            head->word[n->word_length+1] = '\0';
            head->word_length = n->word_length+1;

            if(strcmp(head->word, "once upon time while brahmadatta asking of benares the bodhisatta cameo life at the foot onthe himalayas as monkey he grew strong and sturdy big of frame well do") == 0)
                printf("wat '%c', '%c', '%c', '%c', %d\n",
                    head->word[n->word_length], head->word[n->word_length-1],
                    n->c, n->word[n->word_length-1],
                    head->word_length);
        }
    }

    // Go to next col
    n->col++;

    // Score children
    for(int i = 0; i < 26; i++)
    {
        if(n->children[i] == NULL) continue;
        score_lextree_spellcheck_column(n->children[i], string, head, segment);
    }

    if(n->parent == NULL)
    {
    }
}


void prep_lextree_spellcheck_column(lextree_spellcheck_node* n)
{
    n->last_edit_distance = n->edit_distance;
    strcpy(n->last_word,n->word);
    n->last_word_length = n->word_length;

    n->edit_distance = MAXINT/2;
    strcpy(n->word,"");
    n->word_length = 0;

    // Set up children
    for(int i = 0; i < 26; i++)
    {
        if(n->children[i] == NULL) continue;
        prep_lextree_spellcheck_column(n->children[i]);
    }
}


lextree_spellcheck_node* lextree_best_backpointer(lextree_spellcheck_node* n)
{
    lextree_spellcheck_node* backpointer = NULL;

    // If we are a candidate...
    if(n->is_full_word)
        backpointer = n;

    // Our children?
    for(int i = 0; i < 26; i++)
    {
        if(n->children[i] == NULL) continue;

        if(backpointer == NULL)
        {
            backpointer = lextree_best_backpointer(n->children[i]);
        }
        else
        {
            lextree_spellcheck_node* candidate =
                lextree_best_backpointer(n->children[i]);
            if(candidate == NULL) continue;

            if(candidate->last_edit_distance < backpointer->last_edit_distance)
                backpointer = candidate;
        }
    }

    return backpointer;
}












lextree_scored_string** lextree_closest_n_orig(lextree* lex, char* string, int n,
                                          bool segment)
{
    //Allocate results array
    lextree_scored_string** strings = calloc(n, sizeof(lextree_scored_string));

    //Create first node to search, use calloc to zero fields
    lexqueue_node* first = calloc(1,sizeof(lexqueue_node));
    first->tree_node = lex->head;

    //Create queue to keep track of nodes
    lexqueue* q = init_queue();
    push_back(q,first);

    //Initialize current column, and the min edit distance in that column
    //for pruning purposes
    int current_col = 1;
    int min_edit_distance = MAXINT; 

    int seen = 0;
    int pruned = 0;

    // What we do here is somewhat clever. Since we have lots of branching,
    // directly visualizing the stacked trellis is really fucking hard. Instead,
    // we generate the next nodes to check from the current node, and add them
    // to a priority queue. We will then check them in a given order: everything
    // in the same "column" of our "trellis", so as to prune correctly
    while(queue_size(q) > 0)
    {
        //Get next node
        lexqueue_node* next = pop_front(q);
        seen++;


        //Toss this node if we have passed the last column
        if(next->index > strlen(string))
        {
            pruned++;
            free(next);
            continue;
        }


        //Update pruning information
        //If we have a new column, reset the pruning
        if(current_col < next->index)
        {
            current_col = next->index;
            min_edit_distance++;
            printf("Column %d, threshold %d. Pruned %d/%d, %d more.\n",
                   current_col, min_edit_distance, pruned, seen, q->size);
        }

        //Update pruning for next column
        if(next->score < min_edit_distance)
            min_edit_distance = next->score;

        //Toss this node if it exceeds the pruning threshold
        if(next->score > min_edit_distance + LEXTREE_CLOSEST_PRUNING_THRESHOLD)
        {
            pruned++;
            free(next);
            continue;
        }


        //Check if we reached a terminal node
        if(next->tree_node->is_full_word && next->index == strlen(string))
        {
            lextree_add_to_result(strings, n, next->substring,next->score);
            free(next);
            continue;
        }


        //Handle reaching the end of a word in the lextree
        if(next->tree_node->is_full_word && segment)
        {
            // If we are segmenting, allow recursion
            // Slap on a space and loop back to the head of the lextree
            lexqueue_node* new_node = calloc(1,sizeof(lexqueue_node));
            new_node->tree_node = lex->head;
            new_node->index = next->index;
            new_node->depth = next->depth + 1;
            new_node->insertions = next->insertions;
            new_node->deletions = next->deletions + 1;
            new_node->substitutions = next->substitutions;
            new_node->score = next->score+1;

            // If the next char is a space, consume instead of deleting
            if(string[next->index] == ' ')
            {
                new_node->index++;
                new_node->deletions--;
                new_node->score--;
            }

            // insert a space to the new "new_node" node
            strcpy(new_node->substring, next->substring);
            new_node->substring[next->depth] = ' ';
            new_node->substring[next->depth+1] = '\0';

            push_front(q,new_node);
        }


        //Generate children traversal now
        char test_char = string[next->index] - 'a';

        //Generate substituions
        for(int i = 0; i < 26; i++)
        {
            //Only check next child if we have a child node
            if(next->tree_node->children[i] == NULL) continue;

            // What to add - either 1 point for subsitution, or none for
            // exact match
            int score = 1;
            if(test_char == i)
                score = 0;

            lexqueue_node* new_node = malloc(sizeof(lexqueue_node));
            new_node->index = next->index + 1;
            new_node->depth = next->depth + 1;
            new_node->insertions = next->insertions;
            new_node->deletions = next->deletions;
            new_node->substitutions = next->substitutions + score;
            new_node->score = next->score + score;
            new_node->tree_node = next->tree_node->children[i];
            new_node->next = NULL;

            //Build next word
            strcpy(new_node->substring, next->substring);
            new_node->substring[next->depth] = i+'a';
            new_node->substring[next->depth+1] = '\0';

            //Push to queue
            push_back(q,new_node);
        }

        //Generate insertion
        /// no need to check child nodes, since we're not moving further
        /// in the lextree
        lexqueue_node* new_node = malloc(sizeof(lexqueue_node));
        new_node->index = next->index + 1;
        new_node->depth = next->depth;
        new_node->insertions = next->insertions + 1;
        new_node->deletions = next->deletions;
        new_node->substitutions = next->substitutions;
        new_node->score = next->score + 1;
        new_node->tree_node = next->tree_node;
        new_node->next = NULL;

        //Build next word
        strcpy(new_node->substring, next->substring);

        //push to queue
        push_back(q,new_node);

        //Generate deletions
        for(int i = 0; i < 26; i++)
        {
            //Only check next child if we have a child node
            if(next->tree_node->children[i] == NULL) continue;

            lexqueue_node* new_node = malloc(sizeof(lexqueue_node));
            new_node->index = next->index;
            new_node->depth = next->depth + 1;
            new_node->insertions = next->insertions;
            new_node->deletions = next->deletions + 1;
            new_node->substitutions = next->substitutions;
            new_node->score = next->score + 1;
            new_node->tree_node = next->tree_node->children[i];
            new_node->next = NULL;		

            //Build next word
            strcpy(new_node->substring, next->substring);
            new_node->substring[next->depth] = i+'a';
            new_node->substring[next->depth+1] = '\0';

            //push to front of queue
            push_front(q,new_node);
        }


        //Dispose of this node
        free(next);
    }

    return strings;
}


void lextree_add_to_result(lextree_scored_string** strings, int n,
                           char* string, int score)
{
    int index = already_in_nbest(strings, n, string);

    if (-1 != index) {
        /// word is already in the list
        /// if the instance in the list has a higher cost, replace it with this one
        if (strings[index]->score > score)
        {
            lextree_scored_string* node = strings[index];
            while (index > 0 && strings[index-1]->score > score)
            {
                strings[index] = strings[index-1];
                index--;
            }
            strings[index] = node;
        }

        return;
    }

    for(int i = 0; i < n; i++)
    {
        //If we see an empty slot, then put our item at the bottom
        if(strings[i] == NULL)
        {
            lextree_scored_string* node = malloc(sizeof(lextree_scored_string));
            strcpy(node->string, string);
            node->score = score;

            strings[i] = node;
            return;
        }

        if(score < strings[i]->score)
        {
            lextree_scored_string* node = malloc(sizeof(lextree_scored_string));
            strcpy(node->string, string);
            node->score = score;

            lextree_scored_string* temp1 = node;
            lextree_scored_string* temp2 = strings[i];
            for (int j = i; j < n; j++)
            {
                strings[j] = temp1;
                temp1 = temp2;
                if (j+1 < n) temp2 = strings[j+1];
            }

            free(temp2);
            return;
        }
    }
}

int already_in_nbest(lextree_scored_string** strings, int n, char* string)
{
    for (int i = 0; i < n; i++)
    {
        /// reached the end of the list
        if (!strings[i])
            break;

        if (!strcmp(strings[i]->string, string))
            return i;
    }
    return -1;
}


void lextree_print_n_best(lextree_scored_string** strings, int n)
{
    printf("\nPrinting up to %d best matches and their edit distance:\n", n);
    for(int i = 0; i < n; i++)
    {
        //If we ran out of words...
        if(strings[i] == NULL)
            return;

        printf("    %s  (%d)\n", strings[i]->string, strings[i]->score);
    }
    printf("\n");
}


void lextree_free_n_best(lextree_scored_string** strings, int n)
{
    for(int i = 0; i < n; i++)
    {
        //If we ran out of words...
        if(strings[i] == NULL)
            break;

        free(strings[i]);
    }
    free(strings);
}
