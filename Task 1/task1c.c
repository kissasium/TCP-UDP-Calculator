#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define PORT 12345
#define MAXLENGTH 1052

int main() 
{
    int client_socket;
    char buffer[MAXLENGTH];
    char message[MAXLENGTH];
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in client_address;
    client_address.sin_family = AF_INET;
    client_address.sin_port = htons(PORT);
    client_address.sin_addr.s_addr = INADDR_ANY;

    connect(client_socket, (struct sockaddr*)&client_address, sizeof(client_address));

    while (1) 
    {
        printf("Enter two numbers and operation (e.g: 2,3,modulus): \n");
        fgets(buffer, MAXLENGTH, stdin);

        if (strcmp(buffer, "exit\n") == 0) 
        {
            send(client_socket, buffer, MAXLENGTH, 0);
            printf("Client is disconnected\n");
            break;
        } 
        else 
        {
            send(client_socket, buffer, MAXLENGTH, 0);

            // Receive result and hash from server
            int received = recv(client_socket, message, MAXLENGTH, 0);
            message[received] = '\0';

            int result, hash;
           // printf(message);
            sscanf(message, "Result: %d\nHash: %d", &result, &hash);   // extract result and hash 
            
            // eskay aagay jo hai ussey extract karey. Result:
           // eskay aagay jo hai ussey extract karey. Hash:
            
            int calculatedHash = 0;
            for (int i = 0; buffer[i] != '\0'; ++i)
                calculatedHash += buffer[i];

            if (hash == calculatedHash) 
            {
                printf("Result received from server: %d\n", result);
            } else 
            {
                printf("Data integrity compromised! :( \n");
            }
        }
    }

    close(client_socket);
    return 0;
}

