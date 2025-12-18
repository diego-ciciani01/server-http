#ifndef RESPOSE
#define RESPOSE

#include "datastruct/dictionary.h"

typedef struct {
    char *httpVersion;
    int statusCode;
    char *contextType;
    char *body;
    size_t contentLength;

    dict *headers;
}resposeHttp;

#endif
