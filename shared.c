#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <linux/limits.h>

#include "shared.h"

void safeFree(void **toFree) {
    if (*toFree != NULL || *toFree != NULLPTR) {
        free(*toFree);
        *toFree = NULLPTR;
    }
}

void showError(const char *logFilePath_p) {
    static char staticLogFilePath[PATH_MAX];

    if (logFilePath_p != NULL) {
        strlcpy(staticLogFilePath, logFilePath_p, PATH_MAX);
        return;
    }

    if (staticLogFilePath[0] != 0) {
        FILE *f = fopen(staticLogFilePath, "a");
        if (f == NULL) {
            fprintf(stderr, "Impossible d'ouvrir le fichier de log. :\n%s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        fprintf(f, "%s\n", strerror(errno));
        fclose(f);
        return;
    }
    fprintf(stderr, "%s\n", strerror(errno));
}
