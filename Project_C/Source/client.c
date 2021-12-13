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
    if (argc != 3) {
        fprintf(stderr, "Not enough arguments");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in address; // Client address struct

    char *ip = argv[1];
    int port = strtol(argv[2], NULL, 10);
    //int port = 23456;

    //Valid ip address
    memset(&address, 0, sizeof(address)); // Init the address to 0.0.0.0
    int ipAddress = inet_pton(AF_INET, "127.0.0.1", &(address.sin_addr));
    if (ipAddress <= 0) { // 0 if ip is invalid, -1 if family is invalid
        fprintf(stderr, "Couldn't use IP address: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    // Creating the Client socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        fprintf(stderr, "Couldn't create socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Connecting to Server
    int result = connect(clientSocket, (struct sockaddr *) &address, sizeof(address));
    if (result == -1) {
        fprintf(stderr, "Couldn't connect to server: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    } else {
        printf("> Connection to server's port %d successfull\n", port);
    }

    printf("Tap your command : list , get <filename>, exit, put\n");

    for (;;) {
        char input[256];
        fgets(input, sizeof(input), stdin); // Getting client input
       
        //printf("%d\n", input);
        // Writing input
        char input_cmd[256];
        char input_arg[256]="";
    
        sscanf(input,"%s %s\n",input_cmd,input_arg);
        printf("%s",input_arg);
        write(clientSocket, input_cmd, 256);
           

        // Getting info from server
        unsigned char cmd;  
        read(clientSocket, &cmd, sizeof(cmd));
    
        switch(cmd) {
            case LIST: ;
            // Show every files
                int err = listc();
                if (err != 0 ){
                    fprintf (stderr, " Couldn't list the files.\n");
                }
                break;
            case GET:
                get(input_arg);
                break;
            case EXIT:
                exit(EXIT_SUCCESS);
                break;
            case ERR:
                printf(" Coudn't recognize comment. \n");
            }
    }
      
    return 0;
}