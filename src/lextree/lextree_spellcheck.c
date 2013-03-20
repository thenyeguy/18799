#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lextree_spellcheck.h"
#include "lexqueue_utils.h"


lextree_scored_word** lextree_closest_n_words(lextree* lex, char* word, int n,
                                              bool segment)
{
	//Null prefix the word
	char test_word[LT_WORD_LENGTH];// = "*";
	strcpy(&test_word[0],word);

	//Allocate results array
	lextree_scored_word** words = calloc(n, sizeof(lextree_scored_word));

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
        printf("%d %d %s\n", next->index, next->score, next->substring);

        seen++;
        //Toss this node if we have passed the last column
        if(next->index > strlen(word))
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
            printf("%d/%d %d\n", pruned, seen, q->size);
        }

        //Update pruning for next column
        if(next->score < min_edit_distance)
            min_edit_distance = next->score;

        //Toss this node if it exceeds the pruning threshold
        if(min_edit_distance+LEXTREE_CLOSEST_PRUNING_THRESHOLD > 0 &&
           next->score > min_edit_distance + LEXTREE_CLOSEST_PRUNING_THRESHOLD)
        {
            pruned++;
            free(next);
            continue;
        }



        //printf("%d %s %d\n", next->index, next->substring, next->score);
        //Check if we reached a terminal node
		if(next->tree_node->is_full_word && next->index == strlen(word))
		{
			lextree_add_to_result(words, n, next->substring,next->score);
            free(next);
			continue;
		}

        //Handle reaching the end of a word in the lextree
        if(next->tree_node->is_full_word)
        {
            if(segment)
            {
                // If we are segmenting, allow recursion
                // Slap on a space and loop back to the head of the lextree
                lexqueue_node* temp = next;
                next = calloc(1,sizeof(lexqueue_node));
                next->tree_node = lex->head;
                next->index = temp->index;
                next->depth = temp->depth + 1;
                next->insertions = temp->insertions + 1;
                next->deletions = temp->deletions;
                next->substitutions = temp->substitutions;
                next->score = temp->score+1;
                
                // insert a space to the new "next" node
                strcpy(next->substring, temp->substring);
                next->substring[temp->depth] = ' ';
                next->substring[temp->depth+1] = '\0';

                push_back(q,next);
                free(temp);
            }
        }


        //Generate children traversal now
		char test_char = test_word[next->index] - 'a';


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

		printf("\t%s:\t%d\n", words[i]->word, words[i]->score);
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
