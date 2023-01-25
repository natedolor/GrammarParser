#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include "bf.h"
#include "bv.h"
#include "city.h"

#define N_HASHES 5

struct BloomFilter {
	uint64_t salts[N_HASHES];
	uint64_t n_keys;
	uint64_t n_hits;
	uint64_t n_misses;
	uint64_t n_bits_examined;
	BitVector *filter;
};

static uint64_t default_salts[] = {
	0x5adf08ae86d36f21,
	0x419d292ea2ffd49e,
	0x50d8bb08de3818df,
	0x272347aea4045dd5,
	0x7c8e16f768811a21
};

//init bloomfilter
BloomFilter *bf_create(uint32_t size){
	BloomFilter *bf = (BloomFilter *) malloc(sizeof(BloomFilter));
	if (bf){
		bf->n_keys = bf ->n_hits = 0;
		bf->n_misses = bf ->n_bits_examined = 0;
		for (int i = 0; i < N_HASHES; i++){
			bf->salts[i] = default_salts[i];
		}
		bf->filter = bv_create(size);
		if (bf->filter == NULL){
			free(bf);
			bf = NULL;
		}
	}
	return bf;
}

//deallocate and free bloomfilter
void bf_delete(BloomFilter **bf){
	if (*bf != NULL){
		free(*bf);
		*bf = NULL;
	}
}

//bloomfilter size is bit vector length
uint32_t bf_size(BloomFilter *bf){
	return bv_length(bf->filter);
}

//insert oldspeak into bloomfilter
void bf_insert(BloomFilter *bf, char *oldspeak){
	uint64_t index;
	for (int i = 0; i < N_HASHES; i++){
		index = hash(bf->salts[i], oldspeak) % bf_size(bf);	//hashing
		bv_set_bit(bf->filter, index);				//setting bit
	}
	bf->n_keys += 1;
}

//probe to find oldspeak
bool bf_probe(BloomFilter *bf, char *oldspeak){
	uint64_t index;
	for (int i = 0; i < N_HASHES; i++){
		index = hash(bf->salts[i], oldspeak) % bf_size(bf);	//hashing
		bf->n_bits_examined += 1;
		if (bv_get_bit(bf->filter, index) == 0){		//checking if set
			bf->n_misses += 1;
			//printf("miss\n");
			return false;
		}
	}
	bf->n_hits += 1;
	return true;
}

//counts set bits
uint32_t bf_count(BloomFilter *bf){
	uint32_t set_bits = 0;
	for (uint32_t i = 0; i < bf_size(bf); i++){	//goes through all bits
		if (bv_get_bit(bf->filter, i) != 0){	//sees if bits are set
			set_bits += 1;
		}
	}
	return set_bits;
}

//prints bloomfilter
void bf_print(BloomFilter *bf){
	for (uint32_t i = 0; i < bf_size(bf); i++){
		printf("%" PRIu8 " ", bv_get_bit(bf->filter, i));
	}
	printf("\n");
}

void bf_stats(BloomFilter *bf, uint32_t *nk, uint32_t *nh, uint32_t *nm, uint32_t *ne){
	*nk = (uint32_t) bf->n_keys;
	*nh = (uint32_t) bf->n_hits;
	*nm = (uint32_t) bf->n_misses;
	*ne = (uint32_t) bf->n_bits_examined;
}

