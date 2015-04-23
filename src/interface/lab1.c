#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../database/database.h"
#include "../tweet.h"

/* Color to use on Printf */
#define KNRM  "\x1B[0m"		// Normal
#define KRED  "\x1B[31m"	// Red
#define KGRN  "\x1B[32m"	// Green
#define KYEL  "\x1B[33m"	// Yellow
#define KBLU  "\x1B[34m"	// Blue
#define KMAG  "\x1B[35m"	// Magenta
#define KCYN  "\x1B[36m"	// Cyan
#define KWHT  "\x1B[37m"	// White

#define SET_COLOR 1	// 0 = False ; 1 = True

void printLogo();
Tweet *newTweet();

int main( ) {
	printLogo();
	Database *db = CreateDatabase("", "teste.tdb");
	Tweet *tt = newTweet();
	InsertTweet(db, tt);
	free(tt);
	FreeDatabase(db);
	return 0;
}

void printLogo() {
	if (SET_COLOR == 1) printf(KCYN);
	printf("      _______       _ _   _            \n");
	printf("     |__   __|     (_) | | |           \n");
	printf("        | |_      ___| |_| |_ ___ _ __ \n");
	printf("        | \\ \\ /\\ / / | __| __/ _ \\ '__|\n");
	printf("        | |\\ V  V /| | |_| ||  __/ |   \n");
	printf("        |_| \\_/\\_/ |_|\\__|\\__\\___|_|   \n");
	if (SET_COLOR == 1) printf(KNRM);
}

Tweet *newTweet(){
	Tweet *tt = (Tweet *) calloc(1, sizeof(Tweet));
	tt->flags = 0;
	tt->nextFreeEntry = 1;
	tt->favs = 10;
	tt->views = 100;
	tt->retweets = 2;
	strcpy(tt->text, "Texto muito legal aqui...");
	strcpy(tt->user, "usuario01");
	strcpy(tt->coordinates, "4 8 15 S 16 23 42 O");
	strcpy(tt->language, "Portuguese-BR");
	return tt;
}