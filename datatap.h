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

    File: datatap.h
*/

#ifndef DATATAP_H
#define DATATAP_H

#include "types.h"
#include "mnode.h"
#include "network.h"

#define DT_INT8     1
#define DT_INT16    2
#define DT_INT32    3
#define DT_STRING   4


struct datatap_type
{
    char tap_name[32];

    unsigned int tap_type;
    unsigned int tap_value;
    
    
    void *tap_link;
       
    
    struct datatap_type *next;
        

};


/* Respond to poll request */


void datatap_poll ( mnode_type *m, network_type *n );


/* Got data from other node */
void datatap_data ( mnode_type *m,
                    unsigned int node_id, 
                    unsigned char *data,  
                    unsigned int length );




void data_tap_add ( mnode_type *m, char *name, unsigned int tap_type, void * tap_link );


void data_tap_start ( void );



#endif


