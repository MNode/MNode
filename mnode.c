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
#include <sys/sysinfo.h>


#include "mnode.h"
#include "datatap.h"
#include "network.h"

#define MODULE_NAME "[MNode] "


// system datataps  

unsigned int local_freeram;
unsigned int local_procs;


void (*text_out)(char * format, ...) = NULL;


/* Return a word from inside a buffer */
unsigned int get_word(unsigned char * b)
{
    return (*b << 8) + *(b+1);
}
/* End of get_word */


/* All network traffic is router through here */
void mnode_packet(unsigned char *data, unsigned int length)
{
    unsigned int i;
    unsigned int tx_length   = get_word(data);//data[0]*256+data[1];
    unsigned int tx_src_node_id     = get_word(data+2);//data[2]*256+data[3];
    unsigned int tx_tar_node_id     = get_word(data+4);//data[2]*256+data[3];
    
    unsigned int tx_type     = data[6];
 //   unsigned int tx_checksum = data[7];


/*    printw("\nMesh packet\n");
    printw(" Length: %d\n", tx_length);
    printw(" Node: %d\n", tx_node);
    printw(" Type: %d\n", tx_type);
    printw(" Checksum: %d\n", tx_checksum);*/
    
    
    if (tx_type == ID_IDENT)
    {
        if (network_add_node(tx_src_node_id) == 0)       // Add to node list
            text_out(MODULE_NAME "Node %d->%d IDENT\n", tx_src_node_id, tx_tar_node_id ); // display if new
            
    } else
    
    if (tx_type == ID_IDENTRQ)
    {
       // printw("[Node %d->%d] IDENTQR\n", tx_src_node_id, tx_tar_node_id );
        network_ident();
    } else


    if (tx_type == ID_DTPOLL)
    {
        //printw("[Node %d->%d] DTPOLL\n", tx_src_node_id, tx_tar_node_id );
        datatap_poll();
    } else

    if (tx_type == ID_DTDATA)
    {
       // printw("[Node %d->%d] DTDATA\n", tx_src_node_id, tx_tar_node_id );
     
       datatap_data(tx_src_node_id, data + TX_DATA_OFS, length-TX_DATA_OFS);
        
    } else

    if (tx_type == ID_STRING)
    {
       unsigned int string_len = tx_length - TX_DATA_OFS;
    
        text_out(MODULE_NAME "Node %d->%d STRING: ", tx_src_node_id, tx_tar_node_id );
        
        for (i = 0; i < string_len; i++)
            text_out("%c", data[i+TX_DATA_OFS]);
        
       text_out("\n");
        
    
    } else
    {
    
        text_out(MODULE_NAME "Unknown packet type from %d->%d\n", tx_src_node_id, tx_tar_node_id );
    
    }
    
}
/* End of mesh_data */








/* Called by networking code every second */
void mnode_update( void )
{
    //printw("Update\n");

    struct sysinfo si;
    sysinfo(&si);
 

    local_freeram = si.freeram;
    local_procs = si.procs;

}
/* End of mnode_update */



/* Start node */
int mnode_start(void (*out_func)(char * format, ...))
//int mnode_start(void)
{
    text_out = out_func;
          
    network_start(mnode_packet, mnode_update, text_out);

     // Add system taps
  
    data_tap_add("freeram", DT_INT32, &local_freeram);
    data_tap_add("procs", DT_INT32, &local_procs);
 
  
    return MN_SUCCESS;
}
/* End of mnode_start */



/* Stop node */
int mnode_stop(void)
{
    return MN_SUCCESS;
}
/* End of mnode_stop */

 

/* Start node */
int mnode_tap_add(char *name, unsigned int tap_type, void * tap_link)
{
     // Add system taps
  
    data_tap_add(name, tap_type, tap_link);
 
    return MN_SUCCESS;
}
/* End of mnode_start */













 
