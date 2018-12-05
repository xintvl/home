#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

void pong(char *buf, int sockfd) 
{
    if (!strncmp(buf, "PING ", 5)) {
        buf[1] = 'O';
        send(sockfd, buf, strlen(buf), 0);
    }
}
int mitto(int sockfd, char *misv)
{
    int bytes = send(sockfd, misv, strlen(misv), 0);
    if (!bytes)
        return(0);
    printf("%d bytes sent.\n", bytes);
    return bytes;
    memset(misv, 0, sizeof(misv));
}
void identify(char *misv, char *nick, char *user, int sockfd)
{
    sprintf(misv, "NICK %s\r\n", nick);
    mitto(sockfd, misv);
    sprintf(misv, "USER %s 0 0 :%s\r\n", user, nick);
    mitto(sockfd, misv);
}
void join(int sockfd, char *chan, char *misv)
{
    sprintf(misv, "JOIN %s\r\n", chan);
    mitto(sockfd, misv);
}
void msg_chan(int sockfd, char *relay, char *chan, char *misv)
{
	sprintf(misv, "PRIVMSG %s :%s\r\n", chan, relay);
	mitto(sockfd, misv);
}
int main() 
{
    char *host = "localhost";
    short unsigned int port = 6667;
    char *nick = "testnick";
    char *user = "testuser";
    char *chan = "#NET";
    char misv[512];
    char relay[512];
    char buf[512];

    struct sockaddr_in serv_addr;
    struct hostent *h_name;
    h_name = gethostbyname(host);
    if (h_name == NULL) 
    {
        fprintf(stderr, "Invalid Hostname\n");
        return(0);
    }
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        close(sockfd);
    }
    bcopy((char *)h_name->h_addr,
            (char *)&serv_addr.sin_addr.s_addr,
            h_name->h_length);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (port < 0 || port > 65535)
        return(0);
    const char *ip = inet_ntoa(serv_addr.sin_addr);
    printf("Connecting to: %s (%s:%d)\n", host, ip, port);
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        fprintf(stderr, "Connection Refused\n");
        close(sockfd);
        return (0);
    }
    identify(misv, nick, user, sockfd);
    while (recv(sockfd, buf, sizeof(buf), 0) > 0) 
    {
        join(sockfd, chan, misv);
        printf("%s", buf);
        // fgets(relay, sizeof(relay), stdin);
        // relay[strcspn(relay, "\n")] = '\0';
        // //if (strcmp(relay, "/quit") == 0)
        //    break;
        pong(buf, sockfd);
        // msg_chan(sockfd, relay, chan, misv);
        //memset(relay, 0, sizeof(relay));
        memset(buf, 0, sizeof(buf));
    }
    close(sockfd);
}
