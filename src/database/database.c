#include "database.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils.h"

#define NO_FRRE_POS UINT32_MAX

struct _Database_t
{
    FILE *dataFile;
    size_t pEntries;        /* Number of phisical entries */
    uint32_t nextFreePos;   /* Top of "stack" of free positions */
};

Database* CreateDatabase(const char* name)
{
    Database *db = malloc(sizeof(Database));
    FATAL(db, 127);

    db->dataFile = fopen(name, "r+");
    if(!db->dataFile) {
        db->dataFile = fopen(name, "w+");
        db->nextFreePos = NO_FRRE_POS;
        db->pEntries = 0;
    } else {
        fread(&(db->nextFreePos), sizeof(uint32_t), 1, db->dataFile);
        FATAL(fseek(db->dataFile, 0, SEEK_END) == 0, 127);
        db->pEntries = (size_t) ((ftell(db->dataFile) - sizeof(uint32_t))
                                / sizeof(Tweet));
    }

    return db;
}

void FreeDatabase(Database* db)
{
    if(db) {
        if(db->dataFile) {
            fseek(db->dataFile, 0, SEEK_SET);
            fwrite(&(db->nextFreePos), sizeof(uint32_t), 1, db->dataFile);
            fclose(db->dataFile);
        }
        free(db);
    }
}

size_t GetSize(const Database* db)
{
    if(db)
        return db->pEntries;
    else
        return SIZE_MAX;
}

int GetTweet(Database* db, uint32_t rrn, Tweet* result)
{
    if(!db) return -1;
    else if(rrn >= db->pEntries) return 1;

    FAIL(fseek(db->dataFile, (rrn * sizeof(Tweet)) + sizeof(uint32_t),
               SEEK_SET) == 0, -2);
    fread(result, sizeof(Tweet), 1, db->dataFile);

    return GET_BIT(result->flags, ACTIVE_BIT) ? 0 : -3;
}

int GetTweetsByUser(Database* db, const char* name, Tweet** result,
                    size_t* nResults)
{
    if(!(db && name && result && nResults))
        return -1;

    *result = NULL;
    *nResults = 0;

    Tweet tmp;
    for(uint32_t rrn = 0; GetTweet(db, rrn, &tmp) != 1; ++rrn) {
        printf(">>> %s\n", tmp.user);
        if(strcmp(tmp.user, name) == 0) {
            *result = realloc(*result, ((*nResults) + 1) * sizeof(Tweet));
            FATAL(*result, 127);
            memcpy(*result + *nResults, &tmp, sizeof(Tweet));
            ++(*nResults);
        }
    }
    return 0;
}

int InsertTweet(Database* db, Tweet* t)
{
    if(!(db && t)) {
        return -1;
    }

    SET_BIT(t->flags, ACTIVE_BIT);
    printf(">>>> %Xu\n", t->flags);

    if (db->nextFreePos == NO_FRRE_POS) {
        FAIL(fseek(db->dataFile, sizeof(uint32_t), SEEK_END) == 0, -2);
        FAIL(fwrite(t, sizeof(Tweet), 1, db->dataFile) == 1, -3);
        ++(db->pEntries);
    } else {
        Tweet tmp;
        FATAL_MSG(GetTweet(db, db->nextFreePos, &tmp) == 2, 127,
                  "Corrupt data file detected!");
        FATAL(fseek(db->dataFile, -sizeof(Tweet), SEEK_CUR) == 0, 127);
        db->nextFreePos = tmp.nextFreeEntry;
        FATAL(fwrite(t, sizeof(Tweet), 1, db->dataFile) == 1, 127);
    }

    fflush(db->dataFile);
    return 0;
}

int RemoveTweet(Database* db, uint32_t rrn)
{
    Tweet tmp;
    int err = GetTweet(db, rrn, &tmp);
    if(err > 0) return 1;
    FAIL_MSG(err >= 0, -1, "Falhou em localizar registro");

    db->nextFreePos = rrn;
    tmp.nextFreeEntry = db->nextFreePos;
    CLEAR_BIT(tmp.flags, ACTIVE_BIT);
    FATAL(fseek(db->dataFile, -sizeof(Tweet), SEEK_CUR) == 0, 127);
    FATAL(fwrite(&tmp, sizeof(Tweet), 1, db->dataFile) == 1, 127);
    fflush(db->dataFile);
    return 0;
}
