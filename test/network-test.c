#include "../src/network.h"

void launch(int sockfd)
{
    struct sockaddr_storage client_test;
    printf("================= WAITING FOR CONNECTION ================\n");
    if(accept(sockfd, (struct sockaddr*) &client_test, sizeof(client_test)) == -1){
        perror("acept");
        continue;
    }


}


int main(void)
{
    char *err, *port = "5678";
    int sockfd;

    sockfd = networkTcpServer(err, port , NULL);
    return 0;
}
