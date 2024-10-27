#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

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
    else if (strcmp(operation, "divide") == 0)
        return num1 / num2;
    else if (strcmp(operation, "modulus") == 0)
        return num1 % num2;
    else 
    {
        printf("Invalid operation\n");
        return 0;
    }
}

// data integrity 
int calculateHash(char* data) // to calculate that the message is same in the both sides 
{
    int hash = 0;
    for (int i = 0; data[i] != '\0'; ++i)
        hash += data[i];
    return hash;
}

int main() 
{
    char buffer[MAXLENGTH];
    char message[256];
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    listen(server_socket, 5);

    int client_socket;
    client_socket = accept(server_socket, NULL, NULL);

    while (1) 
    {
        int received = recv(client_socket, buffer, MAXLENGTH, 0);
        if (received < 0 || strcmp(buffer, "exit\n") == 0) 
        {
            printf("Client has disconnected\n");
            break;
        }

        // Extracting numbers and operation 
        int num1, num2;
        char operation[20];
        sscanf(buffer, "%d,%d,%s", &num1, &num2, operation);    // it is comma separated so automatically extraxct 

        // Perform arithmetic operation
        int result = calculate(num1, num2, operation);

       
        int hash = calculateHash(buffer);

       
        snprintf(message, sizeof(message), "Result: %d\nHash: %d", result, hash);     // store has and result in the message string 

       
        send(client_socket, message, sizeof(message), 0);
    }

    close(server_socket);
    return 0;
}

