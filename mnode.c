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

    File: MNode.c
*/


#include <stdio.h>
#include <string.h>

#include <unistd.h>






#include "mnode.h"
#include "network.h"

#define MODULE_NAME "[MNode] "


int main_running;



#define STATE_WAIT      0
#define STATE_LENGTH    1
#define STATE_DATA      2




/* Return a word from inside a buffer */
unsigned int get_word(unsigned char * b)
{
    return (*b << 8) + *(b+1);
}



/* All network traffic is router through here */
void mesh_packet(unsigned char *data, unsigned int length)
{
    unsigned int i;
    unsigned int tx_length   = get_word(data);//data[0]*256+data[1];
    unsigned int tx_src_node_id     = get_word(data+2);//data[2]*256+data[3];
    unsigned int tx_tar_node_id     = get_word(data+4);//data[2]*256+data[3];
    
    unsigned int tx_type     = data[6];
    unsigned int tx_checksum = data[7];


/*    printf("\nMesh packet\n");
    printf(" Length: %d\n", tx_length);
    printf(" Node: %d\n", tx_node);
    printf(" Type: %d\n", tx_type);
    printf(" Checksum: %d\n", tx_checksum);*/
    
    
    if (tx_type == ID_IDENT)
    {
        if (network_add_node(tx_src_node_id) == 0)       // Add to node list
            printf("[Node %d->%d] IDENT\n", tx_src_node_id, tx_tar_node_id ); // display if new
            
    } else
    if (tx_type == ID_STRING)
    {
   
    
        printf("[Node %d->%d] STRING: ", tx_src_node_id, tx_tar_node_id );
        
        for (i = 0; i < tx_length-6; i++)
            printf("%c", data[i+TX_DATA_OFS]);
        
        printf("\n");
        
    
    } else
    {
    
        printf("Unknown packet type from %d->%d\n", tx_src_node_id, tx_tar_node_id );
    
    }
    
}
/* End of mesh_data */




/* Menu command - exit */
void command_exit( void )
{
    main_running = 0;
}
/* End of command_exit */

/* Menu command - send */
void command_ident( void )
{
    network_ident();
}
/* End of command_send */



/* Menu command - help */
void command_string( char *s )
{
    network_string((unsigned char *)s);

}
/* End of command_help */


/* Menu command - help */
void command_nodes( void )
{
    network_list_nodes();

}
/* End of command_help */






/* Menu command - help */
void command_help( void )
{
    printf("Commands\n");
    printf("exit    quit program\n");
    printf("ident   send ident\n");
    printf("send    send string\n");
    printf("nodes   list nodes\n");
    printf("help    this message\n");
}
/* End of command_help */






/* Main */
int main ( void )
{
    char  *s;
    size_t len = 0;
    int    b;

    printf(MODULE_NAME "Startup\n");

    main_running = 1;

    network_start(mesh_packet);

       
    while(main_running)
    {
        printf(">");

        b = getline(&s, &len, stdin);
                
        if (0 < b) s[b-1] = 0; // Erase netline
        
        
        if (!strcmp(s, "exit")) command_exit();
        if (!strcmp(s, "ident")) command_ident();
        if (!strcmp(s, "help")) command_help();
        if (!strcmp(s, "nodes")) command_nodes();
        if (!strcmp(s, "send")) 
        {
        printf("Message: ");
         b = getline(&s, &len, stdin);
        command_string(s);
        }

    }
    
    network_stop();
    
    return 0;
 }
 /* End of main */
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
