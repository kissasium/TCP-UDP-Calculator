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
    char buff[MAXLENGTH];
    struct sockaddr_in server_address;

    // UDP
    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    while (1) 
    {
        printf("Enter two numbers and operation (e.g: 2,3,modulus) or 'exit' to quit: ");
        fgets(buff, sizeof(buff), stdin);

   
        buff[strcspn(buff, "\n")] = '\0';
      

        sendto(client_socket, buff, strlen(buff), 0, (struct sockaddr*)&server_address, sizeof(server_address));
     
        if (strcmp(buff, "exit") == 0) 
        {
            printf("Exiting the program.\n");
            break;
        }

        int recv_msg = recvfrom(client_socket, buff, MAXLENGTH, 0, NULL, NULL);
        buff[recv_msg] = '\0';
   
        
        int result, receivedHash;
        sscanf(buff, "Result: %d\nHash: %d", &result, &receivedHash);

        int calculatedHash = 0;
        for (int i = 0; buff[i] != '\0'; ++i)
            calculatedHash += buff[i];

            printf("Result received from server: %d\n", result);
      
    }

    close(client_socket);
    return 0;
}

