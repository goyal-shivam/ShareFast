//
#include<time.h>
#include<stdio.h>
#include<error.h>
#include<errno.h>
#include<netdb.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>
#include<sys/wait.h>
#include<sys/time.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<sys/socket.h>


void start()
{
	printf("\n                          WELCOME                              \n");
	printf("\n-------------------- PERSONAL INFORMATION ---------------------\n\n");

	char *IP, *IP2, host[100];
	struct hostent *hen;
	if(gethostname(host, sizeof(host)) == -1)
	{
		perror("GETHOSTNAME");
		exit(0);
	}

	if((hen = gethostbyname(host))==NULL)
	{
		perror("GETHOSTINFO");
		exit(0);
	}
	IP = inet_ntoa(*((struct in_addr*) hen->h_addr_list[0]));
	printf("Your Name -> %s\n",host);
	printf("Your IP   -> %s\n",IP);

}




void main()
{
	start();

}
