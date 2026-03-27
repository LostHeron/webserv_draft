#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int	main(int argc, char **argv)
{
	struct sockaddr_in	addr_data;
	int					s;
	int					ret;

	
	// checking a port i given
	if (argc != 2)
	{
		dprintf(2, "usage: ./program port\n");
		return (1);
	}
	// initializing struct
	memset(&addr_data, 0, sizeof(addr_data));

	// we want an IPv4 address
	addr_data.sin_family = AF_INET;

	// we get the port given by the user and put it in network order
	addr_data.sin_port = htons((short)atoi(argv[1]));

	// getting and addr corresponding to the loopback address
	// and putting it in network order
	uint32_t tmp_addr;
	tmp_addr = 0;
	// uncomment following lines to listen only to 
	// loop back interface
	/*
	tmp_addr = 127 << (8 * 3);
	tmp_addr += 2;
	*/
	// the following would make the bind call fail,
	// because not a local address
	// tmp_addr = (151 << (8 * 3)) + 3;
	addr_data.sin_addr.s_addr = htonl(tmp_addr);

	// creating a socket that will take
	// and IPv4 address, and using a SOCK_STREAM,
	// with this two paramaters, no need to specify
	// protocol (i think so, but not 100% sure),
	// it will automatically be a socket of TCP type.
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s < 0)
	{
		perror("error socket");
		return (1);
	}
	printf("socket successfully created\n");

	// binding the socket to the local address and port 
	// given by the user, using the data previously entered in addr_data
	ret = bind(s, (struct sockaddr *)&addr_data, sizeof(addr_data));
	if (ret < 0)
	{
		perror("error bind");
		return (1);
	}
	printf("successfully bound socket\n");

	// putting that socket in listening mode, to accept 
	// connection comming from outside
	ret = listen(s, 50);
	if (ret < 0)
	{
		perror("error listen");
		return (1);
	}
	printf("socket is now listening for entering connection");

	printf("\nwaiting for incomming connection ...\n");
	struct sockaddr	peer_addr;
	unsigned int	peer_len;

	// waiting for connection comming from the outside world
	int peer_fd = accept(s, &peer_addr, &peer_len); 
	if (peer_fd < 0)
	{
		perror("accept");
	}
	printf("a connection was accecpted\n");

	// reading data send by the peer
	char buf[BUFSIZ];
	int	nb_read = read(peer_fd, buf, BUFSIZ);
	if (nb_read < 0)
	{
		perror("read");
		return (1);
	}

	printf("the other end sent:\n'%.*s'\n", nb_read, buf);

	sleep(10);
	close(peer_fd);
	close(s);
}
