#ifndef CRACK_H
#define CRACK_H

#include <stddef.h>
#include <stdbool.h>

typedef struct CharList_s { // List of char for bruteforce step
    size_t arraySize;
    char *charList;
} charList_t;

typedef struct bruteforceResult_s { // store result of bruteforce
    bool found;
    size_t resultSize;
    char *result;
} bruteforceResult_t;

int bruteforce(char *prefix, unsigned short length, char *toCrackHash, bruteforceResult_t *bruteforceResult,
               charList_t *charList); // bruteforce a specific length, with a specific array of char.

void incrementalBruteforce(unsigned short length, char *toCrackHash_p, bruteforceResult_t *bruteforceResult_p,
                           charList_t *charList_p); // bruteforce from 0 to max length with an array of char. Take (max) length, pointer for hash to crack, bruteForceResult_s structure, charList_s structure.

charList_t *generateAsciiRange(unsigned short begin,
                               unsigned short end); // Generate array of char with range of ASCII character, take begin of range, end of range. Return pointer to charList_s structure.

void freeCharList_s(charList_t *s); // free charList_s, take pointer to structure

bruteforceResult_t *initBruteforceResult_s(void); // Initialise bruteforceResult_s, return pointer to structure.

void freeBruteforceResult_s(bruteforceResult_t *s); // free bruteforceResult_s, take pointer to structure.

void bruteforceWordlist(const char *toCrackHash_p, const char *wordlist_p,
                        bruteforceResult_t *bruteforceResult_p); // Bruteforce with wordlist, pointer for hash to crack, a bruteforceResult_s structure and a pointer for currentConfig.

#endif
