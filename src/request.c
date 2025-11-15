#include <stdio.h>
#include <string.h>

#include "request.h"

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

/* Extract the header field data */
void extractHeaderFields(httpRequest *httpRequest, char *headerFild)
{
    char fields[strLen(headerFild)], *field, *header, *key, *value;
    queue *headers;
    dict *headerdict;

    copyString(headerFild, fields);

    headers = createQueue();
    field = strtok(fields, "\n");
    while(field){
        headers->push(headers, field);
        field = strtok(NULL, "\n");
    }
    headerdict = createDictionary(10);

    do{
        header = (char*)headers->peak(headers);

        key = strtok(header, ":");
        value = strtok(NULL, "\n");
        /*remove some space */
        if(value){
            if (value[0] == ' ') value++;
            addItem(headerdict, key, value, TYPE_STRING);
        }
            headers->pop(headers);

    }while(header);

    queueDestruction(headers);
}

/* Extracrt the body fild, but only if the Content-Type key is presente on the dict */
void extractBodyField(httpRequest *httpRequest, char *bodyField)
{
    char *bodydata, *key, *value, *field, *bodytype;
    dict *bodydict;
    queue *qbody;

    if ((bodytype =searchKey(httpRequest->headerfields, "Content-Type"))){
        bodydict = createDictionary(10);
        if (strcmp(bodytype, "application/x-www-form-urlencoded") == 0){
            qbody = createQueue();
            /* Extract all the key - value from body and push on the queue */
            field = strtok(bodyField, "&");
            while(field){
                qbody->push(qbody, field);
                field = strtok(NULL, "&");
            }
            do{
                bodydata = qbody->peak(qbody);
                key = strtok(bodydata, "=");
                value = strtok(NULL, "\0");

                if(value) addItem(bodydict, key, value, TYPE_STRING);
                qbody->pop(qbody);
                bodydata = qbody->peak(qbody);
            }while(bodydata);
            queueDestruction(qbody);
        }else{
            addItem(bodydict, "all_data", bodyField, TYPE_STRING);

        }
        httpRequest->body = bodydict;
    }
}

void extractRequestLine( httpRequest *httpRequest, char *requestLine)
{
    char field[strLen(requestLine)];
    copyString(requestLine, field);

    char *method = strtok(field, " ");
    char *URI = strtok(NULL, " ");
    char *httpVersion = strtok(NULL, " ");
    dict *request =  createDictionary(10);
    addItem(request, "method", method, TYPE_STRING);
    addItem(request, "URI", URI, TYPE_STRING);
    addItem(request, "version", httpVersion, TYPE_STRING);

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

    extractRequestLine(myrequest, requestline);
    extractHeaderFields(myrequest, headerfilds);
    extractBodyField(myrequest, body);
    return myrequest;
}

