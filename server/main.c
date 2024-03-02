#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

#include "../default.h"
#include "../shared.h"

#include "config.h"

#include "main.h"


void printHelp(void) {
    printf("Options are : \n  -a: IPv4 address for socket\n  -p: Port to listen\n  -s: Hash to crack\n  -h: This message\n");
}

int main(int argc, char *argv[]) {

    config_t currentConfig; // Global config structure

    int setConfigReturn = setConfig(argc, argv, &currentConfig);
    if (setConfigReturn != 0) {
        return setConfigReturn;
    }

    if (currentConfig.logToFile == true) {
        showError((char *) &currentConfig.logFile);
    }

    int serverSockD = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    if ((setsockopt(serverSockD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) == -1) {
        showError(NULL);
        close(serverSockD);
        return errno;
    }
    struct sockaddr_in serverAddress;

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(currentConfig.serverPort);
    serverAddress.sin_addr.s_addr = currentConfig.serverIP;

    if (bind(serverSockD, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1) {
        showError(NULL);
        close(serverSockD);
        return errno;
    }

    if (listen(serverSockD, 1) == -1) {
        showError(NULL);
        close(serverSockD);
        return errno;
    }
    int clientSockD = accept(serverSockD, NULL, NULL);
    if (clientSockD == -1) {
        showError(NULL);
        close(serverSockD);
        return errno;
    }

    char clientResponseBuffer[8192];
    for (;;) {
        ssize_t recvResult;
        recvResult = recv(clientSockD, clientResponseBuffer, sizeof(clientResponseBuffer) - 1, 0);
        if (recvResult == -1) {
            showError(NULL);
            close(clientSockD);
            close(serverSockD);
            return errno;
        }
        clientResponseBuffer[recvResult] = 0;

        if (strncmp(clientResponseBuffer, "hash", sizeof(clientResponseBuffer)) == 0) {
            if (send(clientSockD, (char *) &currentConfig.hash, sizeof(currentConfig.hash) - 1, 0) == -1) {
                showError(NULL);
                close(clientSockD);
                close(serverSockD);
                return errno;
            }
        } else if (strncmp(clientResponseBuffer, "found", sizeof(clientResponseBuffer)) == 0) {
            recvResult = recv(clientSockD, clientResponseBuffer, sizeof(clientResponseBuffer) - 1, 0);
            if (recvResult == -1) {
                showError(NULL);
                close(serverSockD);
                close(clientSockD);
                return errno;
            }
            clientResponseBuffer[recvResult] = 0;
            printf("Response is : %s - %s\n", currentConfig.hash, clientResponseBuffer);
            break;
        } else if (strncmp(clientResponseBuffer, "notfound", sizeof(clientResponseBuffer)) == 0) {
            printf("Response not found.\n");
            break;
        }
    }
    close(serverSockD);
    close(clientSockD);
    return 0;
}
