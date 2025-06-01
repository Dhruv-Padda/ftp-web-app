// client.c - legacy FTP client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server;
    char filename[100];
    char buffer[BUFFER_SIZE];
    FILE *fp;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Could not create socket");
        return 1;
    }
    printf("Socket created.\n");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    // Connect to remote server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Connection failed");
        return 1;
    }
    printf("Connected to server.\n");

    // Get file name from user
    printf("Enter filename to send: ");
    scanf("%s", filename);

    fp = fopen(filename, "rb");
    if (fp == NULL) {
        perror("File open failed");
        close(sock);
        return 1;
    }

    // Send filename to server
    send(sock, filename, sizeof(filename), 0);
    sleep(1);  // Let the server get ready

    // Read from file and send to server
    int bytes_read;
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, fp)) > 0) {
        send(sock, buffer, bytes_read, 0);
    }
    printf("File sent successfully.\n");

    fclose(fp);
    close(sock);
    return 0;
}