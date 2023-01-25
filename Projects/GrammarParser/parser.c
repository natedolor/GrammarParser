#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <ctype.h>

#include "parser.h"

#define MAX_PARSER_LINE_LENGTH 1000

struct Parser{
	FILE *f;
	char current_line[MAX_PARSER_LINE_LENGTH + 1];
	uint32_t line_offset;
};

//init parser
Parser *parser_create(FILE *f){
	Parser * p = (Parser *) malloc(sizeof(Parser));
	if (p != NULL){
		p->f = f;
		p->line_offset = 0;
	}
	return p;
}

//deallocate and free parser
void parser_delete(Parser **p){
	if (*p != NULL){
	free(*p);
	*p = NULL;
	}
}

//returns whether word exists after next word
bool next_word(Parser *p, char *word){
	while(1){
		uint32_t index_off = 0;
		//this starts reading the file line by line when offset is 0, ie the beginning and if statement
		if (p->line_offset == 0){
			if (fgets(p->current_line, MAX_PARSER_LINE_LENGTH + 1, p->f) == NULL){
				//printf("1");
				break;
			}
		}
		//this only happens after the first word is read in
		if (p->line_offset != 0){
			//breaks when reaches then end of file 
			if (feof(p->f) == 1 && p->current_line[p->line_offset] == '\0'){
					//printf("2");
					break;
				
			}
			//breaks when null character is found
			if (p->current_line[p->line_offset - 1] == '\0'){
				if (fgets(p->current_line, MAX_PARSER_LINE_LENGTH + 1, p->f) == NULL){
					p->line_offset = 0;
					//printf("3");
					break;
				}
				p->line_offset = 0;
			}
			
		}
		index_off = 0;
        	//reads alphanumeric and dash and '
        	while(isalnum(p->current_line[p->line_offset]) != 0 || p->current_line[p->line_offset] == '-' ||
			       	p->current_line[p->line_offset] == '\''){
                	word[index_off] = tolower(p->current_line[p->line_offset]);
                	index_off += 1;
                	p->line_offset += 1;
        	}
        	word[index_off] = '\0';
        	p->line_offset += 1;
        	return true;
	}
	//if any of the following breaks procede, return false
	//printf("bruh");
	word = NULL;
	return false;
}
