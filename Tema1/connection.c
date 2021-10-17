#include "connection.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


void sendMessage(int socketDescriptor, char* message) {
    int messageLength = strlen(message) + 1;
    write(socketDescriptor, &messageLength, sizeof(int));
    write(socketDescriptor, message, messageLength);
}

char* readMessage(int socketDescriptor) {
    int messageLength;
    read(socketDescriptor, &messageLength, sizeof(int));
    char* receivedMessage = malloc(messageLength);
    read(socketDescriptor, receivedMessage, messageLength);
    return receivedMessage;
}

int initializeConnection(char* IP, int port) {
    int socketDescriptor;
    struct sockaddr_in server;
    socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(IP);
    server.sin_port = htons(port);
    connect(socketDescriptor, (struct sockaddr*)&server, sizeof(struct sockaddr));
    return socketDescriptor;
}

void endConnection(int socketDescriptor) {
    close(socketDescriptor);
}

int initializeServer(int port) {
    struct sockaddr_in server;
    int socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    int on = 1;
    setsockopt(socketDescriptor, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);
    bind(socketDescriptor, (struct sockaddr*)&server, sizeof(struct sockaddr));
    listen(socketDescriptor, 2);
    return socketDescriptor;
}

int acceptClient(int socketDescriptor) {
    struct sockaddr_in from;
    int client;
    socklen_t length = sizeof(from);
    client = accept(socketDescriptor, (struct sockaddr*)&from, &length);
    return client;
}