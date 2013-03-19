#ifndef LEXQUEUE_H
#define LEXQUEUE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lextree.h"

typedef struct lexqueue_node{
	lextree_node * tree_node;
	char substring[64];
	int index;
	int score;
	int depth;

	int insertions;
	int deletions;
	int substitutions;

	struct lexqueue_node * next;
}lexqueue_node;

typedef struct lexqueue{
	lexqueue_node *head;
	int size;
}lexqueue;

lexqueue * init_queue();
int queue_size(lexqueue * queue);
//void push(lexqueue * queue , lextree_node * node);
lexqueue_node * pop_front(lexqueue * queue);
void push_back(lexqueue * queue , lexqueue_node * node);
void push_front(lexqueue * queue,lexqueue_node * node);
lexqueue_node * init_lexqueue_node(lextree_node * tree_node);
void print_queue(lexqueue * queue);
void print_queue_node(lexqueue_node * queue_node);

#endif
