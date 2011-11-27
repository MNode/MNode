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

    File: MNode.c
*/


#include <stdio.h>
#include "mnode.h"
#include "network.h"

#define MODULE_NAME "[MNode] "


int main_running;



#define STATE_WAIT      0
#define STATE_LENGTH    1
#define STATE_DATA      2


int rx_state = STATE_WAIT;
unsigned int rx_len = 0;











/* All network traffic is router through here */
void mesh_packet(unsigned char *data, unsigned int length)
{
    unsigned int tx_length   = data[0]*256+data[1];
    unsigned int tx_node     = data[2]*256+data[3];
    unsigned int tx_type     = data[4];
    unsigned int tx_checksum = data[length-1];

    printf("Mesh packet\n");
    printf("Length: %d\n", tx_length);
    printf("Node: %d\n", tx_node);
    printf("Type: %d\n", tx_type);
    printf("Checksum: %d\n", tx_checksum);
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
    char *s;
    int len = 0;
    int b;

    printf(MODULE_NAME "Startup\n");

    main_running = 1;

    network_start(mesh_packet);

       
    while(main_running)
    {
        printf(">");

        b = getline(&s, &len, stdin);
                
        s[b-1] = 0; // Erase netline
        
        if (!strcmp(s, "exit")) command_exit();
        if (!strcmp(s, "ident")) command_ident();
        if (!strcmp(s, "help")) command_help();

    }
    
    network_stop();
    
    return 0;
 }
 /* End of main */
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
