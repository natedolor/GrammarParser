#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include "ht.h"
#include "ll.h"
#include "city.h"

struct HashTable{
	uint64_t salt;
	uint32_t size;
	uint32_t n_keys;
	uint32_t n_hits;
	uint32_t n_misses;
	uint32_t n_examined;
	bool mtf;
	LinkedList **lists;
};

//allocate mem for ht
HashTable *ht_create(uint32_t size, bool mtf){
	HashTable *ht = (HashTable *) malloc(sizeof(HashTable));
	if (ht != NULL){
		ht->mtf = mtf;
		ht->salt = 0x9846e4f157fe8840;
		ht->n_hits = ht->n_misses = ht->n_examined = 0;
		ht->n_keys = 0;
		ht->size = size;
		ht->lists = (LinkedList **) calloc(size, sizeof(LinkedList *));
		if (!ht->lists){
			free(ht);
			ht = NULL;
		}
	}
	return ht;
}

//size of ht
uint32_t ht_size(HashTable *ht){
        return ht->size;
}

//deallocate and free ht
void ht_delete(HashTable **ht){
	if (*ht != NULL){
		for (uint32_t i = 0; i < (*ht)->size; i++){
			if((*ht)->lists[i] != NULL){
				ll_delete(&(*ht)->lists[i]);
			}
		}
		free((*ht)->lists);
		free(*ht);
		*ht = NULL;
	}
}

//lookups up if oldspeak is in ht
//seeks and links are for stats in ll
//uint32_t seeks = 0, links = 0;
Node *ht_lookup(HashTable *ht, char *oldspeak){
	uint32_t seeks = 0, links = 0;
	ll_stats(&seeks, &links);
	uint32_t seek_count = seeks;
	uint32_t links_count = links;
	uint32_t index = hash(ht->salt, oldspeak) % ht_size(ht);
	Node *n = ll_lookup(ht->lists[index], oldspeak);
	//ll_stats(&seeks, &links);
	//ht->*lists->n_examined += seeks - past_seeks;
	if (n == NULL){		//if null returns null and offsets seeks and link
		ll_stats(&seeks, &links);
		seeks = seek_count - seeks;
		links = links_count - links;
		ht->n_misses += 1;
		ht->n_examined += links;
		return NULL;
	}else{			//if not null return node 
		ll_stats(&seeks, &links);
		seeks -= seek_count;
		links -= links_count;
		ht->n_hits += 1;
		ht->n_examined += links; 
		return n;
	}
}

void ht_insert(HashTable *ht, char *oldspeak, char *newspeak){
	uint32_t index = hash(ht->salt, oldspeak) % ht_size(ht);
	if (ht->lists[index] == NULL){
		ht->lists[index] = ll_create(ht->mtf);
		//ht->n_keys += 1;
	}
	ll_insert(ht->lists[index], oldspeak, newspeak);
	uint32_t prior_len = ll_length(ht->lists[index]);
	if (prior_len == ll_length(ht->lists[index])) {
		return;
	}
	ht->n_keys += 1;
}

uint32_t ht_count(HashTable *ht){
	uint32_t count = 0;
	for (uint32_t i = 0; i < ht_size(ht); i++){
		if (ht->lists[i] != NULL){
			count += 1;
		}
	}
	return count;
}

void ht_print(HashTable *ht){
	for (uint32_t i = 0; i < ht_size(ht); i++){
		if (ht->lists[i] == NULL){
			printf("NULL\t%d index\n\n", i);
		}else{
			printf("ll\t%d index\n", i);
			ll_print(ht->lists[i]);
		}
	}
}

void ht_stats(HashTable *ht, uint32_t *nk, uint32_t *nh, uint32_t *nm, uint32_t *ne){
        *nk = (uint32_t) ht->n_keys;
        *nh = (uint32_t) ht->n_hits;
        *nm = (uint32_t) ht->n_misses;
        *ne = (uint32_t) ht->n_examined;
}

