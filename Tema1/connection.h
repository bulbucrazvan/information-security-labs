void sendMessage(int socketDescriptor, char* message);
char* readMessage(int socketDescriptor);
int initializeConnection(char* IP, int port);
void endConnection(int socketDescriptor);
int initializeServer(int port);
int acceptClient(int socketDescriptor);