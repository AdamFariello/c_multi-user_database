#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <signal.h>
#include <pthread.h>

#include "list.c"
#include "strbuf.c"

int running=1;
struct conArg{
	struct sockaddr_storage addr;
	socklen_t addr_len;
	int fd;
};

void handler(int signal){
	running=0;
}

void set (struct conArg * con, int commandLength) {
	char key[200];
	read(con->fd, key, 200);
	strbuf_t strbufKey;
	sb_init(&strbufKey, 1);
	sb_concatClear(&strbufKey, key);
		
	char data [200];
	read(con->fd, data, 200);
	strbuf_t strbufData;
	sb_init(&strbufData, 1);
	sb_concatClear(&strbufData, data);

	commandLength -= (strlen(strbufKey.data) + strlen(strbufData.data));	
	commandLength-=2;
	if (commandLength < 0) {
		printf("ERR\nLEN\n");
		return;
	}

	list *temp = findList(strbufKey.data);
	if (temp == NULL)
		temp=enqueueList(strbufKey.data, strbufData.data);

	//String SetUp
	strbuf_t strbuf[3];
	for (int i=0; i<3; i++)
		sb_init(&strbuf[i], 1);
	sb_concat(&strbuf[0], "OKS\n");
	sb_concat(&strbuf[1], temp->key);
	sb_append(&strbuf[1], '\n');
	sb_concat(&strbuf[2], temp->data);
	sb_append(&strbuf[2], '\n');	
	
	//Allows printf to function normally again
	if (strstr(strbuf[1].data, "NULL") == NULL && strstr(strbuf[2].data, "NULL") == NULL)
		for (int i = 0; i < 3; i++) 
			printf("%s", strbuf[i].data);	
	else
		printf("ERR\nSRV\n");
	//printf("\n");
}

void get (struct conArg * con, int commandLength) {
	char key [200]; 
	read(con->fd, key, 200);
	strbuf_t strbufKey;
	sb_init(&strbufKey, 1);
	sb_concatClear(&strbufKey, key);
	
	commandLength -= strlen(strbufKey.data);
	commandLength -= 1;
	if (commandLength < 0) {
		printf("ERR\nLEN\n");
		return;
	}

	//String SetUp
	strbuf_t strbuf[3];
	for (int i = 0; i < 3; i++) 
		sb_init(&strbuf[i], 1);
	
	list * temp = findList(strbufKey.data);
	if (temp != NULL) {
		sb_concat(&strbuf[0], "OKG\n");
		sb_concat(&strbuf[1], temp->key);
		sb_append(&strbuf[1], '\n');
		sb_concat(&strbuf[2], temp->data);
		sb_append(&strbuf[2], '\n');	
	}
	else {
		sb_concat(&strbuf[0], "ERR\n");
		sb_concat(&strbuf[1], "KNF");
		sb_concat(&strbuf[2], "\n");
	}

	//Print
	if (strstr(strbuf[1].data, "NULL") == NULL && strstr(strbuf[2].data, "NULL") == NULL)
		for (int i = 0; i < 3; i++) 
			printf("%s", strbuf[i].data);	
	else
		printf("ERR\nSRV\n");
	//printf("\n");
}

void del (struct conArg * con, int commandLength) {
	char key [200]; 
	read(con->fd, key, 200);
	strbuf_t strbufKey;
	sb_init(&strbufKey, 1);
	sb_concatClear(&strbufKey, key);
      
	commandLength -= strlen(strbufKey.data);
	commandLength -= 1;
	if (commandLength < 0) {
		printf("ERR\nLEN\n");
		return;
	}

	//String SetUp
	strbuf_t strbuf[3];
	for (int i = 0; i < 3; i++) 
		sb_init(&strbuf[i], 1);

	list * temp = findList(strbufKey.data);
	if (temp != NULL) {
		sb_concat(&strbuf[0], "OKD\n");
		sb_concat(&strbuf[1], temp->key);
		sb_append(&strbuf[1], '\n');
		sb_concat(&strbuf[2], temp->data);
		sb_append(&strbuf[2], '\n');	
		deleteEntryList(temp);
	}
	else {
		sb_concat(&strbuf[0], "ERR\n");
		sb_concat(&strbuf[1], "KNF");
		sb_concat(&strbuf[2], "\n");
	}

	//Print
	if (strstr(strbuf[1].data, "NULL") == NULL && strstr(strbuf[2].data, "NULL") == NULL)
		for (int i = 0; i < 3; i++) 
			printf("%s", strbuf[i].data);	
	else
		printf("ERR\nSRV\n");
	//printf("\n");
}

/*server*/
void *switcher(void *arg){
	//swtich to the method
	char host[100], port[10];
	struct conArg *con=(struct conArg *)arg;
	
	int name=getnameinfo((struct sockaddr *) &con->addr, con->addr_len, host, 100, port, 10, NI_NUMERICSERV);
	if (name!=0){
		printf("Get name error\n");
		close(con->fd);
		return NULL;
	}
	printf("[%s:%s]connected\n", host, port);

	int readc;
	char command[200];
	char command2[200];	
	while ((readc = read(con->fd, command, 200)) > 0) {		
		read(con->fd, command2, 200);
		int commandLength = atoi(command2);

		strbuf_t temp;
		sb_init(&temp, 1);
		sb_concatClear(&temp, command);
       	
		printf("\n");
		if (strcmp(temp.data, "SET") == 0) 
			set(con, commandLength);
		else if(strcmp(temp.data, "GET") == 0)
			get(con, commandLength);
		else if(strcmp(temp.data, "DEL") == 0) 
			del(con, commandLength);
		else { 
			printf("ERR\nBAD\n");
			if (--temp.used != 3)
				printf("ERR\nLEN\n");
			//printf("\n");
		}
	}
	printf("ERR\nKDE\n");
	close(con->fd);
	free(con);
	return NULL;
}

int server(char* port){
	//hints
	struct addrinfo hint, *resInfo;
	memset(&hint, 0, sizeof(struct addrinfo));
	hint.ai_family=AF_UNSPEC;
	hint.ai_socktype=SOCK_STREAM;
	hint.ai_flags=AI_PASSIVE;

	int err=getaddrinfo(NULL, port, &hint, &resInfo);
	if(err!=0){
		printf("error: getaddrinfo\n");
		return -1;
	}
	
	//socket
	struct addrinfo *ptr;
	int sfd;
	//ptr->ai_protocol -> ptr = ptr = ptr->ai_protocol
	for (ptr=resInfo; ptr!=NULL; ptr = ptr->ai_next){
		sfd=socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (sfd==-1){
			printf("Fail to create socket\n");
			continue;
		}

		//bind and listen
		int b=bind(sfd, ptr->ai_addr, ptr->ai_addrlen);
		int l=listen(sfd, 5);
		if (b==0 && l==0){
			//printf("Bind and listen is good\n");
			break;
		}
		close(sfd);
	}

	if (ptr==NULL){
		printf("No bind\n");
		return -1;
	}
	freeaddrinfo(resInfo);

	struct sigaction act;
	act.sa_handler = handler;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	sigaction(SIGINT, &act, NULL);
	sigset_t mask;						
	sigemptyset(&mask);
	sigaddset(&mask, SIGINT);

	struct conArg *arg;
	pthread_t conThr;
	while (running){
		//printf("Waiting for connection\n");
		arg=malloc(sizeof(struct conArg));
		arg->addr_len=sizeof(struct sockaddr_storage);
		arg->fd=accept(sfd, (struct sockaddr *)&arg->addr, &arg->addr_len);
		if (arg->fd==-1){
			//printf("Fail to accept. sfd:%d\n", sfd);
			perror("accept");
			continue;
		}

		int thr=pthread_create(&conThr, NULL, switcher, arg);
		if (thr!=0){
			printf("Fail to create thread\n");
			close(arg->fd);
			free(arg);
			continue;
		}
		pthread_detach(conThr);
	}


	return 0;
}

int main (int argc, char **argv) {
	if (argc!=2){
		printf("argument line error\n");
		return EXIT_FAILURE;
	}

	if (server(argv[1]) == -1) {
		printf("Fail to run server\n");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
