/*
    Mesh Protocol Test 
        (C) 2012 
            Jason Hunt (nulluser@gmail.com)
            Ling
               
    
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
                Ling

    File: mnode.h
*/


#ifndef MNODE_H
#define MNODE_H



#include "types.h"
#include "datatap.h"
#include "network.h"

#define MN_SUCCESS      0
#define MN_FAIL         1

#define STATE_WAIT      0
#define STATE_LENGTH    1
#define STATE_DATA      2

//typedef struct network_type network_type;
//typedef struct mnode_type mnode_type;

struct mnode_type
{
    // system datataps  
    unsigned int local_freeram;
    unsigned int local_procs;

    void (*text_out)(char * format, ...);
    
   network_type * network;         // All Network Data

};


int mnode_start(void (*out_func)(char * format, ...));
int mnode_stop(void);

int mnode_tap_add(char *name, unsigned int tap_type, void * tap_link);




void mnode_ident ( void );
void mnode_string ( unsigned char *s );
void mnode_list_nodes( void );
void mnode_datatap_poll( void );





#endif


