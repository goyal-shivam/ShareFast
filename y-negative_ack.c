#include "header.h"

void end(int sig)		////////////////////// SIGNAL HANDLER
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



void start()			///////////////////////// Display host IP and bind P_FD to P_PORT
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
					if(((ch[0]!='Y')&&(ch[0]!='N')&&(ch[0]!='y')&&(ch[0]!='n'))||(strlen(ch)!=1))
					{
						printf("Please enter either Y or N\n>> ");
						continue;
					}
					else break;
				}


				int todo;
				if(ch[0]=='N' || ch[0] == 'n')
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


					int send_FD = socket(AF_INET, SOCK_DGRAM, 0);
					struct timeval before, after, result;
					gettimeofday(&before, NULL);


				/// receive the file
				/// create a file

				/// assign bytes to the file

/*
				STEPS:
					1. allocate a file of those many number of bytes
					2. find out number of blocks & create ack array; structures for sending and receiving data
					3. receive a block, register it in your database, send ack for it
						check if same block is being sent twice
					4. continue 3. until all the blocks have been received
*/

				// 1. allocate a file of those many number of bytes
				FILE* ptr = fopen(ii.filename, "w");
				for(int i=0;i<ii.size;++i){
					fprintf(ptr, "a");
				}
				
				// 2. find out number of blocks
				int num_packets = (ii.size-1)/MAX_DATA_PACKET_SIZE + 1, rcvd_packets =  0;
				int rcvd[num_packets];
				
				for(int j=0;j<num_packets; ++j)
					rcvd[j] = 0;
				
				dstoc DSTOC;
				dctos DCTOS;
				int fwrite_return;
				
				struct pollfd fds;
				fds.fd = next_FD;
				fds.events = POLLIN;
				int poll_return;
				
			
				// 3. receive a block, register it in your database, send ack for it
				//		check if same block is being received twice
		
				
				printf("Total packets -> %d\n",num_packets);	
				printf("starting to receive data\n");
				
				
				
				int all_done=0;
				int overall_mark=0;

				while(!all_done)
				{

					recvfrom(next_FD, &DSTOC, sizeof(DSTOC), 0, (struct sockaddr *)&clientaddress, &address_len);
					clientaddress.sin_port = htons(ii.port);
					
					if(DSTOC.block!=0)
					{	
						//printf("Block %d received\n", DSTOC.block);
						if(rcvd[DSTOC.block-1]==1)
							continue;



						int num_chars = 0;

						if(DSTOC.block == num_packets)
							num_chars = (ii.size-1) % MAX_DATA_PACKET_SIZE + 1;
						
						else
							num_chars = MAX_DATA_PACKET_SIZE;
							
						putblock(ptr, DSTOC.data, DSTOC.block, num_chars);				



						rcvd[DSTOC.block-1]=1;
					}



					else
					{
						printf("Flushing ngative acknowledgements\n");
						
						int counter=0;
						for(int xx=0; xx<num_packets && counter<WINDOW_SIZE; xx++)
							if(rcvd[xx]==0)
							{
								printf("%d. Flush: -ve block -> %d\n",counter+1, xx+1);
								DCTOS.array[counter++]=xx+1;
							}

						DCTOS.array[counter]=0;
						printf("%d/%d packets remaining\n",counter,num_packets);
						DCTOS.mark_num= ++overall_mark;
						
						if(counter==0){ 
							all_done=1;
							// send FLUSH_ACK;
							DCTOS.mark_num = 0;
						}

						sendto(send_FD, &DCTOS, sizeof(DCTOS), 0, (struct sockaddr *)&clientaddress, sizeof(clientaddress));
						clientaddress.sin_port = htons(ii.port);
					}



				}

				


					

				fclose(ptr);

				printf("successfully received the file completely\n");
						
						
				gettimeofday(&after, NULL);
				timersub(&after, &before, &result);
				printf("Time elapsed : %ld.%06ld\n\t\n", (long int)result.tv_sec, (long int)result.tv_usec);

				printf("Speed = %f Mb/s\n",(0.000001*(float)ii.size)/((float)(result.tv_sec)+((float)(result.tv_usec)/1000000)));

				recvd =1;
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

int putblock(FILE* fp, char*arr, int block, int num_chars){
	int offset = block -1;
	if(offset<0){
		printf("SERVER REQUEST: Error in putblock, block number entered is %d\n",block);
		return -1;
	}
	
	offset *= MAX_DATA_PACKET_SIZE;
	
	fseek(fp, offset, SEEK_SET);
	
	int fwrite_return = fwrite(arr, sizeof(char), num_chars, fp);
		
	return fwrite_return;
}

int getblock(FILE* fp, char* arr,int block, int num_chars){

	int offset = block-1;
	if(offset < 0){
		printf("SERVER REQUEST: Error in getblock, block number entered is %d\n",block);
		return -1;
	}
	offset *= MAX_DATA_PACKET_SIZE;
	fseek(fp, offset, SEEK_SET);
	
	int fread_return = fread(arr, sizeof(char), num_chars, fp);
		
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


				clientaddress.sin_port = htons(ic.port);
				
				if(ic.ack==0)
				{
					printf("Receiver has accepted the send request\n");
					///////////////// Transfer data here -> next_FD
					/////////////////		     -> clientaddress, [NOTE that:    address_len = sizeof(clientaddress)]

					int send_FD = socket(AF_INET, SOCK_DGRAM, 0);

					struct timeval before, after, result;
					gettimeofday(&before, NULL);


	/*
					ff is the file pointer

					STEPS :-
					1. find out #blocks, initialize acknowledgements array
					2. keep sending all the blocks, if they have not been sent, and simultaneously check poll also
						if poll arrives, check it
					3. keep doing 2. until all blocks have been acknowledged
					
	*/

					// 1. find out #blocks, initialize acknowledgements array
					int num_packets = (cc.size - 1) / MAX_DATA_PACKET_SIZE + 1;
					int ack[num_packets];
					
					for(int j=0;j<num_packets;++j)
						ack[j] = 0;
					
					//int next_block=-1;
					int next_block=0;
					dstoc DSTOC;
					dctos DCTOS;
					int fread_return;
					
					struct pollfd fds;
					fds.fd = next_FD;
					fds.events = POLLIN;
					int num_chars;
					int overall_mark; // Keep track of ack flush arrays recieved till now

					// 2. keep sending all the blocks, if they have not been sent, and simultaneously check poll also
					//	if poll arrives, check it

					int all_done=0;  // This will be set to 1 when all blocks have been done
					while(!all_done){
						// ++next_block;


						for(int sending_packet=1; sending_packet <= num_packets; sending_packet++)
						{
						
							if(ack[sending_packet-1]==1)
								continue;
							

							//printf("Sending packet -> %d\n",sending_packet);





							if(sending_packet == num_packets)
								num_chars = (cc.size-1) % MAX_DATA_PACKET_SIZE + 1;

							else
								num_chars = MAX_DATA_PACKET_SIZE;

							

							DSTOC.block = sending_packet;
							fread_return = getblock(ff, DSTOC.data, sending_packet, num_chars);
							
							// struct pollfd poll_out;
							// poll_out.fd = send_FD;
							// poll_out.events = POLLOUT;
							
							// if(poll(poll_out, 1, ))
							
							if((sendto(send_FD, &DSTOC, sizeof(DSTOC), 0, (struct sockaddr *)&clientaddress, sizeof(clientaddress)))<sizeof(DSTOC))
							{
								printf("Sendto error\n");
								return;
							}
							clientaddress.sin_port = htons(ic.port);


							printf("Packet %d sent\n",sending_packet);
							usleep(50);
						}
						
						for(int z=0; z<num_packets; z++){
							ack[z] = 1;
						}
						
						DSTOC.block=0;          // Sending flusk ACK request
						if((sendto(send_FD, &DSTOC, sizeof(DSTOC), 0, (struct sockaddr *)&clientaddress, sizeof(clientaddress)))<sizeof(DSTOC))
						{
							printf("Sendto error\n");
							return;
						}
						if((sendto(send_FD, &DSTOC, sizeof(DSTOC), 0, (struct sockaddr *)&clientaddress, sizeof(clientaddress)))<sizeof(DSTOC))
						clientaddress.sin_port = htons(ic.port);
							printf("Sendto error\n");
						if((sendto(send_FD, &DSTOC, sizeof(DSTOC), 0, (struct sockaddr *)&clientaddress, sizeof(clientaddress)))<sizeof(DSTOC))
						clientaddress.sin_port = htons(ic.port);
							printf("Sendto error\n");
						if((sendto(send_FD, &DSTOC, sizeof(DSTOC), 0, (struct sockaddr *)&clientaddress, sizeof(clientaddress)))<sizeof(DSTOC))
							printf("Sendto error\n");
						clientaddress.sin_port = htons(ic.port);
						printf("Flush ACK request sent\n");	



						fds.fd = next_FD;
						fds.events = POLLIN;
						int poll_ret=0, counter=0;
						while(1){
							// receive acknowledgements
						
								
							if(counter==0)
							{

								poll_ret = poll(&fds,1,100000);
								if(poll_ret==0)
								{
									printf("Reciever is offline!\n");
									return;
								}

								recvfrom(next_FD, &DCTOS, sizeof(DCTOS), 0, (struct sockaddr *)&clientaddress, &address_len);
								clientaddress.sin_port = htons(ic.port);
								counter++;
							}
							
							else
							{
								poll_ret = poll(&fds,1,0);
								if(poll_ret==0)
									break;
								
								recvfrom(next_FD, &DCTOS, sizeof(DCTOS), 0, (struct sockaddr *)&clientaddress, &address_len);
								clientaddress.sin_port = htons(ic.port);
							}
								
							printf("NACK array %d has been received\n",DCTOS.mark_num);
							if(DCTOS.array[0]==0)
							{
								all_done=1;
								break;
							}				
			

							if(DCTOS.mark_num>overall_mark)
							{
								printf("Processing\n");
								for(int z=0; z<MAX_DATA_PACKET_SIZE; z++)
								{
									if(DCTOS.array[z]==0)
										break;

									// ack[DCTOS.array[z]-1]=1;
									ack[DCTOS.array[z]-1]=0;
									
								}
							

							}

						}
							
						fds.fd = next_FD;
						fds.events = POLLIN;
						
					
					}


					
					fclose(ff);
					printf("successfully sent the file completely\n");
						
						
					gettimeofday(&after, NULL);
					timersub(&after, &before, &result);
					printf("Time elapsed : %ld.%06ld\n\t\n", (long int)result.tv_sec, (long int)result.tv_usec);
				
					printf("Speed = %f Mb/s\n",(0.000001*(float)cc.size)/((float)(result.tv_sec)+((float)(result.tv_usec)/1000000)));


					recvd =1;
					break;	
				}
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
