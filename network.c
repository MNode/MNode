/*
    Mesh Protocol Test 
        (C) 2012 
            Jason Hunt (nulluser@gmail.com)
            Ling
               
    
    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

    Contact:    nulluser@gmail.com
                Ling

    File: network.c
*/

#define MODULE_NAME "[Network] "

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <memory.h>
#include <errno.h>
#include <arpa/inet.h>


#include "network.h"


#define BUFLEN 4096 
#define LISTEN_PORT 9998
#define BCAST_IP "255.255.255.255"

int server_sd;              // server socket

pthread_t network_t;

int node_id = 0;

struct sockaddr_in si_local, si_remote;
void (*mesh_parser)(unsigned char *, unsigned int) = NULL;


int network_running = 0;




/* Compute checksum of packet */
unsigned int get_checksum(unsigned char *buff, unsigned int len)
{
	unsigned char ck = 0;
	
	while(len)
	{
		ck += buff[--len];
	}

	return(ck);
}
/* End of get_checksum */





void network_ident (void )
{

unsigned char buffer[100];

    unsigned int length = 6;

    buffer[0] = length >> 8;
    buffer[1] = length &  0xff;

    buffer[2] = node_id >> 8;
    buffer[3] = node_id &  0xff;

    buffer[4] = ID_IDENT;
    buffer[5] = get_checksum(buffer, 5);
    
    network_send(buffer, length);

}



void network_string (char *s )
{

/*unsigned char buffer[100];

    unsigned int length = 6;

    buffer[0] = length >> 8;
    buffer[1] = length &  0xff;

    buffer[2] = node_id >> 8;
    buffer[3] = node_id &  0xff;

   buffer[4] = ID_IDENT;
    
    
    buffer[5] = get_checksum(buffer, 5);
    
    network_send(buffer, length);*/

}











/* Send data to all nodes */
int network_send(unsigned char *data, unsigned int length)
{
    struct sockaddr_in si_remote_temp;
    
    int s, i, slen=sizeof(si_remote_temp);
    
   

    int yes = 1; 
    int status;


    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
    {
        printf("socket error\n");
        return 1;
    }
    
    
   
    memset((char *) &si_remote_temp, 0, sizeof(si_remote_temp));
    si_remote_temp.sin_family = AF_INET;
    si_remote_temp.sin_port = htons(LISTEN_PORT);
        
           
      status = setsockopt(s, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(int) );
    //  printf("Setsockopt Status = %d\n", status);        
        
        
    if (   inet_pton(AF_INET, BCAST_IP, (struct sockaddr *)&si_remote_temp.sin_addr) == 0 )
    {
        fprintf(stderr, "inet_aton() failed\n");
        return 1;
    }
          
    //sprintf(buf, "%s", data);
    
    if (sendto(s, data, length, 0, (struct sockaddr *)&si_remote_temp, slen)==-1)
    {
        printf("Sendto fail\n");
        return 1;
       
    }
    
    close(s);

    return 0;
}
/* End of network_send */



/* Core network thread */
void *network_thread( void *threadid )
{
    printf(MODULE_NAME "Network Thread - Started\n");
    
    char buf[BUFLEN];

    int  slen=sizeof(si_remote);
    
    while(network_running)
    {
        int ret = recvfrom(server_sd, buf, BUFLEN, 0, (struct sockaddr *)&si_remote, &slen);
       
        if (ret ==-1)
        {
            printf("recvfrom fail");
            continue;
            
        }
       
        mesh_parser(buf, ret);
        
/*                        
        printf("Data from: %s:%d\n", inet_ntoa(si_remote.sin_addr),
                                    ntohs(si_remote.sin_port));
                                   
          printf(" %s\n", buf);*/
    }
    
   
    printf(MODULE_NAME "Shutdown\n");

    pthread_exit(NULL); // exit thread
}
/* end of network_thread */




/* init the network */
int network_init( void )
{

    // load node id
    
    node_id = 0;
    FILE *f = fopen("node_id", "rt");
    if (!f)
    {
        printf("Unable to load node_id\n");
        return 1;
    }

    fscanf(f, "%d", &node_id);
    
    if (node_id == 0)
    {
        printf("node_id 0 not allowed\n");
        return  1;
    }

    printf("Node id: %d\n", node_id);


    int i;
    
    if ((server_sd=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
    {
        printf(MODULE_NAME "socket error\n");
        return 1;
    }
    
    memset((char *) &si_local, 0, sizeof(si_local));
    
    si_local.sin_family = AF_INET;
    si_local.sin_port = htons(LISTEN_PORT);
    si_local.sin_addr.s_addr = htonl(INADDR_ANY);

     int on = 1;
     if (setsockopt(server_sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) != 0)
     {
     
        printf(MODULE_NAME "setsockopt fail");
        return 1;
     }
 
    if (bind(server_sd, (struct sockaddr *)&si_local, sizeof(si_local))==-1)
    {
        printf(MODULE_NAME "bind error %d\n", errno);
        return 1;
    }
    
    network_running = 1;
    
    return 0;
}
/* End of network_init */

/* Start network layer */
void network_start( void (*mesh_parser_link)(unsigned char *, unsigned int) )
{
    // Link Parser
    mesh_parser = mesh_parser_link;

    if (network_init())
    {
        printf(MODULE_NAME "Network setup failed\n");
        return;
    }

    pthread_create(&network_t, NULL, network_thread, NULL);
}
/* End of network_start */

/* Stop network layer */
void network_stop( void )
{
    network_running = 0;
 
    sleep(1);
  
    close(server_sd);
}
/* End of network stop*/



