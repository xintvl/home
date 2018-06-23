#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

int mitto(int sockfd, char *misv) {
	int bytes = send(sockfd, misv, strlen(misv), 0);
	if (!bytes)
		return 0;
	return bytes;
}

int main()
{
	char *nick = "";
	char *user = "";
	char *host = "chat.freenode.net";
	char *port = "";
	char *chan = "";


	struct sockaddr_in serv_addr;
	struct hostent *h_in;
	h_in = gethostbyname(host);
	if (h_in == NULL) {
		fprintf(stderr, "Invalid Hostname\n");
		return 0;
	}
	int sockfd;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		return 0;
	bcopy((char *)h_in->h_addr,
		(char *)&serv_addr.sin_addr.s_addr,
		h_in->h_length);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(port));
	const char *ip = inet_ntoa(serv_addr.sin_addr); //comment
	if (port < 0)
		return 0;
	printf("Connecting to: %s (%s:%s)\n", host, ip, port);

	close(sockfd);
}
