#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/poll.h>
#include <signal.h>
#include <iostream>
#include <fstream>
#include <wait.h>

#define SERVER_PORT 3325

int MAX_CLIENTS = 128;
int MAX_FILE_SIZE = 1024 * 1024;

using namespace std;

bool terminate_ = 0;

void signal_handler(int sig)
{
    switch(sig)
    {
        case SIGTERM:
        case SIGHUP:
            terminate_ = 1;
            break;
    }
}

int write_to_file(char filename[], char data[], int datalen, int fileDescriptor)
{
    ofstream outfile(filename, ofstream::app);
    outfile.write(data, datalen);
    outfile.close();
    return 0;
}

int daemon(int port)
{
    int listener;
    struct sockaddr_in addr;

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    int enable = 1;
    if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");

    fcntl(listener, F_SETFL, O_NONBLOCK);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    listen(listener, 1);

    if (signal(SIGTERM, signal_handler) == SIG_ERR || signal(SIGHUP, signal_handler) == SIG_ERR)
    {
        perror("signal");
        exit(EXIT_FAILURE);
    }

    struct pollfd fd[MAX_CLIENTS];
    int numclients = 0;
    fd[0].fd = listener;
    fd[0].events = POLLIN;
    for (int i = 1; i < MAX_CLIENTS; i++)
        fd[i].fd = -1;
    while(!terminate_)
    {
        int ret = poll(fd, numclients + 1, -1);
        if (ret == -1)
        {
            perror("poll");
            break;
        }
        else if (ret == 0)
        {
            printf("Timeout\n");
            break;
        }
        else
        {
            if (fd[0].revents & POLLIN)
            {
                fd[0].revents = 0;
                int sock = accept(listener, NULL, NULL);
                if(sock < 0)
                {
                    perror("accept");
                    exit(EXIT_FAILURE);
                }

                int i;
                for(i = 1; i < MAX_CLIENTS; i++)
                {
                    if(fd[i].fd < 0)
                    {
                        fd[i].fd = sock;
                        fd[i].events = POLLIN;
                        numclients++;
                        break;
                    }
                }
                if(i == MAX_CLIENTS)
                {
                    continue;
                }
                if (--ret <= 0)
                    continue;
            }
            for (int i = 0; i <= numclients; i++)
            {
                if (fd[i].fd < 0)
                    continue;
                if(fd[i].revents & POLLIN)
                {
                    int recdatalen;
                    int filenamelen, datalen;
                    recdatalen = read(fd[i].fd, &filenamelen, sizeof(int));
                    char *filename = new char[filenamelen];
                    recdatalen = read(fd[i].fd, &datalen, sizeof(int));

                    if (datalen > MAX_FILE_SIZE) { // Check file size
                        printf("Error: File size exceeds the maximum allowed size of %d bytes.\n", MAX_FILE_SIZE);
                        close(fd[i].fd);
                        fd[i].fd = -1;
                        int response = -1;
                        send(fd[i].fd, &response, sizeof(int), 0);
                        continue;
                    }

                    char *data = new char[datalen];
                    recdatalen = read(fd[i].fd, filename, filenamelen);
                    for(int j = 0; j < datalen; j += recdatalen)
                    {
                        recdatalen = read(fd[i].fd, data + j, 32768);
                    }
                    close(fd[i].fd);
                    fd[i].fd = -1;
                    numclients--;
                    write_to_file(filename, data, datalen, fd[i].fd);
                    delete[] data;
                    delete[] filename;
                }
            }

        }
    }
    if (terminate_)
    {
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            if (fd[i].fd != -1)
                close(fd[i].fd);
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    // Check if there are at least two arguments (first is program name, second is port)
    if (argc < 3)
    {
        cerr << "Usage: " << argv[0] << " <port> <absolute_path_to_folder> [max_clients] [max_file_size]" << endl;
        return EXIT_FAILURE;
    }

    // Check if the third and fourth arguments exist (max_clients and max_file_size)
    if (argc >= 4)
    {
        MAX_CLIENTS = stoi(argv[3]);
    }

    if (argc >= 5)
    {
        MAX_FILE_SIZE = stoi(argv[4]);
    }

    struct stat buffer;
    if (stat(argv[2], &buffer) != 0)
    {
        cerr << "Folder does not exist" << endl;
        return EXIT_FAILURE;
    }
    else if (atoi(argv[1]) < 0 || atoi(argv[1]) > 65535 )
    {
        cerr << "Invalid port" << endl;
        return EXIT_FAILURE;
    }

    int port = atoi(argv[1]);

    pid_t parpid, sid;

    parpid = fork();
    if(parpid < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if(parpid != 0)
    {
        exit(EXIT_SUCCESS);
    }

    umask(0);
    sid = setsid();
    if(sid < 0)
    {
        perror("setsid");
        exit(EXIT_FAILURE);
    }

    if(chdir(argv[2]) < 0)
    {
        perror("chdir");
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    daemon(port);
    return EXIT_SUCCESS;
}
