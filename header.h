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
#include<dirent.h>


#define MAX_FILE_NAME 50 // Maximum length of file name




#define R_WAIT 10 // No. of seconds to wait while recieving
#define S_WAIT 10 // No. of seconds to wait for receiver to respond

#define P_PORT 502 // The port to initially connect




#define MAX_CLIENTS 10	// Size of FD pool
int open_FDs[MAX_CLIENTS];  // ready FDs alotted for use
int used_FDs[MAX_CLIENTS];  // 1: FD busy   0: FD free





#define MAX_DATA_PACKET_SIZE 512

#define directory "."

typedef struct istoc{   // initialize server to client
    char filename[MAX_FILE_NAME];
    int size;   // denotes the number of bytes that this file contains
    int port;
} istoc;

typedef struct ictos{   // initialize client to server
    int ack;
    int port;
} ictos;

typedef struct dstoc{   // data transfer server to client
    int block;  // block number of this data
    char data[MAX_DATA_PACKET_SIZE];
} dstoc;

typedef struct dctos{   // data transfer server to client
    int block;    // block number for which acknowledgement is being sent
} dctos;


int 	P_FD;	   // The UDP FD for P_PORT 

