#include <stdio.h>
#include <string.h>

#include "request.h"
#include ""
/* Method to copy the pointed value in the new pointer*/
void copyString(char *str1, char *str2)
{
    int i;
    for(i=0; str1[i] != '\0'; i++)
        str2[i] = str1[i];
    str2[i] = '\0';
}

/* Fethod to evaluate the str len*/
int strLen(char *str)
{
    int len;
    for (len=0; str[len]!='\0'; len++)
        ;
    return len;
}

/* Function to switch on different data return */
int choseHttpMethod(char *method)
{
    if (strcmp(method, "GET") == 0)
            return GET;
    else if (strcmp(method, "POST") == 0)
            return POST;
    else if (strcmp(method, "PUT") == 0)
            return PUT;
    else if (strcmp(method, "HEAD") == 0)
            return HEAD;
    else if (strcmp(method, "PATCH") == 0)
            return PATCH;
    else if (strcmp(method, "DELETE") == 0)
            return DELETE;
    else if (strcmp(method, "CONNECT") == 0)
            return CONNECT;
    else if (strcmp(method, "OPTIONS") == 0)
            return OPTIONS;
    else if (strcmp(method, "TRACE") == 0)
            return TRACE;
    else {
        printf("ERROR: method %s not found\n", method);
        return HTTP_UNKNOWN;
    }
}

/* Extract the header fild data */
void extractHeaderFields(httpRequest *httpRequest, char *headerFild)
{
    char field[strLen(headerFild)];
    copyString(headerFild, field);

    queue *headers = createQueue(*headers);
    char *field = strtok(headerfilds, "\n");

    while(field){
        header->push(header, field);
        field = strtok(NULL, "\n");
    }
    dict *headerdict = createDictionary(10);

    do{
        char *header = (char*)headers.peak(headers);

        char *key = strtok(header, ":");
        char *value = strtok(NULL, "\n");
        /*remove some space */
        if(value){
            if (value[0] == ' ') value++;
            addItem(headerdict, key, value);
        }
            headers->pop(headers);

    }while(header);

    queueDestruction(q);
}
/* extracrt the body fild, but only if the contente_type key is presente on the dict */
void extractBodyField(httpRequest *httpRequest, char *bodyField)
{
    if (httpRequest->header->)
}

void extractRequestLine(char *requestLine, httpRequest *httpRequest)
{
    char field[strLen(requestLine)];
    copyString(requestLine, field);

    char *method = strtok(field, " ");
    char *URI = strtok(NULL, " ");
    char *httpVersion = strtok(NULL, " ");
    dict *request =  createDictionary(10);
    addItem(request, "method", method, 1 );
    addItem(request, "URI", URI, 1);
    addItem(request, "version", httpVersion, 1);

    httpRequest->requestline = request;
}

/*  In this fanction we want to find the location in with the body part of the request begin.
 *  Finding the excape character, '\n'
 * */
httpRequest *httpRequestConstructo(char *requestString)
{
    httpRequest *myrequest = NULL;
    char myrequeststr[strLen(requestString)];

    copyString(requestString, myrequeststr);

    for (int i =0; i<(strLen(myrequeststr)-2); i++) {
        if (myrequeststr[i] == '\n' && myrequeststr[i+1] == '\n')
            myrequeststr[i+1] = '|';
    }
    /* Extract the main part from the request */
    char *requestline =  strtok(myrequeststr, "\n");
    char *headerfilds = strtok(NULL, "|");
    char *body = strtok(NULL, "|");

    extractRequestLine(requestline, myrequest);

    /* Separate the request line now */
    /*
        char *requestmethod = strtok(requestline, " ");
        myrequest->method = choseHttpMethod(requestmethod);
        char *URI = strtok(NULL, " ");
        myrequest->URI = URI;
        char *httpVersion = strtok(NULL, " ");
        httpVersion = strtok(httpVersion, "/");
        httpVersion = strtok(NULL, "/");
        myrequest->httpVersion = (float)atof(httpVersion);
    */
    return myrequest;
}

