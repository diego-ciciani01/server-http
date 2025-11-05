#ifndef LINKED_LIST
#define LINKED_LIST

typedef struct listNode{
    void *value;
    struct listNode *prev;
    struct listNode *next;

}listNode;

typedef struct{
    struct listNode* head;
    struct listNode* tail;
    void (*free)(void *ptr);
    int len;
}list;

typedef struct{
    struct listNode *prev;
    struct listNode *next;
    int direction;

}listIter;

/* Generic Method * */
list *inizializeLinkedList(void);
list *addInHead(list *list, void *value);
list *addInTail(list *list, void *value);
listIter *listGetIterator(list *list, int direction);
listNode *getNextNode(listIter *iter);
void listReleaseIterator(listIter *iter);
void listDelNode(list *list, listNode *node);
void listDelOnIndex(list *list, int index);

/* Some useful macro */
#define listLength(l) ((l)->len)
#define listFirst(l) ((l)->head)
#define listLast(l) ((l)->tail)
#define nodeValue(n) ((n)->value)

/* Define for the direction of iterator */
#define ITER_DIRECTION_HEAD 0
#define ITER_DIRECTION_TAIL 1

#endif
