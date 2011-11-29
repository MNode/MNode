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

    File: network.h
*/

#ifndef NETWORK_H
#define NETWORK_H


#include <pthread.h>
#include <arpa/inet.h>


#include "types.h"
#include "mnode.h"
#include "datatap.h"


#define BUFLEN 4096 
#define LISTEN_PORT 9998
#define BCAST_IP "255.255.255.255"


#define TX_DATA_OFS 8       // Packet header size

#define ID_IDENT            1               // Node sendind own node id
#define ID_IDENTRQ          2               // Node requesting node ids
#define ID_STRING           10              // Sending string 


#define ID_DTPOLL           20              // send datatap poll request
#define ID_DTDATA           21              // send datatap poll request



/* Node entry */
struct node_entry
{
    unsigned int node_id;
    struct node_entry *next;
};



//typedef struct mnode_type;


struct network_type
{
    int server_sd;              // server socket

    pthread_t network_t;

    int node_id;

    struct sockaddr_in si_local, si_remote;
    void (*mesh_packet)(mnode_type *, unsigned char *, unsigned int);
    void (*mesh_update)(mnode_type *);
    
    
    void (*text_out)(char * format, ...);

    int running;
    
    node_entry *node_list; // Node list
    
    
    mnode_type *mnode;
    

};


int network_init( network_type *n );

int network_start( network_type *n, mnode_type *m);
                   
void network_stop( network_type *n);
int network_send( network_type *n, unsigned char *data, unsigned int length);

void network_ident ( network_type *n);
void network_string ( network_type *n, unsigned char *s );

int network_add_node( network_type *n, unsigned int tx_node_id);
void network_list_nodes( network_type *n );
void network_free_nodes( network_type *n );

void network_datatap_poll ( network_type *n);



void network_packet ( network_type *n, 
                         unsigned char packet_type, 
                         unsigned char *data,
                         unsigned int data_length);


#endif


