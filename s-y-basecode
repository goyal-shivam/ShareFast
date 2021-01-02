#include "header.h"

void end(int sig)////////////////////// SIGNAL HANDLER
{
	close(P_FD);
	printf("\n                         THANK YOU                              \n");
	exit(0);

}


void start()///////////////////////// Display host IP and bind P_FD to P_PORT
{
	printf("\n                          WELCOME                              \n\n");
	printf("\n------------------- INFO AND INITIALIZATION -------------------\n");

	char *IP, *IP2, host[100];
	struct hostent *hen;
	if(gethostname(host, sizeof(host)) == -1)
	{
		perror("Host information error");
		end(1);
	}

	if((hen = gethostbyname(host))==NULL)
	{
		perror("Host information error");
		end(1);
	}

		
	


	struct sockaddr_in localbind;
	P_FD = socket(AF_INET, SOCK_DGRAM, 0);
	if(P_FD==-1) 
	{ 
		perror("Socket error");
		end(1);
	}

	int tttt = 1; 
	if (setsockopt(P_FD, SOL_SOCKET, SO_REUSEADDR, &tttt , sizeof(int)) < 0)
    	{
		perror("Setsockopt error");
		end(1);
	}

	localbind.sin_family       = AF_INET;
	localbind.sin_addr.s_addr  = htonl(INADDR_ANY);
	localbind.sin_port         = htons(P_PORT);
	if (bind(P_FD, (struct sockaddr *) &localbind, sizeof(localbind)) == -1)
	{ 
		perror("Bind error");
		end(1);
	}
	

	IP = inet_ntoa(*((struct in_addr*) hen->h_addr_list[0]));
	printf("Your Name       -> %s\n", host);
	printf("Your IP         -> %s\n", IP);
	printf("Public UDP Port -> %d\n", P_PORT);



}

void recieve()/////////////// RECIEVE
{
		printf("\n--------------------------- RECIEVE ---------------------------\n\n");
		printf("Listening for %d seconds",R_WAIT);
		for(int i=0; i<R_WAIT; i++)
		{
			fflush(stdout);
			sleep(1);
			printf(".");
		}
		printf("\n\nNo sender has contacted\n");

}

int checkfile(char* filename, struct istoc* x, FILE** fp){
	DIR* d = opendir(directory);
	struct dirent* dir;
	
	if(d)
	{
		while((dir = readdir(d)) != NULL)
			if(strcmp(dir->d_name, filename) == 0){
				char newfile[MAX_FILE_NAME];
				newfile[0] = '\0';
				strcat(newfile, directory);
				strcat(newfile, "/");
				strcat(newfile, filename);
				
				*fp = fopen(newfile, "r");
				fseek(*fp,0L,SEEK_END);				
				x->size = ftell(*fp);
				fseek(*fp,0L,SEEK_SET);
			
				return 0;
			}
		
		closedir(d);
	}
	
	return -1;
}

int getblock(FILE* fp, char* arr,int block){

	int offset = block-1;
	if(offset < 0){
		printf("SERVER REQUEST: Error, block number entered is %d\n",block);
		return -1;
	}
	offset *= MAX_DATA_PACKET_SIZE;
	fseek(fp, offset, SEEK_SET);
	
	int fread_return = fread(arr, sizeof(char), MAX_DATA_PACKET_SIZE, fp);
		
	return fread_return;
}

void sendd()/////////////// SEND
{
		char IP[30];
		char fname[MAX_FILE_NAME]; //Store the file name
		printf("\n---------------------------- SEND -----------------------------\n\n");
		printf("Enter the destination IP: ");
		scanf("%s", IP);
		
		printf("Enter the location of file to send: ");
		scanf("%s", fname);


		// 
		// if checkfile( fname, struct *istoc, file ** ) > 0 then show error and return.
		
		//else
		//1. open the file
		//2. check no. of bytes
		

		//                       //



		//int  getblock( file*, char*, int blockno   )  -> returns no. of bytes read
		
		
		

}


void main()
{
	signal(SIGINT, end);	
	start();
	char choice[10];
	
	printf("\n---------------------------- START ----------------------------\n\n");

	printf("Enter the option number to execute -    1: Recieve\n\t\t\t\t\t2: Send\n\t\t\t\t\t3: Exit\n>> ");
	while(1)
	{
		scanf("%s",choice);
		if(((choice[0]!='1')&&(choice[0]!='2')&&(choice[0]!='3'))||(strlen(choice)!=1))
			printf("Please enter a valid option!\n>> ");
		
		else break;
	}


	if(choice[0]=='1')
	{
		recieve();
	}

	else if(choice[0]=='2')
	{
		sendd();	
	}
	
	else if(choice[0]=='3')
		end(1);


}
