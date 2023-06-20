#One day we'll add -fsanitize=address ...
CC = gcc
CFLAGS = -g -Wall -Werror -gdwarf-2 -lpthread 

all: server
allNew: serverNew 
allDepends: list strbuf

server: server.c 
	$(CC) $(CFLAGS) server.c -o server

serverNew: server.new.c
	$(CC) $(CFLAGS) server.new.c -o server

list: list.c
	$(CC) $(CFLAGS) list.c -o list
	rm -f list

strbuf: strbuf.c
	$(CC) $(CFLAGS) strbuf.c -o strbuf
	rm -f strbuf

clean: 
	rm -f server
