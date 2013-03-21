#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lexqueue_utils.h"


lexqueue* init_queue()
{
    lexqueue* queue = (lexqueue *) malloc(sizeof(lexqueue));
    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}


int queue_size(lexqueue* queue)
{
    return queue->size;
}


lexqueue_node* pop_front(lexqueue* queue)
{
    lexqueue_node* popped = queue->head;
    if(popped)
    {
        queue->head = popped->next;
        queue->size--;
    }
    return popped;
}


void push_front(lexqueue* queue, lexqueue_node* node)
{
    lexqueue_node* temp = queue->head;
    if(!temp)
    {
        queue->head = node;
        queue->tail = node;
    }
    else
    {
        node->next = queue->head;
        queue->head = node;
    }
    queue->size++;
}

void push_back(lexqueue* queue, lexqueue_node* node){
    lexqueue_node* temp = queue->head;
    if(!temp)
    {
        queue->head = node;
        queue->tail = node;
    }
    else
    {	
        queue->tail->next = node;
        queue->tail = node;
    }
    queue->size++;
}


void print_queue(lexqueue* queue)
{
    printf("======QUEUE=====\n");
    lexqueue_node* temp = queue->head;
    printf("Size: %d\n",queue->size);
    while(temp)
    {
        print_queue_node(temp);
        temp = temp->next;
    }

}


void print_queue_node(lexqueue_node* queue_node)
{
    printf("%c\n",queue_node->tree_node->c);

    /*
       printf("Children:\n");
       for(int i=0; i<26; i++){
       if(queue_node->tree_node->children[i]!=NULL){
       printf("%c",queue_node->tree_node->children[i]->c);
       }
       }
     */

    printf("\n");
}
