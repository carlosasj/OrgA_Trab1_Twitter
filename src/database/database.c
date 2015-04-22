#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"

#define CLR '&'	// Character "logically removed"

struct _Database_t
{
	FILE *f;	/* File Pointer */
	uint32_t nreg_phys;	/* Number of Registers Physical */
	uint32_t nreg_log;	/* Number of Registers Logical */
	uint32_t nextFree;	/* Next Register "Free" (logically removed) || Next Register to be (over)written */
};

const Database *CreateDatabase(const char *path, const char *name) {
	if (path == NULL && name == NULL) { return NULL; }

	char rm; // Character "logically removed"

	Database *db = (Database *) malloc(sizeof(Database));
	if (db == NULL) { return db; }

	/* Generate Filename */
	char *path_name = (char *) calloc((strlen(path) + strlen(name) +1), sizeof(char));
	if (path_name == NULL) { FreeDatabase(db); return db; }
	strcpy(path_name, path);
	strcat(path_name, name);

	/* Open the File */
	db->f = fopen(path_name, "ab");
	free(path_name);
	if (db->f == NULL) { FreeDatabase(db); return db; }
	
	/* Get db->nregs_phys */
	fseek(db->f, 0, SEEK_END);
	db->nextFree =
		db->nreg_log =
		db->nreg_phys = (((uint32_t)ftell(db->f)) /((uint32_t) TW_BIN_LEN));

	/* Creates a pseudo-index, to easily insert new registers */
	if (db->nreg_phys != 0){
		uint32_t i = db->nreg_phys -1;
		do{	// Read the registers backwards
			fseek(db->f, (long int)(i*TW_BIN_LEN), SEEK_SET);
			fread(&rm, sizeof(char), 1, db->f);
			if (rm == CLR){							// If it's Logically Removed...
				fwrite(&db->nextFree, sizeof(uint32_t), 1, db->f);	// ... write db->nextFree...
				db->nreg_log--;
				db->nextFree = i;					// ... and update db->nextFree.
			}
			i--;
		}while(i != 0);
	}

	return db;
}

void FreeDatabase(Database *db) {
	if (db->f != NULL) fclose(db->f);
	if (db    != NULL) free(db);
}

int InsertTweet(Database *db, const Tweet *t) {
	if (db == NULL || t == NULL) { return 1; }
	uint32_t rrn = db->nextFree;	// Save actual RRN

	if (db->nextFree == db->nreg_phys){
		db->nextFree++;
		db->nreg_phys++;
	}
	else{	// Get the next free RRN
		fseek(db->f, (long int)(rrn*TW_BIN_LEN + sizeof(char)), SEEK_SET);
		fread(&db->nextFree, sizeof(uint32_t), 1, db->f);
	}

	fseek(db->f, (long int)(rrn*TW_BIN_LEN), SEEK_SET);
	fwrite(t->user, sizeof(char), TW_USER_LEN, db->f);
	fwrite(t->text, sizeof(char), TW_TEXT_LEN, db->f);
	fwrite(t->coordinates, sizeof(char), TW_COORDINATES_LEN, db->f);
	fwrite(&(t->favorite_count), sizeof(uint32_t), 1, db->f);
	fwrite(t->language, sizeof(char), TW_LANG_LEN, db->f);
	fwrite(&(t->retweet_count), sizeof(uint32_t), 1, db->f);
	fwrite(&(t->views_count), sizeof(uint32_t), 1, db->f);

	db->nreg_log++;
	return 0;
}

int GetTweet(Database *db, uint32_t rrn, Tweet *result) {
	if (db == NULL || result == NULL) { return 1; }
	if (rrn >= db->nreg_phys) { return 2; }

	fseek(db->f, (long int)(rrn*TW_BIN_LEN), SEEK_SET);
	fread(result->user, sizeof(char), TW_USER_LEN, db->f);
	fread(result->text, sizeof(char), TW_TEXT_LEN, db->f);
	fread(result->coordinates, sizeof(char), TW_COORDINATES_LEN, db->f);
	fread(&result->favorite_count, sizeof(uint32_t), 1, db->f);
	fread(result->language, sizeof(char), TW_LANG_LEN, db->f);
	fread(&result->retweet_count, sizeof(uint32_t), 1, db->f);
	fread(&result->views_count, sizeof(uint32_t), 1, db->f);
	
	return 0;
}

/*
 *	Return NULL if had any error; Return 0 if success.
 */
int GetUserByRRN(Database *db, uint32_t rrn, char *result){
	if (db == NULL || result == NULL) { return 1; }
	if (rrn >= db->nreg_phys) { return 2; }

	fseek(db->f, (long int)(rrn*TW_BIN_LEN), SEEK_SET);
	fread(&result, sizeof(char), TW_USER_LEN, db->f);
	return 0;
}

/*
 *	\param rrn RRN to start the search
 */
uint32_t GetNextRRNforUser(Database *db, uint32_t rrn, char *user){
	if (db == NULL || user == NULL) { return 1; }
	if (rrn >= db->nreg_phys) { return 2; }

	char result[TW_USER_LEN];

	uint32_t i;
	for (i = rrn; i < db->nreg_phys; i++){
		GetUserByRRN(db, i, result);
		if (result[0] == CLR) {}
		else if (strcmp(user, result) == 0) { return i; }
	}
}

int GetTweetsByUser(Database *db, const char *name, Tweet **result, size_t *nResults) {
	if (db == NULL
		|| name == NULL
		|| result == NULL
		|| nResults == NULL) { return 1; }
	
	return 0;
}

int RemoveTweet(Database *db, uint32_t rrn){
	if (db == NULL) { return 1; }
	if (rrn >= db->nreg_phys) { return 2; }

	char result[TW_USER_LEN];
	GetUserByRRN(db, rrn, result);
	if (result[0] == CLR) { return 0; }

	result[0] = CLR;
	fseek(db->f, (long int)(rrn*TW_BIN_LEN), SEEK_SET);
	fwrite(&result, sizeof(char), 1, db->f);
	fwrite(&db->nextFree, sizeof(uint32_t), 1, db->f);
	db->nextFree = rrn;

	db->nreg_log--;

	return 0;
}
