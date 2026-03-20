#include <netinet/in.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SIZE 100

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("./client port");
		return (1);
	}
	printf("AF_INET = %i\n", AF_INET);
	printf("AF_INET6 = %i\n", AF_INET6);
	printf("SOCK_STREAM = %i\n", SOCK_STREAM);
	printf("SOCK_SEQPACKET = %i\n", SOCK_SEQPACKET);
	printf("SOCK_DGRAM = %i\n", SOCK_DGRAM);
	struct addrinfo *list;
	struct addrinfo	param;
	memset(&param, 0, sizeof(param));
	param.ai_family = AF_INET;
	param.ai_socktype = 0;
	param.ai_protocol = 0;
	//param.ai_flags = AI_PASSIVE;
	char *port = argv[1];
	int	a = getaddrinfo(NULL, port, &param, &list);
	if (a != 0)
	{
		printf("getaddrinfo: %s\n", gai_strerror(a));
		return (1);
	}
	struct addrinfo *tmp;
	for (tmp = list; tmp; tmp = tmp->ai_next)
	{
		printf("got a list !\n");
		printf("tmp->ai_family = %i\n", tmp->ai_family);
		printf("tmp->ai_socktype = %i\n", tmp->ai_socktype);
		printf("tmp->ai_protocol = %i\n", tmp->ai_protocol);
		printf("\n");
		if (tmp->ai_family == AF_INET)
			break;
	}
	// ok so here my tmp of ipv4 with of type SOCK_STREAM
	// and with protocol TCP
	// ok so here I should be able to bind this socket 
	// aaah and after the bind i need to listen i guess, to be able to accept 
	// connection ?
	// ok no, i can just create a socket with the info in tmp:
	
	uint16_t reverse_port = ((struct sockaddr_in *)tmp->ai_addr)->sin_port;
	char *tt = (char *)&reverse_port;
	char ttmp = tt[0];
	tt[0] = tt[1];
	tt[1] = ttmp;
	
	printf("port that will be used is %i\n", reverse_port);

	// socket creation :
	int fd = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);
	if (fd < 0)
	{
		perror("socket: ");
		freeaddrinfo(list);
		return (1);
	}
	printf("socket created successfully\n");

	int ret = connect(fd, tmp->ai_addr, tmp->ai_addrlen);
	if (ret != 0)
	{
		perror("connect: ");
		freeaddrinfo(list);
		return (1);
	}
	printf("successfully connected to the socket\n");
	
	char buffer[BUF_SIZE];
	memset(buffer, 0, BUF_SIZE);
	if (read(fd, buffer, BUF_SIZE) < 0)
		perror("read");
	else
		printf("client as read '%s' from server", buffer);


	close(fd);
	freeaddrinfo(list);
}
