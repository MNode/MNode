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
#include <ncurses.h>


#include "mnode.h"
#include "datatap.h"
#include "network.h"

#define MODULE_NAME "[Monitor] "


int main_running;
   int row,col;				/* to store the number of rows and */


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



void command_data( void )
{
    printw("Not implemented\n");
}


void command_nodedata( void )
{
    printw("Sending Request\n");
    network_datatap_poll();
}




/* Menu command - help */
void command_help( void )
{
    printw("Commands\n");
    printw("exit        Quit program\n");
    printw("ident       Send ident\n");
    printw("send        Send string\n");
    printw("nodes       List nodes\n");
    printw("data        List data for this node\n");
    printw("nodedata    List all nodedata\n");
    
    printw("help        Display this message\n");
}
/* End of command_help */




void draw_screen( void )
{    
        mvprintw(0,col/2-10,"[MNode Monitor]");
        mvprintw(1,0,"");


    	refresh();	
}



/* Main */
int main ( void )
{
    char  s[1000];
    size_t len = 0;
    int    b;
    int count = 0;

 
	initscr();			/* Start curses mode 		  */

    getmaxyx(stdscr,row,col);
 
    draw_screen();
  
 
    printw(MODULE_NAME "Startup\n");

    main_running = 1;


    mnode_start();


    mnode_tap_add("count", DT_INT32, &count);

       
    while(main_running)
    {
        draw_screen();

        mvprintw(row-1,0,"> ");

        getstr(s);

        mvprintw(1,0," ");
 

        //b = getline(&s, &len, stdin);
                
//        if (0 < b) s[b-1] = 0; // Erase newline
        
        
        if (!strcmp(s, "exit")) command_exit();
        if (!strcmp(s, "ident")) command_ident();
        if (!strcmp(s, "help")) command_help();
        if (!strcmp(s, "nodes")) command_nodes();
        if (!strcmp(s, "data")) command_data();
        if (!strcmp(s, "nodedata")) command_nodedata();        
        if (!strcmp(s, "send")) 
        {
            printw("Message: ");
            // b =getline(&s, &len, stdin);
            getstr(s);
            command_string(s);
        }

        count++;
    }
    
    mnode_stop();

 	endwin();			/* End curses mode		  */
    
    return 0;
 }
 /* End of main */
 
 

 
 
 
 
 
 
 
