#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lextree_spellcheck.h"
#include "lexqueue_utils.h"



lextree_scored_word** lextree_closest_n_words(lextree* lex, char* word, int n)
{
    //Null prefix the word
    char test_word[LT_WORD_LENGTH] = "*";
    strcpy(&test_word[0],word);

    //Allocate results array
    lextree_scored_word** words = calloc(n, sizeof(lextree_scored_word));

    //Create first node to search, use calloc to zero fields
    lexqueue_node* first = calloc(1,sizeof(lexqueue_node));
    first->tree_node = lex->head;

    lexqueue * q = init_queue();
    push_back(q,first);

    //Initialize current column, and the min edit distance in that column
    //for pruning purposes
    int current_col = 1;
    
    // What we do here is somewhat clever. Since we have lots of branching,
    // directly visualizing the stacked trellis is really fucking hard. Instead,
    // we generate the next nodes to check from the current node, and add them
    // to a priority queue. We will then check them in a given order: everything
    // in the same "column" of our "trellis", and then within that column the
    // nodes with the lowest score first
    
    while(queue_size(q) > 0)
    {
        //Get next node, and potentially throw it away
        lexqueue_node* next = pop_front(q);

	// TODO: figure out a good pruning threshold, right now it's 2
	if(next->score>LEXTREE_CLOSEST_PRUNING_THRESHOLD){
		free(next);
		continue;
	}

    if(next->index > strlen(test_word) ||
        next->depth > lex->depth)
    {
        continue;
    }
	
    printf("a\b");
    //If this is a full word, then add it to our results
    //We may need to delete the rest of the word to make it match
    if(next->tree_node->is_full_word)
    {
	//Removed the -1 from num_deletions to get correct distances
        int num_deletions = strlen(test_word) - next->index;
	if(next->score+num_deletions<=LEXTREE_CLOSEST_PRUNING_THRESHOLD){
        	lextree_add_to_result(words, n, next->substring,
            	next->score + num_deletions);
	}
    }
	
        printf("b\b");
        //Update current column and pruning
        if(next->index > current_col)
            current_col = next->index;

        char test_char = test_word[next->index] - 'a';

        printf("c\b");
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
	    
            //Build next word
            strcpy(new_node->substring, next->substring);
            new_node->substring[next->depth] = i+'a';
            new_node->substring[next->depth+1] = '\0';

	    //Push to queue
	    push_back(q,new_node);
        }
	
        printf("d\b");

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


        //Build next word
        strcpy(new_node->substring, next->substring);

        //push to queue
	push_back(q,new_node);

	
        printf("e\b");
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

	    //Build next word
            strcpy(new_node->substring, next->substring);
       	    new_node->substring[next->depth] = i+'a';
            new_node->substring[next->depth+1] = '\0';

	    	//push to front of queue
	    	push_front(q,new_node);
        }

        printf("f\b");

        //Dispose of this node
        free(next);
    }

    return words;
}


void lextree_add_to_result(lextree_scored_word** words, int n,
                           char* word, int score)
{
    lextree_scored_word* node = malloc(sizeof(lextree_scored_word));
    strcpy(node->word, word);
    node->score = score;
	
	int index = already_in_nbest(words, n, word);

	if (-1 != index) {
		/// word is already in the list
		/// if the instance in the list has a higher cost, replace it with this one
		if (words[index]->score > score) {
			while (index > 0 && words[index-1]->score > score) {
				words[index] = words[index-1];
				index--;
			}
			words[index] = node;
		}

		return;
	}

    for(int i = 0; i < n; i++)
    {
        //If we see an empty slot, then put our item at the bottom
        if(words[i] == NULL)
        {
            words[i] = node;
            return;
        }

        if(score < words[i]->score)
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

int already_in_nbest(lextree_scored_word** words, int n, char* word) {
	for (int i = 0; i < n; i++) {
		if (!words[i])
			/// reached the end of the list
			break;

		if (!strcmp(words[i]->word, word)) {
			return i;
		}
	}
	return -1;
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
