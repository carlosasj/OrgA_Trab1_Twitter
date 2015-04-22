/**
 * This file defines the functions and data structures to deal with tweets.
 * 
 * All text data is UTF-8 encoded and MAY NOT BE NULL TERMINATED. Also note
 * that special characters are counted only once.
 * For instance: the tweet "Café" is 4 characters long.
 */

#ifndef _TWEET_H_
#define _TWEET_H_

#include <stdint.h>
#include <stddef.h>

#define TW_TEXT_LEN (560)
#define TW_USER_LEN (48)
#define TW_COORDINATES_LEN (128)
#define TW_LANG_LEN (16)

/* Binary Length of the Tweet */
#define TW_BIN_LEN ((TW_TEXT_LEN + TW_USER_LEN + TW_COORDINATES_LEN + TW_LANG_LEN)*sizeof(char) + 4*sizeof(uint32_t))

/**
 * This struct represents a tweet. It is guaranteed to be "disk friendly" by
 * ensuring that the size is optimal to fit well inside the disk pages.
 */
typedef struct {
    uint32_t flags;             /* Flags used to store extra information */
    uint32_t favorite_count;              /* Favorite counter */
    uint32_t views_count;             /* View counter */
    uint32_t retweet_count;          /* Retweet counter */
    char text[TW_TEXT_LEN];     /* Actual text of the tweet */
    char user[TW_USER_LEN];     /* Username of the author */
    char coordinates[TW_COORDINATES_LEN]; /* Location at the time of tweet */
    char language[TW_LANG_LEN];
} Tweet;

#endif /* _TWEET_H_ */
