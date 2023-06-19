#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef DEBUG
#define DEBUG 0
#endif

/*strbuf_t stuf*/
typedef struct strbuf_t {
    int length;
    int used;
    char *data;
} strbuf_t;

/*strbuf_t methods*/
int sb_init(strbuf_t * L, int length) {
    L->data = malloc(sizeof(char) * length);
    if (!L->data) 
	    return 1;

    L->data[0]='\0';
    L->length = length;
    L->used   = 1;

    return 0;
}

void sb_destroy(strbuf_t *L){
    free(L->data);
}

int sb_append(strbuf_t *L, char item) {
    if (L->used == L->length) {
	size_t size = L->length * 2;
	char *p = realloc(L->data, sizeof(char) * size);
	if (!p) 
		return 1;

	L->data = p;
	L->length = size;

	if (DEBUG) 
		printf("Increased size to %lu\n", size);
    }

    L->data[L->used-1] = item;
    L->data[L->used] = '\0';
    ++L->used;

    return 0;
}

int sb_remove(strbuf_t *L, char *item)
{
    if (L->used == 0) 
	    return 1;
    --L->used;
    if (item) 
	    *item = L->data[L->used-1];
    L->data[L->used-1]='\0';

    return 0;
}


int sb_insert(strbuf_t *list, int index, char item) {
	while((index > list->length)||(list->used==list->length)){
		size_t size= list->length*2;
		char *p=realloc(list->data, sizeof(char)*size);
		if (!p) 
			return 1;
		list->data=p;
		list->length=size;
	}
	
	if (list->used > index){
		int m=list->used-1;
		while(m>=index){
			list->data[m+1]=list->data[m];
			--m;
		}
		++list->used;
	}else list->used=index+1;
	
	list->data[index]=item;

	return 0;
}

char sb_get(strbuf_t *list, int i){
	return list->data[i];
}


int sb_concat(strbuf_t *sb, char *str) {
	while((sb->used+strlen(str))>sb->length){
		size_t size= sb->length*2;
		char *p=realloc(sb->data, sizeof(char)*size);
		if (!p) 
			return 1;
		sb->data=p;
		sb->length=size;
	}
	for(int i=0; i<strlen(str); ++i)
		sb_append(sb, str[i]);
	
	return 0;
}

int sb_concatClear (strbuf_t * sb, char * str) {
	for (int i = 0; i < strlen(str); i++) {
		if (isspace(str[i]) == 0) 
			sb_append(sb, str[i]);
		else break;
	}
	return 0;
}

void sb_print(strbuf_t * sb) {
	for (int i = 0; i < sb->used; i++)
		printf("%c", sb->data[i]);
	printf("\n");
}
void sb_printList(int length, strbuf_t sb[length]) {
	for (int i = 0; i < length; i++)
		sb_print(&sb[i]);
}	
