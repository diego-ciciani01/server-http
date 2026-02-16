#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>

#include "request.h"
#include "network.h"
#include "data-struct/linkedList.h"
#include "malloc-utils/malloc_utils.h"

/* Error Codes */
#define ERR  -1
#define OK 0

/* Logs level */
#define WARNING 2
#define NOTICE 1
#define DEBUG 0

/* Server Configuration */
#define SERVER_PORT "6700"
#define BUFFER_SIZE 1000

/*========================================== Data Tipes ========================================= */
struct httpServer{
    char *port;
    int fd;
    list *clients;
    char neterr[NETWORK_ERR_LEN];
    list *freeObjctList;
};

typedef struct contextClient{
    int fd;
    char *clientAddr;
    const struct httpServer *serverConfig;
    char readBuffer[BUFFER_SIZE];
    size_t bytesRead;
}contextClient;

/* ========================================== Global Vars ====================================== */
static struct httpServer server;

/* ========================================= Utility Function ================================= */
/* This function is wrap for some out of memory error */
static void oom(const char *msg)
{
    fprintf(stderr, "%s: out of memory\n", msg);
    fflush(stderr);
    sleep(1);
    abort();
}

/* Here function for a dynamic number of argumets to pass for the function */
static void serverLogs( int level,  const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    if(level >= 0 && level <=2){
        char *c = ".-*";
        fprintf(stdout,"%c ",c[level]);
        vfprintf(stdout, fmt, ap);
        fprintf(stdout,"WARNING\n");
        fflush(stdout);
    }
    va_end(ap);
}

/*========================================= Server Network ==================================== */
static void initServer()
{
    signal(SIGHUP, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);

    server.clients = inizializeLinkedList();
    server.freeObjctList = inizializeLinkedList();
    server.port = SERVER_PORT;
    if (!server.clients)
        oom("SERVER INITIALIALIZATION");

    server.fd  = networkTcpServer(server.neterr, server.port, NULL );
    if (server.fd == ERR) {
        serverLogs(WARNING, "Opening TCP port: %s", server.neterr);
        exit(1);
    }
    serverLogs(NOTICE, "Server Started " );
}
/* Process client function, to get the client and reply a response */
static void processClient(contextClient *c)
{
    int nbyte = networkRecv(server.neterr, c->fd, c->readBuffer, BUFFER_SIZE-1);
    httpRequest *req = NULL;
    if(nbyte < 0){
        serverLogs(WARNING, "Number read byte 0", server.neterr);
        return;
    }

    c->readBuffer[nbyte] = '\0';
    c->bytesRead = nbyte;
   if((req = httpRequestConstructor(c->readBuffer)) == NULL){
       serverLogs(WARNING, "Error parsing the http request");
   }
   else{
       char *method = (char*)searchKey(req->requestline, "method");
       char *uri = (char*)searchKey(req->requestline, "URI");

       char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 12\r\n\r\nHello World!";
       networkSend(server.neterr, c->fd, response, strlen(response));
    }

    close(c->fd);

}

static int createClient(int cfd, char *ip, contextClient *c)
{
    c->fd = cfd;
    c->clientAddr = strdup(ip);
    if (c->clientAddr == NULL) return ERR;

    c->serverConfig = &server;
    c->bytesRead = 0;
    return OK;
}

static void acceptHandler(int fd)
{
    int cport, cfd;
    char cip[128];
    contextClient *c = safeMalloc(sizeof(*c));

    cfd = networkAccept(server.neterr, fd, cip, &cport);
    if (cfd == ERR) {
        serverLogs(DEBUG, "Accepting client connection: %s ", server.neterr);
        return;
    }
    serverLogs(DEBUG, "Accepted  %s:%d \n", cip, cport);
    if (createClient(cfd, cip, c) == ERR) {
        serverLogs(WARNING, "Error creating the resource for the client");
        close(cfd);  /* Close the file descriptor */
        return;
    }
    /* Start the process */
    processClient(c);

}

int main(void){
    initServer();
    while(1)
        acceptHandler(server.fd);

    return 0;
}
