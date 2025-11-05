#include "queue.h"
#include "../malloc-utils/malloc_utils.h"

void pushOnQueue(queue *q, void *data)
{
    addInHead(q->list, data);
}

void* peakOnQueue(queue *q)
{
    listIter *iter = listGetIterator(q->list, ITER_DIRECTION_HEAD);
    return getNextNode(iter);
}

void popOnQueue(queue *q)
{
    listDelOnIndex(q->list, 0);
}

queue *createQueue(void)
{
    queue *q = safeMalloc(sizeof(q));
    q->list =  inizializeLinkedList();
    q->peak = peakOnQueue;
    q->push = pushOnQueue;
    q->pop = popOnQueue;

    return q;
}


