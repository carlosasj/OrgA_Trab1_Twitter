/**
 * This file defines the functions necessary to interact with the database.
 * Unless otherwise noted all functions return an integer with the following
 * convention: 0 = Success, <0 = Failure, >0 = Warning.
 */
#ifndef _DATABASE_H_
#define _DATABASE_H_

#include "../tweet.h"

/**
 * This prototype is here to hide the actual Database struct.
 * 
 * The actual struct shall be defined in the database.c to avoid unnecessary
 * data exposure.
 */
typedef struct _Database_t Database;

/**
 * Creates a data base at the given path, with the given name.
 *
 * If the given path+name already exists then the existing contents will be
 * loaded.
 *
 * \param path Path to the *directory* where the data base shall be created.
 * \param name Name
 * \return NULL in case of failure.
 */
const Database *CreateDatabase(const char *path, const char *name);

/**
 * Closes and releases all resources associated with the given database.
 */
void FreeDatabase(Database *db);

/**
 * Inserts a tweet into the database.
 *
 * Tries to inset a *copy* of the given tweet into the database.
 */
int InsertTweet(Database *db, const Tweet *t);

/**
 * Gets a tweet based on the RRN.
 *
 * Tries to find the tweet and copies it to result if found.
 */
int GetTweet(Database *db, uint32_t rrn, Tweet *result);

/**
 * Gets an array of all tweets by the given user.
 *
 * After calling, result shall point to a contiguous array of Tweets with
 * nResults entries. It's the caller's responsibility to free result.
 */
int GetTweetsByUser(Database *db, const char *name, Tweet **result,
                    size_t *nResults);

/**
 * Removes a tweet.
 *
 * Tries to remove the given tweet based on the given RRN. Note that this
 * simply marks the entry as deleted whithout actually erasing anything
 * untill the space is reused by some other new entry.
 */
int RemoveTweet(Database *db, uint32_t rrn);

#endif /* _DATABASE_H_ */
