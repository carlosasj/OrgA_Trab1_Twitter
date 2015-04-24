#include "io.h"

#include <stdio.h>
#include <inttypes.h>

void readUntil(char *dest, size_t len, char stop)
{
    if(len < 2) return;

    while((dest[0] = getchar()) == stop);

    size_t i;
    for(i = 1; i < len - 1; ++i) {
        int c = getchar();
        if(c == EOF || c == stop) break;
        dest[i] = c;
    }
    dest[i] = 0;

    if(i == len - 1) while((getchar() != '\n'));
}


void printTweet(const Tweet* t)
{
    printf("--------------------------- Info ---------------------------\n");
    printf("VIEWS: %-10u RETWEETS: %-10u FAVORITES: %-10u\n", t->views,
           t->retweets, t->favs);
    printf("COORDINATES: %s LANG: %s \n", t->coordinates, t->language);
    printf("--------------------------- Text ---------------------------\n");
    printf("USER: %-32s\n", t->user);
    printf("TEXT:\n%s\n", t->text);
    printf("--------------------------- END  ---------------------------\n");
}

void readTweet(Tweet* t)
{
    t->flags = ACTIVE;
    printf("Novo Tweet:\n");
    printf("\tVIEWS: ");
    scanf(" %u", &(t->views));
    printf("\tRETWEETS: ");
    scanf(" %u", &(t->retweets));
    printf("\tFAVORITES: ");
    scanf(" %u", &(t->favs));
    printf("\tUSER: ");
    readUntil(t->user, TW_USER_LEN, '\n');
    printf("\tLANG: ");
    readUntil(t->language, TW_LANG_LEN, '\n');
    printf("\tCOORDINATES: ");
    readUntil(t->coordinates, TW_COORDINATES_LEN, '\n');
    printf("\tTEXT: (CTRL+D para terminar)\n");
    readUntil(t->text, TW_TEXT_LEN, '\0');
}