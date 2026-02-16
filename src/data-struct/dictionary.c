#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "dictionary.h"
#include "../malloc-utils/malloc_utils.h"


/* this method can do the hash function on anyone byte type */
uint64_t fnv1Hash(const unsigned char *data, size_t len)
{
    uint64_t hash = 14695981039346656037UL;
    uint64_t prime = 1099511628211UL;

    for(size_t i = 0; i < len; i++) {
       hash ^= (uint64_t)data[i];
       hash *= prime;
    }
    return hash;
}

/* this function is used to choese the different sign of function in base of the data type */
uint64_t genericHashFunction(dict *dict, generickey *key)
{
    switch(key->type){
        case TYPE_INT:
            return fnv1Hash((const unsigned char *)&key->data.intval, sizeof(key->data.intval));
        case TYPE_STRING:
            if(key->data.strval == NULL){
                fprintf(stderr, "ERROR: invalid string value\n");
                return 0;
            }
            return fnv1Hash((const unsigned char *)key->data.strval, strlen(key->data.strval));
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
            return (key1->data.intval == key2->data.intval);
        case TYPE_STRING:
            return (strcmp(key1->data.strval, key2->data.strval) == 0);
        default:
            return NOT_EQUAL;
    }

}

void addItem(dict *dict, void *key, void *value, short TYPE)
{
    generickey *generickey;
    entry *myentry, *node;

    generickey = safeMalloc(sizeof(*generickey));

    if (TYPE == TYPE_STRING){
        generickey->type = TYPE_STRING;
        generickey->data.strval = strdup((char*)key);
        //strcpy( generickey->data.strval, (char*)key);
    }
    else if (TYPE == TYPE_INT){
        generickey->type = TYPE_INT;
        generickey->data.intval = *(int *)key;
    }
    else{
        printf("not data supported\n");
        free(generickey);
        return;
    }
    uint64_t hashposition = genericHashFunction(dict, generickey) % dict->len;
    myentry = dict->bucket[hashposition];
    while (myentry != NULL){
        if (compareGenericKey(myentry->key, generickey)) {
            if (TYPE == TYPE_STRING && myentry->data != NULL) {
                free(myentry->data);
            }
            if (TYPE == TYPE_STRING) {
                myentry->data = strdup((char*)value);
            } else {
                myentry->data = value;
            }

            if (TYPE == TYPE_STRING) free(generickey->data.strval);
            free(generickey);
            return;
        }
        myentry = myentry->next;
    }
    node = safeMalloc(sizeof(*node));

    if (TYPE == TYPE_STRING) {
        node->data = strdup((char*)value);
    } else {
        node->data = value;
    }

    node->key   = generickey;
    node->next  = dict->bucket[hashposition];
    dict->bucket[hashposition] = node;
}

dict *createDictionary(size_t size)
{
    dict *dict = safeMalloc(sizeof(*dict));
    dict->count = 0;
    dict->len = size;

    dict->bucket = calloc(size, sizeof(entry*));

    return dict;
}

void deleteItem(dict *dict, void *key, short TYPE)
{
    if (dict == NULL) return;

    generickey *generickey;
    entry *node, *myentrysup;

    generickey = safeMalloc(sizeof(*generickey));

    if (TYPE == TYPE_STRING){
        generickey->type = TYPE_STRING;
        generickey->data.strval = (char*)key;
    }
    else if (TYPE == TYPE_INT){
        generickey->type = TYPE_INT;
        generickey->data.intval = *(int*)key;
    }
    else{
        printf("not data supported\n");
        return;
    }
    uint64_t index = genericHashFunction(dict, key) % dict->len;
    node = dict->bucket[index];

    while (node != NULL) {
         if (compareGenericKey(node->key, generickey)) {
           myentrysup = node;
           node = myentrysup->next;
           free(myentrysup->data);
           free(myentrysup->key);
           free(myentrysup);
            return;
        }
        if (node->next != NULL && compareGenericKey(node->next->key, key)){
            myentrysup = node->next;
            node = myentrysup->next;
            free(myentrysup->key);
            free(myentrysup->data);
            free(myentrysup);
            return;
        }
        node = node->next;
    }
    return;
}

void destoyDict(dict *dict)
{
    entry *current, *node;
    for (int i=0; dict->len; i++){
        node = dict->bucket[i];
        if((node != NULL)){
            while (node != NULL){
                current = node;
                free(current->key);
                free(current->data);
                free(current);
                node=node->next;
            }
        }
    }
    free(dict);
    return;
}

void *searchKey(dict *dict, void *key)
{
if (dict == NULL || key == NULL) return NULL;

    size_t key_len = strlen((char*)key);
    uint64_t index = fnv1Hash((const unsigned char *)key, key_len) % dict->len;

    entry *node = dict->bucket[index];

    generickey *temp_key = safeMalloc(sizeof(*temp_key));
    temp_key->type = TYPE_STRING;
    temp_key->data.strval = (char *)key;

    while (node != NULL) {
        if (compareGenericKey(node->key, temp_key)) {
            free(temp_key);
            return node->data;
        }
        node = node->next;
    }

    free(temp_key);
    return NULL;
}
