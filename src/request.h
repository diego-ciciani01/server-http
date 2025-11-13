#ifndef REQUEST_HTTP
#define REQUEST_HTTP

#define HTTP_UNKNOWN -1

#include "data-struct/dictionary.h"
#include "data-struct/queue.h"
enum httpMethod
{
    POST,
    GET,
    PUT,
    HEAD,
    DELETE,
    UPDATE,
    CONNECT,
    OPTIONS,
    PATCH,
    TRACE
};

typedef struct
{
   dict *requestline;
   dict *headerfields;
   dict *body;

}httpRequest;

httpRequest *httpRequestConstructo(char *requestString);
void copyString(char *str1, char *str2);
int strlen(char *str);
int choseHttpMethod(char *method);
void extractRequestLine(char *requestLine, httpRequest *httpRequest);
void extractHeaderFields(httpRequest *httpRequest, char *headerField);
void extractBodyField(httpRequest *httpRequest, char *bodyField);
#endif
