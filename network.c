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
#include <unistd.h> 
#include <stdlib.h>
#include <memory.h>
#include <pthread.h>
#include <errno.h>

#include "datatap.h"
#include "mnode.h"
#include "network.h"


/* Add node to list */
int network_add_node( network_type *n, unsigned int tx_node_id )
{
    node_entry *tmp = n->node_list;

    // See if node is already in list 

    while (tmp)
    {
        if (tmp->node_id == tx_node_id)
            return MN_FAIL;
    
        tmp = tmp-> next;
    }

    // Insert node
    tmp = (node_entry*) malloc(sizeof(node_entry));

    tmp->next = n->node_list;
    tmp->node_id = tx_node_id;
    n->node_list = tmp;
    
    return MN_SUCCESS;
}
/* End of network_add_node */


/* Display node list */
void network_list_nodes( network_type *n )
{
    node_entry *tmp = n->node_list;
    
    n->text_out("Node | Status\n");
    
    while(tmp)
    {
        n->text_out(" [%d]    ---- \n", tmp->node_id);
        tmp = tmp->next;
    }
    
    n->text_out("\n");
}
/* End of network_list_nodes */


/* Free nodes list */
void network_free_nodes ( network_type *n )
{
    node_entry *tmp = n->node_list;
    
    while(n->node_list)
    {
        tmp = n->node_list->next;
        free (n->node_list);
        n->node_list = tmp;
        
    }
}
/* End of network free nodes */


/* Compute checksum of packet */
unsigned int get_checksum ( unsigned char *buff, unsigned int len )
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
void network_ident ( network_type *n )
{
    unsigned char buffer[100];

    unsigned int length = TX_DATA_OFS;

    unsigned int i = 0;
    
    buffer[i++] = length >> 8;
    buffer[i++] = length &  0xff;

    buffer[i++] = n->node_id >> 8;
    buffer[i++] = n->node_id &  0xff;

    buffer[i++] = 0;//node_id >> 8;
    buffer[i++] = 0;//node_id &  0xff;


    buffer[i++] = ID_IDENT;
    buffer[i++] = 0;  // No checksum for now
    
    buffer[TX_DATA_OFS-1] = get_checksum(buffer, length);
   
    network_send(n, buffer, length);

}
/* End of network_ident */


/* Send IDENT packet */
void network_identrq ( network_type *n )
{
    unsigned char buffer[100];

    unsigned int length = TX_DATA_OFS;

    unsigned int i = 0;
    
    buffer[i++] = length >> 8;
    buffer[i++] = length &  0xff;

    buffer[i++] = n->node_id >> 8;
    buffer[i++] = n->node_id &  0xff;

    buffer[i++] = 0;//node_id >> 8;
    buffer[i++] = 0;//node_id &  0xff;


    buffer[i++] = ID_IDENTRQ;
    buffer[i++] = 0;  // No checksum for now
    
    buffer[TX_DATA_OFS-1] = get_checksum(buffer, length);
   
    network_send(n, buffer, length);

}
/* End of network_ident */


/* Send string packet */
void network_string ( network_type *n, unsigned char *s )
{
    unsigned char buffer[256];
    unsigned int length;
    unsigned int target_node_id = 0;
    unsigned int i = 0;

    if (s == NULL) return;      // do not send zero length string

    length = strlen((char *)s) + TX_DATA_OFS;


    buffer[i++] = length >> 8;
    buffer[i++] = length &  0xff;

    buffer[i++] = n->node_id >> 8;
    buffer[i++] = n->node_id &  0xff;
    
    buffer[i++] = target_node_id >> 8;
    buffer[i++] = target_node_id &  0xff;
        
    buffer[i++] = ID_STRING;
    buffer[i++] = 0; // No checksum for now
   
    strcpy((char*)(buffer + TX_DATA_OFS), (char *)s);
   
    buffer[length] = 0;
      
    // COmpute checksum  
    buffer[TX_DATA_OFS-1] = get_checksum(buffer, length); 
                 
    network_send(n, buffer, length);
}
/* End of network_string */


/* Send IDENT packet */
void network_datatap_poll ( network_type *n )
{
    unsigned char buffer[100];

    unsigned int length = TX_DATA_OFS;

    unsigned int i = 0;
    
    buffer[i++] = length >> 8;
    buffer[i++] = length &  0xff;

    buffer[i++] = n->node_id >> 8;
    buffer[i++] = n->node_id &  0xff;

    buffer[i++] = 0;//node_id >> 8;
    buffer[i++] = 0;//node_id &  0xff;


    buffer[i++] = ID_DTPOLL;
    buffer[i++] = 0;  // No checksum for now
    
    buffer[TX_DATA_OFS-1] = get_checksum(buffer, length);
   
    network_send(n, buffer, length);

}
/* End of network_ident */





/* Send IDENT packet */
void network_broadcast ( network_type *n, 
                         unsigned char packet_type, 
                         unsigned char *data,
                         unsigned int data_length )
{
  unsigned char buffer[100];

    unsigned int length = TX_DATA_OFS + data_length;

    unsigned int i = 0;
    
   
    buffer[i++] = length >> 8;
    buffer[i++] = length &  0xff;

    buffer[i++] = n->node_id >> 8;
    buffer[i++] = n->node_id &  0xff;

    buffer[i++] = 0;//node_id >> 8;
    buffer[i++] = 0;//node_id &  0xff;


    buffer[i++] = packet_type;
    
   buffer[i++] = 0;  // No checksum for now
    

    int j = 0;

    // Copy payload
    for (j= 0; j < data_length; j++)
        buffer[i++] = data[j];
        
    buffer[TX_DATA_OFS-1] = get_checksum(buffer, length);

    network_send(n, buffer, length);

}






/* Send IDENT packet */
void network_datatap_data ( network_type *n, datatap_type *dt )
{
    /*unsigned char buffer[100];

    unsigned int length = TX_DATA_OFS + 1  +32;

    if (dt->tap_type == DT_INT8) length += 1;
    if (dt->tap_type == DT_INT16) length += 2;
    if (dt->tap_type == DT_INT32) length += 4;


    unsigned int i = 0;
    
    
    buffer[i++] = length >> 8;
    buffer[i++] = length &  0xff;

    buffer[i++] = n->node_id >> 8;
    buffer[i++] = n->node_id &  0xff;

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

    network_send(n, buffer, length);*/
}
/* End of network_ident */


/* Send data to all nodes */
int network_send ( network_type *n, unsigned char *data, unsigned int length )
{
    struct sockaddr_in si_remote_temp;
    
    int s, slen=sizeof(si_remote_temp);

    int yes = 1; 
    int status;


    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
    {
        n->text_out(MODULE_NAME "socket error\n");
        return MN_FAIL;
    }
   
    memset((char *) &si_remote_temp, 0, sizeof(si_remote_temp));
    si_remote_temp.sin_family = AF_INET;
    si_remote_temp.sin_port = htons(LISTEN_PORT);
        
           
    status = setsockopt(s, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(int) );
    //  n->text_out("Setsockopt Status = %d\n", status);        
        
        
    if (   inet_pton(AF_INET, BCAST_IP, (struct sockaddr *)&si_remote_temp.sin_addr) == 0 )
    {
        n->text_out(MODULE_NAME "inet_aton() failed\n");
        return MN_FAIL;
    }
          
    //sn->text_out(buf, "%s", data);
    
    if (sendto(s, data, length, 0, (struct sockaddr *)&si_remote_temp, slen)==-1)
    {
        n->text_out(MODULE_NAME "Sendto fail\n");
        return MN_FAIL;
       
    }
    
    close(s);

    return MN_SUCCESS;
}
/* End of network_send */


/* Core network thread */
void *network_thread (  void *threadid )
{
    unsigned char  buf[BUFLEN];

    socklen_t      slen;

    unsigned int time_count = 0;
       
    
    network_type *n = (network_type *) threadid;
    
    
    if (n == NULL)
    {
        pthread_exit(NULL); // exit thread
        return NULL;
    }
    
    
    n->text_out(MODULE_NAME "Thread Started\n");
            
    slen=sizeof(n->si_remote);
        
    
    while(n->running)
    {
        struct timeval tval;

        fd_set readfds;

        tval.tv_sec = 1;
        tval.tv_usec = 0;
    
        FD_ZERO(&readfds);
        FD_SET(n->server_sd, &readfds);


        select(n->server_sd+1, &readfds, NULL, NULL, &tval);

        if (FD_ISSET(n->server_sd, &readfds))
        {
            int ret = recvfrom(n->server_sd, (char *)buf, BUFLEN, 0, (struct sockaddr *)&n->si_remote, &slen);
           
            if (ret ==-1)
            {
               n->text_out(MODULE_NAME "recvfrom fail");
                continue;
            }
       
       
//       int i;
//       for (i = 0; i < ret; i++)
//       n->text_out("%x ", buf[i]);
      
       
            n->mesh_parser(buf, ret);

        }
        else
        {
            time_count++;
        
            if (time_count == 10)
            {
                time_count = 0;
                network_ident(n);            // Set out  keep alive
            }
         
            n->mesh_update();
                              
            // Timeout ?
        }   
        
        //                       
        //n->text_out("Data from: %s:%d\n", inet_ntoa(si_remote.sin_addr),
        //                            ntohs(si_remote.sin_port));
         //                          
         // n->text_out(" %s\n", buf);
          
      //    sleep(1);
    }
    
   
    n->text_out(MODULE_NAME "Shutdown\n");

    pthread_exit(NULL); // exit thread
}
/* End of network_thread */


/* Init the network */
int network_init( network_type *n )
{

    // load node id
    
    n->node_id = 0;
    FILE *f = fopen("node_id", "rt");
    if (!f)
    {
        n->text_out(MODULE_NAME "Unable to load node_id\n");
        return MN_FAIL;
    }

    fscanf(f, "%d", &n->node_id);
    
    if (n->node_id == 0)
    {
        n->text_out(MODULE_NAME "node_id 0 not allowed\n");
        return MN_FAIL;
    }

    n->text_out(MODULE_NAME "Node id: %d\n", n->node_id);


//    int i;
    
    if ((n->server_sd=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
    {
        n->text_out(MODULE_NAME "socket error\n");
        return MN_FAIL;
    }
    
    
/*    int sock = socket(...);
int flags = fcntl(sock, F_GETFL);
flags |= O_NONBLOCK;
fcntl(sock, F_SETFL, flags);*/
        
    
    
    memset((char *) &n->si_local, 0, sizeof(n->si_local));
    
    n->si_local.sin_family = AF_INET;
    n->si_local.sin_port = htons(LISTEN_PORT);
    n->si_local.sin_addr.s_addr = htonl(INADDR_ANY);

     int on = 1;
     if (setsockopt(n->server_sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) != 0)
     {
     
        n->text_out(MODULE_NAME "setsockopt fail");
        return MN_FAIL;
     }
 
    if (bind(n->server_sd, (struct sockaddr *)&n->si_local, sizeof(n->si_local))==-1)
    {
        n->text_out(MODULE_NAME "bind error %d\n", errno);
        return MN_FAIL;
    }
    
    n->running = 1;
    
    return MN_SUCCESS;
}
/* End of network_init */


/* Start network layer */
int network_start ( network_type *n,  void (*mesh_parser_link)(unsigned char *, unsigned int), 
                    void (*mesh_update)(void) ,
                    void (*out_func)(char * format, ...) )
{
//    network = (network_type *) malloc (sizeof(network_type));

     n->text_out= out_func;

    if (0 == n->text_out)
        return MN_FAIL;
  
    
     n->text_out(MODULE_NAME "Start\n");   
    
    // Init values
    n->node_id = 0;
    n->mesh_parser = NULL;
    n->running = 0;


    n->node_list = NULL;


    // Link Parser
    n->mesh_parser = mesh_parser_link;

    // Link Parser
    n->mesh_update = mesh_update;



    if (network_init(n))
    {
        n->text_out(MODULE_NAME "Network setup failed, operating in write-only mode\n");
        return MN_FAIL;
    }

    pthread_create(&n->network_t, NULL, network_thread, (void *)(n));
    
    network_ident (n); // send own init
    network_identrq (n); // request ident from other nodes
    
    
    return MN_SUCCESS;
    
}
/* End of network_start */


/* Stop network layer */
void network_stop ( network_type *n )
{
    n->text_out(MODULE_NAME "Stop\n");
    n->running = 0;
 
    sleep(2);
  
    close(n->server_sd);
    
    network_free_nodes(n);
    
    n->text_out(MODULE_NAME "Stop - end\n");    
}
/* End of network stop*/



