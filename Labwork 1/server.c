#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#define SIZE 1024
#define localhost "127.0.0.1"
#define port 8080
#define IP_PROTOCOL 0

void write_file(int sockfd)
{
  int n;
  FILE *fp;
  char *filename = "recv.txt";
  char buffer[SIZE];

  fp = fopen(filename, "w");
  while (1)
  {
    n = recv(sockfd, buffer, SIZE, IP_PROTOCOL);
    if (n <= 0)
    {
      break;
      return;
    }
    fprintf(fp, "%s", buffer);
    bzero(buffer, SIZE);
  }
  return;
}

int main()
{
  int e;
  int sockfd, new_sock;
  struct sockaddr_in server_addr, new_addr;
  socklen_t addr_size;
  char buffer[SIZE];

  sockfd = socket(AF_INET, SOCK_STREAM, IP_PROTOCOL);
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(localhost);
  e = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  addr_size = sizeof(new_addr);
  new_sock = accept(sockfd, (struct sockaddr *)&new_addr, &addr_size);
  write_file(new_sock);
  printf("Data written in the file successfully.\n");
  return 0;
}