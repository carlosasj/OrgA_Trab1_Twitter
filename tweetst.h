/*
Grupo:
Ana
Carlos Alberto Schneider Junior		NUSP: 9167910
Paul
Renato
*/

#ifndef TWEET_STRUCT_H
#define TWEET_STRUCT_H

typedef struct {
	char TEXT[140];
	char USER[20];
	char COORDINATES[50];
	int FAVORITE_COUNT;
	char LANGUAGE[4];
	int RETWEET_COUNT;
	long int VIEWS_COUNT;
} TWEET;

#endif
