#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    int so;
    char s[100];
    struct sockaddr_in ad;

    socklen_t ad_length = sizeof(ad);
    struct hostent *hep;

    // create socket
    int serv = socket(AF_INET, SOCK_STREAM, 0);

    // init address
    hep = gethostbyname(argv[1]);
    memset(&ad, 0, sizeof(ad));
    ad.sin_family = AF_INET;
    ad.sin_addr = *(struct in_addr *)hep->h_addr_list[0];
    ad.sin_port = htons(12345);

    // connect to server
    connect(serv, (struct sockaddr *)&ad, ad_length);
       FILE *fp;
    while (1) {
        // after connected, it's client turn to chat

        // send some data to server
        printf("client> Enter filename to send: ");
        scanf("%s", s);
        
        if( access( s, F_OK ) == 0 ) {
        	send(serv, s, sizeof(s), 0);
 		fp = fopen(s,"rb+");
       	send_file(fp, serv);
       	printf("Sent to server\n");
	} else {
    		printf("File does not exist\n");
	}  
    }
}

void send_file(FILE *fp, int socket){
       char data[1024] = {0};
       while(fread(data, 1,1024, fp) != 0){
               if ((socket, data, sizeof(data), 0) == -1){
                       printf("Error in sending file");
                       exit(1);
               }
       bzero(data, 1024);
	}
}
                                                                                                              
