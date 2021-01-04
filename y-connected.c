#include "header.h"

void end(int sig)////////////////////// SIGNAL HANDLER
{
	close(P_FD);
	for(int i=0; i<MAX_CLIENTS; i++)
		close(open_FDs[i]);
	
	printf("\n                         THANK YOU                              \n");
	exit(0);

}

int getfreeFD()
{
	for(int i=0; i<MAX_CLIENTS; i++)
		if(used_FDs[i]==0)
		{
			used_FDs[i]=1;
			return open_FDs[i];
		}

	return -1;
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


		printf("Waiting for %d seconds",R_WAIT);
		struct timeval select_time;
		struct sockaddr_in clientaddress;
		int recvd = 0;
		fd_set SET;
		char ch[30];

		for(int i=0; i<R_WAIT; i++)
		{
			select_time.tv_sec  = 1;
			select_time.tv_usec = 0;
			FD_ZERO (&SET);
			FD_SET  (P_FD,&SET);
			istoc ii;

			if(select(P_FD+1, &SET, NULL, NULL, &select_time)>0)
			{
				printf("\nRequest recieved\n");
				int address_len = sizeof(struct sockaddr);

//////////////////////////////////////////////////
				recvfrom(P_FD, &ii, sizeof(ii), 0, (struct sockaddr *)&clientaddress, &address_len);

					
				char IP2[INET_ADDRSTRLEN];
				if(inet_ntop( AF_INET, &clientaddress.sin_addr, IP2, INET_ADDRSTRLEN )<0)
				{

					printf("IP resolve error\n");
					return;
				}

				printf(" %s  IP wants to send the file > %s (%d bytes) <\n",IP2, ii.filename, ii.size);
				recvd = 1;
				printf("Do you want to recieve the file? (Y/N)\n>> ");

				while(1)
				{
					scanf("%s",ch);
					if(((ch[0]!='Y')&&(ch[0]!='N'))||(strlen(ch)!=1))
					{
						printf("Please enter either Y or N\n>> ");
						continue;
					}
					else break;
				}


				int todo;
				if(ch[0]=='N')
				{
					printf("Rejecting send request...\n");
					todo=-1;
				}
				else
				{
					printf("Accepting send request...\n");
					todo=0;
				}

////////////////////////////////// Process request at new port
				
				int next_FD = getfreeFD();
				if(next_FD==-1)
				{	
					printf("Error: Max client limit of %d reached... please wait for a transfer to finish\n",MAX_CLIENTS);
					return;
				}

				struct sockaddr_in sin;
				socklen_t len = sizeof(sin);
				if (getsockname(next_FD, (struct sockaddr *)&sin, &len) == -1)
				{
					perror("getsockname");
					return;
				}
				

				
				ictos reply;
				reply.ack=todo;
				reply.port = ntohs(sin.sin_port);
				clientaddress.sin_port = htons(ii.port);
				sendto(next_FD, &reply,sizeof(reply) ,0, (struct sockaddr*)&clientaddress, sizeof(struct sockaddr));

				if(todo==-1)
					break;

				else
				{
				//////////////// Do transfer here on -> next_FD
				////////////////		     -> clientaddress, [NOTE that:    address_len = sizeof(clientaddress)]

















					
					break;
				}
			
			}

			
			fflush(stdout);
			sleep(1);
			printf(".");
		}
		if(recvd==0)
		{
			printf("\n");
			printf("No sender has contacted\n");
			return;
		}

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


void sendd() /////////////// SEND
{
		char IP[30];
		char fname[MAX_FILE_NAME]; //Store the file name
		printf("\n---------------------------- SEND -----------------------------\n\n");
		printf("Enter the destination IP: ");
		scanf("%s", IP);
		
		printf("Enter the location of file to send: ");
		scanf("%s", fname);

		
		istoc cc;
		strcpy(cc.filename,fname);
		FILE *ff;
		if(checkfile(fname, &cc, &ff)<0)
		{
			printf("Error: File not found on your system!\n");
			return;
		}




		struct sockaddr_in conn;	
		conn.sin_port   = htons(P_PORT);
		conn.sin_family = AF_INET;
		if( inet_pton(AF_INET,IP, &conn.sin_addr) < 0)
		{
			printf("Error with IP \n");
			return;
		}

	
		//if(next_FD == -1)
			//printf();
		

		int next_FD = getfreeFD();
		if(next_FD==-1)
		{	
			printf("Error: Max client limit of %d reached... please wait for a transfer to finish\n",MAX_CLIENTS);
			return;
		}

		struct sockaddr_in sin;
		socklen_t len = sizeof(sin);
		if (getsockname(next_FD, (struct sockaddr *)&sin, &len) == -1)
		{
			perror("getsockname");
			return;
		}
				
		cc.port = ntohs(sin.sin_port);



		sendto(P_FD, &cc, sizeof(cc), 0, (struct sockaddr*)&conn, sizeof(struct sockaddr));
		printf("Waiting for %d seconds",S_WAIT);

		struct timeval select_time;
		struct sockaddr_in clientaddress;
		int recvd = 0;
		fd_set SET;
		ictos ic;

		for(int i=0; i<S_WAIT; i++)
		{
			select_time.tv_sec  = 1;
			select_time.tv_usec = 0;
			FD_ZERO (&SET);
			FD_SET  (next_FD,&SET);

			if(select(next_FD+1, &SET, NULL, NULL, &select_time)>0)
			{
				
				printf("\nRecieved reply\n");
				int address_len = sizeof(struct sockaddr);

//////////////////////////////////////////////////
				recvfrom(next_FD, &ic, sizeof(ictos), 0, (struct sockaddr *)&clientaddress, &address_len);
				
				if(ic.ack==0)
				{
					printf("Receiver has accepted the send request\n");
				///////////////// Transfer data here -> next_FD
				/////////////////		     -> clientaddress, [NOTE that:    address_len = sizeof(clientaddress)]





			













				}
				else
					printf("Receiver has denied the send request\n");
				
				recvd = 1;
			        break;
			
			}

			
			fflush(stdout);
			sleep(1);
			printf(".");
		}

		if(recvd == 0)
		{
			printf("No reply recieved. Please ensure reciever is online.\n");
			return;
		}
		printf("\n");
		
}



void initt() //Initialise the pool of FDs
{
	struct sockaddr_in localbind;
	int tttt = 1; 
	
	for(int i=0; i<MAX_CLIENTS; i++)
	{
		open_FDs[i] = socket(AF_INET, SOCK_DGRAM, 0);
		if(open_FDs[i]==-1) 
		{ 
			perror("Socket error <>\n");
			end(1);
		}
	
	
		localbind.sin_family       = AF_INET;
		localbind.sin_addr.s_addr  = htonl(INADDR_ANY);
		localbind.sin_port         = htons(0);
		if (bind(open_FDs[i], (struct sockaddr *) &localbind, sizeof(localbind)) == -1)
		{ 
			perror("Bind error <>\n");
			end(1);
		}
		used_FDs[i]=0;			
		//printf("Added FD -> %d\n",open_FDs[i]);
	}




}




void main()
{
	signal(SIGINT, end);	
	initt();
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

