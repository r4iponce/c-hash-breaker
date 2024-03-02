#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../default.h"
#include "../shared.h"

#include "config.h"
#include "crack.h"

#include "main.h"

void printHelp(void) {
    printf("Options are : \n  -a: IPv4 address for server\n  -p: Server port\n  -l: Max length\n  -h: This message\n  -b: Ascii generation begin (default 32)\n  -e: Ascii generation end (default 127)\n -f: Log file (default stderr)\n");
}

// Manage config (argv + getopt()), contact server and start cracking.
int main(int argc, char *argv[]) {
    config_t currentConfig;

    errno = 0;
    int setConfigReturn = setConfig(argc, argv, &currentConfig);
    if (setConfigReturn != 0) {
        return setConfigReturn;
    }

    if (currentConfig.logToFile == true) {
        showError((char *) &currentConfig.logFile);
    }

    int clientSockD = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSockD == -1) {
        showError(NULL);
        return errno;
    }
    struct sockaddr_in serverAddress;

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(currentConfig.serverPort);
    serverAddress.sin_addr.s_addr = currentConfig.serverIP;

    if (connect(clientSockD, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1) {
        showError(NULL);
        close(clientSockD);
        return errno;
    }

    char *hash_p = (char *) calloc(MD5_STR_SIZE, sizeof(char));
    if (hash_p == NULL) {
        showError(NULL);
        close(clientSockD);
        return errno;
    }

    if (send(clientSockD, "hash", sizeof("hash"), 0) == -1) {
        showError(NULL);
        close(clientSockD);
        safeFree((void **) &hash_p);
        return errno;
    }

    if (recv(clientSockD, hash_p, MD5_STR_SIZE * sizeof(char), 0) == -1) {
        showError(NULL);
        close(clientSockD);
        safeFree((void **) &hash_p);
        return errno;
    }

    printf("Hash : %s\n", hash_p);


    bruteforceResult_t *bruteforceResult_p = initBruteforceResult_s();
    if (bruteforceResult_p == NULLPTR) {
        close(clientSockD);
        safeFree((void **) &hash_p);
        return errno;
    }

    if (currentConfig.wordlist == true) {
        bruteforceWordlist(hash_p, currentConfig.wordlistPath, bruteforceResult_p);
    } else {
        charList_t *charList = generateAsciiRange(currentConfig.AsciiBegin, currentConfig.AsciiEnd);
        if (charList == NULL) {
            freeBruteforceResult_s(bruteforceResult_p);
            close(clientSockD);
            safeFree((void **) &hash_p);
            return EXIT_FAILURE;
        }

        bruteforceResult_p->resultSize = (currentConfig.maxLength + 1) * sizeof(char);
        bruteforceResult_p->result = NULLPTR;

        incrementalBruteforce(currentConfig.maxLength, hash_p, bruteforceResult_p, charList);
        freeCharList_s(charList);
    }

    if (bruteforceResult_p->found == true) {
        printf("Answer : %s - %s\n", hash_p, bruteforceResult_p->result);

        if (send(clientSockD, "found", sizeof("found"), 0) == -1) {
            showError(NULL);
            close(clientSockD);
            safeFree((void **) &hash_p);
            freeBruteforceResult_s(bruteforceResult_p);
            return errno;
        }

        sleep(1);
        if (send(clientSockD, bruteforceResult_p->result, bruteforceResult_p->resultSize, 0) == -1) {
            showError(NULL);
            close(clientSockD);
            safeFree((void **) &hash_p);
            freeBruteforceResult_s(bruteforceResult_p);
            return errno;
        }

    } else {
        if (send(clientSockD, "notfound", sizeof("notfound"), 0) == -1) {
            showError(NULL);
            close(clientSockD);
            safeFree((void **) &hash_p);
            freeBruteforceResult_s(bruteforceResult_p);
            return errno;
        }

        printf("Response not found.\n");
    }

    close(clientSockD);
    freeBruteforceResult_s(bruteforceResult_p);
    safeFree((void **) &hash_p);
    return EXIT_SUCCESS;
}
