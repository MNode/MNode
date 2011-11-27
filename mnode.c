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


#include "mnode.h"
#include "datatap.h"
#include "network.h"

#define MODULE_NAME "[MNode] "



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


/*    printf("\nMesh packet\n");
    printf(" Length: %d\n", tx_length);
    printf(" Node: %d\n", tx_node);
    printf(" Type: %d\n", tx_type);
    printf(" Checksum: %d\n", tx_checksum);*/
    
    
    if (tx_type == ID_IDENT)
    {
        if (network_add_node(tx_src_node_id) == 0)       // Add to node list
            printf("[Node %d->%d] IDENT\n", tx_src_node_id, tx_tar_node_id ); // display if new
            
    } else
    
    if (tx_type == ID_IDENTRQ)
    {
       // printf("[Node %d->%d] IDENTQR\n", tx_src_node_id, tx_tar_node_id );
        network_ident();
    } else


    if (tx_type == ID_DTPOLL)
    {
        //printf("[Node %d->%d] DTPOLL\n", tx_src_node_id, tx_tar_node_id );
        datatap_poll();
    } else

    if (tx_type == ID_DTDATA)
    {
       // printf("[Node %d->%d] DTDATA\n", tx_src_node_id, tx_tar_node_id );
     
       datatap_data(tx_src_node_id, data + TX_DATA_OFS, length-TX_DATA_OFS);
        
    } else

    if (tx_type == ID_STRING)
    {
   
    
        printf("[Node %d->%d] STRING: ", tx_src_node_id, tx_tar_node_id );
        
        for (i = 0; i < tx_length-6; i++)
            printf("%c", data[i+TX_DATA_OFS]);
        
        printf("\n");
        
    
    } else
    {
    
        printf("Unknown packet type from %d->%d\n", tx_src_node_id, tx_tar_node_id );
    
    }
    
}
/* End of mesh_data */












/* Called by networking code every second */
int mnode_update( void )
{
    //printf("Update\n");




}
/* End of mnode_update */



/* Start node */
int mnode_start(void)
{
    network_start(mnode_packet, mnode_update);

 
    return 0;
}
/* End of mnode_start */



/* Stop node */
int mnode_stop(void)
{
    return 0;
}
/* End of mnode_stop */

 


 
