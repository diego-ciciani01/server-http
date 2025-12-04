#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>

#include "network.h"

/* Utility to wrap the error */
static void networkSetError(char *err, const char *fmt, ...)
{
    va_list ap;

    if (!err) return;
    va_start(ap, fmt);
    vsnprintf(err, NETWORK_ERR_LEN , fmt, ap);
    va_end(ap);
}

/* The method help to interprete the '.' notation in string rapresenting string */
void readAddr(struct sockaddr *addr, char *ipclient)
{
    void *ipversion;

    if (addr->sa_family == AF_INET)  ipversion = &((struct sockaddr_in*)addr)->sin_addr;
    if (addr->sa_family == AF_INET6) ipversion = &((struct sockaddr_in6*)addr)->sin6_addr;

    inet_ntop(addr->sa_family, ipversion, ipclient, sizeof(ipclient));
}

int networkAccept(char *err, int sockfd, char *ip, int *port)
{
    int fd;
    struct sockaddr_storage their_addr;
    socklen_t sin_size;

    while(1){
        sin_size = sizeof(their_addr);
        fd = accept(sockfd, (struct sockaddr*)&their_addr, &sin_size);
        if (fd == -1){
            /* meening if the call, it been stopped from keyboard */
            if (errno == EINTR)
                continue;
            else{
                networkSetError(err, "accept %s\n", strerror(errno));
                return NETWORK_ERR;
            }
        }
        break;
    }
    if (ip) readAddr((struct sockaddr*)&their_addr, ip);
    /* with htohs is possible to tonver the port number for the correct value in respet to the endianess */
    if (port){
        /* case of IPv4 */
        if (their_addr.ss_family == AF_INET){
            struct sockaddr_in *s =  (struct sockaddr_in *)&their_addr;
            *port = ntohs(s->sin_port);
        }
        /* case of IPv6 */
        if (their_addr.ss_family == AF_INET6){
            struct sockaddr_in6 *s = (struct sockaddr_in6 *)&their_addr;
            *port = ntohs(s->sin6_port);
        }

    }
    return fd;
}

int networkSend(char *err, int sockfd, void *buf, size_t len)
{
    if (send(sockfd, buf, len, 0) == -1){
        networkSetError(err, "send: %s\n", strerror(errno));
        return  NETWORK_ERR;
    }

    return sockfd;
}

int networkRecv(char *err, int sockfd, void *buf, size_t len)
{
    int readbyte=0;
    if((readbyte = recv(sockfd, buf, len, 0)) == -1 || readbyte == 0){
        networkSetError(err, "recv: %s\n", strerror(errno));
        return NETWORK_ERR;
    }
    return readbyte;
}

/* This function create the file descriot for the socket for IPv4 and IPv6.
 * Provide the necessary error controlo for the fault of API call.
 * */
int networkTcpServer(char *err, const char *port, char *bindaddr)
{
    /* Variable used to create and handle the socket */
    int fd, rv, on=1;
    struct addrinfo infoSock, *res;
    char ipclient[INET6_ADDRSTRLEN];

    memset(&infoSock, 0, sizeof(infoSock));
    infoSock.ai_family = AF_INET;  /* IPv4 if you want IPv6, subsituite with AF_INET6 */
    infoSock.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(NULL, port, &infoSock, &res) == -1)){
        networkSetError(err, "getaddressinfo: %s\n", gai_strerror(rv));
        return NETWORK_ERR;
    }

    if ((fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1){
        networkSetError(err, "socket: %s\n", strerror(errno));
        close(fd);
        return NETWORK_ERR;
    }
    /*  Set the reutilizzaztion of the socket in a time,
     *  if you try to recall the server after a time of 1-2 min,
     *  the bind call will fail saing 'Addres already in use'.
     *  So for this reason we did, this API call.
     * */
    if ((setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) == -1){
        networkSetError(err, "setsockopt SO_REUSEADDR: %s\n", strerror(errno));
        close(fd);
        return NETWORK_ERR;
    }
    /*
    if (bindaddr){
        networkSetError(err, "address not setted\n");
        return NETWORK_ERR;
    }
    */

    if (bind(fd, res->ai_addr, res->ai_addrlen) == -1){
        networkSetError(err, "bind: %s\n", strerror(errno));
        close(fd);
        return NETWORK_ERR;
    }

    if (listen(fd, BACKLOG) == -1){
        networkSetError(err, "listen: %s\n", strerror(errno));
        close(fd);
        return NETWORK_ERR;
    }

    readAddr(res->ai_addr, ipclient);
    printf("the listen addres is %s\n",ipclient);
    freeaddrinfo(res);

    return fd;
}
