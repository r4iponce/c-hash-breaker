#ifndef SHARED_H
#define SHARED_H

#define NULLPTR ((void*)0) // Point to 0x0, better than just NULL (0)

#include <stdbool.h>

void safeFree(void **toFree);

void showError(const char *logFilePath_p); // Show error, based on errno. Initialise it with logToFile et logFilePath.

#endif
