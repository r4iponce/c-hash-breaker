#include <stdio.h>
#include <bsd/string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sysexits.h>
#include <linux/limits.h>

#include "../default.h"
#include "../shared.h"

#include "main.h"

#include "config.h"

int setConfig(int argc, char *argv[], config_t *currentConfig_p) {
    int numberOptions;

    while ((numberOptions = getopt(argc, argv, "a:p:l:b:e:f:w:h")) != -1) {
        // a: Server address
        // p: Server port
        // l: Max password length (default 8)
        // b: Ascii generation begin (default 32)
        // e: Ascii generation end (default 127)
        // f: Log file
        // w: Wordlist
        // h: Help
        currentConfig_p->AsciiBegin = 32;
        currentConfig_p->AsciiEnd = 127;
        currentConfig_p->maxLength = 8;
        currentConfig_p->logToFile = false;
        currentConfig_p->wordlist = false;
        switch (numberOptions) {
            case 'a':
                if (inet_addr(optarg) == INADDR_NONE) {
                    fprintf(stderr, "Adresse IP du serveur invalide.\n");
                    return EX_CONFIG;
                }
                currentConfig_p->serverIP = inet_addr(optarg);
                break;
            case 'f':
                if (strncmp(optarg, "", PATH_MAX) == 0) { // Max size based on POSIX spec
                    break;
                }
                currentConfig_p->logToFile = true;
                strlcpy(currentConfig_p->logFile, optarg, PATH_MAX);
                break;
            case 'w':
                if (strncmp(optarg, "", PATH_MAX) == 0) { // Max size based on POSIX spec
                    break;
                }
                currentConfig_p->wordlist = true;
                strlcpy(currentConfig_p->wordlistPath, optarg, PATH_MAX);
                break;
            case 'p':
                errno = 0;
                if (strtol(optarg, NULL, 10) > MIN_PORT && strtol(optarg, NULL, 10) <= MAX_PORT) {
                    currentConfig_p->serverPort = (unsigned short) strtol(optarg, NULL, 10);
                } else {
                    fprintf(stderr, "Invalid port.\n");
                    printHelp();
                    return EX_CONFIG;
                }
                if (errno == ERANGE) {
                    showError(NULL);
                    printHelp();
                    return EX_CONFIG;
                }
                break;
            case 'l':
                errno = 0;
                if (strtol(optarg, NULL, 10) < 0) {
                    fprintf(stderr, "length is invalid, must be >0.\n");
                    printHelp();
                    return EX_CONFIG;
                }

                currentConfig_p->maxLength = (unsigned short) strtol(optarg, NULL, 10);
                if (errno == ERANGE) {
                    showError(NULL);
                    printHelp();
                    return EX_CONFIG;
                }
                break;
            case 'b':
                errno = 0;
                if (strtol(optarg, NULL, 10) < 0) {
                    fprintf(stderr, "Invalid settings, must be >0.\n");
                    printHelp();
                    return EX_CONFIG;
                }
                if (strtol(optarg, NULL, 10) > 127) {
                    fprintf(stderr, "Invalid settings, must be =<127.\n");
                    printHelp();
                    return EX_CONFIG;
                }

                currentConfig_p->AsciiBegin = (unsigned short) strtol(optarg, NULL, 10);
                if (errno == ERANGE) {
                    showError(NULL);
                    printHelp();
                    return EX_CONFIG;
                }
                break;
            case 'e':
                errno = 0;
                if (strtol(optarg, NULL, 10) < 0) {
                    fprintf(stderr, "Invalid settings, must be >0.\n");
                    printHelp();
                    return EX_CONFIG;
                }
                if (strtol(optarg, NULL, 10) > 127) {
                    fprintf(stderr, "Invalid settings, must be =<127.\n");
                    printHelp();
                    return EX_CONFIG;
                }

                currentConfig_p->AsciiEnd = (unsigned short) strtol(optarg, NULL, 10);
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
    if (currentConfig_p->maxLength == 0) {
        printHelp();
        return EX_CONFIG;
    }

    if (currentConfig_p->serverPort == 0) {
        printHelp();
        return EX_CONFIG;
    }
    return EXIT_SUCCESS;
}
