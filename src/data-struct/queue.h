#ifndef QUEUE
#define QUEUE

#include "linkedList.h"

/*  This is a easy wrapper on the linked list, done to implement a queue
 *  - Push add an elemnt on the top of the stak
 *  - Pop remove the first item from the stak
 *  - Peak retrive the data infront of the list
 */

typedef struct queue{
    list *list;
    void (*push)(struct queue *q, void *data);
    void (*pop)(struct queue *q);
    void *(*peak)(struct queue *q);
}queue;

/* Some constructor method */
queue *createQueue(void);
void queueDestruction(queue *ptr);

#endif
