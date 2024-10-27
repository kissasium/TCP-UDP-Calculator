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


int calculate(int num1, int num2, char* operation) 
{
    if (strcmp(operation, "add") == 0)
        return num1 + num2;
    else if (strcmp(operation, "subtract") == 0)
        return num1 - num2;
    else if (strcmp(operation, "multiply") == 0)
        return num1 * num2;
    else if (strcmp(operation, "divide") == 0 && num2 != 0)
        return num1 / num2;
    else if (strcmp(operation, "modulus") == 0 && num2 != 0)
        return num1 % num2;
    else
        return -1;  
}


int calculateHash(char* data) 
{
    int hash = 0;
    for (int i = 0; data[i] != '\0'; ++i)
        hash += data[i];
    return hash;
}

int main() 
{
    int server_socket;
    char buff[MAXLENGTH];

    // UDP
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    
    struct sockaddr_in server_address, client_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));

    while (1) 
    {
        socklen_t client_address_size = sizeof(client_address);
        int recv_msg = recvfrom(server_socket, buff, MAXLENGTH, 0 , (struct sockaddr*)&client_address, &client_address_size);
        buff[recv_msg] = '\0';

       
        if (strcmp(buff, "exit") == 0)
        {
            printf("Client has requested to exit. Closing connection ;) .\n");
            break;
        }


        int num1, num2;
        char operation[20];
        sscanf(buff, "%d,%d,%s", &num1, &num2, operation);

        int receivedHash = calculateHash(buff);

     
        int result = calculate(num1, num2, operation);
        if (result != -1) 
        {
           
            int resultHash = calculateHash(buff);
            sprintf(buff, "Result: %d\nHash: %d", result, resultHash);
            sendto(server_socket, buff, strlen(buff), 0, (struct sockaddr*)&client_address, client_address_size);
        } 
        else 
        {
            sendto(server_socket, "Invalid operation", strlen("Invalid operation"), 0, (struct sockaddr*)&client_address, client_address_size);
        }
    }

    close(server_socket);
    return 0;
}

