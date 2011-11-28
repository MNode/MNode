/*
    Mesh Protocol Test 
        (C) 2012 
            Jason Hunt (nulluser@gmail.com)
            Robin Stamer (genoce@gmail.com)
    
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
                genoce@gmail.com

    File: network.c
*/

#define MODULE_NAME "[Network] "

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <memory.h>
#include <pthread.h>
#include <errno.h>

#include "datatap.h"
#include "mnode.h"
#include "network.h"

network_type * network;         // All Network Data


/* Add node to list */
int network_add_node(unsigned int tx_node_id)
{
    node_entry *tmp = network->node_list;

    // See if node is already in list 

    while (tmp)
    {
        if (tmp->node_id == tx_node_id)
            return MN_FAIL;
    
        tmp = tmp-> next;
    }

    // Insert node
    tmp = (node_entry*) malloc(sizeof(node_entry));
    tmp->next = network->node_list;
    tmp->node_id = tx_node_id;
    network->node_list = tmp;
    
    return MN_SUCCESS;
    
}
/* End of network_add_node */


/* Display node list */
void network_list_nodes( void )
{
    node_entry *tmp = network->node_list;
    
    network->text_out("Node | Status\n");
    
    while(tmp)
    {
        network->text_out(" [%d]    ---- \n", tmp->node_id);
        tmp = tmp->next;
    }
    
    network->text_out("\n");
}
/* End of network_list_nodes */


/* Free nodes list */
void network_free_nodes( void )
{
    node_entry *tmp = network->node_list;
    
    while(network->node_list)
    {
        tmp = network->node_list->next;
        free (network->node_list);
        network->node_list = tmp;
        
    }
}
/* End of network free nodes */


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




/* Send IDENT packet */
void network_ident (void )
{
    unsigned char buffer[100];

    unsigned int length = TX_DATA_OFS;

    unsigned int i = 0;
    
    buffer[i++] = length >> 8;
    buffer[i++] = length &  0xff;

    buffer[i++] = network->node_id >> 8;
    buffer[i++] = network->node_id &  0xff;

    buffer[i++] = 0;//node_id >> 8;
    buffer[i++] = 0;//node_id &  0xff;


    buffer[i++] = ID_IDENT;
    buffer[i++] = 0;  // No checksum for now
    
    buffer[TX_DATA_OFS-1] = get_checksum(buffer, length);
   
    network_send(buffer, length);

}
/* End of network_ident */

/* Send IDENT packet */
void network_identrq (void )
{
    unsigned char buffer[100];

    unsigned int length = TX_DATA_OFS;

    unsigned int i = 0;
    
    buffer[i++] = length >> 8;
    buffer[i++] = length &  0xff;

    buffer[i++] = network->node_id >> 8;
    buffer[i++] = network->node_id &  0xff;

    buffer[i++] = 0;//node_id >> 8;
    buffer[i++] = 0;//node_id &  0xff;


    buffer[i++] = ID_IDENTRQ;
    buffer[i++] = 0;  // No checksum for now
    
    buffer[TX_DATA_OFS-1] = get_checksum(buffer, length);
   
    network_send(buffer, length);

}
/* End of network_ident */




/* Send string packet */
void network_string (unsigned char *s )
{
    unsigned char buffer[256];
    unsigned int length = strlen((char *)s) + TX_DATA_OFS;

    unsigned int target_node_id = 0;

    unsigned int i = 0;

    buffer[i++] = length >> 8;
    buffer[i++] = length &  0xff;

    buffer[i++] = network->node_id >> 8;
    buffer[i++] = network->node_id &  0xff;
    
    buffer[i++] = target_node_id >> 8;
    buffer[i++] = target_node_id &  0xff;
        
    buffer[i++] = ID_STRING;
    buffer[i++] = 0; // No checksum for now
   
    strcpy((char*)(buffer + TX_DATA_OFS), (char *)s);
   
    buffer[length] = 0;
      
    // COmpute checksum  
    buffer[TX_DATA_OFS-1] = get_checksum(buffer, length); 
                 
    network_send(buffer, length);
}
/* End of network_string */






/* Send IDENT packet */
void network_datatap_poll (void )
{
    unsigned char buffer[100];

    unsigned int length = TX_DATA_OFS;

    unsigned int i = 0;
    
    buffer[i++] = length >> 8;
    buffer[i++] = length &  0xff;

    buffer[i++] = network->node_id >> 8;
    buffer[i++] = network->node_id &  0xff;

    buffer[i++] = 0;//node_id >> 8;
    buffer[i++] = 0;//node_id &  0xff;


    buffer[i++] = ID_DTPOLL;
    buffer[i++] = 0;  // No checksum for now
    
    buffer[TX_DATA_OFS-1] = get_checksum(buffer, length);
   
    network_send(buffer, length);

}
/* End of network_ident */



/* Send IDENT packet */
void network_datatap_data (datatap_type *dt)
{
    unsigned char buffer[100];

    unsigned int length = TX_DATA_OFS + 1  +32;

    if (dt->tap_type == DT_INT8) length += 1;
    if (dt->tap_type == DT_INT16) length += 2;
    if (dt->tap_type == DT_INT32) length += 4;


    unsigned int i = 0;
    
    
    buffer[i++] = length >> 8;
    buffer[i++] = length &  0xff;

    buffer[i++] = network->node_id >> 8;
    buffer[i++] = network->node_id &  0xff;

    buffer[i++] = 0;//node_id >> 8;
    buffer[i++] = 0;//node_id &  0xff;


    buffer[i++] = ID_DTDATA;
    
        buffer[i++] = 0;  // No checksum for now
    
    buffer[i++] = dt->tap_type;  // No checksum for now
    
      
    // Copy over tap name
    memset(buffer+i, 0, 32);
   strcpy((char *)(buffer+i), dt->tap_name);
    i+=32;
    
    
         unsigned int value =  *((unsigned int *)dt->tap_link);
       
       
       
    if (dt->tap_type == DT_INT8)
    {
        buffer[i++] = value & 0xff;
    }
    
    if (dt->tap_type == DT_INT16)
    {
        buffer[i++] = value >> 8;
        buffer[i++] = value & 0xff;
    }
    
    if (dt->tap_type == DT_INT32)
    {
        buffer[i++] = value >> 24;    
        buffer[i++] = value >> 16;    
        buffer[i++] = value >> 8;
        buffer[i++] = value & 0xff;
    }
        
    buffer[TX_DATA_OFS-1] = get_checksum(buffer, length);

    network_send(buffer, length);
}
/* End of network_ident */







/* Send data to all nodes */
int network_send(unsigned char *data, unsigned int length)
{
    struct sockaddr_in si_remote_temp;
    
    int s, slen=sizeof(si_remote_temp);

    int yes = 1; 
    int status;


    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
    {
        network->text_out("socket error\n");
        return MN_FAIL;
    }
   
    memset((char *) &si_remote_temp, 0, sizeof(si_remote_temp));
    si_remote_temp.sin_family = AF_INET;
    si_remote_temp.sin_port = htons(LISTEN_PORT);
        
           
    status = setsockopt(s, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(int) );
    //  network->text_out("Setsockopt Status = %d\n", status);        
        
        
    if (   inet_pton(AF_INET, BCAST_IP, (struct sockaddr *)&si_remote_temp.sin_addr) == 0 )
    {
        network->text_out("inet_aton() failed\n");
        return MN_FAIL;
    }
          
    //snetwork->text_out(buf, "%s", data);
    
    if (sendto(s, data, length, 0, (struct sockaddr *)&si_remote_temp, slen)==-1)
    {
        network->text_out("Sendto fail\n");
        return MN_FAIL;
       
    }
    
    close(s);

    return MN_SUCCESS;
}
/* End of network_send */



/* Core network thread */
void *network_thread( void *threadid )
{
    unsigned char  buf[BUFLEN];

    socklen_t      slen=sizeof(network->si_remote);

    unsigned int time_count = 0;
    
    network->text_out(MODULE_NAME "Network Thread - Started\n");
    
    while(network->running)
    {
        struct timeval tval;

        fd_set readfds;

        tval.tv_sec = 1;
        tval.tv_usec = 0;
    
        FD_ZERO(&readfds);
        FD_SET(network->server_sd, &readfds);


        select(network->server_sd+1, &readfds, NULL, NULL, &tval);

        if (FD_ISSET(network->server_sd, &readfds))
        {
            int ret = recvfrom(network->server_sd, (char *)buf, BUFLEN, 0, (struct sockaddr *)&network->si_remote, &slen);
           
            if (ret ==-1)
            {
               network->text_out("recvfrom fail");
                continue;
            }
       
            network->mesh_parser(buf, ret);

        }
        else
        {
            time_count++;
        
            if (time_count == 10)
            {
                time_count = 0;
                network_ident();            // Set out  keep alive
            }
         
            network->mesh_update();
                              
            // Timeout ?
        }   
        
        /*                        
        network->text_out("Data from: %s:%d\n", inet_ntoa(si_remote.sin_addr),
                                    ntohs(si_remote.sin_port));
                                   
          network->text_out(" %s\n", buf);*/
    }
    
   
    network->text_out(MODULE_NAME "Shutdown\n");

    pthread_exit(NULL); // exit thread
}
/* End of network_thread */


/* Init the network */
int network_init( void )
{

    // load node id
    
    network->node_id = 0;
    FILE *f = fopen("node_id", "rt");
    if (!f)
    {
        network->text_out("Unable to load node_id\n");
        return MN_FAIL;
    }

    fscanf(f, "%d", &network->node_id);
    
    if (network->node_id == 0)
    {
        network->text_out("node_id 0 not allowed\n");
        return MN_FAIL;
    }

    network->text_out("Node id: %d\n", network->node_id);


//    int i;
    
    if ((network->server_sd=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
    {
        network->text_out(MODULE_NAME "socket error\n");
        return MN_FAIL;
    }
    
    
/*    int sock = socket(...);
int flags = fcntl(sock, F_GETFL);
flags |= O_NONBLOCK;
fcntl(sock, F_SETFL, flags);*/
        
    
    
    memset((char *) &network->si_local, 0, sizeof(network->si_local));
    
    network->si_local.sin_family = AF_INET;
    network->si_local.sin_port = htons(LISTEN_PORT);
    network->si_local.sin_addr.s_addr = htonl(INADDR_ANY);

     int on = 1;
     if (setsockopt(network->server_sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) != 0)
     {
     
        network->text_out(MODULE_NAME "setsockopt fail");
        return MN_FAIL;
     }
 
    if (bind(network->server_sd, (struct sockaddr *)&network->si_local, sizeof(network->si_local))==-1)
    {
        network->text_out(MODULE_NAME "bind error %d\n", errno);
        return MN_FAIL;
    }
    
    network->running = 1;
    
    return MN_SUCCESS;
}
/* End of network_init */


/* Start network layer */
int network_start( void (*mesh_parser_link)(unsigned char *, unsigned int), 
                        void (*mesh_update)(void) ,
                            void (*out_func)(char * format, ...) )
{
    network = (network_type *) malloc (sizeof(network_type));
    
    // Init values
    network->node_id = 0;
    network->mesh_parser = NULL;
    network->running = 0;


    network->node_list = NULL;


    // Link Parser
    network->mesh_parser = mesh_parser_link;

    // Link Parser
    network->mesh_update = mesh_update;

    network->text_out= out_func;

    if (network_init())
    {
        network->text_out(MODULE_NAME "Network setup failed, operating in write-only mode\n");
        return MN_FAIL;
    }

    pthread_create(&network->network_t, NULL, network_thread, NULL);
    
    network_ident (); // send own init
    network_identrq (); // request ident from other nodes
    
    
    return MN_SUCCESS;
    
}
/* End of network_start */


/* Stop network layer */
void network_stop( void )
{
    network->running = 0;
 
    sleep(2);
  
    close(network->server_sd);
    
    network_free_nodes();
}
/* End of network stop*/



