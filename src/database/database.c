#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"

struct _Database_t
{
	FILE *f;	/* File Pointer */
	uint32_t nreg_phys;	/* Number of Registers Physical */
	uint32_t nreg_log;	/* Number of Registers Logical */
	uint32_t nextFree;	/* Next Register "Free" (logically removed) || Next Register to be (over)written */
};

const Database *CreateDatabase(const char *path, const char *name) {
	if (path == NULL && name == NULL) { return NULL; }

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
		db->nreg_phys = (uint32_t) ((long int)ftell(db->f))/((long int)sizeof(Tweet));

	/* Creates a pseudo-index, to easily insert new registers */
	if (db->nreg_phys != 0){
		uint32_t i = db->nreg_phys -1;
		Tweet *tt = (Tweet *) malloc(sizeof(Tweet));
		do{	// Read the registers backwards
			fseek(db->f, (long int)(i*sizeof(Tweet)), SEEK_SET);
			fread(tt, sizeof(Tweet), 1, db->f);
			if (tt->flags == REMOVED){						// If it's Logically Removed...
				tt->nextFreeEntry = db->nextFree;			// ... set the nextFreeEntry...
				fseek(db->f, (long int)(i*sizeof(Tweet)), SEEK_SET);
				fwrite(tt, sizeof(Tweet), 1, db->f);		// ... write...
				db->nreg_log--;
				db->nextFree = i;							// ... and update db->nextFree.
			}
			i--;
		}while(i != 0);
		free(tt);
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
		Tweet *tt = (Tweet *) malloc(sizeof(Tweet));
		fseek(db->f, (long int)(rrn*sizeof(Tweet)), SEEK_SET);
		fread(tt, sizeof(Tweet), 1, db->f);
		db->nextFree = tt->nextFreeEntry;
		free (tt);
	}

	fseek(db->f, (long int)(rrn*sizeof(Tweet)), SEEK_SET);
	fwrite(t, sizeof(Tweet), 1, db->f);

	db->nreg_log++;
	return 0;
}

int GetTweet(Database *db, uint32_t rrn, Tweet *result) {
	if (db == NULL || result == NULL) { return 1; }
	if (rrn >= db->nreg_phys) { return 2; }

	fseek(db->f, (long int)(rrn*sizeof(Tweet)), SEEK_SET);
	fread(&result, sizeof(Tweet), 1, db->f);
	
	return 0;
}

int GetTweetsByUser(Database *db, const char *name, Tweet **result, size_t *nResults) {
	if (db == NULL
		|| name == NULL
		|| result == NULL
		|| nResults == NULL) { return 1; }
	
	Tweet **vtt = (Tweet **) malloc(sizeof(Tweet *));
	vtt[0] = (Tweet *) malloc(sizeof(Tweet));
	uint32_t i;
	
	*nResults = 0;
	
	for (i = 0; i < db->nreg_phys; i++){
		GetTweet(db, i, vtt[*nResults]);
		if (vtt[*nResults]->flags != REMOVED && strcmp(vtt[*nResults]->user, name) == 0){
			*nResults++;
			vtt = (Tweet **) realloc(vtt, (*nResults+1)*sizeof(Tweet *));
			vtt[*nResults] = (Tweet *) malloc(sizeof(Tweet));
		}
	}
	
	free(vtt[*nResults]);
	vtt = (Tweet **) realloc(vtt, (*nResults*sizeof(Tweet *))); 
	
	return 0;
}

void FreeTweetVector(Tweet **vector, size_t nItems) {
	if (vector == NULL) { return; }

	size_t i;

	for (i = 0; i < nItems; i++){
		free(vector[i]);
	}
	free(vector);
}

int RemoveTweet(Database *db, uint32_t rrn){
	if (db == NULL) { return 1; }
	if (rrn >= db->nreg_phys) { return 2; }

	Tweet *tt = (Tweet *) malloc(sizeof(Tweet));
	fseek(db->f, (long int)(rrn*sizeof(Tweet)), SEEK_SET);
	fread(tt, sizeof(Tweet), 1, db->f);

	if (tt->flags == REMOVED) {}
	else{
		tt->flags = REMOVED;
		tt->nextFreeEntry = db->nextFree;
		fseek(db->f, (long int)(rrn*sizeof(Tweet)), SEEK_SET);
		fwrite(tt, sizeof(Tweet), 1, db->f);
		db->nextFree = rrn;

		db->nreg_log--;
	}

	free(tt);
	return 0;
}
