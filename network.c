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

#define BUFLEN 512   
#define LISTEN_PORT 9998
#define BCAST_IP "255.255.255.255"

int server_sd;              // server socket

pthread_t network_t;


struct sockaddr_in si_local, si_remote;
void (*mesh_parser)(char *) = NULL;


int network_running = 0;


/* Send data to all nodes */
int network_send(char *data)
{
    struct sockaddr_in si_remote_temp;
    
    int s, i, slen=sizeof(si_remote_temp);
    
    char buf[BUFLEN];

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
          
    sprintf(buf, "%s", data);
    
    if (sendto(s, buf, BUFLEN, 0, (struct sockaddr *)&si_remote_temp, slen)==-1)
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
        if (recvfrom(server_sd, buf, BUFLEN, 0, (struct sockaddr *)&si_remote, &slen)==-1)
        {
            printf("recvfrom fail");
            continue;
            
        }
       
        mesh_parser(buf);
        
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
void network_start( void (*mesh_parser_link)(char *) )
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



