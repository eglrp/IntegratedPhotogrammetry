/* 
 *  Copyright (c) 2008  Noah Snavely (snavely (at) cs.washington.edu)
 *    and the University of Washington
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 */

/* LinkDirection.h */

#ifndef __link_direction_h__
#define __link_direction_h__

typedef enum {
    DIRECTION_LEFT     = 0,
    DIRECTION_RIGHT    = 1,
    DIRECTION_FORWARD  = 2,
    DIRECTION_BACKWARD = 3,
    DIRECTION_UP       = 4,
    DIRECTION_DOWN     = 5,
    DIRECTION_ZOOM_OUT = 6,
    DIRECTION_ZOOM_IN = 7,
    NUM_LINK_DIRECTIONS = 8
} LinkDirection;

#endif /* __link_direction_h__ */
