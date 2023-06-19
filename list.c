#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/*** Types ***/
typedef struct list {
	char * key; 
	char * data;
	
   	struct list * next;
} list;
list * head; 
list * tail;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

/*** ALLOCATIONS ***/
list * allocate(char * key, char * data){
    	list * temp  = malloc(sizeof(list));

    	temp->key     = key;
    	temp->data    = data;
    	temp->next    = NULL;	
	return temp;
}

/*** INSERTS ***/
list * enqueueList (char * key, char * data){
   	pthread_mutex_lock(&lock);
	
   	list * temp = allocate(key, data);
   	if (head == NULL) {
       		head = tail = temp;
		
		pthread_mutex_unlock(&lock);
        	return temp;
    	}
   	
	tail->next = temp;
   	tail = temp;
    
   	pthread_mutex_unlock(&lock); 
	return temp;
}

list * pushList (char * key, char * data) {
	pthread_mutex_lock(&lock);
	
	list * temp = allocate(key, data);
   	if (head == NULL) {
       		head = tail = temp;
		
		pthread_mutex_unlock(&lock);
        	return temp;
    	}
   	temp->next = head;
	head = temp;

	pthread_mutex_unlock(&lock); 
	return temp;
}

/*** REMOVES ***/
int popList() {
	pthread_mutex_lock(&lock);

	if (head == NULL)  {
		pthread_mutex_unlock(&lock);
		return 1;
	}
 	head = head->next;       
    	pthread_mutex_unlock(&lock); 
    	return 0;
}

int deleteEntryList (list * entry) {
	pthread_mutex_lock(&lock);
	list * temp = head;
	while (temp != entry && temp->next != entry && temp != NULL) 
		temp = temp->next;

	if (temp == NULL) {
		pthread_mutex_unlock(&lock);
		return 1;
	}
	
	temp->next = entry->next;
	pthread_mutex_unlock(&lock); 
	return 0;
}

/*** TOOLS ***/
list * findList (char * key) { 
	list * temp = head; 
	while (temp != NULL && strcmp(temp->key, key) != 0) 
		temp = temp->next;
	return temp;
}

/*** PRINTS ***/
void printList () {
	list * tempKey = head;
	while(tempKey != NULL) {
		printf("%s\t%s\n", tempKey->key, tempKey->data);
		tempKey = tempKey->next;
	}
}
