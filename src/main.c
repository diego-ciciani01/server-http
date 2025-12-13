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

/* Error Codes */
#define ERR  -1
#define OK 0

/* Logs level */
#define WARNIG 2
#define NOTICE 1
#define DEBBUG 0
/* Server Configuration */
#define SERVER_PORT "6700"


/*========================================== Data Tipes ========================================= */
struct httpServer{
    char *port;
    int fd;
    list *clients;
    char neterr[NETWORK_ERR_LEN];
    list *freeObjctList;
};


/* ========================================== Global Vars ====================================== */
static struct httpServer server;


/* ========================================= Utility Function ================================= */
/* This function is wrap some out of memory error */
static void oom(const char *msg)
{
    fprintf(stderr, "%s: out of memory\n", msg);
    fflush(stderr);
    sleep(1);
    abort();
}

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
        serverLogs(WARNIG, "Opening TCP port: %s", server.neterr);
        exit(1);
    }
    serverLogs(NOTICE, "Server Started " );
}

static void acceptHandler(int fd)
{
    int cport, cfd;
    char cip[128];

    cfd = networkAccept(server.neterr, fd, cip, &cport);
    if (cfd == ERR) {
        serverLogs(DEBBUG, "Accepting client connection: %s ", server.neterr);
        return;
    }
    serverLogs(DEBBUG, "Accepted  %s:%d ", cip, cport);

}

int main(void){
    initServer();
    while(1)
        acceptHandler(server.fd);

    return 0;
}
