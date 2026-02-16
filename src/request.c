#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
void extractHeaderFields(httpRequest *httpRequest, char *headerField)
{
if (headerField == NULL || strlen(headerField) == 0) return;

    dict *headerdict = createDictionary(20);

    char *fields_copy = strdup(headerField);
    if (!fields_copy) return;

    char *saveptr_line;
    char *line = strtok_r(fields_copy, "\n", &saveptr_line);

    while (line) {
        char *r_ptr = strchr(line, '\r');
        if (r_ptr) *r_ptr = '\0';

        char *colon = strchr(line, ':');

        if (colon) {
            *colon = '\0';

            char *key = line;
            char *value = colon + 1;

            while (*value == ' ') {
                value++;
            }

            if (strlen(key) > 0) {
                addItem(headerdict, key, value, TYPE_STRING);
            }
        }
        line = strtok_r(NULL, "\n", &saveptr_line);
    }

    httpRequest->headerfields = headerdict;
    free(fields_copy);}

/* Extracrt the body fild, but only if the Content-Type key is presente on the dict */
void extractBodyField(httpRequest *httpRequest, char *bodyField)
{
    if (bodyField == NULL || strlen(bodyField) == 0) return;

    char *bodytype = searchKey(httpRequest->headerfields, "Content-Type");
    if(bodytype == NULL) return;

    dict *bodydict = createDictionary(10);

    if (strcmp(bodytype, "application/x-www-form-urlencoded") == 0){
        char *body_copy = strdup(bodyField);
        char *saveptr_pair;

        char *pair = strtok_r(body_copy, "&", &saveptr_pair);

        while(pair) {
            char *eq = strchr(pair, '=');
            if (eq) {
                *eq = '\0';
                char *key = pair;
                char *value = eq + 1;
                addItem(bodydict, key, value, TYPE_STRING);
            }
            pair = strtok_r(NULL, "&", &saveptr_pair);
        }
        free(body_copy);
    } else {
        addItem(bodydict, "all_data", bodyField, TYPE_STRING);
    }

    httpRequest->body = bodydict;

}

void extractRequestLine( httpRequest *httpRequest, char *requestLine)
{
    if(requestLine == NULL || strlen(requestLine) < 0) return;
    char *field = strdup(requestLine);
    if(field == NULL) return;

    printf("field: \n %s\n", requestLine);
    char *method = strtok(field, " ");
    char *URI = strtok(NULL, " ");
    char *httpVersion = strtok(NULL, " ");
    dict *request =  createDictionary(10);

    if(method == NULL || httpVersion == NULL || httpVersion == NULL){
        free(field);
        return;
    }
    addItem(request, "method", method, TYPE_STRING);
    addItem(request, "URI", URI, TYPE_STRING);
    addItem(request, "version",httpVersion, TYPE_STRING);

    httpRequest->requestline = request;

    free(field);
}

/*  In this fanction we want to find the location in with the body part of the request begin.
 *  Finding the excape character, '\n'
 * */
httpRequest *httpRequestConstructor(char *requestString)
{
    httpRequest *myrequest = NULL;
    char *myrequeststr;
    myrequeststr = strdup(requestString);

    for (int i =0; i<(strlen(myrequeststr)-2); i++) {
        if (myrequeststr[i] == '\n' && myrequeststr[i+1] == '\n')
            myrequeststr[i+1] = '|';
    }
    /* Extract the main part from the request */
    char *requestline =  strtok(myrequeststr, "\n");
    char *headerfields = strtok(NULL, "|");
    char *body = strtok(NULL, "|");

    extractRequestLine(myrequest, requestline);
    extractHeaderFields(myrequest, headerfields);
    extractBodyField(myrequest, body);
    return myrequest;
}

