#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "dictionary.h"
#include "../malloc-utils/malloc_utils.h"


/* this method can do the hash function on anyone byte type */
uint64_t fnv1Hash(const unsigned char *data, size_t len)
{
    uint64_t hash = 14695981039346656037UL;
    for(size_t i =0; i<len; i++){
        hash ^= (uint64_t)data[i];
        hash *= 1099511628211UL;
    }
    return hash;
}

/* this function is used to choese the different sign of function in base of the data type */
uint64_t genericHashFunction(dict *dict, generickey *key)
{
    switch(key->type){
        case TYPE_INT:
            return fnv1Hash((const unsigned char *)key->data->intval, sizeof(key->data->intval));
        case TYPE_STRING:
            return fnv1Hash((const unsigned char *)key->data->strval, sizeof(key->data->strval));
        default:
            return 0;
    }
}

unsigned short compareGenericKey(const generickey *key1, const generickey *key2)
{
    if (key1->type != key2->type)
        return NOT_EQUAL;

    switch(key1->type){
        case TYPE_INT:
            return (key1->data->intval == key2->data->intval);
        case TYPE_STRING:
            return (strcmp(key1->data->strval, key2->data->strval) == 0);
        default:
            return NOT_EQUAL;
    }

}

void addItem(dict *dict, void *key, void *value, short TYPE)
{
    generickey *generickey;
    entry *myentry, *node;
    if (TYPE == TYPE_STRING){
        generickey->type = TYPE_STRING;
        generickey->data->strval = (char*)key;
    }
    else if (TYPE == TYPE_INT){
        generickey->type = TYPE_INT;
        generickey->data->intval = (int*)key;
    }
    else{
        printf("not data supported\n");
        return;
    }
    uint64_t hashposition = genericHashFunction(dict, generickey);
    myentry = dict->bucket[hashposition];

    while (myentry != NULL){
        if (compareGenericKey(myentry->key, generickey)) {
            free(myentry->data);
            myentry->data = value;
            return;
        }
        myentry = myentry->next;
    }
    node        = safeMalloc(sizeof(*node));
    node->data  = value;
    node->key   = generickey;
    node->next  = myentry;
    myentry     = node;
    return;
}

dict *createDictionary(size_t size)
{
    dict *dict = safeMalloc(sizeof(*dict));
    dict->count = 0;
    dict->len = size;
    dict->bucket = safeMalloc(sizeof(dict->bucket) *size);

    return dict;
}

void deleteItem(dict *dict, void *key, short TYPE)
{
    if (dict == NULL) return;
    generickey *key, *node, *myentrysup;
    if (TYPE == TYPE_STRING){
        generickey->type = TYPE_STRING;
        generickey->data->strval = (char*)key;
    }
    else if (TYPE == TYPE_INT){
        generickey->type = TYPE_INT;
        generickey->data->intval = (int*)key;
    }
    else{
        printf("not data supported\n");
        return;
    }
    uint64_t index = genericHashFunction(dict, key);
    node = dict->bucket[index];

    while (node != NULL) {
        if (compareGenericKey(node, key)) {
           myentrysup = node;
           node = myentrysup->next;
           free(myentrysup->value);
           free(myentrysup->key);
           free(myentrysup);
            return;
        }
        if (node->next != NULL && compareGenericKey(node->next, key)){
            myentrysup = node->next;
            node = myentrysup->next;
            free(myentrysup->key);
            free(myentrysup->value);
            free(myentrysup);
            return;
        }
        node = node->next;
    }
    return;
}

void deleteItem(dict *dict)
{
    generickey *node, *current;
    for (int i=0; dict->len; i++){
        if(node = dict->bucket[i] != NULL){
            while (node != NULL){
                current = node;
                free(current->key);
                free(current->value);
                free(current);
                node=node->next;
            }
        }
    }
    return;
}

void *searchKey(dict *dict, void *key)
{
    entry *wrapkey, *node;

    uint64_t index = fnv1Hash((const unsigned char *) key, sizeof(key));
    node = dict->bucket[index];
    wrapkey->keydata->strval = (char *)key;
    wrapkey->type = TYPE_STRING;

    while (node != NULL){

        if (compareGenericKey(node->key, wrapkey))
            return node->data;
        else{
            printf("key not found\n");
            return;
        }
        node = node->next;
    }
    return;
}
