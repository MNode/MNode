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

    File: network.h
*/


#ifndef NETWORK_H
#define NETWORK_H

int network_init( void );
void network_start( void (*mesh_parser_link)(unsigned char *, unsigned int) );
void network_stop( void );
<<<<<<< HEAD:network.h
int network_send(unsigned char *data, unsigned int length);

void network_ident (void );
void network_string (unsigned char *s );

=======
void network_ident( void );
>>>>>>> b1ead17eda875a034ab87687408d4e9f75a11515:network.h




#define TX_DATA_OFS 6


#define ID_IDENT   1
#define ID_STRING    10




#endif

