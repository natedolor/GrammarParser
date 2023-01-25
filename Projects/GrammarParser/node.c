#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "node.h"

/*
struct Node {
    char *oldspeak;
    char *newspeak;
    Node *next;
    Node *prev;
};
*/
void copy(char destination[], char source[]){
	int i = 0;
	while(1){
		if (source[i] == '\0'){
			break;
		}
		destination[i] = source[i];
		i++;
	}
	destination[i] = '\0';
}

char *dup(char *string){
	if (string == NULL){
		return NULL;
	}
	int str_len = 0;
	for (str_len = 0; string[str_len] != '\0'; ++str_len)
		;
        char *str_mem = malloc(str_len + 1);
	if (str_mem == NULL){
		return NULL;
	}
	copy(str_mem, string);
	return str_mem;	
}

Node *node_create(char *oldspeak, char *newspeak){
	Node *node = (Node *) malloc(sizeof(Node));
	if (node != NULL){
		node->oldspeak = dup(oldspeak);
        	node->newspeak = dup(newspeak);
	}
	return node;	
}

void node_delete(Node **n){
	if (*n != NULL){
		free((*n)->oldspeak);
		free((*n)->newspeak);
		free(*n);
		*n = NULL;
	}
}

void node_print(Node *n){
	if (n->oldspeak != NULL && n->newspeak == NULL){
		printf("%s\n", n->oldspeak);
	}
	if (n->oldspeak != NULL && n->newspeak != NULL){
		printf("%s -> %s\n", n->oldspeak, n->newspeak);
	}
}

