#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

int compare_func(const void *a, const void *b)
{
  return *((int *)b) - *((int *)a);
}

int main(int argc, char* argv[])
{
    int port = atoi(argv[1]);
    char message[1024];
    struct sockaddr_in server_addr, client_addr;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    int sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        perror("Socket error");
        return -1;
    }
    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        server_addr.sin_port = 0;
        if (bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        {
            perror("Bind error");
            return -1;
        }
    }

    socklen_t server_len = sizeof(server_addr);
    getsockname(sock, (struct sockaddr *)&server_addr, &server_len);
    printf("Listening on port: %d\n", ntohs(server_addr.sin_port));

    while (1)
    {
        socklen_t client_len = sizeof(client_addr);
        int n = recvfrom(sock, message, 1024, 0, (struct sockaddr *)&client_addr, &client_len);
        qsort(message, n / sizeof(int), sizeof(int), compare_func);
        sendto(sock, message, n * sizeof(int), 0, (struct sockaddr *)&client_addr, client_len);
    }
}