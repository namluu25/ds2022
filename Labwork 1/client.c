#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define SIZE 1024
#define localhost "127.0.0.1"
#define port 8080
#define IP_PROTOCOL 0

void send_file(FILE *fp, int sockfd)
{
  int n;
  char data[SIZE] = {0};

  while (fgets(data, SIZE, fp) != NULL)
  {
    if (send(sockfd, data, sizeof(data), IP_PROTOCOL) == -1)
    {
      perror("Error sending file.");
      exit(1);
    }
    bzero(data, SIZE);
  }
}

int main()
{
  int e;
  int sockfd;
  struct sockaddr_in server_addr;
  FILE *fp;
  char *filename = "send.txt";

  sockfd = socket(AF_INET, SOCK_STREAM, IP_PROTOCOL);
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(localhost);
  e = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  fp = fopen(filename, "r");
  send_file(fp, sockfd);
  printf("File data sent successfully.\n");
  close(sockfd);
  return 0;
}