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


#include <stdio.h>
#include <string.h>
#include <unistd.h>


#include "datatap.h"
#include "network.h"

datatap_type *data_taps = NULL;





void data_tap_add(char *name, unsigned int tap_type, void * tap_link)
{
    datatap_type *tmp = (datatap_type *)malloc(sizeof(datatap_type));

    strcpy(tmp->tap_name, name);

    tmp->tap_type = tap_type;

    tmp->tap_link = tap_link;

    tmp->next = data_taps;
    
    data_taps = tmp;
    
//    network_datatap_data (&t1);
   

}













/* Respond to poll request */
void datatap_poll( void )
{
    datatap_type *tmp = data_taps;

    while(tmp)
    {
        network_datatap_data (tmp);
        tmp = tmp->next;
    }


/*    struct sysinfo si;
    sysinfo(&si);
  
    strcpy(t1.tap_name,"freeram");
    t1.tap_type = DT_INT32;
    t1.tap_value = si.freeram;
    network_datatap_data (&t1);
    
    strcpy(t1.tap_name,"processes");
    t1.tap_type = DT_INT32;
    t1.tap_value = si.procs;
    network_datatap_data (&t1);   */
    
}



/* Return a word from inside a buffer */
unsigned int get_int8(unsigned char * b)
{
    return *(b);
}

unsigned int get_int16(unsigned char * b)
{
    return (*b << 8) + *(b+1);
}

unsigned int get_int32(unsigned char * b)
{
    return (*(b+0) << 24) + (*(b+1)<<16)  +(*(b+2) << 8) + *(b+3);
}





/* Got data from other node */
void datatap_data( unsigned int node_id, 
                   unsigned char *data,  
                   unsigned int length )
{
    unsigned int tap_type = data[0];
 //   int i;
      
    
   
//    printf("\nNode | Tap                            | Type   | Len  |            Value\n");
   
   printf("[Node %4d]", node_id);
   
//   for (i = 0; i < 32; i++)
   printf("[%30s]", data+1);
      
        
    if (tap_type == DT_INT8)
        printf("[INT8 ][%16d]\n",   get_int8(data+1+32));
        
    if (tap_type == DT_INT16)
        printf("[INT16 ][%16d]\n",   get_int16(data+1+32));
                
    if (tap_type == DT_INT32)
        printf("[INT32 ][%16d]\n",   get_int32(data+1+32));
                     
        
}























