#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>
#include <stdint.h>
#include <linux/limits.h>

typedef struct config_s { // store all config.
    uint32_t serverIP;
    unsigned short serverPort;
    unsigned short maxLength;
    unsigned short AsciiBegin; // Begin of ascii table for array generation
    unsigned short AsciiEnd; // Begin of ascii table for array generation
    bool logToFile; // bool for check if log to file is enabled
    char logFile[PATH_MAX]; // Path for logfile
    bool wordlist; // bool for check if wordlist mode is enabled
    char wordlistPath[PATH_MAX]; // path to wordlist mode.
} config_t;

int setConfig(int argc, char *argv[], config_t *currentConfig_p); // set config from command line argument.

#endif
