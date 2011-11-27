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

#define MODULE_NAME "[MNODE] "


int main_running;



/* All network traffic is router through here */
void mesh_data(char *data)
{
    printf("Mesh: %s\n", data);








}
/* End of mesh_data */

/* Menu command - exit */
void command_exit( void )
{
    main_running = 0;
}
/* End of command_exit */

/* Menu command - send */
void command_send( void )
{
    network_send("Test Data");
}
/* End of command_send */

/* Menu command - help */
void command_help( void )
{
    printf("Commands\n");
    printf("exit    quit program\n");
    printf("send    send to nodes\n");
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

    network_start(mesh_data);

       
    while(main_running)
    {
        printf(">");

        b = getline(&s, &len, stdin);
                
        s[b-1] = 0; // Erase netline
        
        if (!strcmp(s, "exit")) command_exit();
        if (!strcmp(s, "send")) command_send();
        if (!strcmp(s, "help")) command_help();

    }
    
    network_stop();
    
    return 0;
 }
 /* End of main */
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
