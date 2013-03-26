#ifndef LEXQUEUE_H
#define LEXQUEUE_H


/* A generic queue node - contains some arbitrary data pointer, and the
 *     pointers in a doubly linked list.
 */
typedef struct queue_node {
    void* data;

    struct queue_node* next;
} queue_node;


/* A generic queue - contains pointers to the front and back of the queue,
 *     and the number of elements.
 */
typedef struct queue {
	queue_node *front;
    queue_node *back;
	int size;
} queue;


/* new_queue - returns an empty queue;
 */
queue* new_queue();


/* enqueue - adds an element to the back of the queue
 */
void enqueue(queue* q, void* elem);


/* dequeue - removes the front element from the queue and returns it
 */
void* dequeue(queue* q);


/* peek_queue - returns the front element from the queue without removing it
 */
void* peek_queue(queue* q);


#endif
