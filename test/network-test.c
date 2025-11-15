#include "../src/network.h"

void launch(char *err, int sockfd, char *port)
{
    struct sockaddr_storage clientiptest;
    printf("================= WAITING FOR CONNECTION ================\n");
    networkAccept(err, sockfd, &clientiptest, port);
}

int main(void)
{
    char *err, *port = "5678";
    int sockfd;

    sockfd = networkTcpServer(err, port , NULL);
    return 0;
}
