CLIENT := client
SERVER := server
CC := cc
CFLAGS := -Wall -Wextra -Werror -g3

all: $(CLIENT) $(SERVER)

$(CLIENT):  first_test_listening_socket_client.c
	$(CC) $(CFLAGS) first_test_listening_socket_$@.c -o $@.out

$(SERVER):  first_test_listening_socket_server.c
	$(CC) $(CFLAGS) first_test_listening_socket_$@.c -o $@.out
