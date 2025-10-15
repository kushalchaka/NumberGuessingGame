#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

void pexit(char *errmsg) {
        fprintf(stderr, "%s\n", errmsg);
        exit(1);
}

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;

    char buffer[1025];
    time_t ticks;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
                pexit("socket() error.");

    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(buffer, '0', sizeof(buffer));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

        int port = 4999;
        do {
                port++;
        serv_addr.sin_port = htons(port);
    } while (bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0);
        printf("bind() succeeds for port #%d\n", port);

    if (listen(listenfd, 10) < 0)
                pexit("listen() error.");

        int counter=0;
    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
                counter++;
                printf("connected to client %d.\n", counter);
                if (fork() > 0) {
                        close(connfd);
                        continue;
                }
                        int low = 1, high = 100;

                        while (low <= high) {
                                int mid = (low + high) / 2;
                                char guess_msg[50];
                snprintf(guess_msg, sizeof(guess_msg), "Is it %d (<,>,=)? ", mid);
                write(connfd, guess_msg, strlen(guess_msg));
                                char response[11];
                                int n = read(connfd, buffer, sizeof(buffer)-1);
                if (n <= 0) break;
                buffer[n] = '\0';
                                if (buffer[0] == '<')
                                        high = mid - 1;
                                else if (buffer[0] == '>')
                                        low = mid + 1;
                                else if (buffer[0] == '=') {
                                        char *win = "Great game!\n";
                                        write(connfd, win, strlen(win));
                                        break;
                                }
                                else {
                                        char *error = "Bad input\n";
                                        write(connfd, error, strlen(error));
                                }

                        }
                printf("served client %d.\n", counter);
        close(connfd);
                exit(0);
     }
}
