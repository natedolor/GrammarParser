#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include "bv.h"

struct BitVector{
	uint32_t length;
	uint64_t *vector;
};

//allocate for bv
BitVector *bv_create(uint32_t length){
	BitVector *bv = (BitVector *) calloc((length/64) + 1, sizeof(BitVector));
	bv->length = length;
	bv->vector = (uint64_t *) calloc(length, sizeof(uint64_t));
	if (bv == NULL){
		free(bv);
		bv = NULL;
	}
	return bv;
}

//deallocate bv
void bv_delete(BitVector **bv){
	if (*bv != NULL){
                free(*bv);
		//free vector later
                *bv = NULL;
        }

}

//bit vector length 
uint32_t bv_length(BitVector *bv){
	return bv->length;
}

//set bit shifting left
void bv_set_bit(BitVector *bv, uint32_t i){
	// i/64 is the location and i%64 is the byte
	bv->vector[i/64] |= (1UL << (i % 64));
	//bv->vector[i/64] = bv->vector[i/64] >> (i % 64) | ul;	//shift to bit to set
	//bv->vector[i/64] = bv->vector[i/64] << i % 64; 	//shift back
}

//clear bit by shifting left to bit
void bv_clr_bit(BitVector *bv, uint32_t i){
	//i/64 is the location and i%64 is the byte
	bv->vector[i/64] &= (1UL << (i % 64));
        //bv->vector[i/64] = bv->vector[i/64] >> (64 - (i % 64)) & 0;	//shift to bit to clear
        //bv->vector[i/64] = bv->vector[i/64] << (64 - (i % 64)); 	//shift back

}

//gets bit by shifting to right
uint8_t bv_get_bit(BitVector *bv, uint32_t i){
        return bv->vector[i/64] >> (i % 64) & 1UL; //shift to bit to get bit

}

//print out bitvector
void bv_print(BitVector *bv){
	for (uint32_t i = 0; i < bv_length(bv); i++){
		printf("%lu ", bv->vector[i]);
	}
	printf("\n");
}

