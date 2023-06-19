#One day we'll add -fsanitize=address ...
CC = gcc
CFLAGS = -g -Wall -Werror -gdwarf-2 -lpthread 

all: server
all2: list strbuf 

server: server.c 
	$(CC) $(CFLAGS) server.c -o server

list: list.c
	$(CC) $(CFLAGS) list.c -o list
	rm -f list

strbuf: strbuf.c
	$(CC) $(CFLAGS) strbuf.c -o strbuf
	rm -f strbuf

clean: 
	rm -f server
