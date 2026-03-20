#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("./server port");
		return (1);
	}
	printf("AF_INET = %i\n", AF_INET);
	printf("AF_INET6 = %i\n", AF_INET6);
	printf("SOCK_STREAM = %i\n", SOCK_STREAM);
	printf("SOCK_SEQPACKET = %i\n", SOCK_SEQPACKET);
	printf("SOCK_DGRAM = %i\n", SOCK_DGRAM);
	struct addrinfo *list;
	struct addrinfo	param;
	param.ai_family = AF_INET;
	param.ai_socktype = 0;
	param.ai_protocol = 0;
	param.ai_flags = AI_PASSIVE;
	memset(&param, 0, sizeof(param));
	char *port = argv[1];
	int	a = getaddrinfo(NULL, port, &param, &list);
	if (a != 0)
	{
		gai_strerror(a);
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
	
	// socket creation :
	int fd = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);
	if (fd < 0)
	{
		perror("socket: ");
		freeaddrinfo(list);
		return (1);
	}
	printf("socket created successfully\n");

	// bind creation :
	int bfd = bind(fd, tmp->ai_addr, tmp->ai_addrlen);
	if (bfd != 0)
	{
		perror("bind: ");
		close(fd);
		freeaddrinfo(list);
		return (1);
	}
	printf("successfully bind socket\n");

	int lfd = listen(fd, 50);
	if (lfd != 0)
	{
		perror("listen: ");
		close(fd);
		freeaddrinfo(list);
		return (1);
	}
	printf("socket is now listening for entering connection\n");

	struct sockaddr peer_addr;
	socklen_t peer_addr_len;
	int peer_fd = accept(fd, &peer_addr, &peer_addr_len);
	if (peer_fd < 0)
	{
		perror("accept");
		goto end;
	}
	printf("received an external connection\n");

	char *str = "coucou from server";
	if (write(peer_fd, str, strlen(str)) < 0)
		perror("write");
	else
	{
		printf("client as sent '%s' to peer fd\n", str);
	}

	close(peer_fd);
	end:
	sleep(10);
	close(fd);
	freeaddrinfo(list);
}
