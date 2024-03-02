#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>

#include "../default.h"
#include "../shared.h"

#include "calculateHash.h"

#include "crack.h"

void freeCharList_s(charList_t *s) {
    safeFree((void **) &s->charList);
    safeFree((void **) &s);
}

bruteforceResult_t *initBruteforceResult_s(void) {
    bruteforceResult_t *s = (bruteforceResult_t *) calloc(1, sizeof(bruteforceResult_t));
    if (s == NULL) {
        showError(NULL);
        return NULLPTR;
    }
    s->resultSize = (size_t) NULL;
    s->result = (char *) NULL;
    s->found = false;
    return s;
}

void freeBruteforceResult_s(bruteforceResult_t *s) {
    safeFree((void **) &s->result);
    safeFree((void **) &s);
}

int bruteforce(char *prefix_p, unsigned short length, char *toCrackHash_p, bruteforceResult_t *bruteforceResult_p,
               charList_t *charList_p) {
    char *hash_p = NULLPTR;
    if (length == 0) {
        hash_p = generateMD5(prefix_p);
        if (hash_p == NULL) {
            showError(NULL);
            return 1;
        }

        if (strncmp(toCrackHash_p, hash_p, MD5_STR_SIZE * sizeof(char)) == 0) {
            safeFree((void **) &hash_p);
            strlcpy(bruteforceResult_p->result, prefix_p, bruteforceResult_p->resultSize);
            return 0;
        }

        safeFree((void **) &hash_p);
        return 1;
    }

    char c;

    for (unsigned int i = 0; i < length; i++) {
        for (unsigned int j = 0; j < charList_p->arraySize; j++) {
            c = charList_p->charList[j];
            prefix_p[length - 1] = c;
            if (bruteforce(prefix_p, length - 1, toCrackHash_p, bruteforceResult_p, charList_p) == 0) {
                safeFree((void **) &hash_p);
                return 0;
            }
        }
    }

    safeFree((void **) &hash_p);
    return 1;
}

void incrementalBruteforce(unsigned short length, char *toCrackHash_p, bruteforceResult_t *bruteforceResult_p,
                           charList_t *charList_p) {
    char *prefix_p = (char *) calloc((length + 1), sizeof(char));
    if (prefix_p == NULL) {
        showError(NULL);
        return;
    }

    for (unsigned short i = 1; i <= length; i++) {
        bruteforceResult_p->resultSize = (i + 1) * sizeof(char);
        bruteforceResult_p->result = (char *) calloc(1, bruteforceResult_p->resultSize);
        if (bruteforceResult_p == NULL) {
            safeFree((void **) &prefix_p);
            showError(NULL);
            return;
        }

        if (bruteforce(prefix_p, i, toCrackHash_p, bruteforceResult_p, charList_p) == 0) {
            safeFree((void **) &prefix_p);
            bruteforceResult_p->found = true;
            return;
        }
        safeFree((void **) &bruteforceResult_p->result);
        bruteforceResult_p->resultSize = 0;
    }

    safeFree((void **) &prefix_p);
}

charList_t *generateAsciiRange(unsigned short begin, unsigned short end) {
    charList_t *charList_p = (charList_t *) calloc(1, sizeof(charList_t));
    if (charList_p == NULL) {
        showError(NULL);
        return NULL;
    }
    charList_p->arraySize = (size_t) end - begin;
    charList_p->charList = (char *) calloc(charList_p->arraySize + 1, sizeof(char));
    if (charList_p->charList == NULL) {
        showError(NULL);
        return NULLPTR;
    }
    for (unsigned short i = begin; i <= end; i++) {
        charList_p->charList[i - begin] = (char) i;
    }
    return charList_p;
}

void bruteforceWordlist(const char *toCrackHash_p, const char *wordlist_p, bruteforceResult_t *bruteforceResult_p) {
    errno = 0;
    if (bruteforceResult_p == NULL) {
        return;
    }
    FILE *f = fopen(wordlist_p, "r");


    if (f == NULL) {
        showError(NULL);
        return;
    }

    char *line = NULLPTR;
    size_t len = 0;
    ssize_t currentRead;
    char *hash_p = NULLPTR;

    while ((currentRead = getline(&line, &len, f)) != -1) {
        line[strcspn(line, "\r\n")] = 0;
        hash_p = generateMD5(line);
        if (hash_p == NULL) {
            showError(NULL);
            fclose(f);
            safeFree((void **) &line);
            return;
        }

        if (strncmp(toCrackHash_p, hash_p, MD5_STR_SIZE * sizeof(char)) == 0) {
            bruteforceResult_p->resultSize = (unsigned long) currentRead + 1 * sizeof(char);
            bruteforceResult_p->result = (char *) calloc(1, bruteforceResult_p->resultSize);
            if (bruteforceResult_p->result == NULL) {
                showError(NULL);
                freeBruteforceResult_s(bruteforceResult_p);
                safeFree((void **) &line);
                safeFree((void **) &hash_p);
                fclose(f);
                return;
            }
            bruteforceResult_p->found = true;
            strlcpy(bruteforceResult_p->result, line, bruteforceResult_p->resultSize);

            safeFree((void **) &hash_p);
            safeFree((void **) &line);
            fclose(f);
            return;
        } else {
            safeFree((void **) &hash_p);
        }
    }

    fclose(f);
    safeFree((void **) &hash_p);
    safeFree((void **) &line);
}
