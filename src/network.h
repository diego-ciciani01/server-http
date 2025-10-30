#define NETWORK_OK 0
#define NETWORK_ERR -1
#define NETWORK_ERR_LEN 256

#define BACKLOG 5

int networkTcpServer(char *err, char port, char *bindaddr);
static void networkSetError(char *err, const char *fmt, ...);
void readAddr(struct sockaddr *addr, char *ipclient);
int networkAccept(char *err, int sockfd, char *ip, char *port);
int networkSend(char *err, int sockfd, void *buf);
int networkRecv(char *err, int sockfd, void *buf);

