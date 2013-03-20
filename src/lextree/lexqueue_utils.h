#ifndef LEXQUEUE_H
#define LEXQUEUE_H

#include "lextree.h"


/* lexqueue_node is a container struct used by the spellcheck search. It
 *         contains the nessecary state information to determine our location
 *         in the word we are checking, and in the lextree.
 *
 *         Also stores the number of kind of edits seen in that word up to this
 *         point, and the word we have built so far.
 *
 *         Additionally holds a pointer to the next element in our queue
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
    lexqueue_node *tail;
	int size;
} lexqueue;


/* init_queue - allocates an empty lexqueue
 */
lexqueue* init_queue();


/* queue_size - given a lexqueue, returns the size of that queue
 */
int queue_size(lexqueue* queue);


/* pop_front - given a lexqueue, removes the front node from the queue and
 *             returns it
 */
lexqueue_node* pop_front(lexqueue* queue);


/* push_* - given a lexqueue and a new node, place this node at either the front
 *            or back of the queue
 */
void push_back(lexqueue* queue, lexqueue_node* node);
void push_front(lexqueue* queue, lexqueue_node* node);


/* print_* - prints a human readable form of their argument
 */
void print_queue(lexqueue* queue);
void print_queue_node(lexqueue_node* queue_node);

#endif
