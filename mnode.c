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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>




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
    unsigned int tx_node     = get_word(data+2);//data[2]*256+data[3];
    unsigned int tx_type     = data[4];
    unsigned int tx_checksum = data[5];

    printf("\nMesh packet\n");
    printf(" Length: %d\n", tx_length);
    printf(" Node: %d\n", tx_node);
    printf(" Type: %d\n", tx_type);
    printf(" Checksum: %d\n", tx_checksum);
    
    
    if (tx_type == ID_IDENT)
    {
        printf("[Node %d] IDENT\n", tx_node);
    } else
    if (tx_type == ID_STRING)
    {
   
    
        printf("[Node %d] STRING: ", tx_node);
        
        for (i = 0; i < tx_length-6; i++)
            printf("%c", data[i+TX_DATA_OFS]);
        
        printf("\n");
        
    
    } else
    {
    
        printf("Unknown packet type from %d\n", tx_node);
    
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
void command_help( void )
{
    printf("Commands\n");
    printf("exit    quit program\n");
    printf("ident   send ident\n");
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
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
