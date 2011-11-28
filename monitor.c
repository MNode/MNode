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


FILE *log_file = NULL;



/* All output is routed through this function */
static void text_out( char *fmt, ... )
{

    const unsigned int max = 1024;
    char text[max];

    va_list args;

    va_start(args, fmt);

    vsnprintf(text, max, fmt, args);

    printw("%s", text);

    if (log_file != NULL)
        fprintf(log_file, "%s\n", text);

    va_end(args);    
 

}
/* End of output function */



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
    text_out("Not implemented\n");
}


void command_nodedata( void )
{
    text_out("Sending Request\n");
    network_datatap_poll();
}




/* Menu command - help */
void command_help( void )
{
    text_out("Commands\n");
    text_out("exit        Quit program\n");
    text_out("ident       Send ident\n");
    text_out("send        Send string\n");
    text_out("nodes       List nodes\n");
    text_out("data        List data for this node\n");
    text_out("nodedata    List all nodedata\n");
    
    text_out("help        Display this message\n");
}
/* End of command_help */




void draw_screen( void )
{    
        mvprintw(0,col/2-10,"");
        
        text_out("[MNode Monitor]");
        
        mvprintw(1,0,"");


    	refresh();	
}



/* Main */
int main ( void )
{
    char  s[1000];
//    size_t len = 0;
//    int    b;
    int count = 0;


    log_file = fopen("log", "wt");
    
     
	initscr();			/* Start curses mode 		  */

    getmaxyx(stdscr,row,col);
 
    draw_screen();
  
 
    text_out(MODULE_NAME "Startup\n");

    main_running = 1;


    mnode_start(text_out);

    data_tap_start( text_out);

    mnode_tap_add("count", DT_INT32, &count);
      
    while(main_running)
    {
        draw_screen();

        mvprintw(row-1,0,"> ");

        getstr(s);


//        clear();


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
            text_out("Message: ");
            // b =getline(&s, &len, stdin);
            getstr(s);
            command_string(s);
        }

        count++;
    }
    
    mnode_stop();

    fclose(log_file);


 	endwin();			/* End curses mode		  */
    
    return 0;
 }
 /* End of main */
 
 

 
 
 
 
 
 
 
