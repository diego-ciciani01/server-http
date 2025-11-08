#ifndef REQUEST_HTTP
#define REQUEST_HTTP

#define HTTP_UNKNOWN -1

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
    int method;
    char *URI;
    float httpVersion;

}httpRequest;

httpRequest *httpRequestConstructo(char *requestString);

#endif
