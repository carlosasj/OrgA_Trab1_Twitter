/**
 * This file defines the functions and data structures to deal with tweets.
 *
 * All text data is ASCII encoded and MUST BE NULL TERMINATED.
 */

#ifndef _TWEET_H_
#define _TWEET_H_

#include <stdint.h>
#include <stddef.h>

#define TW_TEXT_LEN (144)
#define TW_USER_LEN (32)
#define TW_COORDINATES_LEN (52)
#define TW_LANG_LEN (8)

enum flags {
	ACTIVE,
	REMOVED
};

/**
 * This struct represents a tweet. It is guaranteed to be "disk friendly" by
 * ensuring that the size is optimal to fit well inside the disk pages.
 */
typedef struct {
    uint32_t flags;             /* Flags used to store extra information */
    uint32_t nextFreeEntry;     /* RRN of next free space on the data file */
    uint32_t favs;              /* Favorite counter */
    uint32_t views;             /* View counter */
    uint32_t retweets;          /* Retweet counter */
    char text[TW_TEXT_LEN];     /* Actual text of the tweet */
    char user[TW_USER_LEN];     /* Username of the author */
    char coordinates[TW_COORDINATES_LEN]; /* Location at the time of tweet */
    char language[TW_LANG_LEN];
} Tweet;

#endif /* _TWEET_H_ */
