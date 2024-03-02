#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>
#include <stdint.h>
#include <linux/limits.h>

#include "../default.h"

typedef struct config_s {
    uint32_t serverIP;
    char hash[MD5_STR_SIZE];
    unsigned short serverPort;
    bool logToFile; // bool for check if log to file is enabled
    char logFile[PATH_MAX]; // Path for logfile
} config_t;

int setConfig(int argc, char *argv[], config_t *currentConfig_p);

#endif
