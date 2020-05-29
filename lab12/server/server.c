#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>

// OFFICIALNO SKOMMUNIJJENO SO STACKOVERFLOW
long getFileSize(FILE* file)
{
    fseek(file, 0L, SEEK_END);
    long size = ftell(file);
    rewind(file);
    return size;
}

void print_help()
{
    printf("Commands to use:\n");
    printf("/help - shows existing commangs\n");
    printf("/cls - clear console\n");
    printf("/exit - close the program\n");
}

int main(int argc, char* argv[])
{
    int port = atoi(argv[1]);
    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    int sock = socket(PF_INET, SOCK_STREAM, 0);
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
    listen(sock, 100);
    getsockname(sock, (struct sockaddr *)&server_addr, &server_len);
    printf("Listening on port: %d\n", ntohs(server_addr.sin_port));

    if (fork() == 0)
    {
        while (1)
        {
            struct sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            int newSock = accept(sock, (struct sockaddr *)&client_addr, &client_len);
            
            if (fork() == 0)
            {
                continue;
            }
            while (1)
            {
                char input[128];
                int n = read(newSock, input, sizeof(input));
                if (n == 0)
                {
                    close(newSock);
                    exit(0);
                }

                FILE* file = fopen(input, "r");
                if (!file)
                {
                    long fileSize = -1 ;
                    write(newSock, &fileSize, sizeof(fileSize));
                }
                else
                {
                    long fileSize = getFileSize(file);
                    write(newSock, &fileSize, sizeof(fileSize));
                    char message[fileSize];
                    fread(message, sizeof(char), fileSize, file);
                    fclose(file);
                    write(newSock, message, sizeof(message));
                }
            }
        }
    }
    else
    {
        print_help();
        char message[128];
        while (1)
        {
            scanf("%s", message);

            if (!strcmp(message, "/help"))
            {
                print_help();
            }
            else if (!strcmp(message, "/cls"))
            {
                system("clear");
                print_help();
            }
            else if (!strcmp(message, "/exit"))
            {
                close(sock);
                exit(0);
            }
            else
            {
                printf("You entered a nonexistent command");
            }
        }
    }
}