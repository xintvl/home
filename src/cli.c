#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>

int mitto(int sockfd, char *misv)
{
	int bytes = send(sockfd, misv, strlen(misv), 0);
	if (!bytes)
		return 0;
	return bytes;
}

int main()
{
	char *host = "chat.freenode.net";
	short unsigned int port = 6667;
	char *nick = "";
	char *user = "";
	char *chan = "";
	char *misv;

	struct sockaddr_in serv_addr;
	struct hostent *serv;
	serv = gethostbyname(host);
	if (serv == NULL) {
		fprintf(stderr, "Invalid Hostname\n");
		return 0;
	}
	int sockfd;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		return 0;
	bcopy((char *)serv->h_addr,
			(char *)&serv_addr.sin_addr.s_addr,
			serv->h_length);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	if (port < 0)
		return 0;
	const char *ip = inet_ntoa(serv_addr.sin_addr);
	printf("Connecting to: %s(%s:%d)\n", host, ip, port);
	close(sockfd);
}
