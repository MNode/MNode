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

#define MODULE_NAME "[MNode] "

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/sysinfo.h>

#include "mnode.h"

/* Return a word from inside a buffer */
unsigned int get_word(unsigned char * b)
{
    return (*b << 8) + *(b+1);
}
/* End of get_word */


/* All network traffic is router through here */
void mnode_packet(mnode_type *m, unsigned char *data, unsigned int length)
{
    unsigned int i;
    unsigned int tx_length          = get_word(data);
    unsigned int tx_src_node_id     = get_word(data+2);
    unsigned int tx_tar_node_id     = get_word(data+4);
    
    unsigned int tx_type     = data[6];
 //   unsigned int tx_checksum = data[7];


/*    printw("\nMesh packet\n");
    printw(" Length: %d\n", tx_length);
    printw(" Node: %d\n", tx_node);
    printw(" Type: %d\n", tx_type);
    printw(" Checksum: %d\n", tx_checksum);*/
    
    if (tx_type == ID_IDENT)
    {
        m->text_out(MODULE_NAME "ID_IDENT\n");
    
        if (network_add_node(m->network, tx_src_node_id) == 0)       // Add to node list
            m->text_out(MODULE_NAME "Node %d->%d IDENT\n", tx_src_node_id, tx_tar_node_id ); // display if new
        

    } else
    
    if (tx_type == ID_IDENTRQ)
    {
        m->text_out(MODULE_NAME "ID_IDENTRQ\n");
        
        network_ident(m->network);
    } else


    if (tx_type == ID_DTPOLL)
    {
        m->text_out(MODULE_NAME "ID_DTPOLL\n");
    
    
        //printw("[Node %d->%d] DTPOLL\n", tx_src_node_id, tx_tar_node_id );
        datatap_poll(m->network);
    } else

    if (tx_type == ID_DTDATA)
    {
        m->text_out(MODULE_NAME "ID_DTDATA\n");
    
       // printw("[Node %d->%d] DTDATA\n", tx_src_node_id, tx_tar_node_id );
     
       datatap_data(tx_src_node_id, data + TX_DATA_OFS, length-TX_DATA_OFS);
        
    } else

    if (tx_type == ID_STRING)
    {
        m->text_out(MODULE_NAME "ID_STRING\n");
        
        unsigned int string_len = tx_length - TX_DATA_OFS;
    
        m->text_out(MODULE_NAME "Node %d->%d STRING: ", tx_src_node_id, tx_tar_node_id );
        
        for (i = 0; i < string_len; i++)
            m->text_out("%c", data[i+TX_DATA_OFS]);
        
       m->text_out("\n");
        
    
    } else
    {
    
        m->text_out(MODULE_NAME "Unknown packet type from %d->%d\n", tx_src_node_id, tx_tar_node_id );
    
    }
    
}
/* End of mesh_data */


/* Called by networking code every second */
void mnode_update(mnode_type *m)
{
    //printw("Update\n");

    struct sysinfo si;
    sysinfo(&si);
 

    m->local_freeram = si.freeram;
    m->local_procs = si.procs;

}
/* End of mnode_update */





/*
    Public
*/


/* Start node */
int mnode_start(mnode_type *m, void (*out_func)(char * format, ...))
//int mnode_start(void)
{
//    m = (mnode_type *)malloc(sizeof(mnode_type));

    m->text_out = NULL;

    m->text_out = out_func;
          
          
    m->network = (network_type *) malloc (sizeof(network_type));
          

    m->mesh_packet = mnode_packet;

    m->mesh_update = mnode_update;

          
    network_start(m->network, m);

     // Add system taps
    data_tap_add("freeram", DT_INT32, &m->local_freeram);
    data_tap_add("procs", DT_INT32, &m->local_procs);
 
    return MN_SUCCESS;
}
/* End of mnode_start */


/* Stop node */
int mnode_stop(mnode_type *m)
{
    m->text_out(MODULE_NAME "Stop\n");

    network_stop( m->network );

    free(m->network);


    m->text_out(MODULE_NAME "Stop - end\n");

//    free (mnode);

    return MN_SUCCESS;
}
/* End of mnode_stop */


/* Add datatap */
int mnode_tap_add(mnode_type *m, char *name, unsigned int tap_type, void * tap_link)
{
     // Add system taps
  
    data_tap_add(name, tap_type, tap_link);
 
    return MN_SUCCESS;
}
/* End of mnode_start */











void mnode_ident ( mnode_type *m  )
{
    network_ident(m->network);
}



void mnode_string ( mnode_type *m,  unsigned char *s )
{
    network_string(m->network, s);
}


void mnode_list_nodes( mnode_type *m )
{
    network_list_nodes(m->network);
}


void mnode_datatap_poll( mnode_type *m  )
{
    network_datatap_poll(m->network);
}


















