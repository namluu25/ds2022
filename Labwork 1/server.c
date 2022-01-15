#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ctype.h>
#include <string.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char* argv[]) {
    int portno = atoi(argv[1]);
    int ss, cli, pid;
    struct sockaddr_in server_addr, cli_addr;
    char s[512];
    socklen_t serv_length, cli_len;
    serv_length = sizeof(server_addr);
    cli_len = sizeof(cli_addr);

    // create the socket
    ss = socket(AF_INET, SOCK_STREAM, 0);

    if (ss < 0) {
        error("ERROR opening socker!");
    }

    // bind the socket to given port
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(portno);
    bind(ss, (struct sockaddr *) &server_addr, serv_length);

    // then listen
    listen(ss, 0);

    cli = accept(ss, (struct sockaddr *) &cli_addr, &cli_len);

    if (cli < 0) {
        error("ERROR accepting");
    }

    pid = fork();
    if (pid == 0) {
        // I'm the son, I'll serve this client
        printf("Client sending file...\n");
        FILE *file_pointer;
        int words , ch;
        ch = 0;
        printf("I got here\n");

        file_pointer = fopen("received_file.txt", "a");
        read(cli, &words, sizeof(int));
        printf("Is this magic?\n");

        printf("Passed integer is : %d\n" , words);

        while(ch != words) {
            read(cli, s, 512);
            fprintf(file_pointer, " %s", s);
            printf("%s %d\n", s , ch);
            ch++;
        }
        printf("Almost there\n");

        printf("File was received successfully\n");
        printf("New file created is received_file.txt");
    
        // disconnect
        close(cli);
        close(ss);
        
        return 0;
    }
}
