#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include "functions.h"

#define LIST 0
#define GET 1
#define PUT 3
#define EXIT 2
#define ERR 4

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Not enough arguments\n");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in address; // Server address struct

    // making sure that the port is a int beetwen 1024 and 65535
    //int port = 23456;
    int port = strtol(argv[1], NULL, 10);
    if (port < 1024 || port > 65535) {
        fprintf(stderr, "Port wasn't in the defined range (1024 - 65535)\n");
        exit(EXIT_FAILURE);
    }

    memset(&address, 0, sizeof(address)); // initialize address to 0.0.0.0
    address.sin_family = AF_INET; // Using INADDR_ANY for linking to every network interface of the computer
    address.sin_addr.s_addr = htonl(INADDR_ANY); // valid address
    address.sin_port = htons(port); // valid port

    // Creating the socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0); 
    if (serverSocket == -1) {// checking for socket creation
        fprintf(stderr, "Couldn't create socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    // binding socket to address
    int b_status = bind(serverSocket, (struct sockaddr*) &address, sizeof(address));
    if (b_status == -1) { // checking the bind
        fprintf(stderr, "Couldn't bind socket to address: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Printing port
    printf("Server %s Listening on port: %d\n", inet_ntoa(address.sin_addr), port);

    int queue = 4;
    int l_status = listen(serverSocket, queue); // Passive mode socket
    if (l_status == -1) { // Checking if there's any problem
        fprintf(stderr, "Couldn't listen for clients: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    for(;;) {
        // Init client address
        struct sockaddr_in clientAddress; //Client address struct
        unsigned int clientLength = sizeof(clientAddress); // Getting the length of a client address
        // Accepting connection
        int clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress, &clientLength);

        printf("Client %d with address %s:%d is connected\n", clientSocket, inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));

    // Creating new process for every new connection
        pid_t pid = fork();
        if (pid > 0) { 
            waitpid(pid, NULL, 0); 
        } else if (pid == 0) { 
            pid_t pidS = fork();
            if (pidS > 0) {
                exit(EXIT_SUCCESS);
            } else if (pid == 0){
                // Reading Client input
                // On the client side we have something like 'write(clientSocket, &cmd, 1);
                for(;;){
                    unsigned char cmd;
                    char input[256];
                    
                    read(clientSocket,input, 256);
                    
                    if (strcmp(input, "list") == 0) {
                        cmd = LIST;
                        printf("Listing files:\n");
                    } else if (strcmp(input, "get") == 0) {
                        cmd = GET;
                        printf("Getting file:\n");
                    } else if (strcmp(input, "exit") == 0) {
                        cmd = EXIT;
                        printf("Shut down:\n");
                    } else {
                        cmd = ERR;
                    }

                    write(clientSocket, &cmd, 1);
                    if (cmd == EXIT) {
                        exit(EXIT_SUCCESS);
                    }
                }
            }
            else {
                fprintf (stderr, " Error");
            }
        }
    }
        
return 0 ; 
}

