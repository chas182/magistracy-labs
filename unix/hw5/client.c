#include "io.h"
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define MIN 0
#define MAX 1000000000

uint32_t FindNumber(const int socket);

void PrintUsage(const char* prog) {
    fprintf(stderr, "=== number guessing client ===\n");
    fprintf(stderr, "Usage: %s UNIX_SOCKET_PATH \n\n", prog);
}

bool Equals(const char result) {
    return result == '=';
}

bool Greater(const char result) {
    return result == '>';
}

bool Less(const char result) {
    return result == '<';
}

void closeSocket(const int socketFD) {
	if (close(socketFD) == -1) {
        perror("close socket");
        exit(2);
    }
}

int main(int argc, char* argv[]) {
    int socketFD;

    if (argc != 2) {
        PrintUsage(argv[0]);
        return 2;
    }

    const char* socketPath = argv[1];

    if ((socketFD = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    fprintf(stderr, "Trying to connect...\n");

    struct sockaddr_un remote;
    remote.sun_family = AF_UNIX;
    if (strlen(argv[1]) >= sizeof(remote.sun_path)) {
        fprintf(stderr, "Soctet path is too long.\n");
        exit(1);
    }
    strcpy(remote.sun_path, socketPath);
    socklen_t remoteLen = sizeof(remote);

    if (connect(socketFD, (struct sockaddr *)&remote, remoteLen) == -1) {
        perror("connect");
        exit(1);
    }

    fprintf(stderr, "Connected.\n");

    uint32_t guess = FindNumber(socketFD);
    fprintf(stdout, "%d\n", guess);

    closeSocket(socketFD);
    return 0;
}

uint32_t FindNumber(const int socketFD) {
    uint32_t left = MIN;
    uint32_t right = MAX;
    uint32_t toSend, guess = 0;
    char result = ' ';

    while (!Equals(result) && left <= right) {
        guess = (left + right) / 2;
        fprintf(stderr, "Client try to guess: %d\n", guess);
        toSend = htonl(guess);

        if (!SendAll(socketFD, (char*)&toSend, sizeof(toSend))) {
        	closeSocket(socketFD);
        	exit(3);
            break;
        }

        if (!RecvAll(socketFD, &result, sizeof(result))) {
            fprintf(stderr, "recv");
            closeSocket(socketFD);
        	exit(3);
            break;
        }
        fprintf(stderr, "attempt: x %c %d\n", result, guess);

        if (Less(result)) {
            right = guess - 1;
        } else if (Greater(result)) {
            left = guess + 1;
        }

    }
    return guess;
}
