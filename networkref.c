/*
    Robot server - 2005 Jason Hunt
    
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

    Contact: nulluser@gmail.com



*/



#include <stdio.h>

#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>


#include <pthread.h>
#include <fcntl.h> 
#include <unistd.h>
#include <stdio.h>
#include <memory.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>

#include "version.h"
#include "server.h"
#include "camera.h"
#include "failsafe.h"
#include "controller.h"
#include "network.h"


#define MODULE_NAME "[Network] "


int server_sd;              // server socket

//volatile sig_atomic_t connected = 0;          // connected flag

int connected = 0;          // connected flag


pthread_t network_t;
  
 

/* send the controller data back to the client */
void return_controller_data( int sd )
{
    char data[5];
    data[0] = 0x30;     // send string to client
    
    memcpy(data+1, c_data, 4); // copy the controller data

    if (send(sd, data, 5, 0) == -1)
    {
        connected = 0;
        alert(MODULE_NAME "Unable to send data\n");
    }
            
}
/* end of send_controller_data */
                      
         

/* get data from the client */
int get_data( int sd, char *data )
{
    return(recv(sd, data, REC_BUFFER, 0));
}
/* end of get_data */


/* send version information to client */
void send_string( int sd, char *str )
{
    int len = strlen(str);
    if (len > 255) len = 255;

    char data[len+2];

    data[0] = 0x20;     // send string to client
    data[1] = len;

    memcpy(data+2, str, len); // copy the string to the packet buffer
   
    if (send(sd, data, len + 2, 0) == -1)
    {
        connected = 0;
        alert(MODULE_NAME "Unable to send string\n");
    }     
}
/* end of send string */


/* send version information to client */
void send_version( int sd )
{
    send_string(sd, P_VERSION);
}
/* end of send version */



/* SIGPIPE handle */
void catch_control_neterror( int sig )
{
    alert(MODULE_NAME "Unable to send data\n");

    connected = 0; 
    
    signal (sig, catch_control_neterror);
}
/* End of  catch_control_neterror */



/* init the network */
int setup_network( void )
{
    // first catch error signals
    signal (SIGPIPE, catch_control_neterror);

    server_sd = socket(AF_INET, SOCK_STREAM, 0);

    if(server_sd < 0) 
    {
        alert(MODULE_NAME "Unable to open socket\n");
        return(1);
    }
 
    struct sockaddr_in server_addr;  // server IP structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
 

    // set socket to resuse
    int reuse = 1;
    int reuse_len = sizeof(int);
    if (setsockopt(server_sd, SOL_SOCKET, SO_REUSEADDR, &reuse, reuse_len) == -1)
    {
        alert(MODULE_NAME "Unable to ser REUSE\n");
        return(1);
    }

    if(bind(server_sd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) 
    {
        alert(MODULE_NAME "Unable to bind\n");
        alert("Error: %d\n",errno);

        return(1);
    }
            
    // listen for one client
    if(listen(server_sd, 1) == -1)  // listen for one only client
    {
        alert(MODULE_NAME "Unable to listen\n");
        return(1);
    }
 
    return(0);   
}
/* end of setup network */


/* wait for a client to connect */
int wait_for_client( void )
{
    alert(MODULE_NAME "Waiting for connection\n");

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    return(accept(server_sd, (struct sockaddr *) &client_addr, &client_len));
}
/* end of wait for client */


/* shutdown computer */
void shut_down( void )
{
    system("poweroff");   
}
/* end of shutdown */

/*  core control loop */
void control_network_loop( int client_sd )
{
    connected = 1;  // we are now connected
            
    // send our version data to the client
    send_version(client_sd);

    // data buffer for control commands
    char data[REC_BUFFER];
        
    while(!program_done && connected) // n == 0, client disconnect
    {
        int n = get_data(client_sd, data);
        if (n < 1)
        {
            alert(MODULE_NAME "Unable to get data\n");
            connected = 0;       
        } else
        {   // we have data
            //parse client commands
            // test first command byte
            if (data[0] == CONTROL_KEEP_ALIVE) // keep alive from client
            {
              update_controller(); 
                update_count++;
            } 
            if (data[0] == CONTROL_SHUTDOWN) // keep alive from client
            {
                send_string(client_sd, "Server is shutting down now");
                
                reset_controller();
                shut_down();
            } 
            if (data[0] == CONTROL_FIREON) // keep alive from client
            {
//                fire = 1;
                alert("Fire on\n");
            } 
            if (data[0] == CONTROL_FIREOFF) // keep alive from client
            {
               // fire = 0;
                alert("Fire off\n");
            } 
            if (data[0] == CONTROL_VERSION) // send version
            {
                send_version(client_sd);
            } 
            if (data[0] == CONTROL_STRING) // display string
            {
                unsigned char len = data[1];
                //alert("Len: %d\n", len);
                memcpy(data, data+2, len); // copy over command byte
                data[len] = 0;
                alert(MODULE_NAME "Message from client: %s\n", data); // display the string
            } 
            if (data[0] == CONTROL_UPDATE) // control update
            {
                // make sure we have all of the data first
                if (n < 5)
                {
                    alert(MODULE_NAME "Invalid control packet: %d\n", n);
                    connected = 0;
                    return;
                }

                memcpy(c_data, data+1, 4); // copy over motor data
                 // display the new data

                alert(MODULE_NAME "Data: %X %X %X %X\n", c_data[0], c_data[1], c_data[2], c_data[3]);                    

                   

                update_controller();    // send the net vaules to the motor controller

                return_controller_data(client_sd); //send the values back to the client
                    
                update_count++; // we have new data, tell the failsafe
            }
        } 
    }

    reset_controller();

    // we are disconneced
    close(client_sd);
    connected = 0;    
}


/* core network thread */
void *control_network( void *threadid )
{
    alert(MODULE_NAME "Started\n");


    while(!program_done) 
    {
        // reset controller on new connection, might happen twice
        reset_controller();

        // wait for the client to connect
        int client_sd =  wait_for_client();

        if (client_sd < 0) 
        {
            alert(MODULE_NAME "Unable to accept\n");
            break;
        }

        alert(MODULE_NAME "Client connected\n");

        control_network_loop(client_sd);
    }

    // program has exited
    close(server_sd);
   
    alert(MODULE_NAME "Shutdown\n");

    pthread_exit(NULL); // exit thread
}
/* end of handle_network */







void network_start( void )
{
    if (setup_network())
    {
        alert(MODULE_NAME "Network setup failed\n");
        return;
    }

    pthread_create(&network_t, NULL, control_network, NULL);
}





void network_stop( void )
{
    close(server_sd);


}




