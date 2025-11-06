#ifndef REQUEST_HTTP
#define REQUEST_HTTP

#define HTTP_UNKNOWN -1

enum httpMethod
{
    POST,
    GET,
    PUT,
    HEAD,
    DELATE,
    UPDATE,
    CONNECT,
    OPTIONS,
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
