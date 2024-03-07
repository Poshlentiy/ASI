#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <sys/poll.h>
#include <string.h>

#define SERVER_PORT 3325

using namespace std;

char* read_file(const char path_to_file[], int* datalen) {
    ifstream file(path_to_file, ios::binary);
    if (file.is_open()) {
        file.seekg(0, ios::end);
        int length = file.tellg();
        file.seekg(0, ios::beg);

        char *buffer = new char[length];
        file.read(buffer, length);

        if (!file) {
            fprintf(stderr, "Error: only %ld could be read\n", file.gcount());
            delete[] buffer;
            file.close();
            return NULL;
        }

        file.close();
        *datalen = length;
        return buffer;
    } else {
        fprintf(stderr, "Error: Unable to open file\n");
        return NULL;
    }
}

int get_socket(int ip, char *ipchar, int port) {
    int sock;
    struct sockaddr_in server_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = ip;

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    return sock;
}

int send_data(int sock, char data[], char filename[], int datalen) {
    int filename_len = strlen(filename);

    send(sock, &filename_len, sizeof(int), 0);
    send(sock, &datalen, sizeof(int), 0);
    send(sock, filename, filename_len, 0);
    send(sock, data, datalen, 0);

    return 0;
}

int main(int argc, char* argv[]) {
    char *data;
    int datalen;
    int ip, port;

    if (argc < 4) {
        fprintf(stderr, "Usage: %s <ip> <port> <path_to_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if ((data = read_file(argv[3], &datalen)) == NULL )
        return EXIT_FAILURE;

    port = atoi(argv[2]);
    if (port < 0 || port > 65535 || inet_pton(AF_INET, argv[1], &ip) != 1) {
        fprintf(stderr, "Invalid ip or port\n");
        return EXIT_FAILURE;
    }

    int sock = get_socket(ip, argv[1], port);

    char *filename, *cur_str;
    cur_str = filename = strtok(argv[3], "/\\");
    while (cur_str != NULL) {
        filename = cur_str;
        cur_str = strtok (NULL, "/\\");
    }

    send_data(sock, data, filename, datalen);

    int response;
    recv(sock, &response, sizeof(int), 0);

    if (response == -1) {
        printf("Error: File size exceeds maximum allowed size.\n");
        // Handle other error conditions if necessary
    }

    delete[] data;
    close(sock);

    return EXIT_SUCCESS;
}
