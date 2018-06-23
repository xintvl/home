#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

int dispatch(int sockfd, char *buf) {
	int bytes = send(sockfd, buf, strlen(buf), 0);
	if (!bytes) {
		return 0;
	} else return 1;
}

int identify(int sockfd, char *buf, char *user, char *nick) {
	
	memset(buf, '\0', sizeof(buf));
	/* send nick */
	sprintf(buf, "NICK %s\r\n", nick);
	dispatch(sockfd, buf);

	/* send user */
	sprintf(buf, "USER %s 0 0 :%s\r\n", user, nick);
	dispatch(sockfd, buf);
}


int main () {
	char *nick = "testn";
	char *user = "testu";
	char *host = "chat.freenode.net";
	char *port = "6667";
	char *channel = "testsock";

	/* Create Socket */
	int sockfd;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		perror("");
	struct sockaddr_in serv_addr;
	memset(&serv_addr, '\0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(port));
	
	/* Resolve the hostname */
	struct hostent *h_serv;
	h_serv = gethostbyname(host);
	if (h_serv == NULL) {
		fprintf(stderr, "Invalid Hostname\n");
		return(1); }
	bcopy((char *)h_serv->h_addr,
			(char *)&serv_addr.sin_addr.s_addr,
			h_serv->h_length);
	char *ip = inet_ntoa(serv_addr.sin_addr);

	/* Connect to server */
	printf("Connecting to: %s (%s)\n", host, ip);
	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		perror("");
		return(1);
	} 
	char buf[1024];

	/* Send identity */	
	identify(sockfd, buf, user, nick);
:wq

	/* Join Channel */
	sprintf(buf, "JOIN #%s\r\n", channel);
	dispatch(sockfd, buf);

	/* Response from server */
	while (1) {
		memset(&buf, '\0', sizeof(buf));
		recv(sockfd, buf, 1024, 0);
		puts(buf);
	}
}

