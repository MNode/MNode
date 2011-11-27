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


#define BUFLEN 4096 
#define LISTEN_PORT 9998
#define BCAST_IP "255.255.255.255"


#define TX_DATA_OFS 8       // Packet header size

#define ID_IDENT            1               // Node sendind own node id
#define ID_IDENTRQ          2               // Node requesting node ids
#define ID_STRING           10              // Sending string 


/* Node entry */
typedef struct node_entry
{
    unsigned int node_id;
    struct node_entry *next;
} node_entry;


typedef struct
{
    int server_sd;              // server socket

    pthread_t network_t;

    int node_id;

    struct sockaddr_in si_local, si_remote;
    void (*mesh_parser)(unsigned char *, unsigned int);

    int running;
    
    node_entry *node_list; // Node list

} network_type;


int network_init( void );
void network_start( void (*mesh_parser_link)(unsigned char *, unsigned int) );
void network_stop( void );
int network_send(unsigned char *data, unsigned int length);

void network_ident (void );
void network_string (unsigned char *s );

int network_add_node(unsigned int tx_node_id);
void network_list_nodes( void );
void network_free_nodes( void );


#endif








