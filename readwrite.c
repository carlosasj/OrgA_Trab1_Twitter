// Essas funções não foram testadas ainda.

#include "readwrite.h"
#include "tweetst.h"

void readRRN (TWEET * tt, FILE * f, int rrn){
	fseek(f, SEEK_SET, (rrn * sizeof(TWEET)));
	
	fread(&(tt->USER), sizeof(char), USER_SZ, f);
	fread(&(tt->TEXT), sizeof(char), TEXT_SZ, f);
	fread(&(tt->COORDINATES), sizeof(char), COORD_SZ, f);
	fread(&(tt->FAVORITE_COUNT), sizeof(int), 1, f);
	fread(&(tt->LANGUAGE), sizeof(char), LANG_SZ, f);
	fread(&(tt->RETWEET_COUNT), sizeof(int), 1, f);
	fread(&(tt->VIEWS_COUNT), sizeof(long int), 1, f);
}

void writeRRN (TWEET * tt, FILE * f, int rrn){
	fseek(f, SEEK_SET, (rrn * sizeof(TWEET)));
	
	fwrite(&(tt->USER), sizeof(char), USER_SZ, f);
	fwrite(&(tt->TEXT), sizeof(char), TEXT_SZ, f);
	fwrite(&(tt->COORDINATES), sizeof(char), COORD_SZ, f);
	fwrite(&(tt->FAVORITE_COUNT), sizeof(int), 1, f);
	fwrite(&(tt->LANGUAGE), sizeof(char), LANG_SZ, f);
	fwrite(&(tt->RETWEET_COUNT), sizeof(int), 1, f);
	fwrite(&(tt->VIEWS_COUNT), sizeof(long int), 1, f);
}
