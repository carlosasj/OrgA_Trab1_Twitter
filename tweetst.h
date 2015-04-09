/*
	Grupo:
	Ana
	Carlos Alberto Schneider Junior		NUSP: 9167910
	Paul
	Renato
*/

#ifndef TWEET_STRUCT_H
#define TWEET_STRUCT_H

#define USER_SZ 20
#define TEXT_SZ 141
#define COORD_SZ 50
#define LANG_SZ 4

typedef struct {
	char USER[USER_SZ];
	char TEXT[TEXT_SZ];
	char COORDINATES[COORD_SZ];
	int FAVORITE_COUNT;
	char LANGUAGE[LANG_SZ];
	int RETWEET_COUNT;
	long int VIEWS_COUNT;
} TWEET;

#endif
