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
                
    File: monitor.c
*/

#define MODULE_NAME "[Monitor] "

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>

#include "types.h"
#include "mnode.h"
#include "network.h"
#include "datatap.h"

#include "monitor.h"


monitor_type *monitor;


/* Create log window */
int log_win_setup ( void )
{
    int x_pos = 0;
    int y_pos = 1;
    
    int x_size = monitor->screen_x;
    int y_size = monitor->screen_y - 3;
        
    monitor->log_win = newwin(y_size,x_size, y_pos, x_pos);
    box(monitor->log_win, 0 , 0);
    wattron(monitor->log_win, COLOR_PAIR(2));
    wbkgd(monitor->log_win,COLOR_PAIR(2));
    wrefresh(monitor->log_win);

    monitor->log_win_area = newwin(y_size-2, x_size-2, y_pos+1, x_pos+1);
    wattron(monitor->log_win_area, COLOR_PAIR(2));
    wbkgd(monitor->log_win_area,COLOR_PAIR(2));
    idlok(monitor->log_win_area, 1);
    scrollok(monitor->log_win_area, 1);
    
    wrefresh(monitor->log_win_area);

   
    return MN_SUCCESS;
}
/* End of log_win_setup */


/* All output is routed through this function */
static void log_out ( char *fmt, ... )
{
    const unsigned int max = 1024;
    char text[max];

    va_list args;

    va_start(args, fmt);

    vsnprintf(text, max, fmt, args);

    wprintw(monitor->log_win_area, "%s", text);
   
    wrefresh(monitor->log_win_area);

    // Move curser back home
    //mvprintw(screen_y-1,0,"> ");

    refresh();

    if (monitor->log_file != NULL)
        fprintf(monitor->log_file, "%s\n", text);

    va_end(args);
}
/* End of output function */


/* Menu command - exit */
void command_exit ( void )
{
    monitor->main_running = 0;
}
/* End of command_exit */


/* Menu command - send */
void command_ident ( void )
{

    mnode_ident(monitor->mnode);

//    network_ident();
}
/* End of command_send */


/* Menu command - help */
void command_string( char *s )
{

    mnode_string(monitor->mnode,(unsigned char *)s);

//    network_string((unsigned char *)s);

}
/* End of command_help */


/* Menu command - help */
void command_nodes ( void )
{
//    network_list_nodes();
    mnode_list_nodes(monitor->mnode);


}
/* End of command_help */


/* Local data */
void command_data ( void )
{
    log_out("Not implemented\n");
}
/* End of command_data */


/* End of nodedata */
void command_nodedata ( void )
{
    log_out("Sending Request\n");
    
    mnode_datatap_poll(monitor->mnode);
    
    
}
/* End of command_nodedata */

/* Menu command - help */

void command_help ( void )
{
    log_out("Commands\n");
    log_out("  exit          Quit program\n");
    log_out("  ident         Send ident\n");
    log_out("  send text     Send text string to all stations\n");
    log_out("  nodes         List nodes\n");
    log_out("  data          List data for this node\n");
    log_out("  nodedata      Query all nodedata\n");
    
    log_out("  help          Display this message\n");
}
/* End of command_help */


/* Monitor startup */
void monitor_start ( void )
{
    monitor = (monitor_type*) malloc(sizeof( monitor_type ));
    
    monitor->main_running = 0;
    monitor->screen_x = 0;;
    monitor->screen_y = 0;
    monitor->count = 0;          // Global var for datatap linking
    
    monitor->log_file = NULL;
    monitor->log_win = NULL;
    monitor->log_win_area = NULL;
    
    monitor->mnode = NULL;



    monitor->log_file = fopen("log", "wt");

    initscr(); /* Start curses mode */

    getmaxyx(stdscr,monitor->screen_y,monitor->screen_x);
 
    start_color();		

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_BLUE);

    attron(COLOR_PAIR(1));

    wbkgd(stdscr,COLOR_PAIR(1));
    refresh();
    
      
    log_win_setup();

    log_out(MODULE_NAME "Start\n");

    monitor->main_running = 1;


    monitor->mnode = (mnode_type *) malloc ( sizeof (mnode_type));


    mnode_start(monitor->mnode,log_out);


    mnode_tap_add(monitor->mnode,"count", DT_INT32, &monitor->count);
}
/* End of monitor_start */


/* Monitor stop */
void monitor_stop ( void )
{
    log_out(MODULE_NAME "Stop\n");

    mnode_stop(monitor->mnode);

    fclose(monitor->log_file);

    endwin();
}
/* End of monitor_stop */

/* Main */

int main ( void )
{
    char s[1000];
 
    monitor_start();
   
    while(monitor->main_running)
    {
        mvprintw(0,monitor->screen_x/2-8,"");
        
        printw("[MNode Monitor]");
        
        mvprintw(monitor->screen_y-1,0,">                                             ");
        mvprintw(monitor->screen_y-1,0,"> ");

        refresh();

        getstr(s);

        if (s[0] != 0)
        {
            char delims1[] = " ";
           
            char *str = NULL;
    
            str = strtok( s, delims1 );
            
            if (!strcmp(str, "exit")) command_exit(); else
            if (!strcmp(str, "ident")) command_ident();else
            if (!strcmp(str, "help")) command_help();else
            if (!strcmp(str, "nodes")) command_nodes();else
            if (!strcmp(str, "data")) command_data();else
            if (!strcmp(str, "nodedata")) command_nodedata();else
            if (!strcmp(str, "send"))
            {
                char delims2[] = "\n";
                str = strtok( NULL, delims2 );
                command_string(str);
            } else
            {
                log_out("%s: Command not found\n", str);
            }
    
        }

        monitor->count++;
    }
    
    monitor_stop();
    
    return MN_SUCCESS;
 }
 /* End of main */
 
 
 
 
