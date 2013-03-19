#include "lexqueue_utils.h"

int queue_size(lexqueue * queue){
	return queue->size;
}

void print_queue(lexqueue * queue){
	printf("======QUEUE=====\n");
	lexqueue_node * temp = queue->head;
	printf("Size: %d\n",queue->size);
	while(temp){
		print_queue_node(temp);
		temp = temp->next;
	}

}

void print_queue_node(lexqueue_node * queue_node){
	printf("%c\n",queue_node->tree_node->c);

/*	printf("Children:\n");
	for(int i=0; i<26; i++){
		if(queue_node->tree_node->children[i]!=NULL){
			printf("%c",queue_node->tree_node->children[i]->c);
		}
	}
*/
	printf("\n");
}

lexqueue * init_queue(){
	lexqueue * queue = (lexqueue *) malloc(sizeof(lexqueue));
	queue->size = 0;
	queue->head = NULL;
	return queue;
}


lexqueue_node * pop_front(lexqueue * queue){
	if(queue->size==0){
		return NULL;
	}
	else if(queue->size==1){
		lexqueue_node * last_node = queue->head;
		queue->head=NULL;
		queue->size--;
		return last_node;
	}
	else{
		lexqueue_node * popped = queue->head;
		queue->head = queue->head->next;
		queue->size--;
		return popped;
	}
}
/*
void push(lexqueue * queue , lextree_node * node){
	lexqueue_node * new_node = init_lexqueue_node(node);
	append_node(queue,new_node);
}
*/

void push_front(lexqueue * queue,lexqueue_node * node){
	lexqueue_node * temp = queue->head;
        if(!temp){
                queue->head = node;
        }
	else{
		node->next = queue->head;
		queue->head = node;
	}
}

void push_back(lexqueue * queue , lexqueue_node * node){
	lexqueue_node * temp = queue->head;
	if(!temp){
		queue->head = node;
	}
	else{	
		while(temp->next){
			temp = temp->next;
		}
		temp->next = node;
		node->next = NULL;
	}
	queue->size ++;
}
/*
lexqueue_node * init_lexqueue_node(lextree_node * tree_node){
	lexqueue_node * node = (lexqueue_node *) malloc(sizeof(lexqueue_node));
	node->tree_node = tree_node;
	node->score = 0;
	node->substring[0] = tree_node->c;
	node->index = 1;
	node->next = NULL;
	return node;
}
*/


