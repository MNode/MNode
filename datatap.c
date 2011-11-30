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
                
    File: datatap.c
*/

#define MODULE_NAME "[Datatap] "

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "datatap.h"


void data_tap_start(   )
{
  //  text_out = out_func;
}



void data_tap_add ( mnode_type *m, char *name, unsigned int tap_type, void * tap_link )
{
    datatap_type *tmp = (datatap_type *)malloc(sizeof(datatap_type));

    strcpy(tmp->tap_name, name);

    tmp->tap_type = tap_type;

    tmp->tap_link = tap_link;

    tmp->next = m->data_taps;
    
    m->data_taps = tmp;
}


void data_tap_send ( network_type *n, datatap_type *dt )
{
    unsigned char buffer[100];
  
    unsigned int i = 0;
  
    buffer[i++] = dt->tap_type;  // No checksum for now
      
    // Copy over tap name
    memset(buffer+i, 0, 32);
   strcpy((char *)(buffer+i), dt->tap_name);

    i+=32;
    
    
    unsigned int value =  *((unsigned int *)dt->tap_link);
       
       
    if (dt->tap_type == DT_INT8)
    {
        buffer[i++] = value & 0xff;
    }
    
    if (dt->tap_type == DT_INT16)
    {
        buffer[i++] = value >> 8;
        buffer[i++] = value & 0xff;
    }
    
    if (dt->tap_type == DT_INT32)
    {
        buffer[i++] = value >> 24;    
        buffer[i++] = value >> 16;    
        buffer[i++] = value >> 8;
        buffer[i++] = value & 0xff;
    }
    
    network_packet( n, ID_DTDATA, buffer, i);

}


/* Respond to poll request */
void datatap_poll ( mnode_type *m, network_type *n )
{
    datatap_type *tmp = m->data_taps;

    // Iterate through taps and send to network
    while(tmp)
    {
        data_tap_send (n, tmp);

//        network_datatap_data (n,tmp);
        tmp = tmp->next;
    }   
}
/* End of datatap_poll */




/* Return a word from inside a buffer */
unsigned int get_int8 ( unsigned char * b )
{
    return *(b);
}

unsigned int get_int16 ( unsigned char * b )
{
    return (*b << 8) + *(b+1);
}

unsigned int get_int32 ( unsigned char * b )
{
    return (*(b+0) << 24) + (*(b+1)<<16)  +(*(b+2) << 8) + *(b+3);
}





/* Got data from other node */
void datatap_data(  mnode_type *m,
                    unsigned int node_id, 
                    unsigned char *data,  
                    unsigned int length )

{
    unsigned int tap_type = data[0];
 //   int i;
      
    
   
//    printf("\nNode | Tap                            | Type   | Len  |            Value\n");
   
   m->text_out("[Node %4d]", node_id);
   
//   for (i = 0; i < 32; i++)
   m->text_out("[%30s]", data+1);
      
        
    if (tap_type == DT_INT8)
        m->text_out("[INT8 ][%16d]\n",   get_int8(data+1+32));
        
    if (tap_type == DT_INT16)
        m->text_out("[INT16 ][%16d]\n",   get_int16(data+1+32));
                
    if (tap_type == DT_INT32)
        m->text_out("[INT32 ][%16d]\n",   get_int32(data+1+32));
                     
        
}























