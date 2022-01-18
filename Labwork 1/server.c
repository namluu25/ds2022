#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
bool conn = true;

int main() {
    int ss, cli, pid;
    struct sockaddr_in ad;
    char s[100];
    socklen_t ad_length = sizeof(ad);

    // create the socket
    ss = socket(AF_INET, SOCK_STREAM, 0);

    // bind the socket to port 12345
    memset(&ad, 0, sizeof(ad));
    ad.sin_family = AF_INET;
    ad.sin_addr.s_addr = INADDR_ANY;
    ad.sin_port = htons(12345);
    bind(ss, (struct sockaddr *)&ad, ad_length);

    // then listen
    listen(ss, 0);

    while (1) {
        // an incoming connection
        cli = accept(ss, (struct sockaddr *)&ad, &ad_length);

        pid = fork();
        if (pid == 0) {
            // I'm the son, I'll serve this client
            printf("Client connected\n");
            while (1) {
                if (conn == false){
                       printf("Client disconnected.");
                       break;
}
               // it's client turn to chat, I wait and read message from client
              	char filename[1024];
        	recv(cli, filename,1024,0);
        	char new_name[1024] = "new_";
        	strcat(new_name, filename);
               write_file(cli, new_name);
            }
            return 0;
        }
        else {
            // I'm the father, continue the loop to accept more clients
            continue;
        }
    }
    // disconnect
    close(cli);
}
void write_file(int socket, char *new_name){
       int n;
       FILE *fp;
       // char *filename = &new_name;
       char buffer[1024];

       fp = fopen(new_name, "wb+");
       if (fp == NULL) {
               printf("Error in writing file");
               exit(1);
       }
       while(1){
          	n = recv(socket, buffer, 1024, 0);
              	if(n <= 0){
                       conn = false;
                       fclose(fp);
                       return;
                       break;
              	}
       	fwrite(buffer, 1, 1024, fp);
       	bzero(buffer, 1024);
       }
}


   
