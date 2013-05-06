#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lextree_spellcheck.h"

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
    // Create a new node and copy the lextree structure into it
    lextree_spellcheck_node* result = calloc(1,sizeof(lextree_spellcheck_node));
    result->c = l->c;
    result->is_full_word = l->is_full_word;

    // Prep evaluation state
    result->col = 0;

    // We start off with an empty word, and a score high enough to be "infinite"
    // but low enough it doesn't wrap around to negatives
    result->word_length = 0;
    strcpy(result->word,"");
    result->edit_distance = MAXINT/2;

    result->last_word_length = 0;
    strcpy(result->last_word,"");
    result->last_edit_distance = MAXINT/2;
    
    // Recursively create our children
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
    // Set up self insertion - an insertion is from the last column and in the
    // same row
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

        // A substitution is from the last column and down one row
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

        // A deletion is from the current column and down one row
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

    // Delete into head if we can! Inserts a space
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
}


void prep_lextree_spellcheck_column(lextree_spellcheck_node* n)
{
    // Copy current col into last col
    n->last_edit_distance = n->edit_distance;
    strcpy(n->last_word,n->word);
    n->last_word_length = n->word_length;

    // Resettle current col to initial state
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

    // If we are a candidate, initialize backpointer
    if(n->is_full_word)
        backpointer = n;

    // Search our children
    for(int i = 0; i < 26; i++)
    {
        if(n->children[i] == NULL) continue;

        // If there is no backpointer yet, just take the new one for free
        if(backpointer == NULL)
        {
            backpointer = lextree_best_backpointer(n->children[i]);
        }
        else
        {
            // Otherwise, see if the new result is better than our current one
            lextree_spellcheck_node* candidate =
                lextree_best_backpointer(n->children[i]);
            if(candidate == NULL) continue;

            if(candidate->last_edit_distance < backpointer->last_edit_distance)
                backpointer = candidate;
        }
    }

    // Returns whatever was best
    return backpointer;
}
