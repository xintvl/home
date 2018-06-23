#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int dispatch(int sockfd, char *comm)
{
	int bytes = send(sockfd, comm, strlen(comm), 0);
	if (!bytes)
		return 0;
	return bytes;
}

void identify(char *comm, char *nick, char *user, int sockfd)
{
	sprintf(comm, "NICK %s\r\n", nick);
	dispatch(sockfd, comm);
	sprintf(comm, "USER %s 0 0 :%s\r\n", user, nick);
	dispatch(sockfd, comm);
}
void msg_chan(int sockfd, char *msg, char *channel, char *comm)
{
	sprintf(comm, "PRIVMSG %s :%s\r\n", channel, msg);
	dispatch(sockfd, comm);
}
int main()
{
	char *nick = "testn";
	char *user = "testu";
	char *host = "chat.freenode.net";
	char *port = "6667";
	char *channel = "#testsock";
	char comm[1024];
	char msg[1024];

	int sockfd;
	struct sockaddr_in serv_addr;
	struct hostent *serv;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
	{
		perror("");
	}
	serv = gethostbyname(host);
	if (serv == NULL)
	{
		fprintf(stderr, "Invalid hostname\n");
		return 1;
	}
	bcopy((char *)serv->h_addr,
			(char *)&serv_addr.sin_addr.s_addr,
			serv->h_length);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(port));
	const char *ip = inet_ntoa(serv_addr.sin_addr);
	printf("Connecting to: %s (%s)\n", host, ip);
	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		perror("");
		close(sockfd);
		return 1;
	}
	identify(comm, nick, user, sockfd);
	sprintf(comm, "JOIN %s\r\n", channel);
	dispatch(sockfd, comm);
	for (;;)
	{
		do {
			fgets(msg, sizeof(msg), stdin);
			msg[strcspn(msg, "\n")] = '\0';
			if (strcmp(msg, "quit") == 0)
				break;
			msg_chan(sockfd, msg, channel, comm);
		} while (strcmp(msg, "quit") != 0);
		break;
	}
}
