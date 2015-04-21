#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"

struct _Database_t
{
	FILE *f;	/* File Pointer */
};

const Database *CreateDatabase(const char *path, const char *name) {
	Database *db = (Database *) malloc(sizeof(Database));

	char *path_name = (char *) calloc((strlen(path) + strlen(name) +1), sizeof(char));
	strcpy(path_name, path);
	strcat(path_name, name);

	db->f = fopen(path_name, "ab");

	free(path_name);

	if (db->f == NULL) FreeDatabase(db);

	return db;
}

void FreeDatabase(Database *db) {
	if (db->f    != NULL) fclose(db->f);
	if (db       != NULL) free(db);
}

int InsertTweet(Database *db, const Tweet *t) {
	return 0;
}

int GetTweet(Database *db, uint32_t rrn, Tweet *result) {
	return 0;
}

int GetTweetsByUser(Database *db, const char *name, Tweet **result, size_t *nResults) {
	return 0;
}

int RemoveTweet(Database *db, uint32_t rrn){
	return 0;
}
