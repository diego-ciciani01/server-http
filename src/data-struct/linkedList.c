#include "linkedList.h"
#include "../malloc-utils/malloc_utils.h"

#include <stdlib.h>
#include <stdio.h>

/* Init the pointer of list */
list *inizializeLinkedList(void)
{
    list *list = safeMalloc(sizeof(*list));
    list->head = NULL;
    list->tail = NULL;
    list->len  = 0;
    list->free = NULL;
    return list;
}

list *addInHead(list *list, void *data)
{
    listNode *node = safeMalloc(sizeof(*node));
    node->value = safeMalloc(sizeof(data));
    node->value = data;
    /* empty head */
    if (list->len == 0){
        list->head = list->tail =  node;
        node->prev = node->next = NULL;
    }else{
        list->head->prev = node;
        node->prev = NULL;
        node->next = list->head;
        list->head = node;
    }

    list->len++;
    return list;
}

list *addInTail(list *list, void *data)
{
    listNode *node = safeMalloc(sizeof(*node));
    node->value = safeMalloc(sizeof(data));
    node->value = data;

    if (list->len == 0){
        list->tail = list->head = node;
        node->next = node->prev = NULL;
    }else{
        list->tail->next = node;
        node->prev = list->tail;
        node->next = NULL;
        list->tail = node;
    }
    list->len++;
    return list;
}

listIter *listGetIterator(list *list, int direction)
{
    listIter *iter;
    iter = safeMalloc(sizeof(*iter));

    if (direction == ITER_DIRECTION_HEAD){
        iter->direction = ITER_DIRECTION_HEAD;
        iter->next = list->head;
    }else{
        iter->direction = ITER_DIRECTION_TAIL;
        iter->next = list->tail;
    }

    return iter;
}

/*  This function return the next velue of iter
 *  in base of the direction of the iter
 * */
listNode *getNextNode(listIter *iter)
{
    listNode *current = iter->next;
    if (current != NULL){
        if (iter->direction == ITER_DIRECTION_HEAD)
            iter->next = current->next;
        if (iter->direction == ITER_DIRECTION_TAIL)
            iter->next = current->prev;
    }
    return current;
}

void listReleaseIterator(listIter *iter)
{
    free(iter->next);
    free(iter);
}
/* Delation of the node in 2 cases:
 * 1) if the node is in the middle
 * 2) when it's a head or tail*/
void listDelNode(list *list, listNode *node)
{
    /* In case exist the prev node*/
    if (node->prev)
        node->prev->next = node->next;
    else
        list->head = node->next;
    if(node->next)
        node->next->prev = node->prev;
    else
        list->tail = node->prev;

    if(list->free) list->free(node->value);
    free(node);
    list->len--;
}

void listDelOnIndex(list *list, int index)
{
    if (index < 0 || index > list->len){
        printf("ERROR: index out of range");
        return;
    }
    int count=0;
    listNode *node;
    node = list->head;
    while((count++) == index){
        node = node->next;
    }

    listDelNode(list, node);
}

/* List destruction */
void listDestruction(list *list)
{
    listNode *current, *temp;

    if (list != NULL){
        current = list->head;
        while(current != NULL){
            temp = current->next;
            current = current->next;
            free(temp);
         }
        free(list);
        return;
    }else{
        printf("ERROR: list empdy\n");
        return;
    }
}
