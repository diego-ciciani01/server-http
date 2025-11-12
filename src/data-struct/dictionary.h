#ifndef DICTIONARY
#define DICTIONARY

#define EQUAL 1
#define NOT_EQUAL 0

/* string and int for datatype */
typedef enum{
    TYPE_STRING,
    TYPE_INT
}keytype;

/* Possible value */
typedef union{
    int *intval;
    char *strval;
}keydata;

/* defined type */
typedef struct{
    keytype type;
    keydata *data;
}generickey;

typedef struct entry{
    generickey *key;
    void *data;
    struct entry *next;
}entry;

typedef struct dict{
    struct entry **bucket;
    size_t len;
    size_t count;
}dict;

dict *createDictionary(size_t size);
void addItem(dict *dict, void *key, void *value, short TYPE);
uint64_t genericHashFunction(dict *dict, generickey *key);
uint64_t fnv1Hash(const unsigned char *data, size_t len);
unsigned short compareGenericKey(const generickey *key1, const generickey *key2);
void deleteItem(dict *dict, entry *entry);
void destructDict(dict *dict);
#endif
