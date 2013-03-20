#ifndef LEXQUEUE_H
#define LEXQUEUE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lextree.h"


/* lexqueue_node is a container struct used by the spellcheck search. It
 *         contains the nessecary state information to determine our location
 *         in the word we are checking, and in the lextree.
 *
 *         Also stores the number of kind of edits seen in that word up to this
 *         point, and the word we have built so far.
 */
typedef struct lexqueue_node {
	int index;
	int depth;

	int insertions;
	int deletions;
	int substitutions;
	int score;

	lextree_node * tree_node;
	char substring[64];

	struct lexqueue_node* next;
} lexqueue_node;


typedef struct lexqueue {
	lexqueue_node *head;
	int size;
} lexqueue;

lexqueue* init_queue();
int queue_size(lexqueue* queue);
//void push(lexqueue * queue , lextree_node * node);
lexqueue_node* pop_front(lexqueue* queue);
void push_back(lexqueue* queue, lexqueue_node* node);
void push_front(lexqueue* queue, lexqueue_node* node);
lexqueue_node* init_lexqueue_node(lextree_node* tree_node);
void print_queue(lexqueue* queue);
void print_queue_node(lexqueue_node* queue_node);

#endif
