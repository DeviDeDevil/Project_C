CC = gcc
CFLAGS = -g
LIBFLAGS = 
CLIENT = client 
SERVER = server 
FUNCTIONS = functions

all: $(CLIENT) $(SERVER)

client: client.o functions.o
	@$(CC) -o $@ $^ $(LIBFLAGS)

server: server.o functions.o
	@$(CC) -o $@ $^ $(LIBFLAGS)

%.o: %.c
	@$(CC) -o $@ -c $^ $(CFLAGS)

.PHONY: clean

clean:
	@rm -rf *.o