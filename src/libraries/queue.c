#include <stdlib.h>
#include "queue.h"


queue* new_queue()
{
    queue* new = malloc(sizeof(queue));
    new->front = NULL;
    new->back  = NULL;
    new->size  = 0;

    return new;
}


void enqueue(queue* q, void* elem)
{
    //Create new node
    queue_node* node = malloc(sizeof(queue_node));
    node->data = elem;
    node->next = NULL;

    //Empty case
    if(q->front == NULL)
    {
        q->front = node;
        q->back = node;

        return;
    }

    //Non-empty
    q->back->next = node;
    q->back = node;
}


void* dequeue(queue* q)
{
    //Empty case
    if(q->front == NULL)
        return NULL;
    
    //Remove from queue
    queue_node* node = q->front;
    q->front = q->front->next;

    //Newly empty case
    if(q->front == NULL)
        q->back = NULL;

    //Get data and free node
    void* data = node->data;
    free(node);

    return data;
}


/* peek_queue - returns the front element from the queue without removing it
 */
void* peek_queue(queue* q)
{
    //Empty case
    if(q->front == NULL)
        return NULL;

    return q->front->data;
}
