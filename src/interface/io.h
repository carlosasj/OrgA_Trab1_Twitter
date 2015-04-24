#ifndef _IO_H_
#define _IO_H_

#include "../tweet.h"

#define CLEAR() write(1,"\33[H\33[2J",7)

void readUntil(char *dest, size_t len, char stop);
void readTweet(Tweet *t);
void printTweet(const Tweet *t);

#endif
