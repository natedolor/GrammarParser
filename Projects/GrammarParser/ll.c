#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "ll.h"

struct LinkedList{
	uint32_t length;
	Node *head;
	Node *tail;
	bool mtf;	
};

uint32_t ll_seeks = 0;
uint32_t ll_links = 0;

//helper function compares strings for ll
int compare(char string1[], char string2[]){
	int i = 0;
	while (string1[i] == string2[i]){
		if (string1[i] == '\0'){
			break;
		}
		if (string2[i] == '\0'){
			break;
		}
		i += 1;
	}
	if (string1[i] == '\0' && string2[i] == '\0'){
		return 1;
	}else{
		return 0;
	}
	/*int i = 0;
	while (string1[i] != '\0' && string2[i] != '\0'){
		if (string1[i] != string2[i]){
			return 0;
		}
		i += 1;
	}
	return 1;*/
}

//init ll
LinkedList *ll_create(bool mtf){
	LinkedList *ll = (LinkedList *) malloc(sizeof(LinkedList));
	if (ll != NULL){
		ll->mtf = mtf;
		ll->head = node_create(NULL, NULL);
		ll->tail = node_create(NULL, NULL);
		if (ll->head == NULL || ll->tail == NULL){
			free(ll);
			ll = NULL;
		}
		ll->head->next = ll->tail;
		ll->tail->prev = ll->head;
	}
	return ll;
}

//deallocate and free ll
void ll_delete(LinkedList **ll){
	if (*ll != NULL){
		(*ll)->head = (*ll)->head->next;
		node_delete(&(*ll)->head->prev);
		//goes through every single node to delete
		while((*ll)->head->oldspeak != NULL){
	                Node *next_head;
        	        next_head = (*ll)->head->next;
                	node_delete(&(*ll)->head);
                	(*ll)->head = next_head;
		}
        }
	node_delete(&(*ll)->head);
	free(*ll);
	*ll = NULL;
}

//length of ll
uint32_t ll_length(LinkedList *ll){
	uint32_t len = 0;
	Node *node = ll->head->next;
	//goes through all nodes as long as node has oldspeak
	//if (node != NULL){
		while (node->oldspeak != NULL){
			len += 1;
			node = node->next;
		}
	//}
	return len;
}

Node *ll_lookup(LinkedList *ll, char *oldspeak){
	Node *node = ll->head->next;
	if (node != NULL){
		while (node->oldspeak != NULL){
			if (compare(node->oldspeak, oldspeak) == 1){
				if (ll->mtf == true){
					node->prev->next = node->next;
					node->next->prev = node->prev;
					ll_insert(ll, node->oldspeak, node->newspeak);
				}
				return node;
			}
			node = node->next;
		}
		return NULL;
	}
	return NULL;
}

void ll_insert(LinkedList *ll, char *oldspeak, char *newspeak){
	if (ll_lookup(ll, oldspeak) != NULL){
		return;
	}
	Node *node = node_create(oldspeak, newspeak);
	ll->head->next->prev = node;
	node->next = ll->head->next;
	node->prev = ll->head;
	ll->head->next = node;
}

void ll_print(LinkedList *ll){
	Node *node = ll->head->next;
	if (node != NULL){
		while (node->oldspeak != NULL){
			node_print(node);
			node = node->next;
		}
	}
}

void ll_stats(uint32_t *n_seeks, uint32_t *n_links){
	*n_seeks = ll_seeks;
	*n_links = ll_links;
}

