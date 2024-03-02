#include <stdio.h>
#include <bsd/string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <sysexits.h>

#include "../default.h"
#include "../shared.h"

#include "main.h"

#include "config.h"

int setConfig(int argc, char *argv[], config_t *currentConfig_p) {
    // Set config
    // - currentConfig_p : Actual config
    // return : 0 if success, else if error
    int numberOptions;

    while ((numberOptions = getopt(argc, argv, "a:p:s:f:h")) != -1) {
        currentConfig_p->logToFile = false;
        switch (numberOptions) {
            case 'f':
                if (strncmp(optarg, "", 4096) == 0) { // Max size based on POSIX spec
                    break;
                }
                currentConfig_p->logToFile = true;
                strlcpy(currentConfig_p->logFile, optarg, 4096);
                break;
            case 'a':
                currentConfig_p->serverIP = inet_addr(optarg);
                break;
            case 'p':
                errno = 0;
                if (strtol(optarg, NULL, 10) > MIN_PORT && strtol(optarg, NULL, 10) <= MAX_PORT) {
                    currentConfig_p->serverPort = (unsigned short) strtol(optarg, NULL, 10);
                } else {
                    fprintf(stderr, "Port invalide.\n");
                    printHelp();
                    return EX_CONFIG;
                }
                if (errno == ERANGE) {
                    showError(NULL);
                    printHelp();
                    return EX_CONFIG;
                }
                break;
            case 's':
                errno = 0;
                strlcpy(currentConfig_p->hash, optarg, MD5_STR_SIZE * sizeof(char));
                if (errno == ERANGE) {
                    showError(NULL);
                    printHelp();
                    return EX_CONFIG;
                }
                break;
            case 'h':
                printHelp();
                exit(EXIT_SUCCESS);
            default:
                printHelp();
                return EX_CONFIG;
        }
    }

    // Verify mandatory options.
    if (currentConfig_p->serverPort == 0) {
        fprintf(stderr, "Port invalide ou manquant.\n");
        printHelp();
        return EX_CONFIG;
    }
    return 0;
}
